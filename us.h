#ifndef UNITS_US_H
#define UNITS_US_H

#include "metric.h"

namespace units {
namespace us {

// Distance

const auto inch = std::ratio<254, 10>{} * units::metric::millimeter;
const auto foot = std::ratio<12>{} * inch;
const auto yard = std::ratio<3>{} * foot;
const auto mile = std::ratio<1760>{} * yard;

const auto pica = std::ratio<1, 6>{} * inch;
const auto point = std::ratio<1, 12>{} * pica;

const auto link = std::ratio<33, 50>{} * foot;
const auto rod = std::ratio<25>{} * link;
const auto chain = std::ratio<4>{} * rod;
const auto furlong = std::ratio<10>{} * chain;
const auto survey = std::ratio<8>{} * furlong;
const auto league = std::ratio<3>{} * survey;

const auto fathom = std::ratio<2>{} * yard;
const auto cable = std::ratio<120>{} * fathom;
const auto nautical_mile = std::ratio<1852, 100>{} * units::metric::kilometer;

// Area

const auto sq_foot = foot * foot;
const auto sq_chain = chain * chain;
const auto acre = std::ratio<10>{} * sq_chain;
const auto section = std::ratio<640>{} * acre;

// Volume

const auto cubic_inch = inch * inch * inch;
const auto cubic_foot = foot * foot * foot;
const auto cubic_yard = yard * yard * yard;
const auto acre_foot = acre * foot;

// Liquid volume

const auto pint = std::ratio<473'176'473, 1'000'000>{} * units::metric::milliliter;

const auto quart = std::ratio<2>{} * pint;
const auto gallon = std::ratio<4>{} * quart;
const auto barrel = std::ratio<63, 2>{} * gallon;
const auto hogshead = std::ratio<63>{} * gallon;

const auto cup = std::ratio<1, 2>{} * pint;
const auto gill = std::ratio<1, 2>{} * cup;
const auto fluid_ounce = std::ratio<1, 8>{} * cup;
const auto tablespoon = std::ratio<1, 2>{} * fluid_ounce;
const auto teaspoon = std::ratio<1, 3>{} * tablespoon;

// Dry volume

namespace dry {

const auto pint = std::ratio<3360, 100>{} * cubic_inch;
const auto quart = std::ratio<2>{} * ::units::us::dry::pint;
const auto gallon = std::ratio<4>{} * ::units::us::dry::quart;
const auto peck = std::ratio<2>{} * ::units::us::dry::gallon;
const auto bushel = std::ratio<4>{} * peck;
const auto barrel = std::ratio<7056>{} * cubic_inch;

} /* namespace dry */

// Mass

const auto pound = std::ratio<45359237, 100000>{} * units::metric::gram;

const auto hundredweight = std::ratio<100>{} * pound;
const auto long_hundredweight = std::ratio<112>{} * pound;
const auto ton = std::ratio<2240>{} * pound;

const auto ounce = std::ratio<1, 16>{} * pound;
const auto dram = std::ratio<1, 16>{} * ounce;
const auto grain = std::ratio<1, 7000>{} * pound;

// Other

const auto board_foot = foot * foot * inch;
const auto calorie = std::ratio<4184, 1000>{} * units::metric::joule;
const auto food_calorie = std::ratio<4184, 1000>{} * units::metric::kilojoule;


} /* namespace us */
} /* namespace units */

#endif
