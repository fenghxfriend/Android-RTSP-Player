#ifndef GAudioStreamRecorder_h
#define GAudioStreamRecorder_h

#include <memory>
#include <string>

using namespace std;

class GAudioStreamRecorder
{
public:
	void install();
	void unInstall();
    void start();
    int	 read(void *buffer, int maxSize);
    void stop();
};

#endif
