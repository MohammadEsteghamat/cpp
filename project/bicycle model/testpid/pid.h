#ifndef PID_H_INCLUDED
#define PID_H_INCLUDED
class PID {
    private:
    double kp, ki, kd;          // PID coefficients
    double sum{0};               // Sum for integral calculation
    double derivative{0};        // Derivative value
    int sizeArr;                 // Size of the error array
    double* arr;                 // Array to store errors
    int index{0};                // Current index in the array
    // Function to calculate the integral term
    void Integral() {
        sum = 0;
        for (int i = 0; i < sizeArr; i++) {
            sum += arr[i];          // Sum all errors in the array
        }
    }

    // Function to calculate the derivative term
    void Derivative() {
        if (index == 0) {
            derivative = arr[0] - arr[sizeArr - 1];  // Handle circular buffer condition
        } else {
            derivative = arr[index] - arr[index - 1];  // Standard case
        }
    }
    public:
    // Constructor: initializes the PID controller
    PID(int Size, double p, double i, double d) : kp(p), ki(i), kd(d), sizeArr(Size) {
        arr = new double[sizeArr];  // Allocate memory for the error array
        for (int i = 0; i < sizeArr; i++) {
            arr[i] = 0;            // Initialize the array elements to 0
        }
    }




    // Function to calculate the PID control output
    double PID_control(double error) {
        arr[index] = error;        // Store the current error
        Integral();                // Update the integral term
        Derivative();              // Update the derivative term
        double output = kp * error + ki * sum + kd * derivative;  // PID formula
        index = (index + 1) % sizeArr;  // Move to the next index, using modulo for circular buffer
        return output;             // Return the control output
    }

    // Destructor: releases dynamically allocated memory
    ~PID() {
        delete[] arr;
    }
};


#endif // PID_H_INCLUDED
