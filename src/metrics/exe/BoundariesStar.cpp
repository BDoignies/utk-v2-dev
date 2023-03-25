#include "Parser.hpp"
#include "../src/BoundariesStarDiscrepancy.hpp"

int main(int argc, char** argv)
{
    CLI::App app { "BoundariesStarDiscrepancy calculator" };
    utk::MetricArguments* margs = utk::add_arguments(app);

    CLI11_PARSE(app, argc, argv);

    std::vector<utk::Pointset<double>> ptss = margs->GetAllPointsets();
    auto rslts = utk::BoundariesStarDiscrepancy().compute(ptss);

    auto& ostream = margs->GetOutputStream();
    for (auto&[min, max] : rslts)
        ostream << min << " " << max << '\n';

    delete margs;
}