#include "filesystem.h"

int FileSystem::updateStatus(int sector_no, int x) {
    /*
    objective: to update status of a given sector
    input:
        sector_no = sector number
        x = status (0 = FREE, 1 = BUSY, other: continuation sector number)
    return:
        (failure) -1
        (success) otherwise
    effect: a sector is marked in disk
    */
    int status = getStatus(sector_no);
    if (status != RESERVED && status != NOT_FOUND) {
        TypeCastInt cast;
        int pos = (sector_no - kReservedSectors)*4;

        // sector to read from disk
        int sector_offset = pos / kSectorSize;

         // bytes to extract from given sector at sector offset
        int byte_in_sector = pos % kSectorSize;

        char buffer[kSectorSize];
        cast.num = x;
        std::fstream fp;
        fp.open(kDiskTitle, std::ios::binary | std::ios::in | std::ios::out);
        fp.seekg(sector_offset * kSectorSize);
        fp.read(buffer, kSectorSize);
        // edit buffer
        for (int i = 0, j = 0; j < 4; ++i, ++j) {
            buffer[byte_in_sector + i] = cast.str[j];
        }
        fp.seekp(sector_offset * kSectorSize);
        fp.write(buffer, kSectorSize);
        fp.close();
        return x;
    } else {
        return -1;
    }
}