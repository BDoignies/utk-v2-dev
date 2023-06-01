#pragma once

#include <cstdint>

#ifdef _WIN32
    // As windows definitely won't update OpenMP, a remapping is necessary 
    using OPENMP_UINT = long int;
#else
    using OPENMP_UINT = uint32_t;
#endif