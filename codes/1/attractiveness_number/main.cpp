#include <iostream>
using std::cout;
using std::cin;


int attractiveness_number(int);

int main()
{
    int number1{0}, number2{0}, counter{0};
    cin >> number1 >> number2;

    if (number2 < number1) {
        int temp = number2;
        number2 = number1;
        number1 = temp;
    }

    for (int i = ++number1; i < number2; i++) {
        if (attractiveness_number(i) == 0) counter++;  \
    }

    cout << counter;
    return 0;
}


int attractiveness_number(int number) {
    int counter{0}, sum{0};


    while (number != 0) {
        sum += number % 10;
        number /= -10;
        counter++;
    }

    return (counter % 2 == 0) ? -sum : sum;
}