#ifndef GTasksKiller_h
#define GTasksKiller_h

#include <jni.h>
#include <memory>
#include <string>

using namespace std;

class GTasksKiller
{
public:
    static GTasksKiller &sharedInstance();
    void sendKillCommand(int id);
    void sendKilledMessage(int id);
    void sendAllKilledMessage();
};

#endif
