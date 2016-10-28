#ifndef UNITS_IMPERIAL_H
#define UNITS_IMPERIAL_H

#include "metric.h"

namespace units {
namespace imperial {

// Length

auto yard    = std::ratio<9144, 10000>{} * meter;
auto foot    = std::ratio<1, 3>{} * yard;
auto inch    = std::ratio<1, 12>{} * foot;
auto chain   = std::ratio<22>{} * yard;
auto furlong = std::ratio<10>{} * chain;
auto mile    = std::ratio<8>{} * furlong;
auto league  = std::ratio<3>{} * mile;

// Mass

auto pound = std::ratio<45359237, 100000>{} * gram;
auto stone = std::ratio<14>{} * pound;
auto ton   = std::ratio<2240>{} * pound;

// Area

auto acre = furlong * chain;

} /* namespace imperial */
} /* namespace units */

#endif
