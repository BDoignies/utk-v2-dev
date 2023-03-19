/*
 * Coded by Hélène Perrier helene.perrier@liris.cnrs.fr
 * Copyright (c) 2018 CNRS Université de Lyon
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the Halton project.
 */

#include "../../pointsets/Pointset.hpp"
#include "../utils/FastPRNG.hpp"
#include <random>

template <typename IntegerType = uint32_t>
class ScramblingOwen
{
public:
    static constexpr unsigned char MAX_BITS = (sizeof(IntegerType) * __CHAR_BIT__);
   
    ScramblingOwen(unsigned int dp = MAX_BITS) : 
        depth(dp)
    { }

    void setOwenDepth(unsigned int dp)
    {
        if (dp < MAX_BITS)
            depth = dp
        else
            depth = MAX_BITS;
    }

    void setRandomSeed(long unsigned int arg_seed) 
    { 
        mt.seed(arg_seed);
    }

    void setRandomSeed() 
    { 
        setRandomSeed(std::random_device{}());
    }

    template<typename T>
    void Scramble(Pointset<T>& in)
    {
        std::vector<unsigned long long int> seeds(in.Ndim());
        for (unsigned int d = 0; d < in.Ndim(); d++)
            seeds[d] = mt();
        
        for (unsigned int i = 0; i < in.Npts(); i++)
        {
            for (unsigned int d = 0; d < in.Dim())
            {
                in[i][d] = owen(in[i][d], d, depth, seeds[d]);
            }
        }
    }

    template<typename T, typename D>
    void Scramble(const Pointset<T>& in, Pointset<D>& out)
    {
        out.Resize(in.Ndpts(), in.Ndim());

        std::vector<unsigned long long int> seeds(in.Ndim());
        for (unsigned int d = 0; d < in.Ndim(); d++)
            seeds[d] = mt();
        
        for (unsigned int i = 0; i < in.Npts(); i++)
        {
            for (unsigned int d = 0; d < in.Dim())
            {
                out[i][d] = convert<D>(owen(in[i][d], d, depth, seeds[d]));
            }
        }

    }
    
private:
    static IntegerType owen(
            IntegerType int i,
            unsigned int dim, 
            unsigned char depth,
            const unsigned long long int* seeds
        )
        {
            std::bitset<MAX_BITS> digitsBase2 = i;
            std::bitset<MAX_BITS> newDigits = i;

            RNG rng;
            for (unsigned int idigit = 0; idigit < depth; idigit++)
            {
                unsigned int indPermut = (1 << idigit) - 1 + (digitsBase2 >> (MAX_BITS - idigit)).to_ulong();
                unsigned long long int seed = seeds[dim] + indPermut;
                
                rng.seed(seed);
                int thisDigitPermut = rng.sample() & 1;
                newDigits[(MAX_BITS - 1) - idigit] = (thisDigitPermut ^ digitsBase2[(MAX_BITS - 1) - idigit]);
            }

            return RadicalInverse(newDigits.to_ulong());
        }

        static IntegerType ReverseBits(IntegerType n)
        {
            static_assert(MAX_BITS == 32 || MAX_BITS == 64);
            if constexpr(MAX_BITS == 32)
            {               
                n = ReverseBits32(n);
            }
            else if constexpr (MAX_BITS == 64)
            {
                IntegerType n0 = ReverseBits32((uint32_t) n);
                IntegerType n1 = ReverseBits32((uint32_t) (n >> 32));
                n = (n0 << 32) | n1;
            }

            return n;
        } 
    
        static uint32_t ReverseBits32(uint32_t n) 
        {
            n = (n << 16) | (n >> 16);
            n = ((n & 0x00ff00ff) << 8) | ((n & 0xff00ff00) >> 8);
            n = ((n & 0x0f0f0f0f) << 4) | ((n & 0xf0f0f0f0) >> 4);
            n = ((n & 0x33333333) << 2) | ((n & 0xcccccccc) >> 2);
            n = ((n & 0x55555555) << 1) | ((n & 0xaaaaaaaa) >> 1);
            return n;
        }

        template<typename T>
        static T convert(IntegerType x)
        {
            if constexpr (std::is_integral_v<T>)
            {
                return static_cast<T>(x);
            }
            else
            {
                T r = 0.0;
                T b = 0.5;
                while (x)
                {
                    r += (x & 1) * b;

                    b *= 0.5;
                    x >>= 1;
                }

                return r;
            }
        }
private:
    unsigned int depth;
    std::mt19937 mt;
};