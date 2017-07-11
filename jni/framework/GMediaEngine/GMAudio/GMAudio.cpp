#include "GMAudio.h"
#include "Controller/GMAudioController.h"
#include "../GMUtility/GMUtility.h"
#include "RTSPServer/GMAudioRTSPServer.h"

void GMAudioInstall()
{
    GMAudioRTSPServer::sharedInstance().install();
}

void GMAudioClientStart(const char *url, void *didplaycallback, void *didstopcallback, void *data)
{
    GMAudioController::sharedInstance()->start(url, (GMRTSPDIDPLYFUN)didplaycallback, (GMRTSPDIDPLYFUN)didstopcallback, data);
}

void GMAudioClientStop()
{
    GMAudioController::sharedInstance()->stop();
}

void GMAudioServerStart()
{
    GMAudioRTSPServer::sharedInstance().start();
}

void GMAudioServerStop()
{
    GMAudioRTSPServer::sharedInstance().stop();
}

void GMAudioUnInstall()
{
    GMAudioRTSPServer::sharedInstance().unInstall();
}

unsigned short  GMAudioServerPort()
{
    return GMAudioRTSPServer::sharedInstance().serverPort();
}

bool GMAudioParseFileDuration(char const* fileName, int isSystemMusic)
{
    return GMAudioRTSPServer::sharedInstance().parseAudioFileDuration(fileName, isSystemMusic);
}
