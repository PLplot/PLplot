#include <jni.h>

#include "plplot/plplot.h"

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    adv
 * Signature: (I)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_adv( JNIEnv *env, jobject jthis, jint page )
{
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
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    col0
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_plplot_core_PLStream_col0( JNIEnv *env, jobject jthis, jint icol )
{
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
    plfontld(fnt);
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    init
 * Signature: ()V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_init( JNIEnv *env, jobject jthis )
{
    plinit();
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
    plvsta();
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    wind
 * Signature: (FFFF)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_wind( JNIEnv *env, jobject this,
                                jfloat xmin, jfloat xmax,
                                jfloat ymin, jfloat ymax )
{
    plwind( xmin, xmax, ymin, ymax );
}

