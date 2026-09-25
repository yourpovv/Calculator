#include "calculator.h"
#include <cerrno>
#include <cmath>
#include <iomanip>
#include <sstream>

namespace calculator {

namespace {

bool isZero(double number) {
    return std::fabs(number) < 1e-12;
}

Result finiteOrError(double value, const char* errorMessage) {
    if (!std::isfinite(value)) {
        return {0.0, false, errorMessage};
    }
    return {value, true, ""};
}

std::string formatDecimal(double number) {
    if (!std::isfinite(number)) {
        return "undefined";
    }

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(6) << number;
    std::string formattedNumber = stream.str();
    while (!formattedNumber.empty() && formattedNumber.back() == '0') {
        formattedNumber.pop_back();
    }
    if (!formattedNumber.empty() && formattedNumber.back() == '.') {
        formattedNumber.pop_back();
    }
    return formattedNumber;
}

}

Result add(double firstNumber, double secondNumber) {
    return finiteOrError(firstNumber + secondNumber, "result is out of range");
}

Result subtract(double firstNumber, double secondNumber) {
    return finiteOrError(firstNumber - secondNumber, "result is out of range");
}

Result multiply(double firstNumber, double secondNumber) {
    return finiteOrError(firstNumber * secondNumber, "result is out of range");
}

Result divide(double firstNumber, double secondNumber) {
    if (isZero(secondNumber)) {
        return {0.0, false, "you can't divide by zero"};
    }
    return finiteOrError(firstNumber / secondNumber, "result is out of range");
}

Result remainder(double firstNumber, double secondNumber) {
    if (isZero(secondNumber)) {
        return {0.0, false, "you can't find remainder with zero"};
    }
    return finiteOrError(std::fmod(firstNumber, secondNumber), "could not calculate remainder");
}

Result power(double baseNumber, double exponent) {
    errno = 0;
    const double value = std::pow(baseNumber, exponent);
    if (errno != 0 || !std::isfinite(value)) {
        return {0.0, false, "could not calculate power"};
    }
    return {value, true, ""};
}

std::string imaginaryRoot(double magnitude) {
    const std::string numericText = formatDecimal(magnitude);
    return std::fabs(magnitude - 1.0) < 1e-12 ? "i" : numericText + "i";
}

Result squareRoot(double number) {
    if (number < 0.0) {
        const double magnitude = std::sqrt(std::fabs(number));
        if (!std::isfinite(magnitude)) {
            return {0.0, false, "could not calculate square root"};
        }
        return {magnitude, true, "", imaginaryRoot(magnitude)};
    }
    return finiteOrError(std::sqrt(number), "could not calculate square root");
}

Result percentage(double number, double percent) {
    return finiteOrError(number * (percent / 100.0), "result is out of range");
}

std::string format(double number) {
    return formatDecimal(number);
}

}
