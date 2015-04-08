#include "metric.h"
#include "us.h"

using namespace std;
using namespace units;
using namespace units::metric;


int main(int argc, char const *argv[]) {
    cout << 1.0 * us::furlong / fortnight << endl;
    cout << 1.0 * us::furlong / us::mile << endl;

}
