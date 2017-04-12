#include "filesystem.h"

void FileSystem::listDirectoryContents() {
    /*
    objective: prints all files and directories in disk
    input: None
    return: None
    output: list of files and directories in disk
    */
    char buf[kSectorSize];
    TypeCastEntry entry;
    for (int i = 0; i < sectorsForDir; ++i) {
        readSector(currentDir + i, buf);
        for (int j = 0; j < kSectorSize; j += 32) {
            for (int k = 0; k < 32; ++k) {
                entry.str[k] = buf[j+k];
            }
                if (entry.entry.type == 'F') {
                    std::cout << std::left << std::setw(22) <<
                    entry.entry.name << std::right <<
                    std::setw(10) << entry.entry.size << " bytes" << std::endl;
                } else if (entry.entry.type == 'f') {
                    std::cout << std::left << std::setw(22) << "(" <<
                    entry.entry.name << ")" << std::right <<
                    std::setw(10) << entry.entry.size << " bytes" << std::endl;
                } else if ((entry.entry.type == 'D')) {
                    std::cout << std::left << KBLU << std::setw(22) <<
                    entry.entry.name << KRST << std::right <<
                    std::setw(10) << "[DIR]" << std::endl;
                }
        }
    }
}