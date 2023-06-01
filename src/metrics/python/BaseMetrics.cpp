#include <pyutk/utils.hpp>

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
#include <utk/metrics/MinDist.hpp>

#include <utk/metrics/IntegrationTest.hpp>
#include <utk/metrics/Integrands/GaussianIntegrands.hpp>
#include <utk/metrics/Integrands/HeavisideIntegrands.hpp>

template<typename T>
void ExposeIntegrationTest(py::module& m, const std::string& name)
{
    using namespace utk;

    py::class_<IntegrationTest>(m, name.c_str())
        .def(py::init<>())
        .def("BuildDatabase", &IntegrationTest::BuildDatabase<T>)
        .def("ReadDatabse"  , &IntegrationTest::ReadDatabase<T>)
        .def("compute"      , GetComputeFunction<IntegrationTest, double, IntegrationTest::ErrorReport>());
}

void init_Metrics(py::module& m)
{
    using namespace utk;

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

    py::class_<MinDist>(m, "MinDistance")
        .def(py::init<double>(), py::arg("toroidal") = false)
        .def("compute", GetComputeFunction<MinDist, double>());

    // Expose error reporting
    py::class_<IntegrationTest::ErrorReport>(m, "IntegrationErrorReport")
        .def_readonly("min", &IntegrationTest::ErrorReport::min)
        .def_readonly("min", &IntegrationTest::ErrorReport::max)
        .def_readonly("min", &IntegrationTest::ErrorReport::mean)
        .def_readonly("min", &IntegrationTest::ErrorReport::var)
        .def_readonly("min", &IntegrationTest::ErrorReport::count);

    ExposeIntegrationTest<GaussianIntegrand> (m, "GaussianIntegrationTest");
    ExposeIntegrationTest<HeavisideIntegrand>(m, "HeavisideIntegrationTest");
    
}