#include "Parser.hpp"
#include "../src/PCF.hpp"

int main(int argc, char** argv)
{
    CLI::App app { "PCF calculator" };
    utk::MetricArguments* margs = utk::add_arguments(app);

    bool toroidal = false;
    uint32_t bins = 200;
    double rmin = 0.01;
    double rmax = 0.15;
    double smoothing = 0.001;

    app.add_flag("-t,--toroidal", toroidal, "When set, use toroidal distance")->default_val(toroidal);
    app.add_option("-b,--nbbins", bins, "Number of bins")->default_val(bins);
    app.add_option("--rmin", rmin, "Minimal radius to consider")->default_val(rmin);
    app.add_option("--rmax", rmax, "Maximal radius to consider")->default_val(rmin);
    app.add_option("-s,--smoothin", smoothing, "Smoothing value")->default_val(smoothing);

    CLI11_PARSE(app, argc, argv);

    std::vector<utk::Pointset<double>> ptss = margs->GetAllPointsets();
    if (!utk::CheckPointsets(ptss))
        return 1;
        
    auto rslts = utk::PCF(toroidal, rmin, rmax, bins, smoothing).compute(ptss);

    auto& ostream = margs->GetOutputStream();
    for (auto rslt : rslts)
        ostream << rslt << '\n';

    delete margs;
}