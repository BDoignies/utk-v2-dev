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

namespace utk
{ 
    class CranleyPattersonScrambling
    {
    public:
        CranleyPattersonScrambling(double md = 1.0, double ds = 1.0) : 
            maxDispacement(md), domainSize(ds)
        { }

        void setMaxDispacement(double mv = 1.0) 
        {
            maxDispacement = mv;
        }

        void setDomainSize(double ds = 1.0)
        {
            domainSize = ds;
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
            std::uniform_real_distribution<T> dist(-maxDispacement, maxDispacement);
            std::vector<T> shift(in.Ndim());
            
            for (unsigned int d = 0; d < in.Ndim(); d++)
                shift[d] = dist(mt);

            for (unsigned int i = 0; i < in.Npts(); i++)
            {
                for (unsigned int d = 0; d < in.Ndim(); d++)
                {
                    in[i][d] = fmodf64(in[i][d] + shift[d], domainSize);
                }
            }
        }

        template<typename T>
        void Scramble(const Pointset<T>& in, Pointset<T>& out)
        {
            std::uniform_real_distribution<T> dist(-maxDispacement, maxDispacement);
            std::vector<T> shift(in.Ndim());
            
            out.Resize(in.Npts(), in.Ndim());

            for (unsigned int d = 0; d < in.Ndim(); d++)
                shift[d] = dist(mt);

            for (unsigned int i = 0; i < in.Npts(); i++)
            {
                for (unsigned int d = 0; d < in.Ndim(); d++)
                {
                    out[i][d] = fmodf64(in[i][d] + shift[d], domainSize);
                }
            }
        }

    private:
        double domainSize;
        double maxDispacement;
        std::mt19937 mt;
    };
}