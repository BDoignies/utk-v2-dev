#include <pyutk/utils.hpp>

void init_Metrics(py::module& m);

void init_pyutkMetrics(py::module& m)
{
    init_Metrics(m);
}