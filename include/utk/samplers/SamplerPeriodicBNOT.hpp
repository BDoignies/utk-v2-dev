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

#include <utk/utils/Pointset.hpp>
#include <PeriodicBNOT/BNOT.hpp>
#include <utk/utils/log.hpp>
#include <random>

namespace utk
{

class SamplerPeriodicBNOT
{
protected:
public:

	SamplerPeriodicBNOT(unsigned int i = 20, unsigned int ni = 20, double e = 0.2) : 
		its(i), nits(ni), epsilon(e)
	{ setRandomSeed(); }

    uint32_t GetDimension() const { return 2; }

	void setIterations(unsigned int i) { its = i; }
	void setNewtonIterations(unsigned int i) { nits = i; }
	void setEpsilon(double e) { epsilon = e; }

	void setRandomSeed( uint64_t arg_seed ) { m_mersenneTwister.seed(arg_seed); }
	void setRandomSeed() { m_mersenneTwister.seed(std::random_device{}()); }

	template<typename T>
	bool generateSamples(Pointset<T>& arg_pts, uint32_t N)
	{
		std::uniform_real_distribution<double> unif(0.0, 0.1);

		Scene bnot_scene;
		std::vector<Point> points;
		std::vector<FT> weights(N, 0.0);
		std::vector<FT> noise(N, 0.0);

		points.reserve(N);
		for (uint32_t i = 0; i < N; i++)
			points.push_back(Point(unif(m_mersenneTwister), unif(m_mersenneTwister)));
		
		bnot_scene.construct_triangulation(points, weights, noise);
		bnot_scene.optimize_all(0.0, 0.0, its, epsilon, nits, std::cout, true);

		points.clear();

		std::vector<Point> outPoints;
		bnot_scene.collect_visible_points(outPoints);

		arg_pts.Resize(outPoints.size(), 2);
        if (N != static_cast<uint32_t>(outPoints.size()))
            UTK_WARN("Sampler BoundedBNOT, returning {} samples when asked for {}", outPoints.size(), N);

		for (uint32_t i = 0; i < outPoints.size(); i++)
		{
			arg_pts[i][0] = outPoints[i].x() + 0.5;
			arg_pts[i][1] = outPoints[i].y() + 0.5;
		}

		return true;
	};

protected:
    std::mt19937 m_mersenneTwister;

	unsigned int its;
	unsigned int nits;
	double epsilon;
};

}
