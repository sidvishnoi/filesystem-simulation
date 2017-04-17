#include "filesystem.h"

int FileSystem::createDir(const char *title) {
    /*
    objective: creates a directory in directory corresponing to currentDir
    input: title of directory
    return:
        0: success
        1: error
    effect:
        a directory entry is passed into the currentDir, and
        sectors are reserved for directory entries that will
            correspond to files/directories in created directory
    */
    if (strcmp(title, "/") == 0) {
        // there can only be one root dir, anywhere
        return 1;
    }
    TypeCastEntry entry;
    strcpy(entry.entry.name, title);
    entry.entry.type = 'D';
    entry.entry.parent = parentDir;
    entry.entry.size = 0;


    // look for free space and reserve space for this directory's entries
    std::vector<int> sectorsFree;
    int sec = kReservedSectors + sectorsForDir;
    while (sectorsFree.size() != sectorsForDir) {
        int status = getStatus(sec);
        if (status == FREE) {
            sectorsFree.push_back(sec);
        } else {
            sectorsFree.clear();
        }
        if (status == NOT_FOUND) {
            std::cout << "Not enough space" << std::endl;
            return 1;
        }
        ++sec;
    }

    entry.entry.startsAt = sectorsFree[0];
    std::cout << "Found free space for new dir at sector = " <<
        entry.entry.startsAt << std::endl;

    // reserve space
    for (int i = 0; i < sectorsForDir; ++i) {
        updateStatus(sectorsFree[i], DIR_ENTRY);
    }

    // find position for file entry
    bool positionFound = false;
    int byteforEntry = 0;
    int sectorForEntry = 0;
    char buf[kSectorSize];
    for (int s = 0; s < sectorsForDir; ++s) {
        readSector(currentDir + s, buf);
        for (int b = 0; b < kSectorSize; b += 32) {
            TypeCastEntry test;
            for (int k = 0; k < 32; ++k) {
                test.str[k] = buf[b+k];
            }
            if (strlen(test.entry.name) == 0) {
                positionFound = true;
                sectorForEntry = s;
                byteforEntry = b;
                break;
            } else if (strcmp(test.entry.name, entry.entry.name) == 0) {
                std::cout << "Directory with same name already exists in current directory."
                 << std::endl;
                 return 1;
            }
        }
        if (positionFound) break;
    }

    // update buffer for current dir
    std::cout << "CREATE DIR WITH PARENT = " << entry.entry.parent << std::endl;
    for (int i = byteforEntry, j = 0; j < 32; ++i, ++j) {
        buf[i] = entry.str[j];
    }

    // write the file entry for current (parent) dir
    writeSector(currentDir + sectorForEntry, buf);
    updateStatus(currentDir + sectorForEntry, DIR_ENTRY);

    return 0;
}