#ifndef DATACLASS_H
#define DATACLASS_H

#include <string>
#include <QString>

using namespace std;

class CpuInfo{

public:
    int CpuNum;
    string name;
    string CapOfCache;
    string freq;

};

class MemInfo{
public:
    string MemTotal;
    string MemFree;
    string Buffers;
    string Cached;
    string SwapTotal;
    string SwapFree;
};

class LoadInfo{
public:
    double m_OneMin;
    double m_FivMin;
    double m_FiftMin;
};

class BasicInfo{
public:
    string hostname;
    string kernel_version;
    string gcc_version;
    string cpu_name;
};

class DiskInfo{
public:
    QString capacity;
    QString free;
    double used_pecent;
};


#endif // DATACLASS_H
