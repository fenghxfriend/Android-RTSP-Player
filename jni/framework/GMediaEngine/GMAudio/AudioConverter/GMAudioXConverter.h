#ifndef GMAudioXConverter_h
#define GMAudioXConverter_h

#include <iostream>
#include <stdint.h>

using namespace std;

class GMAudioXConverter
{
public:
    GMAudioXConverter();
    ~GMAudioXConverter();
    bool   loadFile(const char *path);
    void   readData(void *buffer, unsigned &actualframes, unsigned maxframes);
    void   seek(double time);
    double duration();
    bool   isEOF();
    static float mediaDuration(char const* fileName);

private:
    void init();
    void destory();
    int fileSize();

private:
    FILE *mFile;
    int  mFileSize;
    int  mFileRemainSize;
    int  mActiveReadSize;
    int  mActiveReadedSize;
    bool mLoaded;
};

#endif
