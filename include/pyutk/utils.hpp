#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <utk/utils/Pointset.hpp>

namespace py = pybind11;
using namespace utk;

// Global constants for various samplers
namespace utk
{
    inline static constexpr uint64_t NO_SEED = 0;
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
