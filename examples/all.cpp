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

#include <utk/samplers/SamplerSobol.hpp>

#include <utk/metrics/GeneralizedL2.hpp>
#include <utk/metrics/IntegrationTest.hpp>
#include <utk/metrics/Integrands/GaussianIntegrands.hpp>
#include <utk/metrics/Integrands/HeavisideIntegrands.hpp>

#include <vector>

constexpr static unsigned int nmin   =    16;     // 2^4
constexpr static unsigned int nmax   = 32769;     // 2^15 + 1
constexpr static unsigned int factor =     2;     // 
constexpr static unsigned int m      =    16;     // Number of pointsets to 
                                                  // generate for each size
constexpr static unsigned int nInt   = nmax * 2;  // N for integral computation
                                                  // Make sure it is way above nmax 
constexpr static unsigned int mInt   =  4096;     // Number of integrands

struct Evaluator
{
public:
    struct Eval
    {
        Eval& operator+=(const Eval& other)
        {
            gl2   += other.gl2;
            gauss += other.gauss;
            heavi += other.heavi;

            return *this;
        }

        Eval operator+(const Eval& other)
        {
            Eval ret = *this;
            ret += other;
            return ret;
        }

        Eval& operator/=(unsigned int N)
        {
            gl2 /= N;
            gauss /= N;
            heavi /= N;

            return *this;
        }

        Eval operator/(unsigned int N)
        {
            Eval ret = *this;
            ret /= N;
            return ret;
        }

        unsigned int N = 0;
        unsigned int d = 0;
        double gl2   = 0.;
        double gauss = 0.;
        double heavi = 0.;
    };

    Evaluator(long long int seed, unsigned int dim)
    {
        utk::Pointset<double> sobol;
        utk::SamplerSobol<uint32_t>(dim, 32).generateSamples(sobol, nInt);
        
        gauss.BuildDatabase<utk::GaussianIntegrand> ("tmp_gauss.txt", dim, mInt, seed, {}, sobol);
        heavi.BuildDatabase<utk::HeavisideIntegrand>("tmp_heavi.txt", dim, mInt, seed, {}, sobol);
    }

    Eval EvalPointset(const utk::Pointset<double>& pts) const
    {
        double gl2    = utk::GL2Discrepancy().compute(pts);
        double gaussi = gauss.compute(pts).mean;
        double heavii = heavi.compute(pts).mean; 

        return Eval { .N = pts.Npts(), .d = pts.Ndim(), .gl2 = gl2, .gauss = gaussi, .heavi = heavii };
    }

    utk::IntegrationTest  gauss;
    utk::IntegrationTest heavi;
};

template<typename T>
std::vector<Evaluator::Eval> TestSampler(unsigned int dim, const Evaluator& evaluator, T sampler)
{
    std::vector<Evaluator::Eval> ret;
    utk::Pointset<double> pts(nmax, dim);
    for (unsigned int i = nmin; i < nmax; i *= factor)
    {
        Evaluator::Eval eval {.N = i, .d = dim};
        for (unsigned int k = 0; k < m; k++)
        {
            sampler.generateSamples(pts, i);
            eval += evaluator.EvalPointset(pts);
        }

        ret.push_back(eval / m);
    }

    return ret;
}

template<typename... T>
std::pair<std::vector<const char*>, std::vector<std::vector<Evaluator::Eval>>> TestSamplers(
    unsigned int dim, 
    const Evaluator& evaluator, 
    const T&... samplers)
{
    std::vector<std::vector<Evaluator::Eval>> evals = { 
        { TestSampler(dim, evaluator, samplers.second)... } 
    };
    std::vector<const char*> names = { { samplers.first... } };

    return {names, evals};
}

void WriteTest(
    const char* out, 
    const std::vector<const char*>& names, 
    const std::vector<std::vector<Evaluator::Eval>>& evals)
{
    std::ofstream samplers_data { "evals.csv" };
    samplers_data << "name" << "," << "N" << "," << "dim" << "," << "metric" << "," << "value" << "\n";
    for (unsigned int i = 0; i < names.size(); i++)
    {
        for (unsigned int j = 0; j < evals[i].size(); j++)
        {
            samplers_data << names[i] << "," << evals[i][j].N << "," << evals[i][j].d << ",";
            samplers_data << "gl2" << "," << evals[i][j].gl2 << '\n';

            samplers_data << names[i] << "," << evals[i][j].N << "," << evals[i][j].d << ",";
            samplers_data << "gauss" << "," << evals[i][j].gauss << '\n';
            
            samplers_data << names[i] << "," << evals[i][j].N << "," << evals[i][j].d << ",";
            samplers_data << "heavi" << "," << evals[i][j].heavi << '\n';
        }
    }
    samplers_data.close();
}

int main(int argc, char** argv)
{ 
    Evaluator eval(20081976, 2);

    {
        auto [names, evals] = TestSamplers(2, eval,  
            std::make_pair("WN"     , utk::SamplerWhitenoise(2)),
            std::make_pair("Strat"  , utk::SamplerStratified(2)),
            std::make_pair("Grid"   , utk::SamplerRegularGrid(2)),
            std::make_pair("HexGrid", utk::SamplerHexagonalGrid()),
        );    
        WriteTest("classical.csv", names, evals);
    }

    return 0;
}
