
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "json.h"

using namespace zzjson;

std::string getJsonStr(const std::string& filename) {
    std::ifstream ifstrm(filename);
    if(!ifstrm.is_open()) {
        throw std::runtime_error("Can't open " + std::string(filename));
    }
    std::string jsonStr;
    while(ifstrm) {
        std::string line;
        std::getline(ifstrm, line);
        jsonStr += line + "\n";
    }
    return jsonStr;
}

void failJson(const std::string& filename) {
    std::string jsonStr = getJsonStr(filename);
    std::string errMsg;
    Json json_ = Json::parse(jsonStr, errMsg);
    if(errMsg != "") {
        std::cerr << "ERROR! Expect fail, but passed!" << std::endl;
        std::cerr << "File: " << filename << std::endl;
        std::cerr << jsonStr << std::endl;
        std::cerr << std::endl;
    }
}

void passJson(const std::string& filename) {
    std::string jsonStr = getJsonStr(filename);
    std::string errMsg;
    Json json_ = Json::parse(jsonStr, errMsg);
    if(errMsg != "") {
        std::cerr << "ERROR! Expect pass, but failed!" << std::endl;
        std::cerr << "File: " << filename << std::endl;
        std::cerr << jsonStr << std::endl;
        std::cerr << std::endl;
    }
}

int main() {
    struct stat stat;
    
    if(lstat(".", &stat) < 0) {
        std::cerr << "Lstat Error!" << std::endl;
        return 0;
    }

    DIR* dp = opendir("../Data");
    if(!dp) {
        perror("Can't open data.");
        return 0;
    }

    dirent* dirp;
    while((dirp = readdir(dp)) != nullptr) {
        std::string filename = "../Data";
        switch (dirp->d_name[0])
        {
            case 'f': {
                failJson(filename + dirp->d_name);
                break;
            }
            case 'p': {
                passJson(filename + dirp->d_name);
                break;
            }
            default: {
                break;
            }
        }
    }
    closedir(dp);
}