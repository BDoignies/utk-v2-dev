#include "../src/IntegrationTest.hpp"
#include "../src/Integrands/GaussianIntegrands.hpp"

#include "Parser.hpp"

int main(int argc, char** argv)
{
    CLI::App app { "Gaussian Integration calculator" };
    
    std::string file;
    std::string sampler;
    utk::MetricArguments* margs = utk::add_arguments(app);
    
    app.add_option("-f,--file,--integrands", file, "Integrand File")->required();

    CLI11_PARSE(app, argc, argv);

    std::vector<utk::Pointset<double>> ptss = margs->GetAllPointsets();
    if (!utk::CheckPointsets(ptss))
        return 1;

    if (ptss.size() == 0)
        return 0;

    utk::IntegrationTest test;
    test.ReadDatabase<utk::GaussianIntegrand>(file, ptss[0].Ndim());

    auto reports = test.compute(ptss);
    
    auto& ostream = margs->GetOutputStream();
    for (const auto& report : reports)
        ostream << report.min << " " << report.mean << " " << report.var << " " << report.max << '\n';

    return 0;
}