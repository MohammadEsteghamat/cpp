#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

const double pi = 3.14159265359;
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
    int i{0};
    int error{0};
    State state{0.0, 0.0, 0.5 * pi};

    double v = 0.1;
    double delta = -0.5235;
    double dt = 0.1;

    ofstream outFile("trajectory.txt");
    while (true) {
        i++;
        updateState(state, v, delta, dt);
        outFile << state.x << " " << state.y << " " << state.theta << "\n";

        if(state.theta>1.57)delta =-0.5235;
        if(state.theta<-1.57)delta = 0.5235;

        if(i==2000)break;
    }


    outFile.close();

    cout << "Trajectory data saved to trajectory.txt" << endl;


    return 0;
}
