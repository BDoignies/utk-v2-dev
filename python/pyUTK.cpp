#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <utk/utils/log.hpp>

#include <utk/samplers/SamplerWhitenoise.hpp>
#include <utk/samplers/SamplerHalton.hpp>
#include <utk/samplers/SamplerHammersley.hpp>
#include <utk/samplers/SamplerRegularGrid.hpp>
#include <utk/samplers/SamplerStratified.hpp>
#include <utk/samplers/SamplerHexagonalGrid.hpp>
#include <utk/samplers/SamplerNRooks.hpp>
#include <utk/samplers/SamplerCMJ.hpp>
#include <utk/samplers/SamplerFaure.hpp>
#include <utk/samplers/SamplerNiederreiter.hpp>
#include <utk/samplers/SamplerLutLDBN.hpp>
#include <utk/samplers/SamplerPMJ.hpp>
#include <utk/samplers/SamplerDartThrowing.hpp>
#include <utk/samplers/SamplerProjectiveBlueNoise.hpp>
#include <utk/samplers/SamplerSobol.hpp>
#include <utk/samplers/SamplerR1.hpp>
#include <utk/samplers/SamplerKronecker.hpp>
#include <utk/samplers/SamplerKorobov.hpp>
#include <utk/samplers/SamplerAAPatterns.hpp>
#include <utk/samplers/SamplerART.hpp>
#include <utk/samplers/SamplerPenrose.hpp>
#include <utk/samplers/SamplerFastPoisson.hpp>
#include <utk/samplers/SamplerFattal.hpp>
#include <utk/samplers/SamplerForcedRandom.hpp>
#include <utk/samplers/SamplerStep.hpp>
#include <utk/samplers/SamplerSinglePeak.hpp>

#include <utk/scrambling/Owen.hpp>
#include <utk/scrambling/OwenTreeScrambling.hpp>
#include <utk/scrambling/CranleyPatterson.hpp>
#include <utk/scrambling/DigitalShift.hpp>
#include <utk/scrambling/MicroJittering.hpp>

#include <utk/metrics/Diaphony.hpp>
#include <utk/metrics/BoundariesStarDiscrepancy.hpp>
#include <utk/metrics/CenteredL2.hpp>
#include <utk/metrics/GeneralizedL2.hpp>
#include <utk/metrics/L2.hpp>
#include <utk/metrics/SymmetricL2.hpp>
#include <utk/metrics/UnanchoredL2.hpp>

#include <utk/metrics/Spectrum.hpp>
#include <utk/metrics/RadialSpectrum.hpp>
#include <utk/metrics/PCF.hpp>

namespace py = pybind11;
using namespace utk;

// Global constants for various samplers
namespace utk
{
    std::vector<uint32_t> HALTON_BASIS; 
    std::vector<double>   KRONECKER_ALPHAS;

    static constexpr uint64_t NO_SEED = 0;
};

template<typename DataType>
auto ToNumpyArray(Pointset<DataType>* pts)
{
    py::capsule freeArray(pts, [](void* data) {
        Pointset<DataType>* toFree = reinterpret_cast<Pointset<DataType>*>(data);
        delete toFree;
    }); 

    return py::array_t<DataType>(
        { pts->Npts(), pts->Ndim() },
        { pts->Ndim() * sizeof(DataType), sizeof(DataType) },
          pts->Data(),
          freeArray
    );       
}

template<typename T, typename DataType = double>
auto GetSampleFunction()
{
    return [](T& sp, uint32_t n) {
        // Use pointer to avoid 'automatic' deletion
        Pointset<DataType>* pts = new Pointset<DataType>;
        bool rslt = sp.generateSamples(*pts, n);

        if (rslt)
            return ToNumpyArray<DataType>(pts);

        throw std::runtime_error("Error while generating samples");
    };
}

