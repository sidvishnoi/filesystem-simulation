#include "filesystem.h"

int handleInput(FileSystem &disk) {
    /*
    objective: to create a terminal like CLI to handle disk operations
    input: reference to disk
    return:
        0: continue
        1: exit
    */
    std::string command;
    std::cout << KGRN << "(myFileSystem)> " << KRST;
    getline(std::cin, command);
    if (command.empty()) return 1;

    // split command into function and argument part
    std::vector<std::string> tokens;
    tokens = split(command, ' ');
    std::string fun = tokens[0];
    std::string arg = "";
    if (tokens.size() > 1) {
        arg = tokens[1];
    }

    std::string arg2 = "";
    if (tokens.size() > 2) {
        arg2 = tokens[2];
    }

    enum Choice {
        HELP, INFO, TREE, READ, CHECK, WRITE, FORMAT, EXIT, NEW_FILE, CAT, DEL, UNDEL, MKDIR, CD, PWD, LS, END
    };
    std::string CHOICES[] = {
        "help", "info", "tree", "read", "check", "write", "format", "exit", "new", "cat", "del", "undel", "mkdir", "chdir", "pwd", "list", ""
    };
    std::string choices[] = {
        "h", "info", "tree", "r", "c", "w", "f", "q", "n", "cat", "del", "undel", "mkdir", "cd", "pwd", "ls", ""
    };

    Choice selected = END;
    for (int i = 0; i < END; ++i) {
        if (CHOICES[i] == fun || choices[i] == fun) {
            selected = static_cast<Choice>(i);
            break;
        }
    }

    int sector;
    // handle command
    switch (selected) {
        case READ: {
            if (arg.empty()) {
                std::cout << "usage: read SECTOR" << std::endl;
                return 0;
            }
            // convert string to int
            std::istringstream(arg) >> sector;
            char readBuffer[kSectorSize_g];
            disk.readSector(sector, readBuffer);
            for (int i = 0; i < kSectorSize_g; ++i) {
                std::cout << readBuffer[i];
            }
            std::cout << std::endl;
            break;
        }
        case WRITE: {
            if (arg.empty()) {
                std::cout << "usage: write SECTOR" << std::endl;
                return 0;
            }
            std::istringstream(arg) >> sector;
            if (disk.getStatus(sector) == FileSystem::BUSY) {
                std::cout << "Overwrite sector data (y/n)? _\b";
                char t;
                std::cin >> t;
                if (t == 'y' || t == 'Y') {
                    char buf[kSectorSize_g];
                    createBuffer('.', buf);
                    disk.writeSector(sector, buf);
                } else {
                    std::cin.ignore(32767, '\n');
                    return 0;
                }
            } else {
                char buf[kSectorSize_g];
                createBuffer('.', buf);
                disk.writeSector(sector, buf);
                disk.updateStatus(sector, 1);
            }
            break;
        }
        case CHECK: {
            if (arg.empty()) {
                std::cout << "usage: check SECTOR" << std::endl;
                return 0;
            }
            std::istringstream(arg) >> sector;
            int status = disk.getStatus(sector);
            std::cout << "sector #" << sector << " is ";
            if (status < disk.END && status > -1) {
                std::cout << disk.statusMsg[status] << std::endl;
            } else {
                std::cout << "pointing to sector #" << status << std::endl;
            }
            break;
        }
        case FORMAT:
            disk.format();
            break;
        case HELP: {
            std::ifstream helpFile;
            helpFile.open("help.txt", std::ios::in | std::ios::ate);
            int helpFileSize = helpFile.tellg();
            char helpMsg[helpFileSize];
            helpFile.seekg(0);
            helpFile.read(helpMsg, helpFileSize);
            helpFile.close();
            std::cout << helpMsg << std::endl;
            break;
        }
        case INFO:
            disk.info();
            break;
        case TREE:
            disk.tree();
            break;
        case NEW_FILE: {
            if (arg.empty()) {
                std::cout << "usage: new FILE_NAME [SRC_FILE]" << std::endl;
                return 0;
            }
            disk.createFile(arg.c_str(), arg2.c_str());
            break;
        }
        case CAT: {
            if (arg.empty()) {
                std::cout << "usage: cat FILE_NAME" << std::endl;
                return 0;
            }
            disk.readFile(arg.c_str());
            break;
        }
        case DEL: {
            if (arg.empty()) {
                std::cout << "usage: del FILE_NAME" << std::endl;
                return 0;
            }
            disk.deleteFile(arg.c_str());
            break;
        }
        case UNDEL: {
            if (arg.empty()) {
                std::cout << "usage: undel FILE_NAME" << std::endl;
                return 0;
            }
            disk.undeleteFile(arg.c_str());
            break;
        }
        case MKDIR: {
            if (arg.empty()) {
                std::cout << "usage: mkdir DIR_NAME" << std::endl;
                return 0;
            }
            disk.createDir(arg.c_str());
            break;
        }
        case CD: {
            if (arg.empty()) {
                std::cout << "usage: cd DIR_NAME" << std::endl;
                return 0;
            }
            std::vector<std::string> dir = split(arg, '/');
            for (int i = 0; i < dir.size(); ++i) {
                disk.changeDir(dir[i].c_str());
            }
            break;
        }
        case PWD:
            disk.printWorkingDir();
            break;
        case LS:
            disk.listDirectoryContents();
            break;
        case EXIT:
            return 1;
        default:
            std::cout << "(myFileSystem): command not found: " << fun << std::endl;
            return 0;
    }
    return 0;
}