#include <utk/metrics/IntegrationTest.hpp>
#include <utk/metrics/Integrands/GaussianIntegrands.hpp>
#include <utk/samplers/SamplerSobol.hpp>

#include <utk/utils/MetricsArgumentParser.hpp>

int main(int argc, char** argv)
{
    CLI::App app { "Integrand database builder" };
    
    std::string output;
    std::string target;
    uint32_t dimension;
    uint32_t npts;
    uint32_t m;
    uint64_t seed;

    app.add_option("-o,--output", output, "Output file")->required();
    app.add_option("-d,--dimension", dimension, "Dimension")->required();
    app.add_option("-n,--npts", npts, "Number of points for integral computation.")->required();
    app.add_option("-m", m, "Number of integrands to compute")->required();
    app.add_option("-s", seed, "Seed (0 means random)")->default_val(0);

    double sigmamin;
    double sigmamax;
    app.add_option("--sigmamin", sigmamin, "Min value for variance")->default_val(0.01);
    app.add_option("--sigmamax", sigmamax, "Max value for variance")->default_val(1.00);

    CLI11_PARSE(app, argc, argv);

    if (seed == 0)
        seed = std::random_device{}();

    utk::SamplerSobol<uint32_t> sampler(dimension);
    sampler.setRandomSeed(seed);

    utk::Pointset<double> pts;
    sampler.generateSamples(pts, npts);

    utk::IntegrationTest test;
    test.BuildDatabase<utk::GaussianIntegrand>(output, dimension, m, seed, {{"smin", sigmamin}, {"smax", sigmamax}}, pts);

    return 0;
}