template<typename T, typename InputType, typename OutputType>
auto GetScrambleFunction()
{
    return [](T& scrambler, py::buffer b){
        py::buffer_info info = b.request();

        if (info.format != py::format_descriptor<InputType>::format())
            throw std::runtime_error("Type not supported");

        if (info.ndim != 2)
            throw std::runtime_error("Only 2D array are supported");
        
        std::vector<py::ssize_t> desiredStride = {
            info.shape[1] * static_cast<py::ssize_t>(sizeof(InputType)), sizeof(InputType)
        };
        if (info.strides != desiredStride)
            throw std::runtime_error("Only contiguous C-ordered array are supported");
        
        Pointset<InputType> in = Pointset<InputType>::View(
            static_cast<InputType*>(info.ptr), 
            info.shape[0], info.shape[1]
        );

        Pointset<OutputType>* out = new Pointset<OutputType>;
        scrambler.Scramble(in, *out);
        return ToNumpyArray<OutputType>(out);
    };
}

template<typename T, typename U>
struct ComputeOutType
{ using type = std::variant<T, U>; };

template<typename T>
struct ComputeOutType<T, T>
{ using type = T; };


template<typename T, typename InputType, typename OutputType = InputType, typename VectorizedOutputType = std::vector<OutputType>>
auto GetComputeFunction()
{
    using outtype = typename ComputeOutType<OutputType, VectorizedOutputType>::type;

    return [](T& metrics, py::buffer b){
        py::buffer_info info = b.request();

        if (info.format != py::format_descriptor<InputType>::format())
            throw std::runtime_error("Type not supported");

        if (info.ndim == 2)
        {
            std::vector<py::ssize_t> desiredStride = {
                info.shape[1] * static_cast<py::ssize_t>(sizeof(InputType)), sizeof(InputType)
            };

            if (info.strides != desiredStride)
                throw std::runtime_error("Only contiguous C-ordered array are supported");
            
            Pointset<InputType> in = Pointset<InputType>::View(
                static_cast<InputType*>(info.ptr), 
                info.shape[0], info.shape[1]
            );
            outtype out = metrics.compute(in);
            return out;
        }
        else if (info.ndim == 3)
        {
             std::vector<py::ssize_t> desiredStride = {
                info.shape[2] * info.shape[1] * static_cast<py::ssize_t>(sizeof(InputType)), 
                                info.shape[2] * static_cast<py::ssize_t>(sizeof(InputType)), 
                                                                         sizeof(InputType)
            };

            if (info.strides != desiredStride)
                throw std::runtime_error("Only contiguous C-ordered array are supported");
            
            std::vector<Pointset<InputType>> ptss(info.shape[0]);
            for (uint32_t k = 0; k < info.shape[0]; k++)
            {
                ptss[k] =  Pointset<InputType>::View(
                    static_cast<InputType*>(info.ptr) + k * info.shape[1] * info.shape[2], 
                    info.shape[1], info.shape[2]
                );
            }

            outtype out = metrics.compute(ptss);
            return out;
        }

        throw std::runtime_error("Only 2D/3D array are supported");
        return outtype{};
    };
}

template<typename T, typename InputType = T>
auto GetVComputeFunction()
{
    return [](T& metrics, py::buffer b){
        py::buffer_info info = b.request();

        if (info.format != py::format_descriptor<InputType>::format())
            throw std::runtime_error("Type not supported");

        if (info.ndim != 2)
            throw std::runtime_error("Only 2D array are supported");
        
        std::vector<py::ssize_t> desiredStride = {
            info.shape[1] * static_cast<py::ssize_t>(sizeof(InputType)), sizeof(InputType)
        };
        if (info.strides != desiredStride)
            throw std::runtime_error("Only contiguous C-ordered array are supported");
        
        Pointset<InputType> in = Pointset<InputType>::View(
            static_cast<InputType*>(info.ptr), 
            info.shape[0], info.shape[1]
        );

        auto out = metrics.compute(in);
        return out;
    };
}

