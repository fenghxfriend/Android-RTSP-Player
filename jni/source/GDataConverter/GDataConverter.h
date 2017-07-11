#ifndef GDataConverter_h
#define GDataConverter_h

#include <jni.h>
#include <memory>
#include <string>

using namespace std;

class GDataConverter
{
public:
    static GDataConverter*	sharedInstance();
	bool setIntField		(JNIEnv *env, jobject obj, const char *fieldName, int 		 value			      );
    bool getIntField(JNIEnv *env, jobject obj, const char *fieldName, jint* value);
	int  setByteArrayField	(JNIEnv *env, jobject obj, const char *fieldName, const char *value		, int size);
	bool setIntArrayField	(JNIEnv *env, jobject obj, const char *fieldName, const int  *value		, int size);
	bool set2DInt2ArrayField(JNIEnv *env, jobject obj, const char *fieldName, const int  (*value)[2], int size);
	int  getStringBytes		(JNIEnv *env, jobject obj, const char *fieldName, char 		 *buffer	, int size);
	int  getStringBytes		(JNIEnv *env, jstring obj, char 	  *buffer	, int 		 size				  );
    jstring StringToJString(JNIEnv* env, char* pchPat);
private:
    static GDataConverter*   m_pInstance;
    class Garbo
    {

    public:

        ~Garbo()

        {
            if (GDataConverter::m_pInstance)

            {
                delete GDataConverter::m_pInstance;
                m_pInstance = NULL;
            }
        }
    };

    static Garbo garbo;
};

#endif
