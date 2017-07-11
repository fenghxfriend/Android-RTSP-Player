#ifndef GMAudioRecorder_h
#define GMAudioRecorder_h

#include <iostream>
#include <memory>
#include <string>

using namespace std;

class GMAudioRecorder
{
public:
    void install();
    void start();
    int	 read(void *buffer, int maxSize);
    void stop();
    void unInstall();
};

#endif
