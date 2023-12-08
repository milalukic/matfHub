#include "plotter.hpp"

int main() {
    Plotter *plt = new Plotter("sin");
    plt->plot_sin();
}