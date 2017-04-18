#include "filesystem.h"

int FileSystem::createFile(const char *title, const char *srcFile) {
    /*
    objective: to create a file that can be written in multiple non-continuous sectors
    input:
        title: title of file
        srcFile: source file to read from
    return:
        0: success
        1: error
    effect: a new file is created in disk and directory entry is passed, if success
    */
    TypeCastEntry entry;
    if (strlen(title) == 0) {
        std::cout << "File title: ";
        std::cin >> entry.entry.name;
        std::cin.ignore(32767, '\n');
    } else {
        strcpy(entry.entry.name, title);
    }

    // default parent is root
    entry.entry.parent = currentDir;

    // not a directory, is a file
    entry.entry.type = 'F';

    // file contents & size
    char file_content[MAX_FILE_SIZE];
    if (strlen(srcFile) == 0) {
        std::cout << "Enter content of file:" << std::endl;
        std::cin.getline(file_content, sizeof(file_content));
        entry.entry.size = strlen(file_content);
    } else {
        std::ifstream fsrc;
        fsrc.open(srcFile, std::ios::binary | std::ios::in);
        fsrc.read(file_content, MAX_FILE_SIZE);
        fsrc.close();
        fsrc.open(srcFile, std::ios::binary | std::ios::in | std::ios::ate);
        entry.entry.size = fsrc.tellg();
        fsrc.close();
    }

    if (entry.entry.size < 0) {
        return 1;
    }


    // find sectors to write in
    int sectorsNeeded = (entry.entry.size - 1) / kSectorSize + 1;
    std::vector<int> sectorsFree(sectorsNeeded);
    if (findFreeSectors(sectorsNeeded, sectorsFree) == 1) {
        return 1;
    };

    entry.entry.startsAt = sectorsFree[0];

    char buf[kSectorSize];
    // find position for file entry
    bool positionFound = false;
    int byteforEntry = 0;
    int sectorForEntry = 0;
    for (int s = 0; s < sectorsForDir; ++s) {
        readSector(currentDir + s, buf);
        for (int b = 0; b < kSectorSize; b += 32) {
            TypeCastEntry test;
            for (int k = 0; k < 32; ++k) {
                test.str[k] = buf[b+k];
            }
            if (strlen(test.entry.name) == 0) {
                positionFound = true;
                sectorForEntry = s;
                byteforEntry = b;
                break;
            } else if (strcmp(test.entry.name, entry.entry.name) == 0) {
                std::cout << "File with same name already exists in directory."
                 << std::endl;
                 return 1;
            }
        }
        if (positionFound) break;
    }
    if (!positionFound) {
        std::cout << "Cannot contain more than " << sectorsForDir*16 << " entries" << std::endl;
        return 1;
    }
    // update buffer
    for (int i = byteforEntry, j = 0; j < 32; ++i, ++j) {
        buf[i] = entry.str[j];
    }
    // write the file entry
    writeSector(currentDir + sectorForEntry, buf);
    updateStatus(currentDir + sectorForEntry, DIR_ENTRY);

    // write data and update sector status
    for (int i = 0; i < sectorsNeeded; ++i) {
        for (int j = 0; j < kSectorSize; ++j) {
            buf[j] = file_content[kSectorSize*i + j];
        }
        writeSector(sectorsFree[i], buf);
        updateStatus(sectorsFree[i],
            (i != sectorsNeeded-1) ? sectorsFree[i+1] : 1);
    }
    std::cout << "CREATED FILE WITH" << std::endl;
    std::cout << "TITLE = " << entry.entry.name << std::endl;
    std::cout << "SIZE = " <<
        entry.entry.size << " bytes" << std::endl;
    std::cout << "STORED IN SECTORS = ";
    for (int i = 0; i < sectorsNeeded; ++i)
        std::cout << sectorsFree[i] << " ";
    std::cout << std::endl;

    return 0;
}
