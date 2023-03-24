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

#include <vector>
#include <random>
#include <string>

#include "../../externals/CLI11.hpp"
#include "../../pointsets/PointsetIO.hpp"
#include "../../pointsets/Pointset.hpp"

namespace utk
{
    struct SamplerArguments
    {
        static constexpr unsigned int ANY_DIM = 0;

        using Type = double;

        unsigned int N;
        unsigned int D;
        unsigned int M;
        long long unsigned int seed;
        std::string outFile;

        std::vector<Pointset<Type>> GetPointsets()
        {
            std::vector<Pointset<Type>> pts(M, Pointset<Type>(N, D));
            return pts;
        }

        bool WritePointsets(const std::vector<Pointset<Type>>& pts)
        {
            return write_text_pointsets(outFile, pts);
        }
    };

    inline SamplerArguments* add_arguments(
        CLI::App& app,
        unsigned int force_dim = SamplerArguments::ANY_DIM,
        bool seedable = true
    )
    {
        SamplerArguments* args = new SamplerArguments();
        args->seed = std::random_device{}();

        app.add_option("-n", args->N, "Number of points")->required();

        if (force_dim == 0)
            app.add_option("-d", args->D, "Dimensions")->required();
        else
            args->D = force_dim;

        if (seedable)
            app.add_option("-s,--seed", args->seed, "Seed (unspecified means 'random')");

        app.add_option("-m", args->M, "Number of pointsets")->default_val(1);
        app.add_option("-o,--out", args->outFile, "Output file")->default_val("out.dat");

        return args;
    }
};
