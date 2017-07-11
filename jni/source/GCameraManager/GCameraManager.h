#ifndef GCameraManager_h
#define GCameraManager_h

#include <jni.h>
#include <sys/types.h>
#include <memory>
#include <string>
#include <time.h>

using namespace std;

#define	GCameraManagerActionInterval 	( 1000000 ) // 1.0√Î
#define	GCameraManagerTakePhotoInterval ( 1500000 ) // 1.5√Î

class GCameraManager
{
public:
    static  GCameraManager &sharedInstance();
    void	install();
    int		moveUp();
    int		moveDown();
    void	stop();
    int		currentCanTakePhoto();
    void	unInstall();

private:
    bool 	isActionValid();

private:
    struct timeval	mLastActionDateTime;
    struct timeval	mActiveDateTime;
    struct timeval  mLastTakePhotoDateTime;
};

#endif
