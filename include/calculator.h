#pragma once

#include <string>

namespace calculator {

struct Result {
    double resultValue = 0.0;
    bool success = false;
    std::string errorMessage;
    std::string formattedResult;
};

Result add(double firstNumber, double secondNumber);
Result subtract(double firstNumber, double secondNumber);
Result multiply(double firstNumber, double secondNumber);
Result divide(double firstNumber, double secondNumber);
Result remainder(double firstNumber, double secondNumber);
Result power(double baseNumber, double exponent);
Result squareRoot(double number);
Result percentage(double number, double percent);

std::string format(double number);

}
