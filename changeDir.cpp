#include "filesystem.h"

int FileSystem::changeDir(const char *title) {
    /*
    objective: to change the current working directory
    input: directory to change to
    return:
        0: sucess
        1: fail (child directory doesn not exists in parent directory)
    effect: the currentDir and parentDir are updated
    */
    TypeCastEntry entry;
    strncpy(entry.entry.name, title, 19);

    if (strcmp(entry.entry.name, "..") == 0) {
        // go to one level up in directory
        if (currentDir == parentDir) {
            return 0;
        }
        char buf[kSectorSize];
        TypeCastEntry test;
        for (int i = 0; i < sectorsForDir; ++i) {
            readSector(parentDir + i, buf);
            for (int j = 0; j < kSectorSize; j += 32) {
                for (int k = 0; k < 32; ++k) {
                    test.str[k] = buf[j+k];
                }
                if (test.entry.startsAt == currentDir) {
                    currentDir = parentDir;
                    parentDir = test.entry.parent;
                    // std::cout << "currentDir = " << currentDir << std::endl;
                    // std::cout << "parentDir = " << parentDir << std::endl;
                    return 0;
                }
            }
        }
    } else if (strcmp(entry.entry.name, "\\") == 0) {
        // go to root directory
        if (currentDir == parentDir) {
            return 0;
        }
        currentDir = kReservedSectors;
        parentDir = kReservedSectors;
        // std::cout << "currentDir = " << currentDir << std::endl;
        // std::cout << "parentDir = " << parentDir << std::endl;
        return 0;
    } else {
        char buf[kSectorSize];
        TypeCastEntry test;
        for (int i = 0; i < sectorsForDir; ++i) {
            readSector(currentDir + i, buf);
            for (int j = 0; j < kSectorSize; j += 32) {
                for (int k = 0; k < 32; ++k) {
                    test.str[k] = buf[j+k];
                }
                if (strcmp(test.entry.name, entry.entry.name) == 0) {
                    // if right name found in Dir Entry
                    parentDir = currentDir;
                    currentDir = test.entry.startsAt;
                    return 0;
                }
            }
        }
    }

    std::cout << "Directory not found" << std::endl;
    return 1;
}