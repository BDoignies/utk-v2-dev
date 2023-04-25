#include <pyutk/utils.hpp>

#include <utk/scrambling/Owen.hpp>
#include <utk/scrambling/OwenTreeScrambling.hpp>
#include <utk/scrambling/CranleyPatterson.hpp>
#include <utk/scrambling/DigitalShift.hpp>
#include <utk/scrambling/MicroJittering.hpp>

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