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
#include "../utils/RadicalInversion.hpp"
#include <random>
#include <vector>
#include <algorithm>

namespace utk
{
    // Owen scrambling where tree is partially stored
    
    template <typename IntegerType = uint32_t>
    class OwenTree
    {
    public:
        OwenTree() { }

        IntegerType StringToInt(const std::string& pattern)
        {
            IntegerType i = 0;
            IntegerType p = 1;
            for (auto it = pattern.crbegin(); it != pattern.crend(); ++it)
            {
                i += (*it == '1') * p;
                p <<= 1;
            }

            return i;
        }

        void setSeed(uint64_t s) { seed = s; }

        bool GetBitAt(IntegerType index)
        {
            if (index < permuts.size()) return permuts[index];
            
            RNG rng;
            rng.seed(seed + index);
            return rng.sample() & 1;
        }

        void SetBitPattern(std::string pattern, bool value = true)
        {
            std::vector<std::string> allPatterns;
            GetAllPatterns(0, pattern, allPatterns);

            for (const auto& pat : allPatterns)
            {
                IntegerType length = pat.size();
                
                IntegerType idx = (1 << length) - 1 + StringToInt(pat);
                if (permuts.size() <= idx)
                {
                    auto n = permuts.size();
                    permuts.resize(1 << (length + 1), false);
                }
                permuts[idx] = value;
            }
        }
    
    public:
        uint64_t seed;
        std::vector<bool> permuts;
    private:
        void GetAllPatterns(unsigned char idx, const std::string& pattern, std::vector<std::string>& allPatterns)
        {
            if (idx == pattern.size()) 
            {
                allPatterns.push_back(pattern);
                return;
            }
            if (pattern[idx] == '*') 
            {
                std::string newPattern = pattern;
                newPattern[idx] = '0'; GetAllPatterns(idx + 1, newPattern, allPatterns);
                newPattern[idx] = '1'; GetAllPatterns(idx + 1, newPattern, allPatterns);
            }
            else
            {
                GetAllPatterns(idx + 1, pattern, allPatterns);
            }
        }

    };

    template <typename IntegerType = uint32_t>
    class OwenTreeScrambling
    {
    public:
        static constexpr unsigned char MAX_BITS = (sizeof(IntegerType) * __CHAR_BIT__);
    
        OwenTreeScrambling(unsigned int dp = MAX_BITS) : 
            depth(dp)
        { }

        void setBitPattern(unsigned int d, const std::string& pattern, bool value = true)
        {
            if (trees.size() <= d)
            {
                trees.resize(d + 1);
            }
            trees[d].SetBitPattern(pattern, value);
        }

        void setOwenDepth(unsigned int dp)
        {
            if (dp < MAX_BITS)
                depth = dp;
            else
                depth = MAX_BITS;
        }

        void setRandomSeed(long unsigned int arg_seed) 
        { 
            std::cout << arg_seed << std::endl;
            mt.seed(arg_seed);
        }

        void setRandomSeed() 
        { 
            setRandomSeed(std::random_device{}());
        }

        template<typename T>
        void Scramble(Pointset<T>& in)
        {
            if (trees.size() <= in.Ndim())
            {
                trees.resize(in.Ndim() + 1);
            }

            std::vector<unsigned long long int> seeds(in.Ndim());
            for (unsigned int d = 0; d < in.Ndim(); d++)
                trees[d].setSeed(mt());
            
            for (unsigned int i = 0; i < in.Npts(); i++)
            {
                for (unsigned int d = 0; d < in.Ndim(); d++)
                {
                    in[i][d] = owen(in[i][d], d, depth);
                }
            }
        }

        template<typename T, typename D>
        void Scramble(const Pointset<T>& in, Pointset<D>& out)
        {
            out.Resize(in.Npts(), in.Ndim());
            if (trees.size() <= in.Ndim())
            {
                trees.resize(in.Ndim() + 1);
            }

            std::vector<unsigned long long int> seeds(in.Ndim());
            for (unsigned int d = 0; d < in.Ndim(); d++)
                trees[d].setSeed(mt());
            
            for (unsigned int i = 0; i < in.Npts(); i++)
            {
                for (unsigned int d = 0; d < in.Ndim(); d++)
                {
                    out[i][d] = convertFullRadicalInverseBase2<D>(owen(in[i][d], d, depth));
                }
            }
        }
        
    private:
        static IntegerType ConvertBitset(const std::bitset<MAX_BITS>& in)
        {
            if constexpr (sizeof(IntegerType) == 4)
                return in.to_ulong();
            else
                return in.to_ullong();
        }

        IntegerType owen(
            IntegerType i,
            unsigned int dim, 
            unsigned char depth
        )
        {
            std::bitset<MAX_BITS> digitsBase2 = i;
            std::bitset<MAX_BITS> newDigits = i;

            RNG rng;
            for (unsigned int idigit = 0; idigit < depth; idigit++)
            {
                IntegerType indPermut = (1 << idigit) - 1 + ConvertBitset(digitsBase2 >> (MAX_BITS - idigit));
                
                auto thisDigitPermut = trees[dim].GetBitAt(indPermut);
                newDigits[(MAX_BITS - 1) - idigit] = (thisDigitPermut ^ digitsBase2[(MAX_BITS - 1) - idigit]);
            }

            return ConvertBitset(newDigits);
        }
    private:
        unsigned int depth;
        std::vector<OwenTree<IntegerType>> trees;
        std::mt19937 mt;
    };
};