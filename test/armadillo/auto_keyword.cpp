/**
 * @file auto_keyword.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Test the auto keyword with armadillo.
 * @version 0.2.0
 * @date 2022-07-26
 * 
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 * 
 * Command line usage:
 *   g++ auto_keyword.cpp -I/opt/local/include -L/opt/local/lib -larmadillo -std=c++17
 * This is used on MacOS and the related path should be changed.
 * 
 * Findings:
 * It is best (and almost everywhere) to avoid the use of 'auto' keyword
 * when it is related to Armadillo.
 * Sometimes, it will not compile.
 * And some other time it will lead to wrong answers
 * even though there is no compilation error!
 * 
 */

#include <armadillo>
using namespace arma;
using std::operator""i;

int main(int argc, char* argv[]) {
    arma_rng::set_seed_random();
    cx_mat A = randn<cx_mat>(3, 4);
    cx_mat B = randu<cx_mat>(3, 4);
    A.print("A:");
    B.print("B:");
    auto A_copy = A;
    A_copy.print("A copy:");
    auto plus1 = A + 1;
    plus1.print("A + 1:");
    auto A_plus_B = A + B;
    A_plus_B.print("A + B");
    auto A_abs = abs(A);
    A_abs.print("abs(A)");
    auto A_abs_plus_B = A_abs + B;
    A_abs_plus_B.print("abs(A) + B:");
    cx_vec x = std::sqrt(0.5) * (randn<vec>(4) + 1.0i * randn<vec>(4)); // this should not be auto
    x.print("x:");
    auto A_times_x = A * x;
    A_times_x.print("A * x:");
    auto abs_A_times_x = abs(A) * x;
    abs_A_times_x.print("abs(A) * x");
    auto abs_A_times_abs_x = abs(A) * abs(x);
    abs_A_times_abs_x.print("abs(A) * abs(x)");
    abs(A * x).print("abs(A * x)");
    cx_vec y = randu<vec>(3) + 1.0i * randu<vec>(3); // this should not be auto
    x = solve(A, y.eval());
    x.print("x:");
    double diff = std::pow(norm(y - A * x, "fro") / norm(y, "fro"), 2);
    std::cout << "diff: " << diff << std::endl;
    return 0;
}
