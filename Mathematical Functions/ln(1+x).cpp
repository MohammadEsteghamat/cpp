#include <iostream>

#include <iostream>

// Approximate ln(x) using a series expansion
double natural_log(double x, int max_iterations = 100) {
    if (x <= 0) {
        std::cerr << "Error: ln(x) is undefined for x <= 0\n";
        return -9999;  // Error value instead of NaN
    }

    // Normalize x close to 1 using ln(x) = ln(2^k * y) = k * ln(2) + ln(y)
    int k = 0;
    while (x > 2) {
        x /= 2;
        ++k;
    }
    while (x < 0.5) {
        x *= 2;
        --k;
    }

    // Use ln((1+y)/(1?y)) = 2(y + y^3/3 + y^5/5 + ...) where y = (x-1)/(x+1)
    double y = (x - 1) / (x + 1);
    double y_power = y;
    double result = 0.0;

    for (int n = 1; n <= max_iterations; n += 2) {
        result += y_power / n;
        y_power *= y * y;
    }

    return 2 * result + k * 0.69314718; // ln(2) ? 0.69314718
}

double ln_1_plus_x(double x, int max_iterations = 100) {
    if (x <= -1) {
        std::cerr << "Error: ln(1 + x) is undefined for x <= -1\n";
        return -9999;  // Error value instead of NaN
    }

    // For x > 1, use transformation: ln(1 + x) = ln(x) + ln(1 + 1/x)
    if (x > 1) {
        return ln_1_plus_x(1.0 / x, max_iterations) + natural_log(x);
    }

    // Use Taylor series for |x| < 1
    double result = 0.0;
    double term = x;

    for (int n = 1; n <= max_iterations; ++n) {
        result += term / n;
        term *= -x;
    }

    return result;
}

int main() {
    double  i = 0;

    for (i = -1;i < 30;i += 0.2) {
        double result = ln_1_plus_x(i);
        std::cout << "ln(1 + " << i << ") = " << result << std::endl;
    }

    return 0;
}

