#pragma once

struct Config
{
    unsigned int seed   = 20081976;
    unsigned int dim    =     2;
    unsigned int nmin   =    16;     // 2^4
    unsigned int nmax   = 32769;     // 2^15 + 1
    unsigned int factor =     2;     // 
    unsigned int m      =    16;     // Number of pointsets to
                                     // generate for each size
    unsigned int nInt   = nmax * 2;  // N for integral computation
                                     // Make sure it is way above nmax 
    unsigned int mInt   =  4096;     // Number of integrands
};  