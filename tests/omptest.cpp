// OpenMP Test

#include <iostream>

int main(int argc, char* argv[])
{
    double ans1 = 0.0;
    double ans2 = 0.0;

    std::cout << "No OpenMP" << std::endl;
    {
        for (int i = 0; i < 1000000000; ++i) {
            ans1 += 2.0;
        }
    }
    std::cout << "ans1 = " << ans1 << std::endl;

    std::cout << "OpenMP" << std::endl;
    #pragma omp parallel
    {
        #pragma omp for reduction(+:ans2)
        for (int i = 0; i < 1000000000; ++i) {
            ans2 += 2.0;
        }
    }
    std::cout << "ans2 = " << ans2 << std::endl;

    return 0;
}
