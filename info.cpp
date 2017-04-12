#include "filesystem.h"

void printTable(std::string str, int val1, int val2 = 0) {
    /*
    formats output as a table
    */
    if (val2 == 0) {
        std::cout << std::left << std::setw(32) << str <<
            ": " << val1 << std::endl;
    } else {
        std::cout << std::left << std::setw(32) << str <<
            ": (" << val1 << "/" << val2 << ")" << std::endl;
    }
}

void FileSystem::info() {
    /*
    objective: prints info about file system
    input: None
    return: None
    output: info about file system
    */
    int sectorsFree = 0, sectorsDeleted = 0, sectorsBusy = 0,
    sectorsForDir = 0;
    for (int i = kReservedSectors; i < kNumberOfSectors; ++i) {
        int status = getStatus(i);
        if (status == FREE) {
            ++sectorsFree;
        } else if (status < 0) {
            ++sectorsDeleted;
        } else if (status == DIR_ENTRY) {
            ++sectorsForDir;
        } else {
            ++sectorsBusy;
        }
    }
    printTable("FileSystem Size (bytes)", kDiskSize);
    printTable("Sector Size (bytes)", kSectorSize);
    printTable("Number of sectors", kNumberOfSectors);
    printTable("Reserved Sectors", kReservedSectors, kNumberOfSectors);
    printTable("Sectors Free", sectorsFree, kNumberOfSectors);
    printTable("Sectors Busy", sectorsBusy, kNumberOfSectors);
    printTable("Sectors for DIR_ENTRY", sectorsForDir, kNumberOfSectors);
    printTable("Sectors marked DELETED", sectorsDeleted, kNumberOfSectors);
}