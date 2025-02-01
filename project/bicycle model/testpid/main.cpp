#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <limits>
#include "pid.h"  // Assuming this includes the PID class from earlier
using namespace std;

const double pi = 3.14159265359;
const double maxSpeed = 0.5;                      // Maximum speed of the vehicle
const double maxSteeringAngle = 30.0 * pi / 180.0;  // Maximum steering angle in radians
const double L = 1.0;  // Length of the vehicle

struct State {
    double x;
    double y;
    double theta;

    // Constructor to initialize the state
    State(double x_, double y_, double theta_) : x(x_), y(y_), theta(theta_) {}
};

// Update the vehicle's state using the bicycle model
void updateState(State& state, double v, double delta, double dt) {
    state.x += v * cos(state.theta) * dt;               // Update x position
    state.y += v * sin(state.theta) * dt;               // Update y position
    state.theta += (v / L) * tan(delta) * dt;           // Update orientation (theta)
}

// Calculate the angle to the goal (desired steering angle)
double angleToGoal(double x, double y, double x_goal, double y_goal) {
    return atan2(y_goal - y, x_goal - x);  // Return the angle between the current position and the goal
}

// Calculate the Euclidean distance to the goal
double distance(double x, double y, double x_goal, double y_goal) {
    return sqrt(pow(x_goal - x, 2) + pow(y_goal - y, 2));  // Return the distance to the goal
}

// Normalize the angle to the range [-pi, pi]
double normalizeAngle(double angle) {
    while (angle > pi) angle -= 2 * pi;
    while (angle < -pi) angle += 2 * pi;
    return angle;
}

double testPID(double Kp1, double Ki1, double Kd1,double Kp, double Ki, double Kd, double dt, double x_goal, double y_goal) {
    PID steeringPID(50, Kp, Ki, Kd);
    PID velocityPID(50, Kp1, Ki1, Kd1); // Keeping velocity PID constant for simplicity
    State state(0.0, 0.0, 0.0);

    double total_distance_error = 0.0;
    int i;
    for ( i = 0; i < 1000; i++) {
        double theta_goal = angleToGoal(state.x, state.y, x_goal, y_goal);
        double delta = steeringPID.PID_control(normalizeAngle(theta_goal - state.theta));
        double v_control = velocityPID.PID_control(distance(state.x, state.y, x_goal, y_goal));

        v_control = min(v_control, maxSpeed);
        delta = min(max(delta, -maxSteeringAngle), maxSteeringAngle);

        updateState(state, v_control, delta, dt);

        total_distance_error = distance(state.x, state.y, x_goal, y_goal);

        if (distance(state.x, state.y, x_goal, y_goal) < 0.1) break;
    }

    return i;
}

int main() {
    double bestKp = 0, bestKi = 0, bestKd = 0;
    double bestKp1 = 0, bestKi1 = 0, bestKd1 = 0;
    double minError = std::numeric_limits<double>::max();

    for (double Kp1 = 0.1; Kp1 <= 2.0; Kp1 += 0.1) {
        for (double Ki1 = 0.0; Ki1 <= 0.1; Ki1 += 0.01) {
            for (double Kd1 = 0.01; Kd1 <= 0.5; Kd1 += 0.05){
                for (double Kp = 0.1; Kp <= 2.0; Kp += 0.1) {
                    for (double Ki = 0.0; Ki <= 0.1; Ki += 0.01) {
                        for (double Kd = 0.01; Kd <= 0.5; Kd += 0.05) {
                            double error = testPID(Kp, Ki, Kd,Kp1, Ki1, Kd1, 0.1, 3.0, 4.0);
                            if (error < minError) {
                                minError = error;
                                bestKp = Kp;
                                bestKi = Ki;
                                bestKd = Kd;
                                bestKp1 = Kp1;
                                bestKi1 = Ki1;
                                bestKd1 = Kd1;
                            }
                        }
                    }
                }
            }
        }
    }

    std::cout << "Best Kp: " << bestKp << ", Best Ki: " << bestKi << ", Best Kd: " << bestKd << std::endl;
    std::cout << "Best Kp1: " << bestKp1 << ", Best Ki1: " << bestKi1 << ", Best Kd1: " << bestKd1 << std::endl;
    cout<<minError;

    ofstream outFile("trajectory.txt");
    outFile << "Best Kp: " << bestKp << ", Best Ki: " << bestKi << ", Best Kd: " << bestKd
    << "Best Kp1: " << bestKp1 << ", Best Ki1: " << bestKi1 << ", Best Kd1: " << bestKd1 ;

    outFile.close();
    return 0;
}

