#include "Parser.hpp"
#include "../src/RadialSpectrum.hpp"

int main(int argc, char** argv)
{
    CLI::App app { "RadialSpectrum calculator" };
    utk::MetricArguments* margs = utk::add_arguments(app);
    
    uint32_t res = 0;
    bool cancelDc = false; 
    double scale = 0.5;
    uint32_t bins = 0;

    app.add_option("-r,--res", res, "Spectrum resolution (0 means automatic)")->default_val(res);
    app.add_flag("--canceldc", cancelDc, "When set, cancel the DC peak")->default_val(cancelDc);
    app.add_option("-s,--scale", scale, "Scale for distance to origin")->default_val(0.5);
    app.add_option("-b,--bins", bins, "Number of bins (0 means automatic)")->default_val(bins);

    CLI11_PARSE(app, argc, argv);

    std::vector<utk::Pointset<double>> ptss = margs->GetAllPointsets();
    auto rslts = utk::RadialSpectrum(bins, scale, res, cancelDc).compute(ptss);

    auto& ostream = margs->GetOutputStream();
    for (auto rslt : rslts)
        ostream << rslt << '\n';

    delete margs;
}