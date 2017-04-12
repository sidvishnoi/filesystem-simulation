#include "filesystem.h"

void createBuffer(char temp, char* buf) {
    // creates a buffer based on a char
    if (temp == '.') {
        std::cout << "Enter alphabet: ";
        std::cin >> temp;
        std::cin.ignore(32767, '\n');
    }
    for (int i = 0; i < kSectorSize_g; i++) {
        buf[i] = temp;
    }
}
