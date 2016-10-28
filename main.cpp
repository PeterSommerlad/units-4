#include "metric.h"
#include "us.h"

using namespace std;
using namespace units;
using namespace units::metric;

area<double> get_area(dist<double> width, dist<double> height) {
    return width * height;
}

velocity<double> get_speed(acceleration<double> accel, time<double> elapsed) {
    return accel * elapsed;
}

frequency<double> get_frequency(time<double> x) {
    return scalar_double{1.0} / x;
}

int main(int argc, char const *argv[]) {
    cout << get_speed(9.8 * mps2, 10.0 * second) << endl;
    cout << get_frequency(100.0 * second) << endl;
}
