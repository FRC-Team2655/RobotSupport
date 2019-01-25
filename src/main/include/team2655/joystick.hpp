/**
 * joystick.hpp
 * Contains FRC Team 2655's joystick helper code
 * Allows creation of cubic and deadband configurations for use with WPILib Joystick axes
 *
 * @author Marcus Behel
 * @version 1.1 8-30-2018 Changed names to make easier to understand and implement
 *
 * Copyright (c) 2018 FRC Team 2655 - The Flying Platypi
 * See LICENSE file for details
 */

#pragma once

#include <cstdlib>
#include <vector>
#include <array>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <map>
#include <string>

namespace team2655{
namespace jshelper{ // The js namespace contains helper functions for use with WPILib Joysticks.

/**
 * A type for all the needed coefficients for joysticks
 */
typedef std::array<double, 5> AxisConfig;

/**
 * Fit a polynomial function to a set of data points (regression modeling)
 * @param x - The x coordinates
 * @param y - The y coordinates
 * @param order - What is the degree of the polynomial function
 * @param coeffs Where to output coefficients
 * @return Was the fitting successful
 */
template <class TYPE>
bool polyfit(const std::vector<TYPE> & x, const std::vector<TYPE> & y, const int &order, std::vector<TYPE> &coeffs);

/**
 * Get cubic function coefficients for a joystick axis.
 * @param deadband The threshold an axis must pass before it is treated as non-zero.
 * @param minPower The minimum value a scaled axis will return after moving past the deadband
 * @param midPower Where the "flat" part of the function should be
 * @return The coefficients for the cubic function (and the deadband). 5th item is deadband. 4th (as normal) is start of cubic function.
 */
AxisConfig createAxisConfig(double deadband, double minPower, double midPower);

/**
 * Get a set of coefficients that are setup for a non-modified axis with an applied deadband.
 * @param deadband
 * @return The coefficients for the linear function (and the deadband). 5th item is deadband. 4th (as normal) is start of "cubic" function (a=0, b=2, c=1, d=0).
 */
AxisConfig createAxisConfig(double deadband);

/**
 * Get the scaled value of a joystick axis using the pre-calculated cubic function coefficients.
 * @param coeffs The coefficients for the cubic function for this axis
 * @param axisValue The non-scaled value of the axis.
 * @return The scaled (by cubic function) value of the axis.
 */
double getAxisValue(const AxisConfig config, const double axisValue, bool deadbandOnly = false);

}
}
