#include "Parser.hpp"
#include "../src/L2.hpp"

int main(int argc, char** argv)
{
    CLI::App app { "L2 discrepancy calculator" };
    utk::MetricArguments* margs = utk::add_arguments(app);

    CLI11_PARSE(app, argc, argv);

    std::vector<utk::Pointset<double>> ptss = margs->GetAllPointsets();
    auto rslts = utk::L2Discrepancy().compute(ptss);

    auto& ostream = margs->GetOutputStream();
    for (double rslt : rslts)
        ostream << rslt << '\n';

    delete margs;
}