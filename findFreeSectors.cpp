#include "filesystem.h"

int FileSystem::findFreeSectors(int sectorsNeeded, std::vector<int> &sectorsFree) {
    int sectorsFound = 0;
    int sector = kReservedSectors + sectorsForDir;
    while (sectorsFound < sectorsNeeded) {
        int status = getStatus(sector);
        if (status == FREE) {
            // add sector to usable list
            sectorsFree[sectorsFound++] = sector;
        } else if (status == NOT_FOUND) {
            // look in deleted sectors now
            std::cout << "No more FREE sectors." << std::endl;
            break;
        }
        ++sector;
    }
    // look for available sectors that are marked deleted
    sector = kReservedSectors + sectorsForDir;
    while (sectorsFound < sectorsNeeded) {
        int status = getStatus(sector);
        if (status < 0) {
            sectorsFree[sectorsFound++] = sector;
        } else if (status == NOT_FOUND) {
            std::cout << "Not enough space in disk!" << std::endl;
            return 1;
        }
        ++sector;
    }
    return 0;
}