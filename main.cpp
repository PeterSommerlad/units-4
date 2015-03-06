#include "number.h"
#include "metric.h"

#include <iomanip>

using namespace std;
using namespace units;
using namespace units::metric;

u_double<Velocity> compute_velocity(u_double<Distance> dist, u_double<Time> duration) {
    return dist / duration;
}

int main(int argc, char const *argv[]) {
    auto v = 230.0 * kilogram * meter / (second * second);
    cout << v << endl;
    cout << v / volt << endl;
    return 0;
}
