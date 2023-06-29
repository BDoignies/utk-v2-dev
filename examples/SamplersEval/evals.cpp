#include "Config.hpp"
#include "Eval.hpp"

#include <utk/samplers/SamplerAAPatterns.hpp>
#include <utk/samplers/SamplerART.hpp>
#include <utk/samplers/SamplerCMJ.hpp>
#include <utk/samplers/SamplerDartThrowing.hpp>
#include <utk/samplers/SamplerFastPoisson.hpp>
#include <utk/samplers/SamplerFattal.hpp>
#include <utk/samplers/SamplerFaure.hpp>
#include <utk/samplers/SamplerForcedRandom.hpp>
#include <utk/samplers/SamplerHalton.hpp>
#include <utk/samplers/SamplerHammersley.hpp>
#include <utk/samplers/SamplerHexagonalGrid.hpp>
#include <utk/samplers/SamplerKorobov.hpp>
#include <utk/samplers/SamplerKronecker.hpp>
#include <utk/samplers/SamplerLDBN.hpp>
#include <utk/samplers/SamplerLutLDBN.hpp>
#include <utk/samplers/SamplerNiederreiter.hpp>
#include <utk/samplers/SamplerNRooks.hpp>
#include <utk/samplers/SamplerPenrose.hpp>
#include <utk/samplers/SamplerPMJ.hpp>
#include <utk/samplers/SamplerProjectiveBlueNoise.hpp>
#include <utk/samplers/SamplerR1.hpp>
#include <utk/samplers/SamplerRegularGrid.hpp>
#include <utk/samplers/SamplerSinglePeak.hpp>
#include <utk/samplers/SamplerSobol.hpp>
#include <utk/samplers/SamplerStep.hpp>
#include <utk/samplers/SamplerStratified.hpp>
#include <utk/samplers/SamplerVanDerCorput.hpp>
#include <utk/samplers/SamplerWhitenoise.hpp>

#include <vector>

template<typename T>
std::vector<Evaluator::Eval> TestSampler(const Config& config, const Evaluator& evaluator, T sampler)
{
    std::vector<Evaluator::Eval> ret;
    utk::Pointset<double> pts(config.nmax, config.dim);
    for (unsigned int i = config.nmin; i < config.nmax; i *= config.factor)
    {
        Evaluator::Eval eval {.N = i, .d = config.dim};
        for (unsigned int k = 0; k < config.m; k++)
        {
            sampler.generateSamples(pts, i);
            eval += evaluator.EvalPointset(pts);
        }

        ret.push_back(eval / config.m);
    }

    return ret;
}

template<typename... T>
std::pair<std::vector<const char*>, std::vector<std::vector<Evaluator::Eval>>> TestSamplers(
    const Config& config, 
    const Evaluator& evaluator, 
    const T&... samplers)
{
    std::vector<std::vector<Evaluator::Eval>> evals = { 
        { TestSampler(config, evaluator, samplers.second)... } 
    };
    std::vector<const char*> names = { { samplers.first... } };

    return {names, evals};
}

int main(int argc, char** argv)
{ 
    Config config;
    Evaluator eval(config);

    {
        auto [names, evals] = TestSamplers(config, eval,  
            // Classical samplers
            std::make_pair("WN"     , utk::SamplerWhitenoise(2)),
            std::make_pair("Strat"  , utk::SamplerStratified(2)),
            std::make_pair("Grid"   , utk::SamplerRegularGrid(2)),
            std::make_pair("HexGrid", utk::SamplerHexagonalGrid()),

            // Blue noise
            std::make_pair("Poisson-to"  , utk::SamplerDartThrowing(2, true)),
            std::make_pair("Poisson-eu"  , utk::SamplerDartThrowing(2, true, false)),
            std::make_pair("Poisson-fast", utk::SamplerFastPoisson()),
            std::make_pair("Poisson-proj", utk::SamplerProjectiveBlueNoise(2)),
            std::make_pair("Step"        , utk::SamplerStep()),
            std::make_pair("SinglePeak"  , utk::SamplerSinglePeak()),
            std::make_pair("Fattal"      , utk::SamplerFattal()),
            std::make_pair("ForcedRandom", utk::SamplerForcedRandom()),

            // Low discrepancy
            std::make_pair("Hammersley"  , utk::SamplerHammersley()), 
            std::make_pair("Halton"      , utk::SamplerHalton(2)),
            std::make_pair("LutLDBN"     , utk::SamplerLutLDBN()),
            std::make_pair("Niederrieter", utk::SamplerNiederreiter(2)),
            std::make_pair("Faure"       , utk::SamplerFaure(2)),
            std::make_pair("PMJ"         , utk::SamplerPMJ()),
            std::make_pair("Sobol"       , utk::SamplerSobol<uint32_t>(2,  0)),
            std::make_pair("Owen32"      , utk::SamplerSobol<uint32_t>(2, 32)),

            // Lattices
            std::make_pair("R1"     , utk::SamplerRank1()),
            std::make_pair("R1-fibo", utk::SamplerRank1(false, true)),
            std::make_pair("Korobov", utk::SamplerKorobov(2)),
            std::make_pair("Kron"   , utk::SamplerKronecker(2)),
            
            // Tiles
            std::make_pair("ART"    , utk::SamplerART()),
            std::make_pair("Penrose", utk::SamplerPenrose()),
            
            // Misc
            std::make_pair("AAPatterns", utk::SamplerAAPatterns()),
            std::make_pair("CMJ"       , utk::SamplerCMJ()),
            std::make_pair("Nrooks"    , utk::SamplerNRooks(2))

        );    
        WriteEval("all.csv", names, evals);
    }
    return 0;
}
