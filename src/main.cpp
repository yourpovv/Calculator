#include "banner.h"
#include "calculator.h"
#include "terminal.h"
#include "theme.h"
#include <windows.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace {

bool readNumber(const std::string& numberPrompt, double& number) {
    std::cout << numberPrompt;
    std::string inputText;
    if (!std::getline(std::cin, inputText) || inputText.empty()) {
        return false;
    }

    try {
        size_t charactersRead = 0;
        number = std::stod(inputText, &charactersRead);
        return charactersRead == inputText.size();
    } catch (const std::exception&) {
        return false;
    }
}

void showResult(const calculator::Result& calculation) {
    if (!calculation.success) {
        std::cout << COL_ERROR << "Error: " << calculation.errorMessage << RESET << "\n";
        return;
    }

    const std::string resultText = calculation.formattedResult.empty()
        ? calculator::format(calculation.resultValue)
        : calculation.formattedResult;

    std::cout << OK << "Result: " << resultText << RESET << "\n";
}

bool readTwoNumbers(const std::string& firstPrompt,
                    const std::string& secondPrompt,
                    double& firstNumber,
                    double& secondNumber) {
    return readNumber(firstPrompt, firstNumber) &&
           readNumber(secondPrompt, secondNumber);
}

template <typename Operation>
void doTwoNumberCalculation(const std::string& firstPrompt,
                            const std::string& secondPrompt,
                            Operation calculate) {
    double firstNumber = 0.0;
    double secondNumber = 0.0;

    if (!readTwoNumbers(firstPrompt, secondPrompt, firstNumber, secondNumber)) {
        std::cout << COL_ERROR << "Invalid numbers entered\n" << RESET;
        return;
    }

    showResult(calculate(firstNumber, secondNumber));
}

template <typename Operation>
void doOneNumberCalculation(const std::string& numberPrompt, Operation calculate) {
    double number = 0.0;
    if (!readNumber(numberPrompt, number)) {
        std::cout << COL_ERROR << "Invalid number entered\n" << RESET;
        return;
    }

    showResult(calculate(number));
}

void displayMenu() {
    const std::vector<std::string> menuOptions = {
        "[1] Add",
        "[2] Subtract",
        "[3] Multiply",
        "[4] Divide",
        "[5] Remainder",
        "[6] Power",
        "[7] Square Root",
        "[8] Percent",
        "[0] Exit",
        ""
    };

    const int menuColumnWidth = 18;
    const int columnGap = 4;
    const int menuWidth = (2 * menuColumnWidth) + columnGap;
    const int terminalWidth = calculator::getTerminalWidth();
    const int menuLeftPadding = std::max(0, (terminalWidth - menuWidth) / 2 - 3);

    std::cout << "\n";
    for (size_t optionIndex = 0; optionIndex < menuOptions.size(); optionIndex += 2) {
        std::string leftOption = menuOptions[optionIndex];
        std::string rightOption = menuOptions[optionIndex + 1];

        leftOption.resize(menuColumnWidth, ' ');
        rightOption.resize(menuColumnWidth, ' ');

        std::cout << std::string(menuLeftPadding, ' ');
        std::cout << SECONDARY << leftOption << RESET;
        std::cout << std::string(columnGap, ' ');
        std::cout << DEFAULT << rightOption << RESET;
        std::cout << "\n";
    }

    std::cout << "\n" << std::string(menuLeftPadding + 2, ' ');
    std::cout << SECONDARY << "Choose an operation: " << RESET;
}

void waitToContinue() {
    std::cout << "\nPress Enter to go back";
    std::string ignored;
    std::getline(std::cin, ignored);
}

}

int main() {
    SetConsoleTitleW(L"Calculator");
    HWND consoleWindow = GetConsoleWindow();
    HICON applicationIcon = static_cast<HICON>(LoadImageW(
        GetModuleHandleW(nullptr),
        MAKEINTRESOURCEW(101),
        IMAGE_ICON,
        0,
        0,
        LR_DEFAULTSIZE));
    if (consoleWindow != nullptr && applicationIcon != nullptr) {
        SendMessageW(consoleWindow, WM_SETICON, ICON_SMALL,
                     reinterpret_cast<LPARAM>(applicationIcon));
        SendMessageW(consoleWindow, WM_SETICON, ICON_BIG,
                     reinterpret_cast<LPARAM>(applicationIcon));
    }
    calculator::enableUtf8Console();

    for (;;) {
        calculator::clearScreen();
        calculator::showBanner();
        displayMenu();

        std::string selectedOption;
        if (!std::getline(std::cin, selectedOption)) {
            return 0;
        }

        if (selectedOption.empty()) {
            std::cout << WARN << "Please choose an option.\n" << RESET;
            waitToContinue();
            continue;
        }

        if (selectedOption == "0") {
            calculator::clearScreen();
            printf("\ngood Baii~ ♡\n");
            Sleep(2000);
            return 0;
        }

        if (selectedOption == "1") {
            doTwoNumberCalculation("First number: ", "Second number: ", calculator::add);
        } else if (selectedOption == "2") {
            doTwoNumberCalculation("First number: ", "Second number: ", calculator::subtract);
        } else if (selectedOption == "3") {
            doTwoNumberCalculation("First number: ", "Second number: ", calculator::multiply);
        } else if (selectedOption == "4") {
            doTwoNumberCalculation("First number: ", "Second number: ", calculator::divide);
        } else if (selectedOption == "5") {
            doTwoNumberCalculation("First number: ", "Second number: ", calculator::remainder);
        } else if (selectedOption == "6") {
            doTwoNumberCalculation("Base: ", "Exponent: ", calculator::power);
        } else if (selectedOption == "7") {
            doOneNumberCalculation("Value: ", calculator::squareRoot);
        } else if (selectedOption == "8") {
            doTwoNumberCalculation("Value: ", "Percent: ", calculator::percentage);
        } else {
            std::cout << COL_ERROR << "Invalid option.\n" << RESET;
        }

        waitToContinue();
    }
}
