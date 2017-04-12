#include "filesystem.h"

FileSystem::FileSystem(
    std::string title = "Disk.bin",
        int diskSize = kOneMiB_g,
        bool alreadyExists = false) :
        kDiskTitle(title),
        kDiskSize(diskSize) {
    // constructor
    kNumberOfSectors = kDiskSize/kSectorSize;
    kReservedSectors = kNumberOfSectors/128;
    currentDir = kReservedSectors;  // root dir
    parentDir = kReservedSectors;
    create(false, alreadyExists);
}