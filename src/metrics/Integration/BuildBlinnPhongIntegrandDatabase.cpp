/*
 * Coded by Hélène Perrier helene.perrier@liris.cnrs.fr
 * and Bastien Doignies bastien.doignies@liris.cnrs.fr 
 * and David Coeurjolly David.coeurjolly@liris.cnrs.fr
 *
 * Copyright (c) 2023 CNRS Université de Lyon
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
 * either expressed or implied, of the UTK project.
 */
#include <utk/metrics/IntegrationTest.hpp>
#include <utk/metrics/Integrands/BlinnPhong.hpp>
#include <utk/samplers/SamplerSobol.hpp>

#include <utk/utils/MetricsArgumentParser.hpp>

int main(int argc, char** argv)
{
    CLI::App app { "Integrand database builder" };
    
    std::string output;
    std::string target;
    uint32_t dimension = 2;
    uint32_t npts;
    uint32_t m;
    uint64_t seed;

    app.add_option("-o,--output", output, "Output file")->required();
    // app.add_option("-d,--dimension", dimension, "Dimension")->required();
    app.add_option("-n,--npts", npts, "Number of points for integral computation.")->required();
    app.add_option("-m", m, "Number of integrands to compute")->required();
    app.add_option("-s", seed, "Seed (0 means random)")->default_val(0);

    double pmin, amin;
    double pmax, amax;
    app.add_option("--pmin", pmin, "Min distance of position to the center of quad")->default_val(0.1);
    app.add_option("--pmax", pmax, "Max distance of position to the center of quad")->default_val(10.00);
    app.add_option("--amin", amin, "Min size of specular part")->default_val(0.0);
    app.add_option("--amax", amax, "Max size of specular part")->default_val(100.00);

    CLI11_PARSE(app, argc, argv);

    if (seed == 0)
        seed = std::random_device{}();

    utk::SamplerSobol<uint32_t> sampler(dimension);
    sampler.setRandomSeed(seed);

    utk::Pointset<double> pts;
    sampler.generateSamples(pts, npts);

    utk::IntegrationTest test;
    test.BuildDatabase<utk::BlinnPhong>(output, dimension, m, seed, 
        {
            {"pmin", pmin}, 
            {"pmin", pmax},
            {"alphamin", amin},
            {"alphamax", amax}
        }, 
    pts);

    return 0;
}