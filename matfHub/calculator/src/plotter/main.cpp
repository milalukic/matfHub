#include "./src/plotter.hpp"

using namespace matplot;
//TODO zasto nije radilo
int main() {
    Plotter *plt = new Plotter("sin");

    double a, b, n;
    std::cin >> a >> b >> n;

    auto x = plt->linSpace(a, b, n);
    auto y = matplot::linspace(a, b, n);

    plt->plotSin(x);

    // std::cout << "Press Enter to continue" << std::endl;

    //TODO smisli bolje
    std::cin >> a;

    return 0;


    // plt->plotSin(x);

}