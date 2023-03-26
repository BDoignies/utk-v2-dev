#include "../src/IntegrationTest.hpp"
#include "../src/Integrands/GaussianIntegrands.hpp"
#include "../../samplers/src/SamplerRegularGrid.hpp"

int main()
{
    utk::IntegrationTest test;

    utk::Pointset<double> pts;
    utk::SamplerRegularGrid(2).generateSamples(pts, 1024);
    utk::GenerationParameter params = {
        {"smin", 0.01}, {"smax", 1.0}
    };
    test.BuildDatabase<utk::GaussianIntegrand>("test.dat", 3, 1024, 0, params, pts);
}