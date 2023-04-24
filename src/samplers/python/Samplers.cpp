#include <pyutk/utils.hpp>

void init_BaseSampler(py::module& m);
void init_Scrambling(py::module& m);
void init_Externals(py::module& m);

void init_pyutkSampler(py::module& m)
{
    init_BaseSampler(m);
    init_Scrambling(m);
    init_Externals(m);
}