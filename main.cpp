#include "src/tony_machine.h"
#include <iostream>

int main() {
    TonyMachine machine;

    while (true) {
        std::string type;
        std::cin >> type;

        if (type == "Identical") {
            std::cout << "Print identity parameter: ";
            int percentage;
            std::cin >> percentage;
            std::cout << std::endl;
            if (!(0 <= percentage && percentage <= 100)) {
                std::cout << "Wrong identity paramater: must be percentage" << std::endl;
            }
            machine.PrintIdentical(percentage / 100.0);
            std::cout << "----------------------------------" << std::endl;
        } else if (type == "Same") {
            machine.PrintSame();
            std::cout << "----------------------------------" << std::endl;
        } else if (type == "FirstMinusSecond") {
            machine.FirstMinusSecond();
            std::cout << "----------------------------------" << std::endl;
        } else if (type == "SecondMinusFirst") {
            machine.SecondMinusFirst();
            std::cout << "----------------------------------" << std::endl;
        } else if (type == "End") {
            break;
        } else {
            std::cout << "Do not support such queries" << std::endl;
            std::cout << "----------------------------------" << std::endl;
        }
    }

    return 0;
}