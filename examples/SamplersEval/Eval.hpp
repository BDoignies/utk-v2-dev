#pragma once

#include "Config.hpp"
#include <vector>

#include <utk/samplers/SamplerSobol.hpp>
#include <utk/metrics/GeneralizedL2.hpp>
#include <utk/metrics/IntegrationTest.hpp>
#include <utk/metrics/Integrands/GaussianIntegrands.hpp>
#include <utk/metrics/Integrands/HeavisideIntegrands.hpp>

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

    Evaluator(const Config& config)
    {
        utk::Pointset<double> sobol;
        utk::SamplerSobol<uint32_t>(config.dim, 32).generateSamples(sobol, config.nInt);
        
        gauss.BuildDatabase<utk::GaussianIntegrand> ("tmp_gauss.txt", config.dim, config.mInt, config.seed, {}, sobol);
        heavi.BuildDatabase<utk::HeavisideIntegrand>("tmp_heavi.txt", config.dim, config.mInt, config.seed, {}, sobol);
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

inline void WriteEval(
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