template<typename T>
auto GetSetSeedFunction()
{
    return [](T& sp, uint64_t seed)
    {
        if (seed == NO_SEED)
            sp.setRandomSeed();
        else
            sp.setRandomSeed(seed);
    };
}

/* Init samplers with no dependancies (other than plain old cpp files) */
void init_BasicSamplers(py::module& m)
{
    py::class_<SamplerWhitenoise>(m, "Whitenoise")
        .def(py::init<uint32_t>(), py::arg("d"))
        .def("__repr__", [](const SamplerWhitenoise& wn) { return "Whitenoise(d=" + std::to_string(wn.GetDimension()) +")"; })
        .def("setSeed", GetSetSeedFunction<SamplerWhitenoise>(), py::arg("seed") = NO_SEED)
        .def("sample",  GetSampleFunction <SamplerWhitenoise>(), py::arg("N"));

    py::class_<SamplerHalton>(m, "Halton")
        .def(py::init<uint32_t>(), py::arg("d"))
        .def("__repr__", [](const SamplerHalton& wn) { return "Halton(d=" + std::to_string(wn.GetDimension()) +")"; })
        .def("setBasisList", [](SamplerHalton& ht, const std::vector<uint32_t>& list)
        {
            if (list.size() < ht.GetDimension())
                throw std::runtime_error("Basis list should be greater than sampler dimension");

            HALTON_BASIS = list;
            ht.setBasisList(HALTON_BASIS.data(), list.size());
        })
        .def("sample",  GetSampleFunction <SamplerHalton>()          , py::arg("N"))
        .def("isample", GetSampleFunction <SamplerHalton, uint32_t>(), py::arg("N"));

    py::class_<SamplerHammersley>(m, "Hammersley")
        .def(py::init<>())
        .def("__repr__", [](const SamplerHammersley& wn) { return "Hammersley()"; })
        .def("sample",  GetSampleFunction <SamplerHammersley>()          , py::arg("N"))
        .def("isample", GetSampleFunction <SamplerHammersley, uint32_t>(), py::arg("N"));

    py::class_<SamplerRegularGrid>(m, "RegularGrid")
        .def(py::init<uint32_t>(), py::arg("d"))
        .def("__repr__", [](const SamplerRegularGrid& wn) { return "RegularGrid(d=" + std::to_string(wn.GetDimension()) +")"; })
        .def("sample",  GetSampleFunction <SamplerRegularGrid>(), py::arg("N"));

    py::class_<SamplerStratified>(m, "Stratified")
        .def(py::init<uint32_t>(), py::arg("d"))
        .def("__repr__", [](const SamplerStratified& wn) { return "Stratified(d=" + std::to_string(wn.GetDimension()) +")"; })
        .def("setSeed", GetSetSeedFunction<SamplerStratified>(), py::arg("seed") = NO_SEED)
        .def("sample",  GetSampleFunction <SamplerStratified>(), py::arg("N"));

    py::class_<SamplerHexagonalGrid>(m, "HexGrid")
        .def(py::init<>())
        .def("__repr__", [](const SamplerHexagonalGrid& wn) { return "HexGrid()"; })
        .def("sample",  GetSampleFunction <SamplerHexagonalGrid>() , py::arg("N"));

    py::class_<SamplerCMJ>(m, "CMJ")
        .def(py::init<>())
        .def("__repr__", [](const SamplerCMJ& wn) { return "CMJ()"; })
        .def("setSeed", GetSetSeedFunction<SamplerCMJ>(), py::arg("seed") = NO_SEED)
        .def("sample",  GetSampleFunction <SamplerCMJ>() , py::arg("N"));

    py::class_<SamplerFaure>(m, "Faure")
        .def(py::init<uint32_t>(), py::arg("d"))
        .def("__repr__", [](const SamplerFaure& wn) { return "Faure(d=" + std::to_string(wn.GetDimension()) +")"; })
        .def("sample",  GetSampleFunction <SamplerFaure>()          , py::arg("N"))
        .def("isample", GetSampleFunction <SamplerFaure, uint32_t>(), py::arg("N"));

    py::class_<SamplerNRooks>(m, "NRooks")
        .def(py::init<uint32_t>(), py::arg("d"))
        .def("__repr__", [](const SamplerNRooks& wn) { return "NRooks(d=" + std::to_string(wn.GetDimension()) +")"; })
        .def("setSeed", GetSetSeedFunction<SamplerNRooks>(), py::arg("seed") = NO_SEED)
        .def("sample",  GetSampleFunction <SamplerNRooks>(), py::arg("N"));

    py::class_<SamplerNiederreiter>(m, "Niederreiter")
        .def(py::init<uint32_t, uint32_t>(), py::arg("d"), py::arg("b") = 2)
        .def("__repr__", [](const SamplerNiederreiter& wn) { return "Niederreiter(d=" + std::to_string(wn.GetDimension()) +")"; })
        .def("setBasis", &SamplerNiederreiter::setBasis)
        .def("sample",  GetSampleFunction <SamplerNiederreiter>()          , py::arg("N"))
        .def("isample", GetSampleFunction <SamplerNiederreiter, uint32_t>(), py::arg("N"));

    py::class_<SamplerLutLDBN>(m, "LDBN")
        .def(py::init<>())
        .def("__repr__", [](const SamplerLutLDBN& wn) { return "LDBN()"; })
        .def_static("getAvailableTargets", &SamplerLutLDBN::GetTargets)
        .def("setTarget", &SamplerLutLDBN::setTarget)
        .def("sample",  GetSampleFunction <SamplerLutLDBN>()          , py::arg("N"))
        .def("isample", GetSampleFunction <SamplerLutLDBN, uint32_t>(), py::arg("N"));
    
    py::class_<SamplerPMJ>(m, "PMJ")
        .def(py::init<const std::string&, uint32_t>(), py::arg("method") = "PMJ02", py::arg("nbCandidates") = 10)
        .def("__repr__", [](const SamplerPMJ& wn) { return "PMJ()"; })
        .def_static("getAvailableMethods", &SamplerPMJ::GetMethods)
        .def("setMethod", &SamplerPMJ::setMethod)
        .def("setNbCandidates", &SamplerPMJ::setCandidates)
        .def("setSeed", GetSetSeedFunction<SamplerPMJ>(), py::arg("seed") = NO_SEED)
        .def("sample",  GetSampleFunction <SamplerPMJ>(), py::arg("N"));

    py::class_<SamplerDartThrowing>(m, "DartThrowing")
        .def(py::init<
                uint32_t, bool, bool,
                uint32_t, double, double
            >(), 
                py::arg("d"), py::arg("relaxed") = true, py::arg("toroidal") = true,
                py::arg("trials") = 1000, py::arg("relaxedFactor") = 0.9, py::arg("spherePacking") = -1.0
        )
        .def("__repr__", [](const SamplerDartThrowing& wn) { return "DartThrowing(d=" + std::to_string(wn.GetDimension()) +")"; })
        .def("setRelaxed" ,      &SamplerDartThrowing::setRelaxed)
        .def("setToroidal",      &SamplerDartThrowing::setToroidal)
        .def("setMaxTrials",     &SamplerDartThrowing::setMaxIters)
        .def("setRelaxedFactor", &SamplerDartThrowing::setRelaxedFactor)
        .def("setSpherePacking", &SamplerDartThrowing::setSpherePacking, py::arg("packing") = -1.0)
        .def("setSeed", GetSetSeedFunction<SamplerDartThrowing>(), py::arg("seed") = NO_SEED)
        .def("sample",  GetSampleFunction <SamplerDartThrowing>(), py::arg("N"));

    py::class_<SamplerProjectiveBlueNoise>(m, "ProjectiveBlueNoise")
        .def(py::init<
                uint32_t, bool, bool,
                uint32_t, double
            >(), 
                py::arg("d"), py::arg("relaxed") = true, py::arg("toroidal") = true,
                py::arg("trials") = 1000, py::arg("relaxedFactor") = 0.9
        )
        .def("__repr__", [](const SamplerProjectiveBlueNoise& wn) { return "ProjectiveBlueNoise(d=" + std::to_string(wn.GetDimension()) +")"; })
        .def("setRelaxed" ,      &SamplerProjectiveBlueNoise::setRelaxed)
        .def("setToroidal",      &SamplerProjectiveBlueNoise::setToroidal)
        .def("setMaxTrials",     &SamplerProjectiveBlueNoise::setMaxIters)
        .def("setRelaxedFactor", &SamplerProjectiveBlueNoise::setRelaxedFactor)
        .def("setSeed", GetSetSeedFunction<SamplerProjectiveBlueNoise>(), py::arg("seed") = NO_SEED)
        .def("sample",  GetSampleFunction <SamplerProjectiveBlueNoise>(), py::arg("N"));

    py::class_<SamplerFastPoisson>(m, "FastPoisson")
        .def(py::init<
                const std::string&, double, 
                uint32_t, uint32_t, 
                bool, bool, uint32_t 
            >(), 
                py::arg("method") = "DartThrowing", py::arg("radius") = 0.1, 
                py::arg("throwMultiplier") = 1, py::arg("throws") = 1000,
                py::arg("maximize") = false, py::arg("tiled") = false,
                py::arg("relaxCount") = 0
        )
        .def("__repr__", [](const SamplerFastPoisson& wn) { return "FastPoisson()"; })
        .def_static("getAvailableMethods", &SamplerFastPoisson::GetMethods)
        .def("setMethod" ,         &SamplerFastPoisson::setMethod)
        .def("setRadius",          &SamplerFastPoisson::setRadius)
        .def("setThrowMultiplier", &SamplerFastPoisson::setThrowMultiplier)
        .def("setThrows",          &SamplerFastPoisson::setThrows)
        .def("setMaximize",        &SamplerFastPoisson::setMaximize)
        .def("setTiled",           &SamplerFastPoisson::setTiled)
        .def("setRelaxCount",      &SamplerFastPoisson::setRelaxCount)
        .def("sample",  GetSampleFunction <SamplerFastPoisson>(), py::arg("N"));

    py::class_<SamplerRank1>(m, "Rank1")
        .def(py::init<bool, bool>(), py::arg("exhaustiveSearch") = false, py::arg("fibonnaci") = false)
        .def("__repr__", [](const SamplerRank1& wn) { return "Rank1()"; })
        .def("setExhaustiveSearch", &SamplerRank1::setExhaustSearch)
        .def("setFibonnacci"      , &SamplerRank1::setFibonnacci)
        .def("sample",  GetSampleFunction <SamplerRank1>(), py::arg("N"));

    py::class_<SamplerKorobov>(m, "Korobov")
        .def(py::init<uint32_t, uint32_t>(), py::arg("d"), py::arg("generator") = 3)
        .def("__repr__", [](const SamplerKorobov& wn) {  return "Korobov(d=" + std::to_string(wn.GetDimension()) +")"; })
        .def("setGenerator", &SamplerKorobov::setGeneratorA)
        .def("sample",  GetSampleFunction <SamplerKorobov>(), py::arg("N"));

    py::class_<SamplerKronecker>(m, "Kronecker")
        .def(py::init<uint32_t>(), py::arg("d"))
        .def("__repr__", [](const SamplerKronecker& wn) {  return "Kronecker(d=" + std::to_string(wn.GetDimension()) +")"; })
        .def("setAlphas", [](SamplerKronecker& kronecker, const std::vector<double>& alphas)
        {
            if (alphas.size() < kronecker.GetDimension())
                throw std::runtime_error("Alphas should have a greater count than sampler");    

            KRONECKER_ALPHAS = alphas;
            kronecker.setAlphas(KRONECKER_ALPHAS.data());
        })
        .def("setAlphas", [](SamplerKronecker& kronecker, const std::string& name)
        {
            const double* alphas = SamplerKronecker::ParseAlpha(name, kronecker.GetDimension());
            if (alphas == nullptr)
                throw std::runtime_error("Alphas do not match dimension");
            
            kronecker.setAlphas(alphas);
        })
        .def_static("getAvailableAlphasNames", &SamplerKronecker::GetAlphas)
        .def("sample",  GetSampleFunction <SamplerKronecker>(), py::arg("N"));

    // TODO : Kronecker

    py::class_<SamplerAAPatterns>(m, "AAPatterns")
        .def(py::init<>())
        .def("__repr__", [](const SamplerAAPatterns& wn) { return "AAPatterns()"; })
        .def_static("getAvailableTiling", &SamplerAAPatterns::GetDefaultFiles)
        .def("setSeed", GetSetSeedFunction<SamplerAAPatterns>(), py::arg("seed") = NO_SEED)
        .def("sample",  GetSampleFunction <SamplerAAPatterns>(), py::arg("N"));

    py::class_<SamplerART>(m, "ART")
        .def(py::init<>())
        .def("__repr__", [](const SamplerART& wn) { return "ART()"; })
        .def("sample",  GetSampleFunction <SamplerAAPatterns>(), py::arg("N"));

    py::class_<SamplerPenrose>(m, "Penrose")
        .def(py::init<>())
        .def("__repr__", [](const SamplerPenrose& wn) { return "ART()"; })
        .def("sample",  GetSampleFunction <SamplerAAPatterns>(), py::arg("N"));

    
    py::class_<SamplerFattal>(m, "Fattal")
        .def(py::init<double, uint32_t>(), py::arg("temperature") = 0.5, py::arg("level") = 2)
        .def("__repr__", [](const SamplerFattal& wn) { return "Fattal()"; })
        .def("setTemperature", &SamplerFattal::setTemperature)
        .def("setLevel"      , &SamplerFattal::setLevel)
        .def("sample",  GetSampleFunction <SamplerFattal>(), py::arg("N"));

    py::class_<SamplerForcedRandom>(m, "ForcedRandom")
        .def(py::init<uint32_t, uint32_t>(), py::arg("sparcity") = 8, py::arg("matrixSize") = 512)
        .def("__repr__", [](const SamplerForcedRandom& wn) { return "ForcedRandom()"; })
        .def("setMatrixSparsity", &SamplerForcedRandom::setMatrixSparsity)
        .def("setMatrixSize"    , &SamplerForcedRandom::setMatrixSize)
        .def("sample",  GetSampleFunction <SamplerForcedRandom>(), py::arg("N"));

    py::class_<SamplerStep>(m, "Step")
        .def(py::init<float, float>(), py::arg("critFreq") = 0.606, py::arg("smooth") = 8.f)
        .def("__repr__", [](const SamplerStep& wn) { return "STEP()"; })
        .def("setSeed", GetSetSeedFunction<SamplerStep>(), py::arg("seed") = NO_SEED)
        .def("setCriticalFrequency", &SamplerStep::setCriticalFrequency)
        .def("setSmoothing"        , &SamplerStep::setSmoothing)
        .def("sample",  GetSampleFunction <SamplerStep>(), py::arg("N"));

    py::class_<SamplerSinglePeak>(m, "SinglePeak")
        .def(py::init<
                float, float, float, float
             >(), 
                py::arg("critFreq") = 36.5, py::arg("smooth") = 8.f,
                py::arg("peakSmoothing") = 2.0, py::arg("peakPower") = 16.0
        )
        .def("__repr__", [](const SamplerSinglePeak& wn) { return "SinglePeak()"; })
        .def("setSeed", GetSetSeedFunction<SamplerSinglePeak>(), py::arg("seed") = NO_SEED)
        .def("setPeakPower"        , &SamplerSinglePeak::setPeakPower)
        .def("setPeakSmoothing"    , &SamplerSinglePeak::setPeakSmoothing)
        .def("setCriticalFrequency", &SamplerSinglePeak::setCriticalFrequency)
        .def("setSmoothing"        , &SamplerSinglePeak::setSmoothing)
        .def("sample",  GetSampleFunction <SamplerSinglePeak>(), py::arg("N"));
        
    using SobolSampler = SamplerSobol<uint32_t>; 
    py::class_<SobolSampler>(m, "Sobol")
        .def(
            py::init<uint32_t, uint32_t, const std::string&>(),
            py::arg("d"), py::arg("depth") = 32, py::arg("file") = ""
        )
        .def("__repr__", [](const SobolSampler& wn) { return "Sobol(d=" + std::to_string(wn.GetDimension()) +")"; })
        .def("setDirectionFile", &SobolSampler::setDirectionFile)
        .def("setOwenDepth",     &SobolSampler::setOwenDepth)
        .def("setSeed", GetSetSeedFunction<SobolSampler>()              , py::arg("seed") = NO_SEED)
        .def("sample",  GetSampleFunction <SobolSampler>()              , py::arg("N"))
        .def("isample", GetSampleFunction <SobolSampler, uint32_t>(), py::arg("N"));
}

