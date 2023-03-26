#include "Parser.hpp"
#include "../src/SymmetricL2.hpp"

int main(int argc, char** argv)
{
    CLI::App app { "SymmetricL2 calculator" };
    utk::MetricArguments* margs = utk::add_arguments(app);

    CLI11_PARSE(app, argc, argv);

    std::vector<utk::Pointset<double>> ptss = margs->GetAllPointsets();
    utk::CheckPointsets(ptss);
    
    auto rslts = utk::SymmetricL2Discrepancy().compute(ptss);

    auto& ostream = margs->GetOutputStream();
    for (double rslt : rslts)
        ostream << rslt << '\n';

    delete margs;
}