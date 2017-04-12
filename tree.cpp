#include "filesystem.h"

void FileSystem::tree(int level) {
    /*
    objective: print recursive tree like structure of the filesystem,
        showing it's file and folders, based on current directory
    effect:
        prints filesystem tree.
    */
    if (currentDir == kReservedSectors && level != 1) {
        return;
    }
    TypeCastEntry entry;
    char buf[kSectorSize];
    for (int s = 0; s < sectorsForDir; ++s) {
        readSector(currentDir + s, buf);
        for (int b = 0; b < kSectorSize; b += 32) {
            for (int k = 0; k < 32; ++k) {
                entry.str[k] = buf[b+k];
            }
            if (entry.entry.type == 'F') {
                std::cout << std::string(level, ' ') <<
                    std::string(level, '-') << "> ";
                std::cout << entry.entry.name << std::endl;
            } else if (entry.entry.type == 'D') {
                std::cout << std::string(level, ' ') <<
                    std::string(level, '-') << "> ";
                std::cout << entry.entry.name << std::endl;
                changeDir(entry.entry.name);
                tree(level+1);
                changeDir("..");
            }
        }
    }
}