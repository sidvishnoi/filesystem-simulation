#include "filesystem.h"

void FileSystem::create(bool format, bool alreadyExists) {
    /* (private)
    objective: to reserve disk space to kDiskSize bytes
    input:
        format: whether format disk (write all zeroes) or not
    return: None
    effect: memory=kDiskSize (bytes) for a disk is reserved at path=kDiskTitle
    */
    if (format || !alreadyExists) {
        TypeCastInt cast;
        cast.num = 0;
        char buffer[kSectorSize];
        std::ofstream os;
        os.open(kDiskTitle, std::ios::binary | std::ios::out);
        for(int i = 0; i< kSectorSize; ++i) {
            buffer[i] = cast.str[i%4];
        }
        // reserve disk space by filling 0s everywhere
        for(int i = 0; i < kNumberOfSectors; ++i) {
            os.write(buffer, kSectorSize);
        }
        os.close();
        // write dummy data
        // for(int i = kReservedSectors, j = 0; i < kNumberOfSectors; ++i, j++) {
        //     createBuffer(static_cast<char>(65 + j%10), buffer);
        //     writeSector(i, buffer);
        // }

        // reserve space for root
        for (int i = 0; i < sectorsForDir; ++i) {
            updateStatus(kReservedSectors + i, DIR_ENTRY);
        }

        if (!alreadyExists) {
            std::cout << "New FileSystem created of size = " <<
            kDiskSize/kOneMiB_g << "MiB." << std::endl;
        }
        if (format) {
            std::cout << "FileSystem formatted." << std::endl;
        }
    } else {
        std::cout << "Using pre existing FileSystem : " << kDiskTitle <<
        ", of size = " << kDiskSize/kOneMiB_g << "MiB" << std::endl;
    }
}