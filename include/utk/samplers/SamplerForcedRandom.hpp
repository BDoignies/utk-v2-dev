/**
 * Code from
 * 
 * Cornel, D., Tobler, R. F., Sakai, H., Luksch, C., & Wimmer, M. (2017). 
 * Forced Random Sampling: fast generation of importance-guided blue-noise samples. 
 * The Visual Computer, 1-11.
 * https://www.researchgate.net/publication/317002694_Forced_Random_Sampling_Reference_Implementation
 * 
 *
 *
 *This is free and unencumbered software released into the public domain.
 
 Anyone is free to copy, modify, publish, use, compile, sell, or
 distribute this software, either in source code form or as a compiled
 binary, for any purpose, commercial or non-commercial, and by any
 means.
 
 In jurisdictions that recognize copyright laws, the author or authors
 of this software dedicate any and all copyright interest in the
 software to the public domain. We make this dedication for the benefit
 of the public at large and to the detriment of our heirs and
 successors. We intend this dedication to be an overt act of
 relinquishment in perpetuity of all present and future rights to this
 software under copyright law.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
 
 For more information, please refer to <http://unlicense.org/>
 */
#pragma once

#include "ForcedRandom/src/ForcedRandomSampling/main.hpp"
#include <utk/utils/Pointset.hpp>
#include <random>

namespace utk
{

class SamplerForcedRandom
{
protected:
public:

	SamplerForcedRandom(
		uint32_t sparsity_ = 8, 
		uint32_t matrixSize_ = 512
	) : sparsity(sparsity_) { setMatrixSize(matrixSize_); }

    uint32_t GetDimension() const { return 2; }

	void setMatrixSparsity(int sparsity_) 
	{
		sparsity = sparsity_;
	}

	void setMatrixSize(int matrixSize_)
	{
		matrixSize = matrixSize_;
		if (matrixSize != 512 && matrixSize != 1024 && matrixSize != 2048)
		{
			matrixSize = 512;
		}
	}

	template<typename T>
	bool generateSamples(Pointset<T>& arg_pts, uint32_t N)
	{
        forcedrandom::n = N;
		forcedrandom::sigma = sparsity;
		forcedrandom::w_M = matrixSize;

		std::ostringstream matrixPathSStr;
		matrixPathSStr << UTK_DATA_PATH << "/ForcedRandom/" << matrixSize << "x" << matrixSize << ".matrix";

		forcedrandom::LoadMatrix(matrixPathSStr.str());
		forcedrandom::s_I = V2i(w_I, h_I);
		forcedrandom::GenerateImportanceMap();

		forcedrandom::Setup();
		forcedrandom::CreateBlockwiseSortedList();

		std::vector<V2d> samples = forcedrandom::GenerateSamples();
		arg_pts.Resize(N, 2);
        for(uint32_t i = 0; i < samples.size(); i++)
		{
			arg_pts[i][0] = samples[i].x;
			arg_pts[i][1] = samples[i].y;
		}
		return true;
	};

protected:
	int sparsity;
	int matrixSize;
};

}
