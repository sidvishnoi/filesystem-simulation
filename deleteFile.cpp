#include "filesystem.h"

int FileSystem::deleteFile(const char *title) {
    /*
    objective: to delete a file (in recoverable form) from current directory
    input: file title
    return:
        1: fail
        0: success
    effect: the sectors corresponding to a file are marked deleted (denoted by negative integers)
        the type in directory entry for the file changes for 'F' to 'f', denoting deleted.
    */

    char buf[kSectorSize];
    // find position for file entry
    bool positionFound = false;
    bool alreadyDeleted = false;
    int byteforEntry = 0;
    int sectorForEntry = 0;
    TypeCastEntry test;
    for (int s = 0; s < sectorsForDir; ++s) {
        readSector(currentDir + s, buf);
        for (int b = 0; b < kSectorSize; b += 32) {
            for (int k = 0; k < 32; ++k) {
                test.str[k] = buf[b+k];
            }
            if (strcmp(test.entry.name, title) == 0) {
                if (test.entry.type == 'F') {
                    positionFound = true;
                    sectorForEntry = s;
                    byteforEntry = b;
                    break;
                } else if (test.entry.type == 'f') {
                    alreadyDeleted = true;
                    sectorForEntry = s;
                    byteforEntry = b;
                    break;
                }
            }
        }
        if (positionFound || alreadyDeleted) break;
    }
    if (!positionFound && !alreadyDeleted) {
        std::cout << "No such file found." << std::endl;
        return 1;
    }
    if (alreadyDeleted) {
        // remove permanently
        int status = test.entry.startsAt;
        do {
            int nxt = getStatus(-status);
            updateStatus(-status, 0);
            status = nxt;
        } while (status != -1);

        for (int i = byteforEntry, j = 0; j < 32; ++i, ++j) {
            buf[i] = static_cast<char>(0);
        }
        writeSector(currentDir + sectorForEntry, buf);
        std::cout << "permanently deleted file: " << title << std::endl;
        return 0;
    }


    int status = test.entry.startsAt;
    do {
        int nxt = getStatus(status);
        updateStatus(status, -nxt);
        status = nxt;
    } while (status != 1);

    test.entry.type = 'f';
    test.entry.startsAt = -test.entry.startsAt;
    for (int i = byteforEntry, j = 0; j < 32; ++i, ++j) {
        buf[i] = test.str[j];
    }
    writeSector(currentDir + sectorForEntry, buf);
    std::cout << "deleted file: " << title << std::endl;
    return 0;
}