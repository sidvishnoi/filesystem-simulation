#include "filesystem.h"

void FileSystem::tree(int level, int spacing) {
    /*
    objective: print recursive tree like structure of the filesystem,
        showing it's file and folders, based on current directory
    effect:
        prints filesystem tree.
    */
    if (currentDir == kReservedSectors && level != 1) {
        return;
    }
    if (currentDir == kReservedSectors && level == 1) {
        std::cout << KBLU << "/" << KRST << std::endl;
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
                for (int i = 0; i < level; ++i) {
                    i>0 && std::cout << std::string(spacing, ' ');
                    std::cout << "|";
                }
                std::cout << DASH << entry.entry.name << std::endl;
            } else if (entry.entry.type == 'D') {
                for (int i = 0; i < level; ++i) {
                    i>0 && std::cout << std::string(spacing, ' ');
                    std::cout << "|";
                }
                std::cout << DASH << KBLU << entry.entry.name << KRST << std::endl;
                changeDir(entry.entry.name);
                tree(level+1);
                changeDir("..");
            }
        }
    }
}