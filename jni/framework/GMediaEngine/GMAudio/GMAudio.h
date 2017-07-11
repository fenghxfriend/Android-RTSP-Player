#ifndef GAEAudio_h
#define GAEAudio_h

#ifdef __cplusplus
extern "C"
{
#endif
    void            GMAudioInstall();
    void            GMAudioClientStart(const char *url, void *didplaycallback, void *didstopcallback, void *data);
    void            GMAudioClientStop();
    void            GMAudioServerStart();
    void            GMAudioServerStop();
    void            GMAudioUnInstall();
    unsigned short  GMAudioServerPort();
    bool            GMAudioParseFileDuration(char const* fileName, int isSystemMusic);
#ifdef __cplusplus
}
#endif

#endif
