#include "Parser.hpp"
#include "../src/Spectrum.hpp"

int main(int argc, char** argv)
{
    CLI::App app { "Spectrum calculator" };
    utk::MetricArguments* margs = utk::add_arguments(app);

    uint32_t res = 0;
    bool cancelDc = false; 

    app.add_option("-r,--res", res, "Spectrum resolution (0 means automatic)")->default_val(res);
    app.add_flag("--canceldc", cancelDc, "When set, cancel the DC peak")->default_val(cancelDc);

    CLI11_PARSE(app, argc, argv);

    std::vector<utk::Pointset<double>> ptss = margs->GetAllPointsets();
    auto rslts = utk::Spectrum(res, cancelDc).compute(ptss);

    auto& ostream = margs->GetOutputStream();
    for (auto rslt : rslts)
        ostream << rslt << '\n';

    delete margs;
}