void init_Scrambling(py::module& m)
{
    using IntegerType = uint32_t;
    using OwenScrambling = ScramblingOwen<IntegerType>;
    using OwenTreeScramblingI = OwenTreeScrambling<IntegerType>;
    using DigitalShiftScramplingI = DigitalShift<IntegerType>;

    py::class_<OwenScrambling>(m, "OwenScrambling")
        .def(py::init<uint32_t>(), py::arg("depth") = 32)
        .def("setOwenDepth", &OwenScrambling::setOwenDepth)
        .def("setSeed", GetSetSeedFunction<OwenScrambling>(), py::arg("seed") = NO_SEED)
        .def("scramble",  GetScrambleFunction<OwenScrambling, IntegerType, double>())
        .def("iscramble", GetScrambleFunction<OwenScrambling, IntegerType, IntegerType>());
    
    py::class_<CranleyPattersonScrambling>(m, "CranleyPattersonScrambling")
        .def(py::init<double, double>(), py::arg("max") = 1.0, py::arg("domain") = 1.0)
        .def("setSeed" ,  GetSetSeedFunction <CranleyPattersonScrambling>(), py::arg("seed") = NO_SEED)
        .def("scramble",  GetScrambleFunction<CranleyPattersonScrambling, double, double>());

    py::class_<DigitalShiftScramplingI>(m, "DigitalShift")
        .def(py::init<>())
        .def("setSeed" ,  GetSetSeedFunction <DigitalShiftScramplingI>(), py::arg("seed") = NO_SEED)
        .def("scramble",  GetScrambleFunction<DigitalShiftScramplingI, IntegerType, double>());

    py::class_<MicroJittering>(m, "MicroJittering")
        .def(py::init<double>(), py::arg("intensity") = -1.0)
        .def("setSeed" ,  GetSetSeedFunction <MicroJittering>(), py::arg("seed") = NO_SEED)
        .def("scramble",  GetScrambleFunction<MicroJittering, double, double>());

    
    py::class_<OwenTreeScramblingI>(m, "OwenTreeScrambling")
        .def(py::init<uint32_t>(), py::arg("depth") = 32)
        .def("setOwenDepth", &OwenTreeScramblingI::setOwenDepth)
        .def("setSeed", GetSetSeedFunction<OwenTreeScramblingI>(), py::arg("seed") = NO_SEED)
        .def("setBitPattern", &OwenTreeScramblingI::setBitPattern)
        .def("fillRandom", &OwenTreeScramblingI::FillRandom, py::arg("dim"), py::arg("depth") = 0)
        .def("scramble",  GetScrambleFunction<OwenTreeScramblingI, IntegerType, double>())
        .def("iscramble", GetScrambleFunction<OwenTreeScramblingI, IntegerType, IntegerType>());
}

