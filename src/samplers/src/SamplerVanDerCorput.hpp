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
#include <cmath>

namespace utk
{

	class SamplerVanDerCorput
	{
	protected:

	public:

		SamplerVanDerCorput() { }

		unsigned int GetDimension() const { return 1; }

		template<typename T>
		bool generateSamples(Pointset<T>& arg_pts, unsigned int N)
		{
            arg_pts.Resize(N, 1);
			if constexpr (std::is_integral_v<T>)
			{
				for(unsigned int i = 0; i < N; i++)
				{
					arg_pts[i][0] = radicalInverse_VdC(i);
				}
			}
			else
			{
				for(unsigned int i = 0; i < N; i++)
				{
					arg_pts[i][0] = radicalInverse_VdC(i) * 0x1p-32;
				}
			}
			
			return true;
		};

	protected:
		
        //Code from http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
        static unsigned int radicalInverse_VdC(uint32_t bits) 
        {
            bits = (bits << 16u) | (bits >> 16u);
            bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
            bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
            bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
            bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
            return bits;
    	}

        static unsigned int radicalInverse_VdC64(uint64_t bits) 
        {
            uint64_t n0 = radicalInverse_VdC((uint32_t) bits);
            uint64_t n1 = radicalInverse_VdC((uint32_t) (bits >> 32));
            return (n0 << 32) | n1;
    	}
};

} //end namespace utk