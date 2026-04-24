#include <fstream>
#include <string>

#include "meminfo.h"

std::ifstream &operator>>(std::ifstream &filestream, MemInfo &meminfo)
{

    std::string key, unit;
    float value;

    while (filestream >> key >> value >> unit)
    {
        // Check if key exists and ignore undefined names/keys,
        // in case file entries change with system configuration.
        auto item = meminfo.att_map_.find(key);
        if (item != meminfo.att_map_.end())
        {
            *(item->second) = value;
        }
    }
    return filestream;
};

MemInfo::MemInfo(std::ifstream &ifs)
{
    if (ifs.is_open())
    {
        ifs >> *this;
    }
}
