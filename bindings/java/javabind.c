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
Java_plplot_core_PLStream_box__Ljava_lang_String_2FILjava_lang_String_2FI
( JNIEnv *env, jobject jthis,
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
Java_plplot_core_PLStream_box__Ljava_lang_String_2DILjava_lang_String_2DI
( JNIEnv *env, jobject jthis,
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
    jfloat *x = (*env)->GetFloatArrayElements( env, jx, 0 );
    jfloat *y = (*env)->GetFloatArrayElements( env, jy, 0 );
    PLFLT *bx, *by;
    int must_free_buffers = 0, i;

    if (sizeof(PLFLT) == sizeof(jfloat)) {
    /* Trick: The cast is here to shut up the compiler in the case where
     * PLFLT != float, in which case the /other/ branch is the one that is
     * actually executed. */
        bx = (PLFLT *) x;
        by = (PLFLT *) y;
    } else {
        bx = (PLFLT *) malloc( n * sizeof(PLFLT) );
        by = (PLFLT *) malloc( n * sizeof(PLFLT) );

        for( i=0; i < n; i++ )
        {
            bx[i] = x[i];
            by[i] = y[i];
        }
        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plline( n, bx, by );

    if (must_free_buffers) {
        free( bx );
        free( by );
    }

    (*env)->ReleaseFloatArrayElements( env, jx, x, 0 );
    (*env)->ReleaseFloatArrayElements( env, jy, y, 0 );
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
    jdouble *x = (*env)->GetDoubleArrayElements( env, jx, 0 );
    jdouble *y = (*env)->GetDoubleArrayElements( env, jy, 0 );
    PLFLT *bx, *by;
    int must_free_buffers = 0, i;

    if (sizeof(PLFLT) == sizeof(jdouble)) {
    /* Trick: The cast is here to shut up the compiler in the case where
     * PLFLT != double, in which case the /other/ branch is the one that is
     * actually executed. */
        bx = (PLFLT *) x;
        by = (PLFLT *) y;
    } else {
        bx = (PLFLT *) malloc( n * sizeof(PLFLT) );
        by = (PLFLT *) malloc( n * sizeof(PLFLT) );

        for( i=0; i < n; i++ )
        {
            bx[i] = x[i];
            by[i] = y[i];
        }
        must_free_buffers = 1;
    }

    set_PLStream(env,jthis);
    plline( n, bx, by );

    if (must_free_buffers) {
        free( bx );
        free( by );
    }

    (*env)->ReleaseDoubleArrayElements( env, jx, x, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jy, y, 0 );
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
    jfloat *x = (*env)->GetFloatArrayElements( env, jx, 0 );
    jfloat *y = (*env)->GetFloatArrayElements( env, jy, 0 );

    set_PLStream(env,jthis);
    plpoin( n, x, y, code );
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
