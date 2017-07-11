#ifndef GScreenshotManager_h
#define GScreenshotManager_h

#include <jni.h>
#include <sys/types.h>
#include <memory>
#include <string>
#include <time.h>

using namespace std;

#define	GScreenshotManagerActionInterval ( 5000000 ) // 5.0√Î

class GScreenshotManager
{
public:
    static  GScreenshotManager &sharedInstance();
    GScreenshotManager();
    ~GScreenshotManager();
    void	start();
    int		stop();

private:
    struct timeval	mActionDateTime;
    struct timeval	mActiveDateTime;
};

#endif
