#ifndef UNITS_METRIC_H
#define UNITS_METRIC_H

#include "metric.h"

namespace units {
namespace imperial {

////////////////////////////////// BASE UNITS //////////////////////////////////

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

// Volume


// Velocity

auto mps = meter / second;
using Velocity = UNIT_TYPE(mps);

// Acceleration

auto mps2 = mps / second;
using Acceleration = UNIT_TYPE(mps2);

// Frequency

auto hertz = second.exp<-1>();
using Frequency = UNIT_TYPE(hertz);
SI_ALL_PREFIXES(hertz);

// Force

auto newton = kilogram * meter / (second * second);
using Force = UNIT_TYPE(newton);
SI_ALL_PREFIXES(newton);

// Pressure

auto pascal = newton / (meter * meter);
using Pressure = UNIT_TYPE(pascal);

// Energy

auto joule = newton * meter;
using Energy = UNIT_TYPE(joule);

// Power

auto watt = joule / second;
using Power = UNIT_TYPE(watt);

// Electric charge

auto coloumb = ampere * second;
using ElectricCharge = UNIT_TYPE(coloumb);

// Voltage

auto volt = watt / ampere;
using Voltage = UNIT_TYPE(volt);

// Electric capacitance

auto farad = coloumb / volt;
using ElectricCapacitance = UNIT_TYPE(farad);

// Electric resistance

auto ohm = volt / ampere;
using ElectricResistance = UNIT_TYPE(ohm);

// Electrical conductance

auto siemens = ampere / volt;
using ElectricalConductance = UNIT_TYPE(siemens);

// Magnetic flux

auto weber = volt * second;
using MagneticFlux = UNIT_TYPE(weber);

// Magnetic field strength

auto tesla = weber / meter.exp<2>();
using MagneticField = UNIT_TYPE(tesla);

// Inductance

auto henry = weber / ampere;
using Inductance = UNIT_TYPE(henry);

// Radioactivity

auto becquerel = hertz;
using Radioactivity = UNIT_TYPE(becquerel);

// Absorbed dose (of ionizing radiation)

auto gray = joule / kilogram;
using AbsorbedDose = UNIT_TYPE(gray);

// Equivalent dose (of ionizing radiation) J/kg    m2⋅s−2

auto sievert = joule / kilogram;
using EquivalentDos = UNIT_TYPE(sievert);

// Catalytic activity

auto katal = mole / second;
using CatalyticActivity = UNIT_TYPE(katal);

} /* namespace imperial */
} /* namespace units */

#endif
