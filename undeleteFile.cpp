#include "filesystem.h"

int FileSystem::undeleteFile(const char *title) {
    /*
    objective: (attempt) to undelete a file (recover) from current directory
    input: file title
    return:
        1: fail
        0: success
    effect:
        if success:
        the sectors corresponding to a file are marked as original un-deleted.
        the type in directory entry for the file changes from 'f' to 'F', denoting recovered.
    */

    char buf[kSectorSize];
    // find position for file entry
    bool positionFound = false;
    int byteforEntry = 0;
    int sectorForEntry = 0;
    TypeCastEntry test;
    for (int s = 0; s < sectorsForDir; ++s) {
        readSector(currentDir + s, buf);
        for (int b = 0; b < kSectorSize; b += 32) {
            for (int k = 0; k < 32; ++k) {
                test.str[k] = buf[b+k];
            }
            if (strcmp(test.entry.name, title) == 0
                && test.entry.type == 'f') {
                positionFound = true;
                sectorForEntry = s;
                byteforEntry = b;
                break;
            }
        }
        if (positionFound) break;
    }
    if (!positionFound) {
        std::cout << "No such file found." << std::endl;
        return 1;
    }


    int status = test.entry.startsAt;
    int sectorsToLookFor = test.entry.size / kSectorSize + 1;
    bool isRecoverable = true;
    int sec = 0;
    while (sec < sectorsToLookFor) {
        int nxt = getStatus(-status);
        if (nxt > -1) {
            isRecoverable = false;
            break;
        }
        ++sec;
        status = nxt;
    };


    if (isRecoverable) {
        status = test.entry.startsAt;
        do {
            int nxt = getStatus(-status);
            status = nxt;
            updateStatus(-status, -nxt);
        } while (status != -1);

        test.entry.type = 'F';
        test.entry.startsAt = -test.entry.startsAt;
        for (int i = byteforEntry, j = 0; j < 32; ++i, ++j) {
            buf[i] = test.str[j];
        }
        writeSector(currentDir + sectorForEntry, buf);
        std::cout << "recovered file: " << title << std::endl;
        return 0;
    } else {
        for (int i = byteforEntry, j = 0; j < 32; ++i, ++j) {
            buf[i] = static_cast<char>(0);
        }
        writeSector(currentDir + sectorForEntry, buf);
        std::cout << "un-recoverable file: " << title << std::endl;
        return 1;
    }
}