#include <utk/utils/log.hpp>
#include <pyutk/utils.hpp>

void init_pyutkSampler(py::module& m);
void init_pyutkMetrics(py::module& m);

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

    init_pyutkSampler(m);
    init_pyutkMetrics(m);
};