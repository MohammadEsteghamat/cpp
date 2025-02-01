#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include "pid.h"
using namespace std;

const double pi = 3.14159265359;
const double maxSpeed = 0.5;                      // Maximum speed of the vehicle
const double maxSteeringAngle = 30.0 * pi / 180.0;  // Maximum steering angle in radians
const double L = 1.0;  // Length of the vehicle

struct State {
    double x;
    double y;
    double theta;

    State(double x_, double y_, double theta_) : x(x_), y(y_), theta(theta_) {}
};

// Update the vehicle's state using the bicycle model
void updateState(State& state, double v, double delta, double dt) {
    state.x += v * cos(state.theta) * dt;
    state.y += v * sin(state.theta) * dt;
    state.theta += (v / L) * tan(delta) * dt;
}

// Calculate the angle to the goal (desired steering angle)
double angleToGoal(double x, double y, double x_goal, double y_goal) {
    return atan2(y_goal - y, x_goal - x);
}

// Calculate the Euclidean distance to the goal
double distance(double x, double y, double x_goal, double y_goal) {
    return sqrt(pow(x_goal - x, 2) + pow(y_goal - y, 2));
}

// Normalize the angle to the range [-pi, pi]
double normalizeAngle(double angle) {
    while (angle > pi) angle -= 2 * pi;
    while (angle < -pi) angle += 2 * pi;
    return angle;
}

int main() {
    PID steeringPID(50, 0.1, 0.01, 0.01);  // Steering PID controller
    PID velocityPID(50, 0.5, 0.09, 0.46); // Velocity PID controller


    State state(0.0, 0.0, 0.0);  // Initial state (x, y, theta)

    const double dt = 0.01;       // Time step
    const double x_goal = -5.0;    // Goal position x
    const double y_goal = -0.0;    // Goal position y


    ofstream outFile("trajectory.txt");
    while (true) {
        // Calculate the target angle to the goal (recalculate at each step)
        double theta_goal = angleToGoal(state.x, state.y, x_goal, y_goal);

        // Calculate the control output from the PID controllers
        double delta = steeringPID.PID_control(normalizeAngle(theta_goal - state.theta));  // Steering control
        double v_control = velocityPID.PID_control(distance(state.x, state.y, x_goal, y_goal));  // Velocity control

        // Ensure the velocity and steering angle are within limits
        v_control = min(v_control, maxSpeed);  // Clamp the velocity to the maximum speed
        delta = min(max(delta, -maxSteeringAngle), maxSteeringAngle);  // Clamp the steering angle within the max range

        // Update the vehicle's state using the bicycle model
        updateState(state, v_control, delta, dt);

        outFile <<state.x << " " << state.y << "\n";

        // Stop if the vehicle is close to the goal (within 0.1 meters and angle difference is small)
        if (distance(state.x, state.y, x_goal, y_goal) < 0.1) {
            cout << "Reached the goal!" << endl;
            cout<<distance(state.x, state.y, x_goal, y_goal)<<endl;
            break;
        }
    }

    outFile.close();

    cout << "Trajectory data saved to trajectory.txt" << endl;

    return 0;
}
