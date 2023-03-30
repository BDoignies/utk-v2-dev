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

#include <CLI/App.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Config.hpp>

#include "../../logging/log.hpp"
#include "../../pointsets/PointsetIO.hpp"

#include <iomanip>
#include <vector>
#include <fstream>

namespace utk
{
    struct MetricArguments
    {
        std::string outFile;
        std::string inFile;

        std::ofstream outputFileStream;

        std::vector<Pointset<double>> GetAllPointsets()
        {
            std::ifstream file(inFile);

            std::vector<Pointset<double>> pointsets;
            while (file.good())
            {
                Pointset<double> pointset = read_text_pointset_stream<decltype(file), double>(file);
                if (pointset.Npts() > 0)
                    pointsets.push_back(pointset);
            }
            return pointsets;
        } 

        std::ostream& GetOutputStream()
        {
            if (outFile.empty())
            {
                std::cout << std::setprecision(17) << std::fixed;
                return std::cout;
            }
            
            outputFileStream.open(outFile);
            outputFileStream << std::setprecision(17) << std::fixed;
            return outputFileStream;
        }
    };

    MetricArguments* add_arguments(CLI::App& app)
    {
        MetricArguments* arguments = new MetricArguments;

        app.add_option("-i,--input", arguments->inFile, "Input file")->required()->check(CLI::ExistingFile);
        app.add_option("-o,--output", arguments->outFile, "Output file (empty is stdout)")->default_val("");

        return arguments;
    }

    template<typename T>
    bool CheckPointsets(const std::vector<Pointset<T>>& pts)
    {
        // Can't say it's wrong ...
        if (pts.size() == 0) 
        {
            UTK_WARN("No pointset read...", 0);
            return true;
        }

        bool isValid = true;
        uint32_t N = pts[0].Npts();
        uint32_t D = pts[0].Ndim();

        for (uint32_t i = 1; i < pts.size(); i++)
        {
            if (N != pts[i].Npts() || D != pts[i].Ndim())
            {
                UTK_WARN("Pointset shape mismatch: [0]: {}, {}, [{}]: {}, {}", N, D, pts[i].Npts(), pts[i].Ndim());
                isValid = false;
            }
        }
        return isValid;
    }
}