void init_Metrics(py::module& m)
{
    py::class_<Spectrum>(m, "Spectrum")
        .def(py::init<uint32_t, bool>(), py::arg("resolution") = 0, py::arg("cancelDC") = true)
        .def("setResoluion", &Spectrum::setResolution)
        .def("setCancelDC", &Spectrum::setCancelDC)
        .def("compute", GetComputeFunction<Spectrum, double, std::vector<double>, std::vector<double>>());

    py::class_<RadialSpectrum>(m, "RadialSpectrum")
        .def(py::init<uint32_t, double, uint32_t, bool>(), 
            py::arg("nbins") = 0, py::arg("scale") = 0.5, py::arg("resolution") = 0, py::arg("cancelDC") = true
        )
        .def("setResoluion", &RadialSpectrum::setResolution)
        .def("setScale", &RadialSpectrum::setScale)
        .def("setCancelDC", &RadialSpectrum::setCancelDC)
        .def("setBins", &RadialSpectrum::setBins)
        .def("compute", GetComputeFunction<RadialSpectrum, double, std::vector<double>, std::vector<double>>());
    
    py::class_<PCF>(m, "PCF")
        .def(py::init<bool, double, double, uint32_t, double>(), 
            py::arg("toroidal") = true, py::arg("rmin") = 0.01, py::arg("rmax") = 0.15, 
            py::arg("nbbins") = 200, py::arg("smoothing") = 0.001
        )
        .def("setToroidal", &PCF::setToroidal)
        .def("setRmin", &PCF::setRmin)
        .def("setRmax", &PCF::setRmax)
        .def("setSmoothing", &PCF::setSmoothing)
        .def("setNbBins", &PCF::setNbBins)
        .def("compute", GetComputeFunction<PCF, double, std::vector<double>, std::vector<double>>());

    py::class_<Diaphony>(m, "Diaphony")
        .def(py::init<>())
        .def("compute", GetComputeFunction<Diaphony, double>());


    py::class_<CenteredL2Discrepancy>(m, "CenteredL2Discrepancy")
        .def(py::init<>())
        .def("compute", GetComputeFunction<CenteredL2Discrepancy, double>());

    
    py::class_<L2Discrepancy>(m, "L2Discrepancy")
        .def(py::init<>())
        .def("compute", GetComputeFunction<L2Discrepancy, double>());

    
    py::class_<GL2Discrepancy>(m, "GL2Discrepancy")
        .def(py::init<>())
        .def("compute", GetComputeFunction<GL2Discrepancy, double>());
    

    py::class_<SymmetricL2Discrepancy>(m, "SymmetricL2Discrepancy")
        .def(py::init<>())
        .def("compute", GetComputeFunction<SymmetricL2Discrepancy, double>());


    py::class_<UnanchoredL2Discrepancy>(m, "UnanchoredL2Discrepancy")
        .def(py::init<>())
        .def("compute", GetComputeFunction<UnanchoredL2Discrepancy, double>());

    
    py::class_<BoundariesStarDiscrepancy>(m, "BoundariesStarDiscrepancy")
        .def(py::init<double>(), py::arg("eps") = -1.)
        .def("compute", GetComputeFunction<BoundariesStarDiscrepancy, double, std::pair<double, double>>());
}

PYBIND11_MODULE(pyutk, m) 
{
    m.doc() = "UTK python binding";

    m.def("disableLogs", [](){
        UTK_LOG_DISABLE();
    });
    m.def("enableLogs", [](){
        UTK_LOG_ENABLE();
    });
    m.def("hasLogs", [](){
        #ifdef UTK_LOG
            return true;
        #else 
            return false;
        #endif
    });
    m.def("setLogFile", [](const std::string& file){
        UTK_LOG_FILE(file);
    });

    m.def("setLogConsole", [](){
        UTK_LOG_CONSOLE();
    });

    // TODO : submodules ? 
    init_BasicSamplers(m);
    init_Scrambling(m);
    init_Metrics(m);
};