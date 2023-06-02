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