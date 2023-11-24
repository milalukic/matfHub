#include <armadillo>
#include <iostream>

int main() {
    arma::mat A(3, 3, arma::fill::randu);
    arma::mat B(3, 3, arma::fill::randu);

    std::cout << A << std::endl << B << std::endl;

    std::cout << A*B << std::endl;
}
