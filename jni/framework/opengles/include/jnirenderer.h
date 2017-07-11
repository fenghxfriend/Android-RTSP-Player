#ifndef _ANTS360_JNIRENDERER_H_
#define _ANTS360_JNIRENDERER_H_

#include <jni.h>

extern "C" {

namespace ants360{

/*!
 * \brief   load java native interface for
 *          com.video.draw.Renderer
 */
int LoadRendererJNI(JNIEnv* env);

} // namespace ants360

}

#endif // _ANTS360_JNIRENDERER_IMPL_H_