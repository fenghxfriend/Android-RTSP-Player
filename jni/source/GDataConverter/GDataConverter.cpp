#include "GDataConverter.h"

GDataConverter*	GDataConverter::m_pInstance = NULL;
GDataConverter*	GDataConverter::sharedInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new GDataConverter();
	return m_pInstance;
}

bool GDataConverter::setIntField(JNIEnv *env, jobject obj, const char *fieldName, int value)
{
	bool result = false;

	do
	{
		if ( !env || !obj || !fieldName ) break;

		jclass clazz = ( env )->GetObjectClass( obj );
		if ( !clazz ) break;

		jfieldID fieldID = ( env )->GetFieldID( clazz, fieldName, "I" );
		if ( !fieldID ) break;

		( env )->SetIntField( obj, fieldID, value);

		result = true;

	} while(0);

	return result;
}

bool GDataConverter::getIntField(JNIEnv *env, jobject obj, const char *fieldName, jint* pValue)
{
	bool result = false;

	do
	{
		if ( !env || !obj || !fieldName ) break;

		jclass clazz = ( env )->GetObjectClass( obj );
		if ( !clazz ) break;

		jfieldID fieldID = ( env )->GetFieldID( clazz, fieldName, "I" );
		if ( !fieldID ) break;
		*pValue = ( env )->GetIntField(obj, fieldID);
		result = true;

	} while(0);

	return result;
}

int GDataConverter::setByteArrayField(JNIEnv *env, jobject obj, const char *fieldName, const char *value, int size)
{
	int result = -1;

	do
	{
		if ( !env || !obj || !fieldName || !value || size <= 0 ) break;
		result = strlen(value);
		if ( result <= 0 || result > size ) { break; }

		jclass clazz = ( env )->GetObjectClass( obj );
		if ( !clazz ) { break; }

		jfieldID fieldID = ( env )->GetFieldID( clazz, fieldName, "[B" );
		if ( !fieldID ) { break; }

		jbyteArray byteArray = ( jbyteArray )( ( env )->GetObjectField( obj, fieldID ) );

		if ( !byteArray ) { break; }

		if ( ( ( env )->GetArrayLength( byteArray ) ) != size ) { break; }

		( env )->SetByteArrayRegion( byteArray, 0, result, (jbyte *)value );

	} while(0);

	return result;
}

bool GDataConverter::setIntArrayField(JNIEnv *env, jobject obj, const char *fieldName, const int *value, int size)
{
	bool result = false;

	do
	{
		if ( !env || !obj || !fieldName || !value || size <= 0 ) break;

		jclass clazz = ( env )->GetObjectClass( obj );
		if ( !clazz ) break;

		jfieldID fieldID = ( env )->GetFieldID( clazz, fieldName, "[I" );
		if ( !fieldID ) break;

		jintArray intArray = ( jintArray )( ( env )->GetObjectField( obj, fieldID ) );

		if ( !intArray ) break;

		if ( ( ( env )->GetArrayLength( intArray ) ) != size ) break;

		( env )->SetIntArrayRegion( intArray, 0, size, value );

		result = true;

	} while(0);

	return result;
}

bool GDataConverter::set2DInt2ArrayField(JNIEnv *env, jobject obj, const char *fieldName, const int (*value)[2], int size)
{
	bool result = false;

	do
	{
		if ( !env || !obj || !fieldName || !value || size <= 0 ) break;

		jclass clazz = ( env )->GetObjectClass( obj );
		if ( !clazz ) break;

		jfieldID fieldID = ( env )->GetFieldID( clazz, fieldName, "[[I" );
		if ( !fieldID ) break;

		jobjectArray objectArray = ( jobjectArray )( ( env )->GetObjectField( obj, fieldID) );
		if ( !objectArray ) break;

		if ( ( ( env )->GetArrayLength( objectArray ) ) != size ) break;

		jintArray intArray;
		for ( int i = 0; i < size; i++ )
		{
			intArray = ( jintArray )( ( env )->GetObjectArrayElement( objectArray, i ) );
			( env )->SetIntArrayRegion( intArray, 0, 2, value[i] );
		}

		result = true;

	} while(0);

	return result;
}

int GDataConverter::getStringBytes(JNIEnv *env, jstring obj, char *buffer, int size)
{
	int result = -1;

	do
	{
		if ( !env || !obj || !buffer || size <= 0 ) break;

		const char *stringPtr 	= ( env )->GetStringUTFChars( obj, NULL );
		//printf("RTSP server addr is %s",stringPtr);
		if ( !stringPtr ) break;

		result = strlen( stringPtr );
		//if ( result <= size && result > 0 )
		if ( result < size && result > 0 )
		{
			strncpy( buffer, stringPtr, size );
		}
		( env )->ReleaseStringUTFChars( obj, stringPtr );

	} while(0);

	return result;
}

int GDataConverter::getStringBytes(JNIEnv *env, jobject obj, const char *fieldName, char *buffer, int size)
{
	int result = -1;

	do
	{
		if ( !env || !obj || !fieldName || !buffer || size <= 0 ) break;

		jclass clazz = ( env )->GetObjectClass( obj );
		if ( !clazz ) break;

		jfieldID fieldID = ( env )->GetFieldID( clazz, fieldName, "Ljava/lang/String;" );
		if ( !fieldID ) break;

		jstring stringObject = ( jstring )( ( env )->GetObjectField( obj, fieldID) );
		if ( !stringObject ) break;

		result = getStringBytes( env, stringObject, buffer, size );

	} while(0);

	return result;
}

jstring GDataConverter::StringToJString(JNIEnv* env, char* pchPat)
{
	if(pchPat==NULL||strlen(pchPat) == 0)
		return NULL;
	env->PushLocalFrame(10);
	int nPatLen = strlen(pchPat);
	jclass jclass_String = env->FindClass("java/lang/String");
	jmethodID jinit_method= env->GetMethodID(jclass_String, "<init>", "([BLjava/lang/String;)V");
	jbyteArray jbytes = env->NewByteArray(nPatLen);
	env->SetByteArrayRegion(jbytes, 0, nPatLen, (jbyte*)pchPat);
	jstring jencoding = env->NewStringUTF("UTF-8");
	jobject jobj =env->NewObject(jclass_String, jinit_method, jbytes, jencoding);
	env->DeleteLocalRef(jclass_String);
	env->DeleteLocalRef(jbytes);
	env->DeleteLocalRef(jencoding);
	jstring result=(jstring)env->PopLocalFrame(jobj);

	return result;
}


