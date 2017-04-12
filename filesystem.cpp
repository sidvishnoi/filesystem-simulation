#include "filesystem.h"

int main(int argc, char const *argv[]) {
    std::string diskTitle;
    int diskSizeBytes;

    // get disk title
    if (argc > 1 && argv[1]) {
        diskTitle = argv[1];
    } else {
        std::cout << "Enter path to disk: ";
        std::cin >> diskTitle;
    }

    // check if disk doesn't exists or is empty
    //  and if exists, get it's size in bytes
    bool alreadyExists = false;
    std::fstream fin;
    fin.open(diskTitle, std::ios::binary | std::ios::in | std::ios::ate);
    diskSizeBytes = fin.tellg();
    fin.close();
    alreadyExists = (diskSizeBytes > -1);

    // get size if not exists
    if (!alreadyExists) {
        int diskSizeMB;
        if (argc > 2) {
            std::istringstream iss(argv[2]);
            iss >> diskSizeMB;
        } else {
            std::cout << "Enter disk size in MiB : ";
            std::cin >> diskSizeMB;
            std::cin.ignore(32767, '\n');
        }
        diskSizeBytes = kOneMiB_g*diskSizeMB;
    }

    // initialize disk
    FileSystem disk(diskTitle, diskSizeBytes, alreadyExists);
    std::cout << "Type `help` to see available commands." << std::endl;

    int response = handleInput(disk);
    while (response != 1) {
        response = handleInput(disk);
    }

    return 0;
}