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

	class SamplerCMJ
	{
	protected:
	public:

		SamplerCMJ() { setRandomSeed(); }
        
        unsigned int GetDimension() const { return 2; }

        void setRandomSeed( long unsigned int arg_seed ) { m_mersenneTwister.seed(arg_seed); }
        void setRandomSeed() { m_mersenneTwister.seed(std::random_device{}()); }

		template<typename T>
        bool generateSamples(Pointset<T>& arg_pts, unsigned int N)
        {
            const unsigned int n  = (unsigned int) std::round(std::sqrt(N));
            const double gridSize = 1. / (double)n;
            const double gridSqr  = gridSize * gridSize;

            if (N != (n * n))
                return false;
            
            std::uniform_real_distribution<T> dist(0, 1);
            arg_pts.Resize(N, 2);

            // "Canonical" arrangement
            for (unsigned int i = 0; i < n; i++)
            {
                for (unsigned int j = 0; j < n; j++)
                {
                    arg_pts[j + i * n][0] = i * gridSize + j * gridSqr;
                    arg_pts[j + i * n][1] = j * gridSize + i * gridSqr;
                }
            }

            // Shuffle on x
            for (unsigned int j = 0; j < n; j++)
            {
                const unsigned int k = static_cast<unsigned int>(j + dist(m_mersenneTwister) * (n - j));
                for (unsigned int i = 0; i < n; i++)
                {
                    std::swap(arg_pts[j * n + i][0], arg_pts[k * n + i][0]);
                }
            }
            
            // Shuffle on y
            for (unsigned int i = 0; i < n; i++)
            {
                const unsigned int k = static_cast<unsigned int>(i + dist(m_mersenneTwister) * (n - i));
                for (unsigned int j = 0; j < n; j++)
                {
                    std::swap(arg_pts[j * n + i][1], arg_pts[j * n + k][1]);
                }
            }

            return true;
        }

    protected:
        std::mt19937 m_mersenneTwister;
	};

} 