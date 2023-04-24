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
#include <utk/utils/SamplerArgumentParser.hpp>
#include <utk/samplers/SamplerStep.hpp>

int main(int argc, char** argv)
{
    CLI::App app { "Step sampler" };
    utk::SamplerArguments* args = utk::add_arguments(app, 2);
    
    float criticalFrequency = 0.606f;
    float smoothing = 8.f;
    app.add_option("--criticalFreq", criticalFrequency, "Critical frequency")->default_val(criticalFrequency);
    app.add_option("--smoothing", smoothing, "Smoothing")->default_val(smoothing);
    
    CLI11_PARSE(app, argc, argv);
    
    std::vector<utk::Pointset<double>> pts = args->GetPointsets();
    
    utk::SamplerStep step(criticalFrequency, smoothing);
    step.setRandomSeed(args->seed);

    for (unsigned int i = 0; i < pts.size(); i++)
    {
        if(!step.generateSamples(pts[i], args->N))
        {
            std::cerr << "Sampler returned non-zero output" << std::endl; // No log here, must be visible whatsoever
            return 1;
        }
    }

    args->WritePointsets(pts);

    delete args;
    return 0;
}