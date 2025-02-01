#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

struct State {
    double x;
    double y;
    double theta;
};

void updateState(State& state, double v, double delta, double dt) {
    double L = 1.0;
    state.x += v * cos(state.theta) * dt;
    state.y += v * sin(state.theta) * dt;
    state.theta += (v / L) * tan(delta) * dt;
}

int main() {
    const double dt = 0.01;
    const int numSteps = 500;

    State state = {0.0, 0.0, 0.0};

    double v, delta;
    cout << "Enter velocity (m/s): ";
    cin >> v;
    cout << "Enter steering angle (radians): ";
    cin >> delta;


    ofstream outFile("trajectory.txt");
    for (int i = 0; i < numSteps; i++) {
        if(i%100==0){
            system("cls");
            cout << "Enter velocity (m/s): ";
            cin >> v;
            cout << "Enter steering angle (radians): ";
            cin >> delta;
        }
        updateState(state, v, delta, dt);
        outFile << state.x << " " << state.y << " " << state.theta << "\n";
    }

    outFile.close();

    cout << "Trajectory data saved to trajectory.txt" << endl;

    return 0;
}
