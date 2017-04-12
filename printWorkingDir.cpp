#include "filesystem.h"

int FileSystem::printWorkingDir() {
    /*
    objective: to print the current working directory
    input: none
    effect: the current working directory with respect to root directory
    */
    if (currentDir == kReservedSectors) {
        std::cout << "/" << std::endl;
    } else {
        int par = parentDir;
        int cur = currentDir;
        std::string dirPath = "";
        do {
            char buf[kSectorSize];
            readSector(par, buf);
            for (int j = 0; j < kSectorSize; j += 32) {
                TypeCastEntry entry;
                for (int k = 0; k < 32; ++k) {
                    entry.str[k] = buf[j+k];
                }
                if (entry.entry.startsAt == cur) {
                    cur = par;
                    par = entry.entry.parent;
                    std::string temp = entry.entry.name;
                    dirPath = temp + "/" +  dirPath;
                    break;
                }
            }
        } while (cur != kReservedSectors);

        std::cout << "/" + dirPath << std::endl;
    }
    return 0;
}
