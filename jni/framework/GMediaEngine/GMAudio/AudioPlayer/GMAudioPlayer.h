#ifndef GMAudioPlayer_h
#define GMAudioPlayer_h

#include <iostream>
#include <memory>
#include <string>

using namespace std;

class GMAudioPlayer
{
public:
    void install();
    void start();
    void feedData(void *data, unsigned size);
    void stop();
    void unInstall();
};

#endif
