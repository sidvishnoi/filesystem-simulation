#include "filesystem.h"

int FileSystem::readFile(const char *title) {
    /*
    objective: to read a single file from multiple non-continuous sectors
    input:
        title: title of file
    return:
        0: success
        1: error
    effect: contents of file are printed, if success
    */
    TypeCastEntry entry;
    if (strlen(title) == 0) {
        std::cout << "File title: ";
        std::cin >> entry.entry.name;
        std::cin.ignore(32767, '\n');
    } else {
        strcpy(entry.entry.name, title);
    }

    entry.entry.startsAt = 0;

    char buf[kSectorSize];
    TypeCastEntry test;
    for (int i = 0; i < sectorsForDir; ++i) {
        readSector(currentDir + i, buf);
        for (int j = 0; j < kSectorSize; j += 32) {
            for (int k = 0; k < 32; ++k) {
                test.str[k] = buf[j+k];
            }
            if (strcmp(test.entry.name, entry.entry.name) == 0) {
                entry.entry.startsAt = test.entry.startsAt;
                entry.entry.size = test.entry.size;
                entry.entry.parent = test.entry.parent;
                if (test.entry.type != 'F') return 1;
                break;
            }
        }
        if (entry.entry.startsAt != 0) break;
    }
    if (entry.entry.startsAt == 0) {
        std::cout << "file not found" << std::endl;
        return 1;
    } else {
        // read file content
        std::cout << "READING FILE WITH" << std::endl;
        std::cout << "TITLE = " << test.entry.name << std::endl;
        std::cout << "SIZE = " << entry.entry.size << " bytes." << std::endl;

        int sec = entry.entry.startsAt;
        while (sec != 1) {
            readSector(sec, buf);
            for (int i = 0; i < kSectorSize; ++i) {
                std::cout << buf[i];
            }
            sec = getStatus(sec);
        }
        std::cout << std::endl;
    }
    return 0;
}