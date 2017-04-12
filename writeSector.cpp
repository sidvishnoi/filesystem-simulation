#include "filesystem.h"

void FileSystem::writeSector(int sector_no, char* buf) {
    /*
    objective: to write data into a sector
    input:
        sector_no = sector number to write into
        buf = buffer to write from (size = kSectorSize)
    return: None
    effect:
        data is written into a sector if it is FREE OR BUSY (over-written)
        else: nothing
    */
    int status = getStatus(sector_no);
    if (status == RESERVED) {
        std::cout << "It is a reserved sector" << std::endl;
        return;
    } else if (status == NOT_FOUND) {
        std::cout << "Sector does not exist." << std::endl;
        return;
    } else {
        std::ofstream fp;
        fp.open(kDiskTitle, std::ios::binary | std::ios::in | std::ios::out);
        fp.seekp(sector_no*kSectorSize);
        fp.write(buf, kSectorSize);
        fp.close();
    }
}