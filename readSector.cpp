#include "filesystem.h"

void FileSystem::readSector(int sector_no, char *buf) const {
    /*
    objective: to read a sector from disk
    input:
        sector_no = sector number to read
        buf = char buffer to read into
    return: None
    effect: None
        sector data is printed if it exists & it is not reserved
        sector data is also set into *buf
    */
    int status = getStatus(sector_no);
    if (status == RESERVED) {
        std::cout << "It is a reserved sector." << std::endl;
    } else if (status == NOT_FOUND) {
        std::cout << "Sector does not exist." << std::endl;
        return;
    }
    std::ifstream fin;
    fin.open(kDiskTitle, std::ios::binary | std::ios::in);
    int byteOffset = sector_no*kSectorSize;
    fin.seekg(byteOffset);
    fin.read(buf, kSectorSize);
    fin.close();
    // std::cout << "Reading contents of sector #" <<
    //     sector_no << ", bytes = [" <<
    //     byteOffset << ", " << (byteOffset + kSectorSize - 1) <<
    //     "] in disk." << std::endl;
    // if (status == RESERVED) {
    //     for (int i = 0; i < kSectorSize; i++) {
    //         std::cout << static_cast<int>(buf[i]);
    //     }
    // } else {
    //     for (int i = 0; i < kSectorSize; i++) {
    //         std::cout << buf[i];
    //     }
    // }
    // std::cout << std::endl;
}