#include <jni.h>

#include "plplot/plplot.h"

static jclass cls_PLStream = 0;
static jfieldID fid_sid = 0;

#define set_PLStream(env,obj) plsstrm( (*env)->GetIntField(env,obj,fid_sid) )

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    adv
 * Signature: (I)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_adv( JNIEnv *env, jobject jthis, jint page )
{
    set_PLStream(env,jthis);
    pladv(page);
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    box
 * Signature: (Ljava/lang/String;FILjava/lang/String;FI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_box( JNIEnv *env, jobject jthis,
                               jstring jxopt, jfloat xtick, jint nxsub,
                               jstring jyopt, jfloat ytick, jint nysub )
{
    const char *xopt = (*env)->GetStringUTFChars( env, jxopt, 0 );
    const char *yopt = (*env)->GetStringUTFChars( env, jyopt, 0 );

    set_PLStream(env,jthis);
    plbox( xopt, xtick, nxsub, yopt, ytick, nysub );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    box3
 * Signature: (Ljava/lang/String;Ljava/lang/String;FILjava/lang/String;Ljava/lan
g/String;FILjava/lang/String;Ljava/lang/String;FI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_box3( JNIEnv *env, jobject jthis,
                                jstring xopt, jstring xlabel, jfloat xtick, jint nsubx,
                                jstring yopt, jstring ylabel, jfloat ytick, jint nsuby,
                                jstring zopt, jstring zlabel, jfloat ztick, jint nsubz )
{
    set_PLStream(env,jthis);
    printf( "box3 unimplemented.\n" );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    col0
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_plplot_core_PLStream_col0( JNIEnv *env, jobject jthis, jint icol )
{
    set_PLStream(env,jthis);
    plcol0(icol);
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    end
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_plplot_core_PLStream_end( JNIEnv *env, jobject jthis )
{
    set_PLStream(env,jthis);
    plend();
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    env
 * Signature: (FFFFII)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_env( JNIEnv *env, jobject jthis,
                               jfloat xmin, jfloat xmax,
                               jfloat ymin, jfloat ymax,
                               jint just, jint axis )
{
    set_PLStream(env,jthis);
    plenv( xmin, xmax, ymin, ymax, just, axis );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    fontld
 * Signature: (I)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_fontld( JNIEnv *env, jobject jthis, jint fnt )
{
    set_PLStream(env,jthis);
    plfontld(fnt);
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    gstrm
 * Signature: ()I
 */

JNIEXPORT jint JNICALL
Java_plplot_core_PLStream_gstrm( JNIEnv *env, jobject jthis )
{
    PLINT sid;
    plgstrm( &sid );
    return sid;
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    init
 * Signature: ()V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_init( JNIEnv *env, jobject jthis )
{
    PLINT sid;

    plinit();
    plgstrm( &sid );

    if (cls_PLStream == 0)
    {
        jclass cls = (*env)->GetObjectClass( env, jthis );
        cls_PLStream = (*env)->NewGlobalRef( env, cls );
        fid_sid = (*env)->GetFieldID( env, cls_PLStream, "stream_id", "I" );
    }

    (*env)->SetIntField( env, jthis, fid_sid, sid );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    join
 * Signature: (FFFF)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_join__FFFF( JNIEnv *env, jobject jthis,
                                      jfloat x1, jfloat y1,
                                      jfloat x2, jfloat y2 )
{
    set_PLStream(env,jthis);
    pljoin( x1, y1, x2, y2 );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    join
 * Signature: (DDDD)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_join__DDDD( JNIEnv *env, jobject jthis,
                                      jdouble x1, jdouble y1,
                                      jdouble x2, jdouble y2 )
{
    set_PLStream(env,jthis);
    pljoin( x1, y1, x2, y2 );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    lab
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_lab( JNIEnv *env, jobject jthis,
                               jstring jxlab, jstring jylab, jstring jtlab )
{
    const char *xlab = (*env)->GetStringUTFChars( env, jxlab, 0 );
    const char *ylab = (*env)->GetStringUTFChars( env, jylab, 0 );
    const char *tlab = (*env)->GetStringUTFChars( env, jtlab, 0 );

    set_PLStream(env,jthis);
    pllab( xlab, ylab, tlab );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    line
 * Signature: (I[F[F)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_line( JNIEnv *env, jobject jthis,
                                jint n, jfloatArray jx, jfloatArray jy )
{
    jsize len = (*env)->GetArrayLength( env, jx );
    jfloat *x = (*env)->GetFloatArrayElements( env, jx, 0 );
    jfloat *y = (*env)->GetFloatArrayElements( env, jy, 0 );

    set_PLStream(env,jthis);
    plline( n, x, y );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    poin
 * Signature: (I[F[FI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_poin( JNIEnv *env, jobject jthis,
                                jint n, jfloatArray jx, jfloatArray jy,
                                jint code )
{
    jsize len = (*env)->GetArrayLength( env, jx );
    jfloat *x = (*env)->GetFloatArrayElements( env, jx, 0 );
    jfloat *y = (*env)->GetFloatArrayElements( env, jy, 0 );

    set_PLStream(env,jthis);
    plpoin( n, x, y, code );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    styl
 * Signature: (III)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_styl( JNIEnv *env, jobject jthis,
                                jint nms, jint mark, jint space )
{
    set_PLStream(env,jthis);
    plstyl( nms, (PLINT *) &mark, (PLINT *) &space );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    syax
 * Signature: (II)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_syax( JNIEnv *env, jobject jthis,
                                jint digmax, jint digits )
{
    set_PLStream(env,jthis);
    plsyax( digmax, digits );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    vsta
 * Signature: ()V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_vsta( JNIEnv *env, jobject jthis )
{
    set_PLStream(env,jthis);
    plvsta();
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    wind
 * Signature: (FFFF)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_wind( JNIEnv *env, jobject jthis,
                                jfloat xmin, jfloat xmax,
                                jfloat ymin, jfloat ymax )
{
    set_PLStream(env,jthis);
    plwind( xmin, xmax, ymin, ymax );
}

