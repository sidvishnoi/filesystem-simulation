#ifndef DISK
#define DISK

#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <iomanip>
#include <vector>
#include <sstream>

#ifndef COLORS    // colors for terminal
#define COLORS ""
#define KRST  "\x1B[0m"
#define KGRN "\x1b[32m"
#define KYEL "\x1b[33m"
#define KBLU  "\x1B[34m"
#define DASH u8"\u2015" << u8"\u2015" << " "
#endif

// int to char[4] typecast
typedef struct {
    union {
        int num;
        char str[4];
    };
} TypeCastInt;

// directory entry
typedef struct {
    int  startsAt;
    int  size;
    char type;    // y/n
    int  parent;
    char name[19];
} dirEntry;

typedef struct {
    union {
        dirEntry entry;
        char str[32];
    };
} TypeCastEntry;

const int kSectorSize_g = pow(2, 9);
const int kOneMiB_g = pow(2, 20);   // 2^20 bytes = 1MiB
const int MAX_FILE_SIZE = pow(2, 19);

class FileSystem {
    // FileSystem class
 private:
    const int kSectorSize = kSectorSize_g;
    std::string kDiskTitle;
    int kDiskSize;
    int kReservedSectors;
    int kNumberOfSectors;
    int currentDir;
    int parentDir;
    const int sectorsForDir = 2;
    void create(bool format = false, bool alreadyExists = false);
    int findFreeSectors(int sectorsNeeded, std::vector<int> &sectorsFree);

 public:
    FileSystem(std::string title, int diskSize, bool alreadyExists);
    enum Status {FREE = 0, BUSY, RESERVED, NOT_FOUND, DIR_ENTRY, DELETED, END};
    const std::vector<std::string> statusMsg {
        "FREE", "BUSY", "RESERVED", "NOT_FOUND", "DIR_ENTRY", "DELETED", "END"
    };
    void format();
    int getStatus(int sector_no) const;
    int updateStatus(int sector_no, int x);
    void readSector(int sector_no, char *buf) const;
    void writeSector(int sector_no, char *buf);
    int createFile(const char *title, const char *srcFile);
    int readFile(const char *title);
    int deleteFile(const char *title);
    int undeleteFile(const char *title);
    int createDir(const char *title);
    int changeDir(const char *title);
    int printWorkingDir();
    void listDirectoryContents();
    void info();
    void tree(int level = 1, int spacing = 2);
};

std::vector<std::string> split(const std::string &s, const char delim);

void createBuffer(char temp, char* buf);

int handleInput(FileSystem &disk);

#endif