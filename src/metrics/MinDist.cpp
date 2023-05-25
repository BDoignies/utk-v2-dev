#include <utk/utils/MetricsArgumentParser.hpp>
#include <utk/metrics/MinDist.hpp>

int main(int argc, char** argv)
{
    CLI::App app { "MinDist calculator" };
    utk::MetricArguments* margs = utk::add_arguments(app);
    
    bool euclidean = false;
    app.add_flag("--euclidean", euclidean, "Use euclidean distance (default is wrap around)")->default_val(euclidean);
    
    CLI11_PARSE(app, argc, argv);
    
    std::vector<utk::Pointset<double>> ptss = margs->GetAllPointsets();
    utk::CheckPointsets(ptss);
    
    auto rslts = utk::MinDist(euclidean).compute(ptss);

    auto& ostream = margs->GetOutputStream();
    for (double rslt : rslts)
        ostream << rslt << '\n';

    delete margs;
}