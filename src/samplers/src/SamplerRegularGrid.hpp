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
#pragma once

#include "../../pointsets/Pointset.hpp"
#include <random>
#include <cmath>

namespace utk
{

	class SamplerRegularGrid
	{
	protected:
	public:

		SamplerRegularGrid(unsigned int d) : D(d) { }
        
        void setDimension(unsigned int d) { D = d; }
        unsigned int GetDimension() const { return D; }
        
		template<typename T>
        bool generateSamples(Pointset<T>& arg_pts, unsigned int N)
        {
            const unsigned int n  = (unsigned int) std::round(pow((double) N, 1. / (double) D));
            const double gridSize = 1. / (double)n;

            if (N != static_cast<unsigned int>(std::round(std::pow(n, D))))
                return false;
            
            std::uniform_real_distribution<T> dist(0, 1);
            arg_pts.Resize(N, D);

            for (unsigned int i = 0; i < N; i++)
            {
                unsigned int coords = i;
                for (unsigned int d = 0; d < D; d++)
                {
                    const unsigned int cellStart = static_cast<unsigned int>(coords % n);
                    arg_pts[i][d] = cellStart * gridSize;           

                    coords /= n;         
                }
            }

            return true;
        }

    protected:
        unsigned int D;
	};

} 