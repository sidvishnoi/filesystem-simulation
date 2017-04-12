#include "filesystem.h"

int FileSystem::getStatus(int sector_no) const {
    /*
    objective: to return a Status of a given sector
    input: sector_no = sector number
    return:
        RESERVED: if sector is reserved for status storage
        BUSY: if sector is already in use for data
        FREE: sector is free
        cast.num: otherwise (negative cast.num shows marked as deleted)
    */
    if (sector_no > kNumberOfSectors - 1) {
        return NOT_FOUND;
    } else if (sector_no < kReservedSectors) {
        return RESERVED;
    } else {
        int pos = (sector_no - kReservedSectors)*4;

        // sector to read from disk
        int sector_offset = pos / kSectorSize;

         // bytes to extract from given sector at sector offset
        int byte_in_sector = pos % kSectorSize;

        char buf[kSectorSize];
        std::ifstream fin;
        fin.open(kDiskTitle, std::ios::binary | std::ios::in);
        fin.seekg(sector_offset * kSectorSize);
        fin.read(buf, kSectorSize);
        fin.close();

        TypeCastInt cast;
        for (int i = 0; i < 4; ++i) {
            cast.str[i] = buf[byte_in_sector + i];
        }

        // std::cout << "Reading status of sector #" <<
        //     sector_no << ", in sector = " <<
        //     sector_offset << " and bytes = [" << byte_in_sector <<
        //     ", " << byte_in_sector+ 3 << "] in sector. returned status = " <<
        //     cast.num << std::endl;

        if (cast.num == 1) {
            return BUSY;
        } else if (cast.num == 0) {
            return FREE;
        } else {
            return cast.num;
        }
    }
}