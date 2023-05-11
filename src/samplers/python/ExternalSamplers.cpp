#include <pyutk/utils.hpp>

#ifdef PYUTK_LLOYD 
    #include <utk/samplers/SamplerLloyd.hpp>
    
    void init_Lloyd(py::module& m)
    {
        // TODO
    }
#else 
    void init_Lloyd(py::module& m)
    {
        // Empty, do not compile anything
    }
#endif

#ifdef PYUTK_BNOT
    #include <utk/samplers/SamplerBoundedBNOT.hpp>
    #include <utk/samplers/SamplerPeriodicBNOT.hpp>

    void init_BNOT(py::module& m)
    {
        // TODO
    }
#else
    void init_BNOT(py::module& m)
    {
        // Empty, do not compile anything
    }
#endif

void init_Externals(py::module& m)
{
    init_Lloyd(m);
    init_BNOT(m);
}