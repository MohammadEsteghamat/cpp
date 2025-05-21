#include <stdio.h>


double A = 4.0;
double k = 7.0;
double b = 6.0;
double y1 = 0.25;
double y2 = 1.0;
double T = b * 16.0 / 15.0;

double exp(double x){
	int terms = 50;
	double result = 1.0;
    double term = 1.0;
    int i = 0;
    for( i = 1; i < terms+1 ; ++i){
    	term *= x/i;
    	result += term;
	}
	return result;
}

double g(double x,double A,double k,double b,double y1,double y2){
    double term1 = A * (1 / (1 + exp(-k * x)) - 1 / (1 + exp(-k * (x - b))));
    double term2 = y1 + (y2 - y1) * (1 / (1 + exp(-k * (x - b))));
    return term1 + term2;
}

double natural_log(double x, int max_iterations = 100) {
    int k = 0;
    while (x > 2) {
        x /= 2;
        ++k;
    }
    while (x < 0.5) {
        x *= 2;
        --k;
    }
    double y = (x - 1) / (x + 1);
    double y_power = y;
    double result = 0.0;

    for (int n = 1; n <= max_iterations; n += 2) {
        result += y_power / n;
        y_power *= y * y;
    }

    return 2 * result + k * 0.69314718; 
}

double ln_1_plus_x(double x, int max_iterations = 100) {
    if (x > 1) {
        return ln_1_plus_x(1.0 / x, max_iterations) + natural_log(x);
    }
    double result = 0.0;
    double term = x;
    for (int n = 1; n <= max_iterations; ++n) {
        result += term / n;
        term *= -x;
    }
    return result;
}

double sigmoid_integral(double a, double x) {
    return (1.0 / a) * ln_1_plus_x(exp(a * x));
}

double exact_integral_g(double A, double k, double b, double y1, double y2, double T) {
    double term1 = A * (
        sigmoid_integral(k, T)
        - sigmoid_integral(k, 0)
        - sigmoid_integral(k, T - b)
        + sigmoid_integral(k, -b)
    );

    double term2 = y1 * T + (y2 - y1) * (
        sigmoid_integral(k, T - b)
        - sigmoid_integral(k, -b)
    );

    return term1 + term2;
}

int main() {
	double exact = exact_integral_g(A, k, b, y1, y2, T);
	printf("Exact integral of g(x): %f\n", exact);
	return 0;
}
