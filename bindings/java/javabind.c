/*---------------------------------------------------------------------------//
 * $Id$
 *
 * This file implements the JNI wrappers for the PLplot API functions.  Most
 * of these show up as public native methods of the PLStream.java class.
 *
 * Each of the public methods of PLStream must first set the current PLplot
 * stream, before invoking its corresponding PLplot API function.  This is
 * done by fetching the stream_id field from the PLStream object.
 *
 * mkstrm() is a private method because a Java language user has no need to
 * ever call this PLplot API function directly.  When a Java user wants a new
 * plotting stream, they just instantiate a new PLStream class.  So the
 * PLStream ctor calls mkstrm(), and thus this is where we fetch the PLplot
 * stream id and set the object's stream_id member.
 *
 * Functions definitions are sorted alphabetically, except for mkstrm which
 * occurs first since it is special, as described above.
//---------------------------------------------------------------------------*/

#include <jni.h>

#include "plplot/plplotP.h"

static jclass cls_PLStream = 0;
static jfieldID fid_sid = 0;

/* Expand this macro inside each wrapper function, before calling the PLplot
 * API function. */
#define set_PLStream(env,obj) plsstrm( (*env)->GetIntField(env,obj,fid_sid) )

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    mkstrm
 * Signature: ()I
 */

JNIEXPORT jint JNICALL
Java_plplot_core_PLStream_mkstrm( JNIEnv *env, jobject jthis )
{
    PLINT sid;

/* We have to make sure that the stream_id field id is initialized.  mkstrm
 * is called by the PLStream ctor, so will be called before any of these
 * other native wrappers.  So this is the place to fetch this id. */
    if (cls_PLStream == 0)
    {
        jclass cls = (*env)->GetObjectClass( env, jthis );
        cls_PLStream = (*env)->NewGlobalRef( env, cls );
        fid_sid = (*env)->GetFieldID( env, cls_PLStream, "stream_id", "I" );
    /* Shouldda done some error checking there... */
    }

    plmkstrm( &sid );
    return sid;
}

/*---------------------------------------------------------------------------//
// First the arg parsing support routines.
//---------------------------------------------------------------------------*/

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    ClearOpts
 * Signature: ()V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_ClearOpts( JNIEnv *env, jobject jthis )
{
    set_PLStream(env,jthis);
    plClearOpts();
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    ResetOpts
 * Signature: ()V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_ResetOpts( JNIEnv *env, jobject jthis )
{
    set_PLStream(env,jthis);
    plResetOpts();
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    SetUsage
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_SetUsage( JNIEnv *env, jobject jthis,
                                    jstring program_string, jstring usage_string )
{
    const char *pgm_str = (*env)->GetStringUTFChars( env, program_string, 0 );
    const char *usg_str = (*env)->GetStringUTFChars( env, usage_string, 0 );

    set_PLStream(env,jthis);
    plSetUsage( plstrdup(pgm_str), plstrdup(usg_str) );
}

/* MergeOpts should go here. */

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    ParseOpts
 * Signature: ([Ljava/lang/String;I)I
 */

JNIEXPORT jint JNICALL
Java_plplot_core_PLStream_ParseOpts( JNIEnv *env, jobject jthis,
                                     jobjectArray jargs, jint mode )
{
    int argc;
    char **argv;
    int i, rc;
    set_PLStream(env,jthis);

/* Extract the string args from the java side. */
    argc = (*env)->GetArrayLength( env, jargs );

    argv = (char **) malloc( argc * sizeof(char *) );
    for( i=0; i < argc; i++ )
    {
        jstring jarg = (*env)->GetObjectArrayElement( env, jargs, i );
        argv[i] = plstrdup( (*env)->GetStringUTFChars( env, jarg, 0 ) );
    }

/* Note that in Java we don't get the usual C-style argv[0] program name. */
    rc = plParseOpts( &argc, argv, mode | PL_PARSE_NOPROGRAM );

/* Take stesp to update jargs with modifications made by plParseOpts(). */

    return rc;
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    OptUsage
 * Signature: ()V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_OptUsage( JNIEnv *env, jobject jthis )
{
    set_PLStream(env,jthis);
    plOptUsage();
}

/*---------------------------------------------------------------------------//
// Now the actual plotting API functions.
//---------------------------------------------------------------------------*/

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
Java_plplot_core_PLStream_box__Ljava_lang_String_2FILjava_lang_String_2FI(
    JNIEnv *env, jobject jthis,
    jstring jxopt, jfloat jxtick, jint nxsub,
    jstring jyopt, jfloat jytick, jint nysub )
{
    PLFLT xtick = jxtick, ytick = jytick;
    const char *xopt = (*env)->GetStringUTFChars( env, jxopt, 0 );
    const char *yopt = (*env)->GetStringUTFChars( env, jyopt, 0 );

    set_PLStream(env,jthis);
    plbox( xopt, xtick, nxsub, yopt, ytick, nysub );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    box
 * Signature: (Ljava/lang/String;DILjava/lang/String;DI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_box__Ljava_lang_String_2DILjava_lang_String_2DI(
    JNIEnv *env, jobject jthis,
    jstring jxopt, jdouble jxtick, jint nxsub,
    jstring jyopt, jdouble jytick, jint nysub )
{
    PLFLT xtick = jxtick, ytick = jytick;
    const char *xopt = (*env)->GetStringUTFChars( env, jxopt, 0 );
    const char *yopt = (*env)->GetStringUTFChars( env, jyopt, 0 );

    set_PLStream(env,jthis);
    plbox( xopt, xtick, nxsub, yopt, ytick, nysub );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    box3
 * Signature: (Ljava/lang/String;Ljava/lang/String;FILjava/lang/String;Ljava/lang/String;FILjava/lang/String;Ljava/lang/String;FI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_box3__Ljava_lang_String_2Ljava_lang_String_2FILjava_lang_String_2Ljava_lang_String_2FILjava_lang_String_2Ljava_lang_String_2FI(
    JNIEnv *env, jobject jthis,
    jstring jxopt, jstring jxlabel, jfloat jxtick, jint nsubx,
    jstring jyopt, jstring jylabel, jfloat jytick, jint nsuby,
    jstring jzopt, jstring jzlabel, jfloat jztick, jint nsubz )
{
    PLFLT xtick = jxtick, ytick = jytick, ztick = jztick;
    const char *xopt = (*env)->GetStringUTFChars( env, jxopt, 0 );
    const char *yopt = (*env)->GetStringUTFChars( env, jyopt, 0 );
    const char *zopt = (*env)->GetStringUTFChars( env, jzopt, 0 );
    const char *xlabel = (*env)->GetStringUTFChars( env, jxlabel, 0 );
    const char *ylabel = (*env)->GetStringUTFChars( env, jylabel, 0 );
    const char *zlabel = (*env)->GetStringUTFChars( env, jzlabel, 0 );

    set_PLStream(env,jthis);
    plbox3( xopt, xlabel, xtick, nsubx,
            yopt, ylabel, ytick, nsuby,
            zopt, zlabel, ztick, nsubz );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    box3
 * Signature: (Ljava/lang/String;Ljava/lang/String;DILjava/lang/String;Ljava/lang/String;DILjava/lang/String;Ljava/lang/String;DI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_box3__Ljava_lang_String_2Ljava_lang_String_2DILjava_lang_String_2Ljava_lang_String_2DILjava_lang_String_2Ljava_lang_String_2DI(
    JNIEnv *env, jobject jthis,
    jstring jxopt, jstring jxlabel, jdouble jxtick, jint nsubx,
    jstring jyopt, jstring jylabel, jdouble jytick, jint nsuby,
    jstring jzopt, jstring jzlabel, jdouble jztick, jint nsubz )
{
    PLFLT xtick = jxtick, ytick = jytick, ztick = jztick;
    const char *xopt = (*env)->GetStringUTFChars( env, jxopt, 0 );
    const char *yopt = (*env)->GetStringUTFChars( env, jyopt, 0 );
    const char *zopt = (*env)->GetStringUTFChars( env, jzopt, 0 );
    const char *xlabel = (*env)->GetStringUTFChars( env, jxlabel, 0 );
    const char *ylabel = (*env)->GetStringUTFChars( env, jylabel, 0 );
    const char *zlabel = (*env)->GetStringUTFChars( env, jzlabel, 0 );

    set_PLStream(env,jthis);
    plbox3( xopt, xlabel, xtick, nsubx,
            yopt, ylabel, ytick, nsuby,
            zopt, zlabel, ztick, nsubz );
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
 * Method:    col1
 * Signature: (F)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_col1__F( JNIEnv *env, jobject jthis, jfloat col1 )
{
    set_PLStream(env,jthis);
    plcol1(col1);
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    col1
 * Signature: (D)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_col1__D( JNIEnv *env, jobject jthis, jdouble col1 )
{
    set_PLStream(env,jthis);
    plcol1(col1);
}

/*
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
Java_plplot_core_PLStream_env__FFFFII( JNIEnv *env, jobject jthis,
                                       jfloat jxmin, jfloat jxmax,
                                       jfloat jymin, jfloat jymax,
                                       jint just, jint axis )
{
    PLFLT xmin = jxmin, xmax = jxmax;
    PLFLT ymin = jymin, ymax = jymax;
    set_PLStream(env,jthis);
    plenv( xmin, xmax, ymin, ymax, just, axis );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    env
 * Signature: (DDDDII)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_env__DDDDII( JNIEnv *env, jobject jthis,
                                       jdouble jxmin, jdouble jxmax,
                                       jdouble jymin, jdouble jymax,
                                       jint just, jint axis )
{
    PLFLT xmin = jxmin, xmax = jxmax;
    PLFLT ymin = jymin, ymax = jymax;
    set_PLStream(env,jthis);
    plenv( xmin, xmax, ymin, ymax, just, axis );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    fill
 * Signature: (I[F[F)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_fill__I_3F_3F( JNIEnv *env, jobject jthis,
                                         jint n,
                                         jfloatArray jx, jfloatArray jy )
{
    jsize len = (*env)->GetArrayLength( env, jx );
    jfloat *jxdata = (*env)->GetFloatArrayElements( env, jx, 0 );
    jfloat *jydata = (*env)->GetFloatArrayElements( env, jy, 0 );
    PLFLT *x, *y;
    int must_free_buffers = 0, i;

    if (sizeof(PLFLT) == sizeof(jfloat)) {
    /* Trick: The cast is here to shut up the compiler in the case where
     * PLFLT != float, in which case the /other/ branch is the one that is
     * actually executed. */
        x = (PLFLT *) jxdata;
        y = (PLFLT *) jydata;
    } else {
        x = (PLFLT *) malloc( n * sizeof(PLFLT) );
        y = (PLFLT *) malloc( n * sizeof(PLFLT) );

        for( i=0; i < n; i++ )
        {
            x[i] = jxdata[i];
            y[i] = jydata[i];
        }
        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plfill( n, x, y );

    if (must_free_buffers) {
        free( x );
        free( y );
    }

    (*env)->ReleaseFloatArrayElements( env, jx, jxdata, 0 );
    (*env)->ReleaseFloatArrayElements( env, jy, jydata, 0 );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    fill
 * Signature: (I[D[D)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_fill__I_3D_3D( JNIEnv *env, jobject jthis,
                                         jint n,
                                         jdoubleArray jx, jdoubleArray jy )
{
    jsize len = (*env)->GetArrayLength( env, jx );
    jdouble *jxdata = (*env)->GetDoubleArrayElements( env, jx, 0 );
    jdouble *jydata = (*env)->GetDoubleArrayElements( env, jy, 0 );
    PLFLT *x, *y;
    int must_free_buffers = 0, i;

    if (sizeof(PLFLT) == sizeof(jdouble)) {
    /* Trick: The cast is here to shut up the compiler in the case where
     * PLFLT != double, in which case the /other/ branch is the one that is
     * actually executed. */
        x = (PLFLT *) jxdata;
        y = (PLFLT *) jydata;
    } else {
        x = (PLFLT *) malloc( n * sizeof(PLFLT) );
        y = (PLFLT *) malloc( n * sizeof(PLFLT) );

        for( i=0; i < n; i++ )
        {
            x[i] = jxdata[i];
            y[i] = jydata[i];
        }
        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plfill( n, x, y );

    if (must_free_buffers) {
        free( x );
        free( y );
    }

    (*env)->ReleaseDoubleArrayElements( env, jx, jxdata, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jy, jydata, 0 );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    fill3
 * Signature: (I[F[F[F)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_fill3__I_3F_3F_3F( JNIEnv *env, jobject jthis,
                                             jint n, jfloatArray jx,
                                             jfloatArray jy, jfloatArray jz )
{
    jsize len = (*env)->GetArrayLength( env, jx );
    jfloat *jxdata = (*env)->GetFloatArrayElements( env, jx, 0 );
    jfloat *jydata = (*env)->GetFloatArrayElements( env, jy, 0 );
    jfloat *jzdata = (*env)->GetFloatArrayElements( env, jz, 0 );
    PLFLT *x, *y, *z;
    int must_free_buffers = 0, i;

    if (sizeof(PLFLT) == sizeof(jfloat)) {
    /* Trick: The cast is here to shut up the compiler in the case where
     * PLFLT != float, in which case the /other/ branch is the one that is
     * actually executed. */
        x = (PLFLT *) jxdata;
        y = (PLFLT *) jydata;
        z = (PLFLT *) jzdata;
    } else {
        x = (PLFLT *) malloc( n * sizeof(PLFLT) );
        y = (PLFLT *) malloc( n * sizeof(PLFLT) );
        z = (PLFLT *) malloc( n * sizeof(PLFLT) );

        for( i=0; i < n; i++ )
        {
            x[i] = jxdata[i];
            y[i] = jydata[i];
            z[i] = jzdata[i];
        }
        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plfill3( n, x, y, z );

    if (must_free_buffers) {
        free( x );
        free( y );
        free( z );
    }

    (*env)->ReleaseFloatArrayElements( env, jx, jxdata, 0 );
    (*env)->ReleaseFloatArrayElements( env, jy, jydata, 0 );
    (*env)->ReleaseFloatArrayElements( env, jz, jzdata, 0 );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    fill3
 * Signature: (I[D[D[F)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_fill3__I_3D_3D_3F( JNIEnv *env, jobject jthis,
                                             jint n, jdoubleArray jx,
                                             jdoubleArray jy, jfloatArray jz )
{
    jsize len = (*env)->GetArrayLength( env, jx );
    jdouble *jxdata = (*env)->GetDoubleArrayElements( env, jx, 0 );
    jdouble *jydata = (*env)->GetDoubleArrayElements( env, jy, 0 );
    jdouble *jzdata = (*env)->GetDoubleArrayElements( env, jz, 0 );
    PLFLT *x, *y, *z;
    int must_free_buffers = 0, i;

    if (sizeof(PLFLT) == sizeof(jdouble)) {
    /* Trick: The cast is here to shut up the compiler in the case where
     * PLFLT != double, in which case the /other/ branch is the one that is
     * actually executed. */
        x = (PLFLT *) jxdata;
        y = (PLFLT *) jydata;
        z = (PLFLT *) jzdata;
    } else {
        x = (PLFLT *) malloc( n * sizeof(PLFLT) );
        y = (PLFLT *) malloc( n * sizeof(PLFLT) );
        z = (PLFLT *) malloc( n * sizeof(PLFLT) );

        for( i=0; i < n; i++ )
        {
            x[i] = jxdata[i];
            y[i] = jydata[i];
            z[i] = jzdata[i];
        }
        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plfill3( n, x, y, z );

    if (must_free_buffers) {
        free( x );
        free( y );
        free( z );
    }

    (*env)->ReleaseDoubleArrayElements( env, jx, jxdata, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jy, jydata, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jz, jzdata, 0 );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    flush
 * Signature: ()V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_flush( JNIEnv *env, jobject jthis )
{
    set_PLStream(env,jthis);
    plflush();
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    font
 * Signature: (I)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_font( JNIEnv *env, jobject jthis, jint fnt )
{
    set_PLStream(env,jthis);
    plfont(fnt);
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
 * Method:    hist
 * Signature: (I[FFFII)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_hist__I_3FFFII( JNIEnv *env, jobject jthis,
                                          jint n, jfloatArray jdatarr,
                                          jfloat jdatmin, jfloat jdatmax,
                                          jint nbin, jint oldwin )
{
    PLFLT datmin = jdatmin, datmax = jdatmax;
    jfloat *jdata = (*env)->GetFloatArrayElements( env, jdatarr, 0 );
    PLFLT *data;
    int must_free_buffer = 0, i;
    
    if (sizeof(PLFLT) == sizeof(jfloat)) {
    /* Trick: The cast is here to shut up the compiler in the case where
     * PLFLT != float, in which case the /other/ branch is the one that is
     * actually executed. */
        data = (PLFLT *) jdata;
    } else {
        data = (PLFLT *) malloc( n * sizeof(PLFLT) );

        for( i=0; i < n; i++ )
            data[i] = jdata[i];

        must_free_buffer = 1;
    }

    set_PLStream(env,jthis);
    plhist( n, data, datmin, datmax, nbin, oldwin );

    if (must_free_buffer)
        free(data);

    (*env)->ReleaseFloatArrayElements( env, jdatarr, jdata, 0 );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    hist
 * Signature: (I[DDDII)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_hist__I_3DDDII( JNIEnv *env, jobject jthis,
                                          jint n, jdoubleArray jdatarr,
                                          jdouble jdatmin, jdouble jdatmax,
                                          jint nbin, jint oldwin )
{
    PLFLT datmin = jdatmin, datmax = jdatmax;
    jdouble *jdata = (*env)->GetDoubleArrayElements( env, jdatarr, 0 );
    PLFLT *data;
    int must_free_buffer = 0, i;
    
    if (sizeof(PLFLT) == sizeof(jdouble)) {
    /* Trick: The cast is here to shut up the compiler in the case where
     * PLFLT != float, in which case the /other/ branch is the one that is
     * actually executed. */
        data = (PLFLT *) jdata;
    } else {
        data = (PLFLT *) malloc( n * sizeof(PLFLT) );

        for( i=0; i < n; i++ )
            data[i] = jdata[i];

        must_free_buffer = 1;
    }

    set_PLStream(env,jthis);
    plhist( n, data, datmin, datmax, nbin, oldwin );

    if (must_free_buffer)
        free(data);

    (*env)->ReleaseDoubleArrayElements( env, jdatarr, jdata, 0 );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    init
 * Signature: ()V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_init( JNIEnv *env, jobject jthis )
{
    set_PLStream(env,jthis);
    plinit();
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
 * Class:     plplot_core_PLStream
 * Method:    lightsource
 * Signature: (FFF)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_lightsource__FFF( JNIEnv *env, jobject jthis,
                                            jfloat jx, jfloat jy, jfloat jz )
{
    PLFLT x = jx, y = jy, z = jz;
    set_PLStream(env,jthis);
    pllightsource( x, y, z );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    lightsource
 * Signature: (DDD)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_lightsource__DDD( JNIEnv *env, jobject jthis,
                                            jdouble jx, jdouble jy, jdouble jz )
{
    PLFLT x = jx, y = jy, z = jz;
    set_PLStream(env,jthis);
    pllightsource( x, y, z );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    line
 * Signature: (I[F[F)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_line__I_3F_3F( JNIEnv *env, jobject jthis,
                                         jint n,
                                         jfloatArray jx, jfloatArray jy )
{
    jsize len = (*env)->GetArrayLength( env, jx );
    jfloat *jxdata = (*env)->GetFloatArrayElements( env, jx, 0 );
    jfloat *jydata = (*env)->GetFloatArrayElements( env, jy, 0 );
    PLFLT *x, *y;
    int must_free_buffers = 0, i;

    if (sizeof(PLFLT) == sizeof(jfloat)) {
    /* Trick: The cast is here to shut up the compiler in the case where
     * PLFLT != float, in which case the /other/ branch is the one that is
     * actually executed. */
        x = (PLFLT *) jxdata;
        y = (PLFLT *) jydata;
    } else {
        x = (PLFLT *) malloc( n * sizeof(PLFLT) );
        y = (PLFLT *) malloc( n * sizeof(PLFLT) );

        for( i=0; i < n; i++ )
        {
            x[i] = jxdata[i];
            y[i] = jydata[i];
        }
        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plline( n, x, y );

    if (must_free_buffers) {
        free( x );
        free( y );
    }

    (*env)->ReleaseFloatArrayElements( env, jx, jxdata, 0 );
    (*env)->ReleaseFloatArrayElements( env, jy, jydata, 0 );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    line
 * Signature: (I[D[D)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_line__I_3D_3D( JNIEnv *env, jobject jthis,
                                         jint n,
                                         jdoubleArray jx, jdoubleArray jy )
{
    jsize len = (*env)->GetArrayLength( env, jx );
    jdouble *jxdata = (*env)->GetDoubleArrayElements( env, jx, 0 );
    jdouble *jydata = (*env)->GetDoubleArrayElements( env, jy, 0 );
    PLFLT *x, *y;
    int must_free_buffers = 0, i;

    if (sizeof(PLFLT) == sizeof(jdouble)) {
    /* Trick: The cast is here to shut up the compiler in the case where
     * PLFLT != double, in which case the /other/ branch is the one that is
     * actually executed. */
        x = (PLFLT *) jxdata;
        y = (PLFLT *) jydata;
    } else {
        x = (PLFLT *) malloc( n * sizeof(PLFLT) );
        y = (PLFLT *) malloc( n * sizeof(PLFLT) );

        for( i=0; i < n; i++ )
        {
            x[i] = jxdata[i];
            y[i] = jydata[i];
        }
        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plline( n, x, y );

    if (must_free_buffers) {
        free( x );
        free( y );
    }

    (*env)->ReleaseDoubleArrayElements( env, jx, jxdata, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jy, jydata, 0 );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    line3
 * Signature: (I[F[F[F)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_line3__I_3F_3F_3F( JNIEnv *env, jobject jthis,
                                             jint n, jfloatArray jx,
                                             jfloatArray jy, jfloatArray jz )
{
    jsize len = (*env)->GetArrayLength( env, jx );
    jfloat *jxdata = (*env)->GetFloatArrayElements( env, jx, 0 );
    jfloat *jydata = (*env)->GetFloatArrayElements( env, jy, 0 );
    jfloat *jzdata = (*env)->GetFloatArrayElements( env, jz, 0 );
    PLFLT *x, *y, *z;
    int must_free_buffers = 0, i;

    if (sizeof(PLFLT) == sizeof(jfloat)) {
    /* Trick: The cast is here to shut up the compiler in the case where
     * PLFLT != float, in which case the /other/ branch is the one that is
     * actually executed. */
        x = (PLFLT *) jxdata;
        y = (PLFLT *) jydata;
        z = (PLFLT *) jzdata;
    } else {
        x = (PLFLT *) malloc( n * sizeof(PLFLT) );
        y = (PLFLT *) malloc( n * sizeof(PLFLT) );
        z = (PLFLT *) malloc( n * sizeof(PLFLT) );

        for( i=0; i < n; i++ )
        {
            x[i] = jxdata[i];
            y[i] = jydata[i];
            z[i] = jzdata[i];
        }
        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plline3( n, x, y, z );

    if (must_free_buffers) {
        free( x );
        free( y );
        free( z );
    }

    (*env)->ReleaseFloatArrayElements( env, jx, jxdata, 0 );
    (*env)->ReleaseFloatArrayElements( env, jy, jydata, 0 );
    (*env)->ReleaseFloatArrayElements( env, jz, jzdata, 0 );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    line3
 * Signature: (I[D[D[D)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_line3__I_3D_3D_3D( JNIEnv *env, jobject jthis,
                                             jint n, jdoubleArray jx,
                                             jdoubleArray jy, jdoubleArray jz )
{
    jsize len = (*env)->GetArrayLength( env, jx );
    jdouble *jxdata = (*env)->GetDoubleArrayElements( env, jx, 0 );
    jdouble *jydata = (*env)->GetDoubleArrayElements( env, jy, 0 );
    jdouble *jzdata = (*env)->GetDoubleArrayElements( env, jz, 0 );
    PLFLT *x, *y, *z;
    int must_free_buffers = 0, i;

    if (sizeof(PLFLT) == sizeof(jdouble)) {
    /* Trick: The cast is here to shut up the compiler in the case where
     * PLFLT != double, in which case the /other/ branch is the one that is
     * actually executed. */
        x = (PLFLT *) jxdata;
        y = (PLFLT *) jydata;
        z = (PLFLT *) jzdata;
    } else {
        x = (PLFLT *) malloc( n * sizeof(PLFLT) );
        y = (PLFLT *) malloc( n * sizeof(PLFLT) );
        z = (PLFLT *) malloc( n * sizeof(PLFLT) );

        for( i=0; i < n; i++ )
        {
            x[i] = jxdata[i];
            y[i] = jydata[i];
            z[i] = jzdata[i];
        }
        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plline3( n, x, y, z );

    if (must_free_buffers) {
        free( x );
        free( y );
        free( z );
    }

    (*env)->ReleaseDoubleArrayElements( env, jx, jxdata, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jy, jydata, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jz, jzdata, 0 );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    lsty
 * Signature: (I)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_lsty( JNIEnv *env, jobject jthis, jint lin )
{
    set_PLStream(env,jthis);
    pllsty( lin );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    mesh
 * Signature: ([F[F[[FI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_mesh___3F_3F_3_3FI( JNIEnv *env, jobject jthis,
                                              jfloatArray jx, jfloatArray jy,
                                              jobjectArray jz, jint opt )
{
    int nx = (*env)->GetArrayLength( env, jx );
    int ny = (*env)->GetArrayLength( env, jy );

    jfloat *xdat = (*env)->GetFloatArrayElements( env, jx, 0 );
    jfloat *ydat = (*env)->GetFloatArrayElements( env, jy, 0 );
    jfloat **zdat = (jfloat **) malloc( nx * sizeof(jfloat*) );

    PLFLT *x, *y, **z, *zbuf;
    int must_free_buffers = 0;
    int i, j;

/* Should really check that z.length == nx */

/* Now fetch the arrays of z[] and pull their data pointers. */
    for( i=0; i < nx; i++ )
    {
        jobject zi = (*env)->GetObjectArrayElement( env, jz, i );
        int ziels = (*env)->GetArrayLength( env, zi );
    /* ziels should be ny! */
        zdat[i] = (*env)->GetFloatArrayElements( env, zi, 0 );
    }

    if (sizeof(PLFLT) == sizeof(jfloat)) {
        x = (PLFLT *) xdat;
        y = (PLFLT *) ydat;
        z = (PLFLT **) zdat;
    } else {
        x = (PLFLT *) malloc( nx * sizeof(PLFLT) );
        y = (PLFLT *) malloc( ny * sizeof(PLFLT) );
        z = (PLFLT **) malloc( nx * sizeof(PLFLT *) );
        zbuf = (PLFLT *) malloc( nx * ny * sizeof(PLFLT) );

        for( i=0; i < nx; i++ ) x[i] = xdat[i];
        for( j=0; j < ny; j++ ) y[j] = ydat[j];

        for( i=0; i < nx; i++ ) {
            z[i] = zbuf + i*ny;
            for( j=0; j < ny; j++ )
                z[i][j] = zdat[i][j];
        }

        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plmesh( x, y, z, nx, ny, opt );

    if (must_free_buffers) {
        free(x);
        free(y);
        free(z);
        free(zbuf);
    }
    free(zdat);

    (*env)->ReleaseFloatArrayElements( env, jx, xdat, 0 );
    (*env)->ReleaseFloatArrayElements( env, jy, ydat, 0 );

/* Seems to me we need to release these elements of zarr[i] too, but for some
 * reason the JVM gets sick to its stomach when I do this...  I must be doing
 * something wrong, but I can't see what it is. */

/*     for( i=0; i < nx; i++ ) */
/*     { */
/*         jobject zi = (*env)->GetObjectArrayElement( env, zarr, i ); */
/*         (*env)->ReleaseFloatArrayElements( env, zi, zdat[i], 0 ); */
/*     } */
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    mesh
 * Signature: ([D[D[[DI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_mesh___3D_3D_3_3DI( JNIEnv *env, jobject jthis,
                                              jdoubleArray jx, jdoubleArray jy,
                                              jobjectArray jz, jint opt)
{
    int nx = (*env)->GetArrayLength( env, jx );
    int ny = (*env)->GetArrayLength( env, jy );

    jdouble *xdat = (*env)->GetDoubleArrayElements( env, jx, 0 );
    jdouble *ydat = (*env)->GetDoubleArrayElements( env, jy, 0 );
    jdouble **zdat = (jdouble **) malloc( nx * sizeof(jdouble*) );

    PLFLT *x, *y, **z, *zbuf;
    int must_free_buffers = 0;
    int i, j;

/* Should really check that z.length == nx */

/* Now fetch the arrays of z[] and pull their data pointers. */
    for( i=0; i < nx; i++ )
    {
        jobject zi = (*env)->GetObjectArrayElement( env, jz, i );
        int ziels = (*env)->GetArrayLength( env, zi );
    /* ziels should be ny! */
        zdat[i] = (*env)->GetDoubleArrayElements( env, zi, 0 );
    }

    if (sizeof(PLFLT) == sizeof(jdouble)) {
        x = (PLFLT *) xdat;
        y = (PLFLT *) ydat;
        z = (PLFLT **) zdat;
    } else {
        x = (PLFLT *) malloc( nx * sizeof(PLFLT) );
        y = (PLFLT *) malloc( ny * sizeof(PLFLT) );
        z = (PLFLT **) malloc( nx * sizeof(PLFLT *) );
        zbuf = (PLFLT *) malloc( nx * ny * sizeof(PLFLT) );

        for( i=0; i < nx; i++ ) x[i] = xdat[i];
        for( j=0; j < ny; j++ ) y[j] = ydat[j];

        for( i=0; i < nx; i++ ) {
            z[i] = zbuf + i*ny;
            for( j=0; j < ny; j++ )
                z[i][j] = zdat[i][j];
        }

        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plmesh( x, y, z, nx, ny, opt );

    if (must_free_buffers) {
        free(x);
        free(y);
        free(z);
        free(zbuf);
    }
    free(zdat);

    (*env)->ReleaseDoubleArrayElements( env, jx, xdat, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jy, ydat, 0 );

/* Seems to me we need to release these elements of zarr[i] too, but for some
 * reason the JVM gets sick to its stomach when I do this...  I must be doing
 * something wrong, but I can't see what it is. */

/*     for( i=0; i < nx; i++ ) */
/*     { */
/*         jobject zi = (*env)->GetObjectArrayElement( env, zarr, i ); */
/*         (*env)->ReleaseFloatArrayElements( env, zi, zdat[i], 0 ); */
/*     } */
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    mtex
 * Signature: (Ljava/lang/String;FFFLjava/lang/String;)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_mtex__Ljava_lang_String_2FFFLjava_lang_String_2(
    JNIEnv *env, jobject jthis,
    jstring jside, jfloat jdisp, jfloat jpos,
    jfloat jjust, jstring jtext )
{
    const char *side = (*env)->GetStringUTFChars( env, jside, 0 );
    PLFLT disp = jdisp, pos = jpos, just = jjust;
    const char *text = (*env)->GetStringUTFChars( env, jtext, 0 );

    set_PLStream(env,jthis);
    plmtex( side, disp, pos, just, text );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    mtex
 * Signature: (Ljava/lang/String;DDDLjava/lang/String;)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_mtex__Ljava_lang_String_2DDDLjava_lang_String_2(
    JNIEnv *env, jobject jthis,
    jstring jside, jdouble jdisp, jdouble jpos,
    jdouble jjust, jstring jtext )
{
    const char *side = (*env)->GetStringUTFChars( env, jside, 0 );
    PLFLT disp = jdisp, pos = jpos, just = jjust;
    const char *text = (*env)->GetStringUTFChars( env, jtext, 0 );

    set_PLStream(env,jthis);
    plmtex( side, disp, pos, just, text );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    plot3d
 * Signature: ([F[F[[FII)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_plot3d___3F_3F_3_3FII(
    JNIEnv *env, jobject jthis,
    jfloatArray xarr, jfloatArray yarr, jobjectArray zarr,
    jint opt, jint side )
{
    int nx = (*env)->GetArrayLength( env, xarr );
    int ny = (*env)->GetArrayLength( env, yarr );

    jfloat *xdat = (*env)->GetFloatArrayElements( env, xarr, 0 );
    jfloat *ydat = (*env)->GetFloatArrayElements( env, yarr, 0 );
    jfloat **zdat = (jfloat **) malloc( nx * sizeof(jfloat*) );

    PLFLT *x, *y, **z, *zbuf;
    int must_free_buffers = 0;
    int i, j;

/* Should really check that z.length == nx */

/* Now fetch the arrays of z[] and pull their data pointers. */
    for( i=0; i < nx; i++ )
    {
        jobject zi = (*env)->GetObjectArrayElement( env, zarr, i );
        int ziels = (*env)->GetArrayLength( env, zi );
    /* ziels should be ny! */
        zdat[i] = (*env)->GetFloatArrayElements( env, zi, 0 );
    }

    if (sizeof(PLFLT) == sizeof(jfloat)) {
        x = (PLFLT *) xdat;
        y = (PLFLT *) ydat;
        z = (PLFLT **) zdat;
    } else {
        x = (PLFLT *) malloc( nx * sizeof(PLFLT) );
        y = (PLFLT *) malloc( ny * sizeof(PLFLT) );
        z = (PLFLT **) malloc( nx * sizeof(PLFLT *) );
        zbuf = (PLFLT *) malloc( nx * ny * sizeof(PLFLT) );

        for( i=0; i < nx; i++ ) x[i] = xdat[i];
        for( j=0; j < ny; j++ ) y[j] = ydat[j];

        for( i=0; i < nx; i++ ) {
            z[i] = zbuf + i*ny;
            for( j=0; j < ny; j++ )
                z[i][j] = zdat[i][j];
        }

        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plot3d( x, y, z, nx, ny, opt, side );

    if (must_free_buffers) {
        free(x);
        free(y);
        free(z);
        free(zbuf);
    }
    free(zdat);

    (*env)->ReleaseFloatArrayElements( env, xarr, xdat, 0 );
    (*env)->ReleaseFloatArrayElements( env, yarr, ydat, 0 );

/* Seems to me we need to release these elements of zarr[i] too, but for some
 * reason the JVM gets sick to its stomach when I do this...  I must be doing
 * something wrong, but I can't see what it is. */

/*     for( i=0; i < nx; i++ ) */
/*     { */
/*         jobject zi = (*env)->GetObjectArrayElement( env, zarr, i ); */
/*         (*env)->ReleaseFloatArrayElements( env, zi, zdat[i], 0 ); */
/*     } */
}

/*
 * Class:     plplot_core_PLStream
 * Method:    plot3d
 * Signature: ([D[D[[DII)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_plot3d___3D_3D_3_3DII(
    JNIEnv *env, jobject jthis,
    jdoubleArray xarr, jdoubleArray yarr, jobjectArray zarr,
    jint opt, jint side )
{
    int nx = (*env)->GetArrayLength( env, xarr );
    int ny = (*env)->GetArrayLength( env, yarr );

    jdouble *xdat = (*env)->GetDoubleArrayElements( env, xarr, 0 );
    jdouble *ydat = (*env)->GetDoubleArrayElements( env, yarr, 0 );
    jdouble **zdat = (jdouble **) malloc( nx * sizeof(jdouble*) );

    PLFLT *x, *y, **z, *zbuf;
    int must_free_buffers = 0;
    int i, j;

/* Should really check that z.length == nx */

/* Now fetch the arrays of z[] and pull their data pointers. */
    for( i=0; i < nx; i++ )
    {
        jobject zi = (*env)->GetObjectArrayElement( env, zarr, i );
        int ziels = (*env)->GetArrayLength( env, zi );
    /* ziels should be ny! */
        zdat[i] = (*env)->GetDoubleArrayElements( env, zi, 0 );
    }

    if (sizeof(PLFLT) == sizeof(jdouble)) {
        x = (PLFLT *) xdat;
        y = (PLFLT *) ydat;
        z = (PLFLT **) zdat;
    } else {
        x = (PLFLT *) malloc( nx * sizeof(PLFLT) );
        y = (PLFLT *) malloc( ny * sizeof(PLFLT) );
        z = (PLFLT **) malloc( nx * sizeof(PLFLT *) );
        zbuf = (PLFLT *) malloc( nx * ny * sizeof(PLFLT) );

        for( i=0; i < nx; i++ ) x[i] = xdat[i];
        for( j=0; j < ny; j++ ) y[j] = ydat[j];

        for( i=0; i < nx; i++ ) {
            z[i] = zbuf + i*ny;
            for( j=0; j < ny; j++ )
                z[i][j] = zdat[i][j];
        }

        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plot3d( x, y, z, nx, ny, opt, side );

    if (must_free_buffers) {
        free(x);
        free(y);
        free(z);
        free(zbuf);
    }
    free(zdat);

    (*env)->ReleaseDoubleArrayElements( env, xarr, xdat, 0 );
    (*env)->ReleaseDoubleArrayElements( env, yarr, ydat, 0 );

/* Seems to me we need to release these elements of zarr[i] too, but for some
 * reason the JVM gets sick to its stomach when I do this...  I must be doing
 * something wrong, but I can't see what it is. */

/*     for( i=0; i < nx; i++ ) */
/*     { */
/*         jobject zi = (*env)->GetObjectArrayElement( env, zarr, i ); */
/*         (*env)->ReleaseDoubleArrayElements( env, zi, zdat[i], 0 ); */
/*     } */
}

/*
 * Class:     plplot_core_PLStream
 * Method:    plotsh3d
 * Signature: ([F[F[[FI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_plotsh3d___3F_3F_3_3FI(
    JNIEnv *env, jobject jthis,
    jfloatArray xarr, jfloatArray yarr, jobjectArray zarr, jint side )
{
    int nx = (*env)->GetArrayLength( env, xarr );
    int ny = (*env)->GetArrayLength( env, yarr );

    jfloat *xdat = (*env)->GetFloatArrayElements( env, xarr, 0 );
    jfloat *ydat = (*env)->GetFloatArrayElements( env, yarr, 0 );
    jfloat **zdat = (jfloat **) malloc( nx * sizeof(jfloat*) );

    PLFLT *x, *y, **z, *zbuf;
    int must_free_buffers = 0;
    int i, j;

/* Should really check that z.length == nx */

/* Now fetch the arrays of z[] and pull their data pointers. */
    for( i=0; i < nx; i++ )
    {
        jobject zi = (*env)->GetObjectArrayElement( env, zarr, i );
        int ziels = (*env)->GetArrayLength( env, zi );
    /* ziels should be ny! */
        zdat[i] = (*env)->GetFloatArrayElements( env, zi, 0 );
    }

    if (sizeof(PLFLT) == sizeof(jfloat)) {
        x = (PLFLT *) xdat;
        y = (PLFLT *) ydat;
        z = (PLFLT **) zdat;
    } else {
        x = (PLFLT *) malloc( nx * sizeof(PLFLT) );
        y = (PLFLT *) malloc( ny * sizeof(PLFLT) );
        z = (PLFLT **) malloc( nx * sizeof(PLFLT *) );
        zbuf = (PLFLT *) malloc( nx * ny * sizeof(PLFLT) );

        for( i=0; i < nx; i++ ) x[i] = xdat[i];
        for( j=0; j < ny; j++ ) y[j] = ydat[j];

        for( i=0; i < nx; i++ ) {
            z[i] = zbuf + i*ny;
            for( j=0; j < ny; j++ )
                z[i][j] = zdat[i][j];
        }

        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plotsh3d( x, y, z, nx, ny, side );

    if (must_free_buffers) {
        free(x);
        free(y);
        free(z);
        free(zbuf);
    }
    free(zdat);

    (*env)->ReleaseFloatArrayElements( env, xarr, xdat, 0 );
    (*env)->ReleaseFloatArrayElements( env, yarr, ydat, 0 );

/* Seems to me we need to release these elements of zarr[i] too, but for some
 * reason the JVM gets sick to its stomach when I do this...  I must be doing
 * something wrong, but I can't see what it is. */

/*     for( i=0; i < nx; i++ ) */
/*     { */
/*         jobject zi = (*env)->GetObjectArrayElement( env, zarr, i ); */
/*         (*env)->ReleaseFloatArrayElements( env, zi, zdat[i], 0 ); */
/*     } */
}

/*
 * Class:     plplot_core_PLStream
 * Method:    plotsh3d
 * Signature: ([D[D[[DI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_plotsh3d___3D_3D_3_3DI(
    JNIEnv *env, jobject jthis,
    jdoubleArray xarr, jdoubleArray yarr, jobjectArray zarr, jint side )
{
    int nx = (*env)->GetArrayLength( env, xarr );
    int ny = (*env)->GetArrayLength( env, yarr );

    jdouble *xdat = (*env)->GetDoubleArrayElements( env, xarr, 0 );
    jdouble *ydat = (*env)->GetDoubleArrayElements( env, yarr, 0 );
    jdouble **zdat = (jdouble **) malloc( nx * sizeof(jdouble*) );

    PLFLT *x, *y, **z, *zbuf;
    int must_free_buffers = 0;
    int i, j;

/* Should really check that z.length == nx */

/* Now fetch the arrays of z[] and pull their data pointers. */
    for( i=0; i < nx; i++ )
    {
        jobject zi = (*env)->GetObjectArrayElement( env, zarr, i );
        int ziels = (*env)->GetArrayLength( env, zi );
    /* ziels should be ny! */
        zdat[i] = (*env)->GetDoubleArrayElements( env, zi, 0 );
    }

    if (sizeof(PLFLT) == sizeof(jdouble)) {
        x = (PLFLT *) xdat;
        y = (PLFLT *) ydat;
        z = (PLFLT **) zdat;
    } else {
        x = (PLFLT *) malloc( nx * sizeof(PLFLT) );
        y = (PLFLT *) malloc( ny * sizeof(PLFLT) );
        z = (PLFLT **) malloc( nx * sizeof(PLFLT *) );
        zbuf = (PLFLT *) malloc( nx * ny * sizeof(PLFLT) );

        for( i=0; i < nx; i++ ) x[i] = xdat[i];
        for( j=0; j < ny; j++ ) y[j] = ydat[j];

        for( i=0; i < nx; i++ ) {
            z[i] = zbuf + i*ny;
            for( j=0; j < ny; j++ )
                z[i][j] = zdat[i][j];
        }

        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plotsh3d( x, y, z, nx, ny, side );

    if (must_free_buffers) {
        free(x);
        free(y);
        free(z);
        free(zbuf);
    }
    free(zdat);

    (*env)->ReleaseDoubleArrayElements( env, xarr, xdat, 0 );
    (*env)->ReleaseDoubleArrayElements( env, yarr, ydat, 0 );

/* Seems to me we need to release these elements of zarr[i] too, but for some
 * reason the JVM gets sick to its stomach when I do this...  I must be doing
 * something wrong, but I can't see what it is. */

/*     for( i=0; i < nx; i++ ) */
/*     { */
/*         jobject zi = (*env)->GetObjectArrayElement( env, zarr, i ); */
/*         (*env)->ReleaseDoubleArrayElements( env, zi, zdat[i], 0 ); */
/*     } */
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    poin
 * Signature: (I[F[FI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_poin__I_3F_3FI( JNIEnv *env, jobject jthis,
                                          jint n, jfloatArray jx, jfloatArray jy,
                                          jint code )
{
    jsize len = (*env)->GetArrayLength( env, jx );
    jfloat *jxdata = (*env)->GetFloatArrayElements( env, jx, 0 );
    jfloat *jydata = (*env)->GetFloatArrayElements( env, jy, 0 );
    PLFLT *x, *y;
    int must_free_buffers = 0, i;

    if (sizeof(PLFLT) == sizeof(jfloat)) {
    /* Trick: The cast is here to shut up the compiler in the case where
     * PLFLT != float, in which case the /other/ branch is the one that is
     * actually executed. */
        x = (PLFLT *) jxdata;
        y = (PLFLT *) jydata;
    } else {
        x = (PLFLT *) malloc( n * sizeof(PLFLT) );
        y = (PLFLT *) malloc( n * sizeof(PLFLT) );

        for( i=0; i < n; i++ )
        {
            x[i] = jxdata[i];
            y[i] = jydata[i];
        }
        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plpoin( n, x, y, code );

    if (must_free_buffers) {
        free( x );
        free( y );
    }

    (*env)->ReleaseFloatArrayElements( env, jx, jxdata, 0 );
    (*env)->ReleaseFloatArrayElements( env, jy, jydata, 0 );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    poin
 * Signature: (I[D[DI)V
 */

JNIEXPORT void JNICALL Java_plplot_core_PLStream_poin__I_3D_3DI(
    JNIEnv *env, jobject jthis,
    jint n, jdoubleArray jx, jdoubleArray jy, jint code )
{
    jsize len = (*env)->GetArrayLength( env, jx );
    jdouble *jxdata = (*env)->GetDoubleArrayElements( env, jx, 0 );
    jdouble *jydata = (*env)->GetDoubleArrayElements( env, jy, 0 );
    PLFLT *x, *y;
    int must_free_buffers = 0, i;

    if (sizeof(PLFLT) == sizeof(jdouble)) {
    /* Trick: The cast is here to shut up the compiler in the case where
     * PLFLT != float, in which case the /other/ branch is the one that is
     * actually executed. */
        x = (PLFLT *) jxdata;
        y = (PLFLT *) jydata;
    } else {
        x = (PLFLT *) malloc( n * sizeof(PLFLT) );
        y = (PLFLT *) malloc( n * sizeof(PLFLT) );

        for( i=0; i < n; i++ )
        {
            x[i] = jxdata[i];
            y[i] = jydata[i];
        }
        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plpoin( n, x, y, code );

    if (must_free_buffers) {
        free( x );
        free( y );
    }

    (*env)->ReleaseDoubleArrayElements( env, jx, jxdata, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jy, jydata, 0 );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    poin
 * Signature: (FFI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_poin__FFI( JNIEnv *env, jobject jthis,
                                     jfloat jx, jfloat jy, jint code )
{
    PLFLT x = jx, y = jy;
    set_PLStream(env,jthis);
    plpoin( 1, &x, &y, code );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    poin
 * Signature: (DDI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_poin__DDI( JNIEnv *env, jobject jthis,
                                     jdouble jx, jdouble jy, jint code )
{
    PLFLT x = jx, y = jy;
    set_PLStream(env,jthis);
    plpoin( 1, &x, &y, code );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    psty
 * Signature: (I)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_psty( JNIEnv *env, jobject jthis, jint patt )
{
    set_PLStream(env,jthis);
    plpsty( patt );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    ptex
 * Signature: (FFFFFLjava/lang/String;)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_ptex__FFFFFLjava_lang_String_2
( JNIEnv *env, jobject jthis,
  jfloat jx, jfloat jy, jfloat jdx, jfloat jdy,
  jfloat jjust, jstring jtext )
{
    PLFLT x = jx, y = jy, dx = jdx, dy = jdy, just = jjust;
    const char *text = (*env)->GetStringUTFChars( env, jtext, 0 );

    set_PLStream(env,jthis);
    plptex( x, y, dx, dy, just, text );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    ptex
 * Signature: (DDDDDLjava/lang/String;)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_ptex__DDDDDLjava_lang_String_2
( JNIEnv *env, jobject jthis,
  jdouble jx, jdouble jy, jdouble jdx, jdouble jdy,
  jdouble jjust, jstring jtext )
{
    PLFLT x = jx, y = jy, dx = jdx, dy = jdy, just = jjust;
    const char *text = (*env)->GetStringUTFChars( env, jtext, 0 );

    set_PLStream(env,jthis);
    plptex( x, y, dx, dy, just, text );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    plschr
 * Signature: (FF)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_schr__FF( JNIEnv *env, jobject jthis,
                                    jfloat jdef, jfloat jscale )
{
    PLFLT def = jdef, scale = jscale;
    plschr( def, scale );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    plschr
 * Signature: (DD)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_schr__DD( JNIEnv *env, jobject jthis,
                                    jdouble jdef, jdouble jscale )
{
    PLFLT def = jdef, scale = jscale;
    plschr( def, scale );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    scmap0n
 * Signature: (I)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_scmap0n( JNIEnv *env, jobject jthis, jint ncol0 )
{
    set_PLStream(env,jthis);
    plscmap0n( ncol0 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    scmap1n
 * Signature: (I)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_scmap1n( JNIEnv *env, jobject jthis, jint ncol1 )
{
    set_PLStream(env,jthis);
    plscmap1n( ncol1 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    scmap0
 * Signature: ([I[I[II)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_scmap0( JNIEnv *env, jobject jthis,
                                  jintArray rarr, jintArray garr,
                                  jintArray barr, jint ncol0 )
{
    jint *r = (*env)->GetIntArrayElements( env, rarr, 0 );
    jint *g = (*env)->GetIntArrayElements( env, garr, 0 );
    jint *b = (*env)->GetIntArrayElements( env, barr, 0 );

    set_PLStream(env,jthis);
    plscmap0( (PLINT *) r, (PLINT *) g, (PLINT *) b, ncol0 );

    (*env)->ReleaseIntArrayElements( env, rarr, r, 0 );
    (*env)->ReleaseIntArrayElements( env, garr, g, 0 );
    (*env)->ReleaseIntArrayElements( env, barr, b, 0 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    scmap1
 * Signature: ([I[I[II)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_scmap1( JNIEnv *env, jobject jthis,
                                  jintArray rarr, jintArray garr,
                                  jintArray barr, jint ncol1 )
{
    jint *r = (*env)->GetIntArrayElements( env, rarr, 0 );
    jint *g = (*env)->GetIntArrayElements( env, garr, 0 );
    jint *b = (*env)->GetIntArrayElements( env, barr, 0 );

    set_PLStream(env,jthis);
    plscmap1( (PLINT *) r, (PLINT *) g, (PLINT *) b, ncol1 );

    (*env)->ReleaseIntArrayElements( env, rarr, r, 0 );
    (*env)->ReleaseIntArrayElements( env, garr, g, 0 );
    (*env)->ReleaseIntArrayElements( env, barr, b, 0 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    scol0
 * Signature: (IIII)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_scol0( JNIEnv *env, jobject jthis,
                                 jint icol0, jint r, jint g, jint b )
{
    set_PLStream(env,jthis);
    plscol0( icol0, r, g, b );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    scolbg
 * Signature: (III)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_scolbg( JNIEnv *env, jobject jthis,
                                  jint r, jint g, jint b )
{
    set_PLStream(env,jthis);
    plscolbg( r, g, b );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    scolor
 * Signature: (I)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_scolor( JNIEnv *env, jobject jthis, jint color )
{
    set_PLStream(env,jthis);
    plscolor( color );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    scompression
 * Signature: (I)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_scompression( JNIEnv *env, jobject jthis, jint c )
{
    set_PLStream(env,jthis);
    plscompression( c );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    spause
 * Signature: (I)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_spause( JNIEnv *env, jobject jthis, jint pause )
{
    set_PLStream(env,jthis);
    plspause( pause );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    ssub
 * Signature: (II)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_ssub( JNIEnv *env, jobject jthis,
                                jint nx, jint ny )
{
    set_PLStream(env,jthis);
    plssub(nx,ny);
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
 * Method:    svpa
 * Signature: (FFFF)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_svpa__FFFF( JNIEnv *env, jobject jthis,
                                      jfloat jxmin, jfloat jxmax,
                                      jfloat jymin, jfloat jymax )
{
    PLFLT xmin = jxmin, xmax = jxmax;
    PLFLT ymin = jymin, ymax = jymax;
    set_PLStream(env,jthis);
    plsvpa( xmin, xmax, ymin, ymax );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    svpa
 * Signature: (DDDD)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_svpa__DDDD( JNIEnv *env, jobject jthis,
                                      jdouble jxmin, jdouble jxmax,
                                      jdouble jymin, jdouble jymax )
{
    PLFLT xmin = jxmin, xmax = jxmax;
    PLFLT ymin = jymin, ymax = jymax;
    set_PLStream(env,jthis);
    plsvpa( xmin, xmax, ymin, ymax );
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
 * Class:     plplot_core_PLStream
 * Method:    sym
 * Signature: (I[F[FI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_sym__I_3F_3FI( JNIEnv *env, jobject jthis,
                                         jint n, jfloatArray jx,
                                         jfloatArray jy, jint code )
{
    jsize len = (*env)->GetArrayLength( env, jx );
    PLFLT *x, *y;
    int must_free_buffers = 0, i;
    jfloat *jxdata = (*env)->GetFloatArrayElements( env, jx, 0 );
    jfloat *jydata = (*env)->GetFloatArrayElements( env, jy, 0 );

    if (sizeof(PLFLT) == sizeof(jfloat)) {
    /* Trick: The cast is here to shut up the compiler in the case where
     * PLFLT != float, in which case the /other/ branch is the one that is
     * actually executed. */
        x = (PLFLT *) jxdata;
        y = (PLFLT *) jydata;
    } else {
        x = (PLFLT *) malloc( n * sizeof(PLFLT) );
        y = (PLFLT *) malloc( n * sizeof(PLFLT) );

        for( i=0; i < n; i++ ) {
            x[i] = jxdata[i];
            y[i] = jydata[i];
        }

        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plsym( n, x, y, code );

    if (must_free_buffers) {
        free(x);
        free(y);
    }

    (*env)->ReleaseFloatArrayElements( env, jx, jxdata, 0 );
    (*env)->ReleaseFloatArrayElements( env, jy, jydata, 0 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    sym
 * Signature: (I[D[DI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_sym__I_3D_3DI( JNIEnv *env, jobject jthis,
                                         jint n, jdoubleArray jx,
                                         jdoubleArray jy, jint code )
{
    jsize len = (*env)->GetArrayLength( env, jx );
    PLFLT *x, *y;
    int must_free_buffers = 0, i;
    jdouble *jxdata = (*env)->GetDoubleArrayElements( env, jx, 0 );
    jdouble *jydata = (*env)->GetDoubleArrayElements( env, jy, 0 );

    if (sizeof(PLFLT) == sizeof(jdouble)) {
    /* Trick: The cast is here to shut up the compiler in the case where
     * PLFLT != float, in which case the /other/ branch is the one that is
     * actually executed. */
        x = (PLFLT *) jxdata;
        y = (PLFLT *) jydata;
    } else {
        x = (PLFLT *) malloc( n * sizeof(PLFLT) );
        y = (PLFLT *) malloc( n * sizeof(PLFLT) );

        for( i=0; i < n; i++ ) {
            x[i] = jxdata[i];
            y[i] = jydata[i];
        }

        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plsym( n, x, y, code );

    if (must_free_buffers) {
        free(x);
        free(y);
    }

    (*env)->ReleaseDoubleArrayElements( env, jx, jxdata, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jy, jydata, 0 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    sym
 * Signature: (FFI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_sym__FFI( JNIEnv *env, jobject jthis,
                                    jfloat jx, jfloat jy, jint code )
{
    PLFLT x = jx, y = jy;
    set_PLStream(env,jthis);
    plsym( 1, &x, &y, code );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    sym
 * Signature: (DDI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_sym__DDI( JNIEnv *env, jobject jthis,
                                    jdouble jx, jdouble jy, jint code )
{
    PLFLT x = jx, y = jy;
    set_PLStream(env,jthis);
    plsym( 1, &x, &y, code );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    vpor
 * Signature: (FFFF)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_vpor__FFFF( JNIEnv *env, jobject jthis,
                                      jfloat jxmin, jfloat jxmax,
                                      jfloat jymin, jfloat jymax )
{
    PLFLT xmin = jxmin, xmax = jxmax;
    PLFLT ymin = jymin, ymax = jymax;

    set_PLStream(env,jthis);
    plvpor( xmin, xmax, ymin, ymax );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    vpor
 * Signature: (DDDD)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_vpor__DDDD( JNIEnv *env, jobject jthis,
                                      jdouble jxmin, jdouble jxmax,
                                      jdouble jymin, jdouble jymax )
{
    PLFLT xmin = jxmin, xmax = jxmax;
    PLFLT ymin = jymin, ymax = jymax;

    set_PLStream(env,jthis);
    plvpor( xmin, xmax, ymin, ymax );
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
 * Class:     plplot_core_PLStream
 * Method:    w3d
 * Signature: (FFFFFFFFFFF)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_w3d__FFFFFFFFFFF(
    JNIEnv *env, jobject jthis,
    jfloat jbasex, jfloat jbasey, jfloat jheight,
    jfloat jxmin0, jfloat jxmax0, jfloat jymin0, jfloat jymax0,
    jfloat jzmin0, jfloat jzmax0, jfloat jalt, jfloat jaz )
{
    PLFLT basex = jbasex, basey = jbasey, height = jheight;
    PLFLT xmin0 = jxmin0, xmax0 = jxmax0;
    PLFLT ymin0 = jymin0, ymax0 = jymax0;
    PLFLT zmin0 = jzmin0, zmax0 = jzmax0;
    PLFLT alt = jalt, az = jaz;

    set_PLStream(env,jthis);
    plw3d( basex, basey, height,
           xmin0, xmax0, ymin0, ymax0, zmin0, zmax0,
           alt, az );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    w3d
 * Signature: (DDDDDDDDDDD)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_w3d__DDDDDDDDDDD(
    JNIEnv *env, jobject jthis,
    jdouble jbasex, jdouble jbasey, jdouble jheight,
    jdouble jxmin0, jdouble jxmax0, jdouble jymin0, jdouble jymax0,
    jdouble jzmin0, jdouble jzmax0, jdouble jalt, jdouble jaz )
{
    PLFLT basex = jbasex, basey = jbasey, height = jheight;
    PLFLT xmin0 = jxmin0, xmax0 = jxmax0;
    PLFLT ymin0 = jymin0, ymax0 = jymax0;
    PLFLT zmin0 = jzmin0, zmax0 = jzmax0;
    PLFLT alt = jalt, az = jaz;

    set_PLStream(env,jthis);
    plw3d( basex, basey, height,
           xmin0, xmax0, ymin0, ymax0, zmin0, zmax0,
           alt, az );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    wid
 * Signature: (I)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_wid( JNIEnv *env, jobject jthis, jint width )
{
    set_PLStream(env,jthis);
    plwid(width);
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    wind
 * Signature: (FFFF)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_wind__FFFF( JNIEnv *env, jobject jthis,
                                      jfloat jxmin, jfloat jxmax,
                                      jfloat jymin, jfloat jymax )
{
    PLFLT xmin = jxmin, xmax = jxmax;
    PLFLT ymin = jymin, ymax = jymax;
    set_PLStream(env,jthis);
    plwind( xmin, xmax, ymin, ymax );
}

/*
 * Class:     plplot_0002fcore_0002fPLStream
 * Method:    wind
 * Signature: (DDDD)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_wind__DDDD( JNIEnv *env, jobject jthis,
                                      jdouble jxmin, jdouble jxmax,
                                      jdouble jymin, jdouble jymax )
{
    PLFLT xmin = jxmin, xmax = jxmax;
    PLFLT ymin = jymin, ymax = jymax;
    set_PLStream(env,jthis);
    plwind( xmin, xmax, ymin, ymax );
}

/*---------------------------------------------------------------------------//
 *                            End of javabind.c
//---------------------------------------------------------------------------*/
