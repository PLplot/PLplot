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
 * Class:     plplot_core_PLStream
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
// Array allocation & copy helper routines.  Caller must free memory if
// *must_free_buffers is set.
//
// The initial cast is here to shut up the compiler in the case where
// PLFLT != float, in which case the /other/ branch is the one that is
// actually executed.
//---------------------------------------------------------------------------*/

/* 1d array of floats */
/* Here caller must free(a) if *must_free_buffers is set */

static void 
setup_array_1d_f( PLFLT **pa, jfloat *adat, int n, int *must_free_buffers )
{
    if (sizeof(PLFLT) == sizeof(jfloat)) {
        *pa = (PLFLT *) adat;
    } else {
	int i;
        *pa = (PLFLT *) malloc( n * sizeof(PLFLT) );
        for( i=0; i < n; i++ ) {
            (*pa)[i] = adat[i];
        }
        *must_free_buffers = 1;
    }
}

/* 1d array of doubles */
/* Here caller must free(a) if *must_free_buffers is set */

static void
setup_array_1d_d( PLFLT **pa, jdouble *adat, int n, int *must_free_buffers )
{
    if (sizeof(PLFLT) == sizeof(jdouble)) {
        *pa = (PLFLT *) adat;
    } else {
	int i;
        *pa = (PLFLT *) malloc( n * sizeof(PLFLT) );
        for( i=0; i < n; i++ ) {
            (*pa)[i] = adat[i];
        }
        *must_free_buffers = 1;
    }
}

/* 2d array of floats */
/* Here caller must free(a[0]) and free(a) (in that order) if
   *must_free_buffers is set */ 

static void 
setup_array_2d_f( PLFLT ***pa, jfloat **adat, int nx, int ny, int *must_free_buffers )
{
    int i, j;

    if (sizeof(PLFLT) == sizeof(jfloat)) {
        *pa  = (PLFLT **) adat;
    } else {
        *pa = (PLFLT **) malloc( nx * sizeof(PLFLT *) );
        (*pa)[0] = (PLFLT *) malloc( nx * ny * sizeof(PLFLT) );

        for( i=0; i < nx; i++ )
        {
            (*pa)[i] = (*pa)[0] + i*ny;
            for( j=0; j < ny; j++ )
                (*pa)[i][j] = adat[i][j];
        }

        *must_free_buffers = 1;
    }
}

/* 2d array of doubles */
/* Here caller must free(a[0]) and free(a) (in that order) if
   *must_free_buffers is set */ 

static void
setup_array_2d_d( PLFLT ***pa, jdouble **adat, int nx, int ny, int *must_free_buffers )
{
    int i, j;

    if (sizeof(PLFLT) == sizeof(jdouble)) {
        *pa  = (PLFLT **) adat;
    } else {
        *pa = (PLFLT **) malloc( nx * sizeof(PLFLT *) );
        (*pa)[0] = (PLFLT *) malloc( nx * ny * sizeof(PLFLT) );

        for( i=0; i < nx; i++ )
        {
            (*pa)[i] = (*pa)[0] + i*ny;
            for( j=0; j < ny; j++ )
                (*pa)[i][j] = adat[i][j];
        }

        *must_free_buffers = 1;
    }
}

/*---------------------------------------------------------------------------//
// First the arg parsing support routines.
//---------------------------------------------------------------------------*/

/*
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
 * Method:    arrows
 * Signature: ([F[F[F[FIFFF)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_arrows___3F_3F_3F_3FIFFF(
    JNIEnv *env, jobject jthis,
    jfloatArray ju, jfloatArray jv, jfloatArray jx, jfloatArray jy,
    jint n, jfloat jscale, jfloat jdx, jfloat jdy )
{
    PLFLT scale = jscale, dx = jdx, dy = jdy;
    jsize len = (*env)->GetArrayLength( env, ju );
    jfloat *judata = (*env)->GetFloatArrayElements( env, ju, 0 );
    jfloat *jvdata = (*env)->GetFloatArrayElements( env, jv, 0 );
    jfloat *jxdata = (*env)->GetFloatArrayElements( env, jx, 0 );
    jfloat *jydata = (*env)->GetFloatArrayElements( env, jy, 0 );
    PLFLT *u, *v, *x, *y;
    int must_free_buffers = 0;

    setup_array_1d_f( &u, judata, n, &must_free_buffers );
    setup_array_1d_f( &v, jvdata, n, &must_free_buffers );
    setup_array_1d_f( &x, jxdata, n, &must_free_buffers );
    setup_array_1d_f( &y, jydata, n, &must_free_buffers );

    set_PLStream(env,jthis);
    plarrows( u, v, x, y, n, scale, dx, dy );

    if (must_free_buffers) {
        free( u );
        free( v );
        free( x );
        free( y );
    }

    (*env)->ReleaseFloatArrayElements( env, ju, judata, 0 );
    (*env)->ReleaseFloatArrayElements( env, jv, jvdata, 0 );
    (*env)->ReleaseFloatArrayElements( env, jx, jxdata, 0 );
    (*env)->ReleaseFloatArrayElements( env, jy, jydata, 0 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    arrows
 * Signature: ([D[D[D[DIDDD)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_arrows___3D_3D_3D_3DIDDD(
    JNIEnv *env, jobject jthis,
    jdoubleArray ju, jdoubleArray jv, jdoubleArray jx, jdoubleArray jy,
    jint n, jdouble jscale, jdouble jdx, jdouble jdy )
{
    PLFLT scale = jscale, dx = jdx, dy = jdy;
    jsize len = (*env)->GetArrayLength( env, ju );
    jdouble *judata = (*env)->GetDoubleArrayElements( env, ju, 0 );
    jdouble *jvdata = (*env)->GetDoubleArrayElements( env, jv, 0 );
    jdouble *jxdata = (*env)->GetDoubleArrayElements( env, jx, 0 );
    jdouble *jydata = (*env)->GetDoubleArrayElements( env, jy, 0 );
    PLFLT *u, *v, *x, *y;
    int must_free_buffers = 0;

    setup_array_1d_d( &u, judata, n, &must_free_buffers );
    setup_array_1d_d( &v, jvdata, n, &must_free_buffers );
    setup_array_1d_d( &x, jxdata, n, &must_free_buffers );
    setup_array_1d_d( &y, jydata, n, &must_free_buffers );

    set_PLStream(env,jthis);
    plarrows( u, v, x, y, n, scale, dx, dy );

    if (must_free_buffers) {
        free( u );
        free( v );
        free( x );
        free( y );
    }

    (*env)->ReleaseDoubleArrayElements( env, ju, judata, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jv, jvdata, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jx, jxdata, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jy, jydata, 0 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    axes
 * Signature: (FFLjava/lang/String;FILjava/lang/String;FI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_axes__FFLjava_lang_String_2FILjava_lang_String_2FI(
    JNIEnv *env, jobject jthis,
    jfloat jx0, jfloat jy0,
    jstring jxopt, jfloat jxtick, jint nxsub,
    jstring jyopt, jfloat jytick, jint nysub )
{
    PLFLT x0 = jx0, y0 = jy0, xtick = jxtick, ytick = jytick;
    const char *xopt = (*env)->GetStringUTFChars( env, jxopt, 0 );
    const char *yopt = (*env)->GetStringUTFChars( env, jyopt, 0 );

    set_PLStream(env,jthis);
    plaxes( x0, y0, xopt, xtick, nxsub, yopt, ytick, nysub );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    axes
 * Signature: (DDLjava/lang/String;DILjava/lang/String;DI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_axes__DDLjava_lang_String_2DILjava_lang_String_2DI(
    JNIEnv *env, jobject jthis,
    jdouble jx0, jdouble jy0,
    jstring jxopt, jdouble jxtick, jint nxsub,
    jstring jyopt, jdouble jytick, jint nysub )
{
    PLFLT x0 = jx0, y0 = jy0, xtick = jxtick, ytick = jytick;
    const char *xopt = (*env)->GetStringUTFChars( env, jxopt, 0 );
    const char *yopt = (*env)->GetStringUTFChars( env, jyopt, 0 );

    set_PLStream(env,jthis);
    plaxes( x0, y0, xopt, xtick, nxsub, yopt, ytick, nysub );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    bin
 * Signature: (I[F[FI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_bin__I_3F_3FI( JNIEnv *env, jobject jthis,
                                         jint n,
                                         jfloatArray jx, jfloatArray jy,
                                         jint center )
{
    jsize len = (*env)->GetArrayLength( env, jx );
    jfloat *jxdata = (*env)->GetFloatArrayElements( env, jx, 0 );
    jfloat *jydata = (*env)->GetFloatArrayElements( env, jy, 0 );
    PLFLT *x, *y;
    int must_free_buffers = 0;

    setup_array_1d_f( &x, jxdata, n, &must_free_buffers );
    setup_array_1d_f( &y, jydata, n, &must_free_buffers );

    set_PLStream(env,jthis);
    plbin( n, x, y, center );

    if (must_free_buffers) {
        free( x );
        free( y );
    }

    (*env)->ReleaseFloatArrayElements( env, jx, jxdata, 0 );
    (*env)->ReleaseFloatArrayElements( env, jy, jydata, 0 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    bin
 * Signature: (I[D[DI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_bin__I_3D_3DI( JNIEnv *env, jobject jthis,
                                         jint n,
                                         jdoubleArray jx, jdoubleArray jy,
                                         jint center )
{
    jsize len = (*env)->GetArrayLength( env, jx );
    jdouble *jxdata = (*env)->GetDoubleArrayElements( env, jx, 0 );
    jdouble *jydata = (*env)->GetDoubleArrayElements( env, jy, 0 );
    PLFLT *x, *y;
    int must_free_buffers = 0;

    setup_array_1d_d( &x, jxdata, n, &must_free_buffers );
    setup_array_1d_d( &y, jydata, n, &must_free_buffers );

    set_PLStream(env,jthis);
    plbin( n, x, y, center );

    if (must_free_buffers) {
        free( x );
        free( y );
    }

    (*env)->ReleaseDoubleArrayElements( env, jx, jxdata, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jy, jydata, 0 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    bop
 * Signature: ()V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_bop( JNIEnv *env, jobject jthis )
{
    set_PLStream(env,jthis);
    plbop();
}

/*
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
 * Method:    col1
 * Signature: (D)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_col1__D( JNIEnv *env, jobject jthis, jdouble col1 )
{
    set_PLStream(env,jthis);
    plcol1(col1);
}

static PLFLT f2eval2( PLINT ix, PLINT iy, PLPointer plf2eval_data )
{
    PLfGrid2 *grid = (PLfGrid2 *) plf2eval_data;

    ix = ix % grid->nx;
    iy = iy % grid->ny;

    return grid->f[ix][iy];
}

/*
 * Class:     plplot_core_PLStream
 * Method:    cont
 * Signature: ([[F[F[F[FI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_cont___3_3F_3F_3F_3FI(
    JNIEnv *env, jobject jthis,
    jobjectArray jz, jfloatArray jclev,
    jfloatArray jxg, jfloatArray jyg, jint wrap )
{
    jfloat **zdat;
    jobject *zi;

    PLFLT **z;
    int nx = (*env)->GetArrayLength( env, jxg );
    int ny = (*env)->GetArrayLength( env, jyg );

    jfloat *xgdat = (*env)->GetFloatArrayElements( env, jxg, 0 );
    jfloat *ygdat = (*env)->GetFloatArrayElements( env, jyg, 0 );
    PLFLT *xg, *yg;

    int kx, ky, lx, ly;

    int nlev = (*env)->GetArrayLength( env, jclev );
    jfloat *clevdat = (*env)->GetFloatArrayElements( env, jclev, 0 );
    PLFLT *clev;

    PLfGrid2 fgrid;
    PLcGrid cgrid;

    int must_free_buffers = 0;
    int i, j;

    zi = (jobject *) malloc( nx * sizeof(jobject) );
    zdat = (jfloat **) malloc( nx * sizeof(jfloat *) );

    for( i=0; i < nx; i++ )
    {
        zi[i] = (*env)->GetObjectArrayElement( env, jz, i );
        zdat[i] = (*env)->GetFloatArrayElements( env, zi[i], 0 );
    }

    if (sizeof(PLFLT) == sizeof(jfloat)) {
        clev = (PLFLT *) clevdat;
        xg = (PLFLT *) xgdat;
        yg = (PLFLT *) ygdat;
        z  = (PLFLT **) zdat;
    } else {
/* No, we have to actually copy the data. */
        clev = (PLFLT *) malloc( nlev * sizeof(PLFLT) );
        xg = (PLFLT *) malloc( nx * sizeof(PLFLT) );
        yg = (PLFLT *) malloc( ny * sizeof(PLFLT) );

        z = (PLFLT **) malloc( nx * sizeof(PLFLT*) );
        z[0] = (PLFLT *) malloc( nx * ny * sizeof(PLFLT) );
        for( i=0; i < nx; i++ )
        {
            z[i] = z[0] + i*ny;
            for( j=0; j < ny; j++ )
                z[i][j] = zdat[i][j];
        }

        for( i=0; i < nlev; i++ )
            clev[i] = clevdat[i];

        for( i=0; i < nx; i++ )
            xg[i] = xgdat[i];

        for( i=0; i < ny; i++ )
            yg[i] = ygdat[i];

        must_free_buffers = 1;
    }

    fgrid.f = z;
    fgrid.nx = nx;
    fgrid.ny = ny;

    cgrid.xg = xg;
    cgrid.yg = yg;
    cgrid.nx = nx;
    cgrid.ny = ny;

    kx = 1; lx = nx;
    ky = 1; ly = ny;

    set_PLStream(env,jthis);

    plfcont( f2eval2, &fgrid, nx, ny, kx, lx, ky, ly,
             clev, nlev, pltr1, &cgrid );

    if (must_free_buffers) {
        free( clev );
        free( xg );
        free( yg );
        free( z[0] );
        free( z );
    }

    (*env)->ReleaseFloatArrayElements( env, jclev, clevdat, 0 );
    (*env)->ReleaseFloatArrayElements( env, jxg, xgdat, 0 );
    (*env)->ReleaseFloatArrayElements( env, jyg, ygdat, 0 );

    for( i=0; i < nx; i++ )
        (*env)->ReleaseFloatArrayElements( env, zi[i], zdat[i], 0 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    cont
 * Signature: ([[D[D[D[DI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_cont___3_3D_3D_3D_3DI(
    JNIEnv *env, jobject jthis,
    jobjectArray jz, jdoubleArray jclev,
    jdoubleArray jxg, jdoubleArray jyg, jint wrap )
{
    jdouble **zdat;
    jobject *zi;

    PLFLT **z;
    int nx = (*env)->GetArrayLength( env, jxg );
    int ny = (*env)->GetArrayLength( env, jyg );

    jdouble *xgdat = (*env)->GetDoubleArrayElements( env, jxg, 0 );
    jdouble *ygdat = (*env)->GetDoubleArrayElements( env, jyg, 0 );
    PLFLT *xg, *yg;

    int kx, ky, lx, ly;

    int nlev = (*env)->GetArrayLength( env, jclev );
    jdouble *clevdat = (*env)->GetDoubleArrayElements( env, jclev, 0 );
    PLFLT *clev;

    PLfGrid2 fgrid;
    PLcGrid cgrid;

    int must_free_buffers = 0;
    int i, j;

    zi = (jobject *) malloc( nx * sizeof(jobject) );
    zdat = (jdouble **) malloc( nx * sizeof(jdouble *) );

    for( i=0; i < nx; i++ )
    {
        zi[i] = (*env)->GetObjectArrayElement( env, jz, i );
        zdat[i] = (*env)->GetDoubleArrayElements( env, zi[i], 0 );
    }

    if (sizeof(PLFLT) == sizeof(jdouble)) {
        clev = (PLFLT *) clevdat;
        xg = (PLFLT *) xgdat;
        yg = (PLFLT *) ygdat;
        z  = (PLFLT **) zdat;
    } else {
/* No, we have to actually copy the data. */
        clev = (PLFLT *) malloc( nlev * sizeof(PLFLT) );
        xg = (PLFLT *) malloc( nx * sizeof(PLFLT) );
        yg = (PLFLT *) malloc( ny * sizeof(PLFLT) );

        z = (PLFLT **) malloc( nx * sizeof(PLFLT*) );
        z[0] = (PLFLT *) malloc( nx * ny * sizeof(PLFLT) );
        for( i=0; i < nx; i++ )
        {
            z[i] = z[0] + i*ny;
            for( j=0; j < ny; j++ )
                z[i][j] = zdat[i][j];
        }

        for( i=0; i < nlev; i++ )
            clev[i] = clevdat[i];

        for( i=0; i < nx; i++ )
            xg[i] = xgdat[i];

        for( i=0; i < ny; i++ )
            yg[i] = ygdat[i];

        must_free_buffers = 1;
    }

    fgrid.f = z;
    fgrid.nx = nx;
    fgrid.ny = ny;

    cgrid.xg = xg;
    cgrid.yg = yg;
    cgrid.nx = nx;
    cgrid.ny = ny;

    kx = 1; lx = nx;
    ky = 1; ly = ny;

    set_PLStream(env,jthis);

    plfcont( f2eval2, &fgrid, nx, ny, kx, lx, ky, ly,
             clev, nlev, pltr1, &cgrid );

    if (must_free_buffers) {
        free( clev );
        free( xg );
        free( yg );
        free( z[0] );
        free( z );
    }

    (*env)->ReleaseDoubleArrayElements( env, jclev, clevdat, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jxg, xgdat, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jyg, ygdat, 0 );

    for( i=0; i < nx; i++ )
        (*env)->ReleaseDoubleArrayElements( env, zi[i], zdat[i], 0 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    cont
 * Signature: ([[F[F[[F[[FI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_cont___3_3F_3F_3_3F_3_3FI(
    JNIEnv *env, jobject jthis,
    jobjectArray jz, jfloatArray jclev,
    jobjectArray jxg, jobjectArray jyg, jint wrap )
{
    jfloat **zdat;
    jobject *zi;

    jfloat **xgdat;
    jobject *xgi;

    jfloat **ygdat;
    jobject *ygi;

    int znx  = (*env)->GetArrayLength( env, jz ),  zny  = -1;
    int xgnx = (*env)->GetArrayLength( env, jxg ), xgny = -1;
    int ygnx = (*env)->GetArrayLength( env, jyg ), ygny = -1;

    PLFLT **z, **zwrapped, **zused, **xg, **yg;

    int kx, ky, lx, ly;
    int nx, ny;

    int nlev = (*env)->GetArrayLength( env, jclev );
    jfloat *clevdat = (*env)->GetFloatArrayElements( env, jclev, 0 );
    PLFLT *clev;

    PLcGrid2 cgrid;

    int must_free_buffers = 0;
    int i, j;

/* Extract the z data. */
    zi = (jobject *) malloc( znx * sizeof(jobject) );
    zdat = (jfloat **) malloc( znx * sizeof(jfloat *) );

    for( i=0; i < znx; i++ )
    {
        zi[i] = (*env)->GetObjectArrayElement( env, jz, i );
        zdat[i] = (*env)->GetFloatArrayElements( env, zi[i], 0 );

        if (zny == -1)
            zny = (*env)->GetArrayLength( env, zi[i] );
        else if (zny != (*env)->GetArrayLength( env, zi[i] )) {
            printf( "Misshapen z array.\n" );
            return;
        }
    }

/* Extract the xg data. */
    xgi = (jobject *) malloc( xgnx * sizeof(jobject) );
    xgdat = (jfloat **) malloc( xgnx * sizeof(jfloat *) );

    for( i=0; i < xgnx; i++ )
    {
        xgi[i] = (*env)->GetObjectArrayElement( env, jxg, i );
        xgdat[i] = (*env)->GetFloatArrayElements( env, xgi[i], 0 );

        if (xgny == -1)
            xgny = (*env)->GetArrayLength( env, xgi[i] );
        else if (xgny != (*env)->GetArrayLength( env, xgi[i] )) {
            printf( "Misshapen xg array.\n" );
            return;
        }
    }

/* Extract the yg data. */
    ygi = (jobject *) malloc( ygnx * sizeof(jobject) );
    ygdat = (jfloat **) malloc( ygnx * sizeof(jfloat *) );

    for( i=0; i < ygnx; i++ )
    {
        ygi[i] = (*env)->GetObjectArrayElement( env, jyg, i );
        ygdat[i] = (*env)->GetFloatArrayElements( env, ygi[i], 0 );

        if (ygny == -1)
            ygny = (*env)->GetArrayLength( env, ygi[i] );
        else if (ygny != (*env)->GetArrayLength( env, ygi[i] )) {
            printf( "Misshapen yg array.\n" );
            return;
        }
    }

    if (znx != xgnx || znx != ygnx) {
        printf( "Improper x dimensions.\n" );
        return;
    }
    if (zny != xgny || zny != ygny) {
        printf( "Improper y dimensions.\n" );
        return;
    }

    nx = znx; ny = zny;

/* See if PLFLT is compatible with the java numeric type. */
    if (sizeof(PLFLT) == sizeof(jfloat)) {
    /* Yes, we can just initialize the pointers.  Note the cast is so the
     * compiler will be happy in the case where PLFLT does NOT match the java
     * type, in which case this pathway isn't executed. */
        clev = (PLFLT *) clevdat;
        xg = (PLFLT **) xgdat;
        yg = (PLFLT **) ygdat;
        z  = (PLFLT **) zdat;
    } else {
    /* No, we have to actually copy the data. */
        clev = (PLFLT *) malloc( nlev * sizeof(PLFLT) );

        for( i=0; i < nlev; i++ )
            clev[i] = clevdat[i];

        z = (PLFLT **) malloc( znx * sizeof(PLFLT*) );
        z[0] = (PLFLT *) malloc( znx * zny * sizeof(PLFLT) );
        for( i=0; i < znx; i++ )
        {
            z[i] = z[0] + i*zny;
            for( j=0; j < zny; j++ )
                z[i][j] = zdat[i][j];
        }

        xg = (PLFLT **) malloc( xgnx * sizeof(PLFLT*) );
        xg[0] = (PLFLT *) malloc( xgnx * xgny * sizeof(PLFLT) );
        for( i=0; i < xgnx; i++ )
        {
            xg[i] = xg[0] + i*xgny;
            for( j=0; j < xgny; j++ )
                xg[i][j] = xgdat[i][j];
        }

        yg = (PLFLT **) malloc( ygnx * sizeof(PLFLT*) );
        yg[0] = (PLFLT *) malloc( ygnx * ygny * sizeof(PLFLT) );
        for( i=0; i < ygnx; i++ )
        {
            yg[i] = yg[0] + i*ygny;
            for( j=0; j < ygny; j++ )
                yg[i][j] = ygdat[i][j];
        }

        must_free_buffers = 1;
    }

/* Better hav xgnx == ygnx and xgny == ygny */

    if (wrap == 0) {
        cgrid.xg = xg;
        cgrid.yg = yg;
        cgrid.nx = xgnx;
        cgrid.ny = xgny;
        zused = z;
    } else if (wrap == 1) {
        plAlloc2dGrid( &cgrid.xg, nx+1, ny );
        plAlloc2dGrid( &cgrid.yg, nx+1, ny );
        plAlloc2dGrid( &zwrapped, nx+1, ny );

        cgrid.nx = nx+1;
        cgrid.ny = ny;
        zused = zwrapped;

        for( i=0; i < nx; i++ )
            for( j=0; j < ny; j++ ) {
                cgrid.xg[i][j] = xg[i][j];
                cgrid.yg[i][j] = yg[i][j];
                zwrapped[i][j] = z[i][j];
            }

        for( j=0; j < ny; j++ ) {
            cgrid.xg[nx][j] = cgrid.xg[0][j];
            cgrid.yg[nx][j] = cgrid.yg[0][j];
            zwrapped[nx][j] = zwrapped[0][j];
        }

        ny++;
    } else if (wrap == 2) {
        plAlloc2dGrid( &cgrid.xg, nx, ny+1 );
        plAlloc2dGrid( &cgrid.yg, nx, ny+1 );
        plAlloc2dGrid( &zwrapped, nx, ny+1 );

        cgrid.nx = nx;
        cgrid.ny = ny+1;
        zused = zwrapped;

        for( i=0; i < nx; i++ )
            for( j=0; j < ny; j++ ) {
                cgrid.xg[i][j] = xg[i][j];
                cgrid.yg[i][j] = yg[i][j];
                zwrapped[i][j] = z[i][j];
            }

        for( i=0; i < nx; i++ ) {
            cgrid.xg[i][ny] = cgrid.xg[i][0];
            cgrid.yg[i][ny] = cgrid.yg[i][0];
            zwrapped[i][ny] = zwrapped[i][0];
        }

        ny++;
    } else {
        printf( "Invalid wrapping specification.\n" );
    }

    kx = 1; lx = nx;
    ky = 1; ly = ny;

    set_PLStream(env,jthis);

    plcont( zused, nx, ny, kx, lx, ky, ly,
             clev, nlev, pltr2, &cgrid );

    if (must_free_buffers) {
    /* Need to go through an free all the data arrays we malloced. */
        free( clev );

        free( z[0] );
        free( z );

        free( xg[0] );
        free( xg );

        free( yg[0] );
        free( yg );
    }

    (*env)->ReleaseFloatArrayElements( env, jclev, clevdat, 0 );
    for( i=0; i < znx; i++ )
        (*env)->ReleaseFloatArrayElements( env, zi[i], zdat[i], 0 );
    for( i=0; i < xgnx; i++ )
        (*env)->ReleaseFloatArrayElements( env, xgi[i], xgdat[i], 0 );
    for( i=0; i < ygnx; i++ )
        (*env)->ReleaseFloatArrayElements( env, ygi[i], ygdat[i], 0 );

    if (wrap != 0) {
        plFree2dGrid( cgrid.xg, nx, ny );
        plFree2dGrid( cgrid.yg, nx, ny );
        plFree2dGrid( zwrapped, nx, ny );
    }
}

/*
 * Class:     plplot_core_PLStream
 * Method:    cont
 * Signature: ([[D[D[[D[[DI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_cont___3_3D_3D_3_3D_3_3DI(
    JNIEnv *env, jobject jthis,
    jobjectArray jz, jdoubleArray jclev,
    jobjectArray jxg, jobjectArray jyg, jint wrap )
{
    jdouble **zdat;
    jobject *zi;

    jdouble **xgdat;
    jobject *xgi;

    jdouble **ygdat;
    jobject *ygi;

    int znx  = (*env)->GetArrayLength( env, jz ),  zny  = -1;
    int xgnx = (*env)->GetArrayLength( env, jxg ), xgny = -1;
    int ygnx = (*env)->GetArrayLength( env, jyg ), ygny = -1;

    PLFLT **z, **zwrapped, **zused, **xg, **yg;

    int kx, ky, lx, ly;
    int nx, ny;

    int nlev = (*env)->GetArrayLength( env, jclev );
    jdouble *clevdat = (*env)->GetDoubleArrayElements( env, jclev, 0 );
    PLFLT *clev;

    PLcGrid2 cgrid;

    int must_free_buffers = 0;
    int i, j;

/* Extract the z data. */
    zi = (jobject *) malloc( znx * sizeof(jobject) );
    zdat = (jdouble **) malloc( znx * sizeof(jdouble *) );

    for( i=0; i < znx; i++ )
    {
        zi[i] = (*env)->GetObjectArrayElement( env, jz, i );
        zdat[i] = (*env)->GetDoubleArrayElements( env, zi[i], 0 );

        if (zny == -1)
            zny = (*env)->GetArrayLength( env, zi[i] );
        else if (zny != (*env)->GetArrayLength( env, zi[i] )) {
            printf( "Misshapen z array.\n" );
            return;
        }
    }

/* Extract the xg data. */
    xgi = (jobject *) malloc( xgnx * sizeof(jobject) );
    xgdat = (jdouble **) malloc( xgnx * sizeof(jdouble *) );

    for( i=0; i < xgnx; i++ )
    {
        xgi[i] = (*env)->GetObjectArrayElement( env, jxg, i );
        xgdat[i] = (*env)->GetDoubleArrayElements( env, xgi[i], 0 );

        if (xgny == -1)
            xgny = (*env)->GetArrayLength( env, xgi[i] );
        else if (xgny != (*env)->GetArrayLength( env, xgi[i] )) {
            printf( "Misshapen xg array.\n" );
            return;
        }
    }

/* Extract the yg data. */
    ygi = (jobject *) malloc( ygnx * sizeof(jobject) );
    ygdat = (jdouble **) malloc( ygnx * sizeof(jdouble *) );

    for( i=0; i < ygnx; i++ )
    {
        ygi[i] = (*env)->GetObjectArrayElement( env, jyg, i );
        ygdat[i] = (*env)->GetDoubleArrayElements( env, ygi[i], 0 );

        if (ygny == -1)
            ygny = (*env)->GetArrayLength( env, ygi[i] );
        else if (ygny != (*env)->GetArrayLength( env, ygi[i] )) {
            printf( "Misshapen yg array.\n" );
            return;
        }
    }

    if (znx != xgnx || znx != ygnx) {
        printf( "Improper x dimensions.\n" );
        return;
    }
    if (zny != xgny || zny != ygny) {
        printf( "Improper y dimensions.\n" );
        return;
    }

    nx = znx; ny = zny;

/* See if PLFLT is compatible with the java numeric type. */
    if (sizeof(PLFLT) == sizeof(jdouble)) {
    /* Yes, we can just initialize the pointers.  Note the cast is so the
     * compiler will be happy in the case where PLFLT does NOT match the java
     * type, in which case this pathway isn't executed. */
        clev = (PLFLT *) clevdat;
        xg = (PLFLT **) xgdat;
        yg = (PLFLT **) ygdat;
        z  = (PLFLT **) zdat;
    } else {
    /* No, we have to actually copy the data. */
        clev = (PLFLT *) malloc( nlev * sizeof(PLFLT) );

        for( i=0; i < nlev; i++ )
            clev[i] = clevdat[i];

        z = (PLFLT **) malloc( znx * sizeof(PLFLT*) );
        z[0] = (PLFLT *) malloc( znx * zny * sizeof(PLFLT) );
        for( i=0; i < znx; i++ )
        {
            z[i] = z[0] + i*zny;
            for( j=0; j < zny; j++ )
                z[i][j] = zdat[i][j];
        }

        xg = (PLFLT **) malloc( xgnx * sizeof(PLFLT*) );
        xg[0] = (PLFLT *) malloc( xgnx * xgny * sizeof(PLFLT) );
        for( i=0; i < xgnx; i++ )
        {
            xg[i] = xg[0] + i*xgny;
            for( j=0; j < xgny; j++ )
                xg[i][j] = xgdat[i][j];
        }

        yg = (PLFLT **) malloc( ygnx * sizeof(PLFLT*) );
        yg[0] = (PLFLT *) malloc( ygnx * ygny * sizeof(PLFLT) );
        for( i=0; i < ygnx; i++ )
        {
            yg[i] = yg[0] + i*ygny;
            for( j=0; j < ygny; j++ )
                yg[i][j] = ygdat[i][j];
        }

        must_free_buffers = 1;
    }

/* Better hav xgnx == ygnx and xgny == ygny */

    if (wrap == 0) {
        cgrid.xg = xg;
        cgrid.yg = yg;
        cgrid.nx = xgnx;
        cgrid.ny = xgny;
        zused = z;
    } else if (wrap == 1) {
        plAlloc2dGrid( &cgrid.xg, nx+1, ny );
        plAlloc2dGrid( &cgrid.yg, nx+1, ny );
        plAlloc2dGrid( &zwrapped, nx+1, ny );

        cgrid.nx = nx+1;
        cgrid.ny = ny;
        zused = zwrapped;

        for( i=0; i < nx; i++ )
            for( j=0; j < ny; j++ ) {
                cgrid.xg[i][j] = xg[i][j];
                cgrid.yg[i][j] = yg[i][j];
                zwrapped[i][j] = z[i][j];
            }

        for( j=0; j < ny; j++ ) {
            cgrid.xg[nx][j] = cgrid.xg[0][j];
            cgrid.yg[nx][j] = cgrid.yg[0][j];
            zwrapped[nx][j] = zwrapped[0][j];
        }

        ny++;
    } else if (wrap == 2) {
        plAlloc2dGrid( &cgrid.xg, nx, ny+1 );
        plAlloc2dGrid( &cgrid.yg, nx, ny+1 );
        plAlloc2dGrid( &zwrapped, nx, ny+1 );

        cgrid.nx = nx;
        cgrid.ny = ny+1;
        zused = zwrapped;

        for( i=0; i < nx; i++ )
            for( j=0; j < ny; j++ ) {
                cgrid.xg[i][j] = xg[i][j];
                cgrid.yg[i][j] = yg[i][j];
                zwrapped[i][j] = z[i][j];
            }

        for( i=0; i < nx; i++ ) {
            cgrid.xg[i][ny] = cgrid.xg[i][0];
            cgrid.yg[i][ny] = cgrid.yg[i][0];
            zwrapped[i][ny] = zwrapped[i][0];
        }

        ny++;
    } else {
        printf( "Invalid wrapping specification.\n" );
    }

    kx = 1; lx = nx;
    ky = 1; ly = ny;

    set_PLStream(env,jthis);

    plcont( zused, nx, ny, kx, lx, ky, ly,
             clev, nlev, pltr2, &cgrid );

    if (must_free_buffers) {
    /* Need to go through an free all the data arrays we malloced. */
        free( clev );

        free( z[0] );
        free( z );

        free( xg[0] );
        free( xg );

        free( yg[0] );
        free( yg );
    }

    (*env)->ReleaseDoubleArrayElements( env, jclev, clevdat, 0 );
    for( i=0; i < znx; i++ )
        (*env)->ReleaseDoubleArrayElements( env, zi[i], zdat[i], 0 );
    for( i=0; i < xgnx; i++ )
        (*env)->ReleaseDoubleArrayElements( env, xgi[i], xgdat[i], 0 );
    for( i=0; i < ygnx; i++ )
        (*env)->ReleaseDoubleArrayElements( env, ygi[i], ygdat[i], 0 );

    if (wrap != 0) {
        plFree2dGrid( cgrid.xg, nx, ny );
        plFree2dGrid( cgrid.yg, nx, ny );
        plFree2dGrid( zwrapped, nx, ny );
    }
}

/*
/*
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
 * Method:    eop
 * Signature: ()V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_eop( JNIEnv *env, jobject jthis )
{
    set_PLStream(env,jthis);
    pleop();
}

/*
 * Class:     plplot_core_PLStream
 * Method:    errx
 * Signature: (I[F[F[F)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_errx__I_3F_3F_3F( JNIEnv *env, jobject jthis,
                                            jint n,
                                            jfloatArray jxmin, jfloatArray jxmax,
                                            jfloatArray jy )
{
    jsize len = (*env)->GetArrayLength( env, jxmin );
    jfloat *jxmindata = (*env)->GetFloatArrayElements( env, jxmin, 0 );
    jfloat *jxmaxdata = (*env)->GetFloatArrayElements( env, jxmax, 0 );
    jfloat *jydata = (*env)->GetFloatArrayElements( env, jy, 0 );
    PLFLT *xmin, *xmax, *y;
    int must_free_buffers = 0;

    setup_array_1d_f( &xmin, jxmindata, n, &must_free_buffers );
    setup_array_1d_f( &xmax, jxmaxdata, n, &must_free_buffers );
    setup_array_1d_f( &y, jydata, n, &must_free_buffers );

    set_PLStream(env,jthis);
    plerrx( n, xmin, xmax, y );

    if (must_free_buffers) {
        free( xmin );
        free( xmax );
        free( y );
    }

    (*env)->ReleaseFloatArrayElements( env, jxmin, jxmindata, 0 );
    (*env)->ReleaseFloatArrayElements( env, jxmax, jxmaxdata, 0 );
    (*env)->ReleaseFloatArrayElements( env, jy, jydata, 0 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    errx
 * Signature: (I[D[D[D)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_errx__I_3D_3D_3D( JNIEnv *env, jobject jthis,
                                            jint n,
                                            jdoubleArray jxmin, jdoubleArray jxmax,
                                            jdoubleArray jy )
{
    jsize len = (*env)->GetArrayLength( env, jxmin );
    jdouble *jxmindata = (*env)->GetDoubleArrayElements( env, jxmin, 0 );
    jdouble *jxmaxdata = (*env)->GetDoubleArrayElements( env, jxmax, 0 );
    jdouble *jydata = (*env)->GetDoubleArrayElements( env, jy, 0 );
    PLFLT *xmin, *xmax, *y;
    int must_free_buffers = 0;

    setup_array_1d_d( &xmin, jxmindata, n, &must_free_buffers );
    setup_array_1d_d( &xmax, jxmaxdata, n, &must_free_buffers );
    setup_array_1d_d( &y, jydata, n, &must_free_buffers );

    set_PLStream(env,jthis);
    plerrx( n, xmin, xmax, y );

    if (must_free_buffers) {
        free( xmin );
        free( xmax );
        free( y );
    }

    (*env)->ReleaseDoubleArrayElements( env, jxmin, jxmindata, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jxmax, jxmaxdata, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jy, jydata, 0 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    erry
 * Signature: (I[F[F[F)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_erry__I_3F_3F_3F( JNIEnv *env, jobject jthis,
                                            jint n, jfloatArray jx,
                                            jfloatArray jymin, jfloatArray jymax )
{
    jsize len = (*env)->GetArrayLength( env, jx );
    jfloat *jxdata = (*env)->GetFloatArrayElements( env, jx, 0 );
    jfloat *jymindata = (*env)->GetFloatArrayElements( env, jymin, 0 );
    jfloat *jymaxdata = (*env)->GetFloatArrayElements( env, jymax, 0 );
    PLFLT *x, *ymin, *ymax;
    int must_free_buffers = 0;

    setup_array_1d_f( &x, jxdata, n, &must_free_buffers );
    setup_array_1d_f( &ymin, jymindata, n, &must_free_buffers );
    setup_array_1d_f( &ymax, jymaxdata, n, &must_free_buffers );

    set_PLStream(env,jthis);
    plerry( n, x, ymin, ymax );

    if (must_free_buffers) {
        free( x );
        free( ymin );
        free( ymax );
    }

    (*env)->ReleaseFloatArrayElements( env, jx, jxdata, 0 );
    (*env)->ReleaseFloatArrayElements( env, jymin, jymindata, 0 );
    (*env)->ReleaseFloatArrayElements( env, jymax, jymaxdata, 0 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    erry
 * Signature: (I[D[D[D)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_erry__I_3D_3D_3D( JNIEnv *env, jobject jthis,
                                            jint n, jdoubleArray jx,
                                            jdoubleArray jymin, jdoubleArray jymax )
{
    jsize len = (*env)->GetArrayLength( env, jx );
    jdouble *jxdata = (*env)->GetDoubleArrayElements( env, jx, 0 );
    jdouble *jymindata = (*env)->GetDoubleArrayElements( env, jymin, 0 );
    jdouble *jymaxdata = (*env)->GetDoubleArrayElements( env, jymax, 0 );
    PLFLT *x, *ymin, *ymax;
    int must_free_buffers = 0;

    setup_array_1d_d( &x, jxdata, n, &must_free_buffers );
    setup_array_1d_d( &ymin, jymindata, n, &must_free_buffers );
    setup_array_1d_d( &ymax, jymaxdata, n, &must_free_buffers );

    set_PLStream(env,jthis);
    plerry( n, x, ymin, ymax );

    if (must_free_buffers) {
        free( x );
        free( ymin );
        free( ymax );
    }

    (*env)->ReleaseDoubleArrayElements( env, jx, jxdata, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jymin, jymindata, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jymax, jymaxdata, 0 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    famadv
 * Signature: ()V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_famadv( JNIEnv *env, jobject jthis )
{
    set_PLStream(env,jthis);
    plfamadv();
}

/*
 * Class:     plplot_core_PLStream
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
    int must_free_buffers = 0;

    setup_array_1d_f( &x, jxdata, n, &must_free_buffers );
    setup_array_1d_f( &y, jydata, n, &must_free_buffers );

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
 * Class:     plplot_core_PLStream
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
    int must_free_buffers = 0;

    setup_array_1d_d( &x, jxdata, n, &must_free_buffers );
    setup_array_1d_d( &y, jydata, n, &must_free_buffers );

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
 * Class:     plplot_core_PLStream
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
    int must_free_buffers = 0;

    setup_array_1d_f( &x, jxdata, n, &must_free_buffers );
    setup_array_1d_f( &y, jydata, n, &must_free_buffers );
    setup_array_1d_f( &z, jzdata, n, &must_free_buffers );

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
 * Class:     plplot_core_PLStream
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
    int must_free_buffers = 0;

    setup_array_1d_d( &x, jxdata, n, &must_free_buffers );
    setup_array_1d_d( &y, jydata, n, &must_free_buffers );
    setup_array_1d_d( &z, jzdata, n, &must_free_buffers );

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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
    int must_free_buffers = 0;

    setup_array_1d_f( &data, jdata, n, &must_free_buffers );

    set_PLStream(env,jthis);
    plhist( n, data, datmin, datmax, nbin, oldwin );

    if (must_free_buffers)
        free(data);

    (*env)->ReleaseFloatArrayElements( env, jdatarr, jdata, 0 );
}

/*
 * Class:     plplot_core_PLStream
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
    int must_free_buffers = 0;

    setup_array_1d_d( &data, jdata, n, &must_free_buffers );

    set_PLStream(env,jthis);
    plhist( n, data, datmin, datmax, nbin, oldwin );

    if (must_free_buffers)
        free(data);

    (*env)->ReleaseDoubleArrayElements( env, jdatarr, jdata, 0 );
}

/*
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
    int must_free_buffers = 0;

    setup_array_1d_f( &x, jxdata, n, &must_free_buffers );
    setup_array_1d_f( &y, jydata, n, &must_free_buffers );

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
 * Class:     plplot_core_PLStream
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

    setup_array_1d_d( &x, jxdata, n, &must_free_buffers );
    setup_array_1d_d( &y, jydata, n, &must_free_buffers );

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
 * Class:     plplot_core_PLStream
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
    int must_free_buffers = 0;

    setup_array_1d_f( &x, jxdata, n, &must_free_buffers );
    setup_array_1d_f( &y, jydata, n, &must_free_buffers );
    setup_array_1d_f( &z, jzdata, n, &must_free_buffers );

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
 * Class:     plplot_core_PLStream
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
    int must_free_buffers = 0;

    setup_array_1d_d( &x, jxdata, n, &must_free_buffers );
    setup_array_1d_d( &y, jydata, n, &must_free_buffers );
    setup_array_1d_d( &z, jzdata, n, &must_free_buffers );

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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
    int must_free_buffers = 0;

    setup_array_1d_f( &x, jxdata, n, &must_free_buffers );
    setup_array_1d_f( &y, jydata, n, &must_free_buffers );

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
 * Class:     plplot_core_PLStream
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
    int must_free_buffers = 0;

    setup_array_1d_d( &x, jxdata, n, &must_free_buffers );
    setup_array_1d_d( &y, jydata, n, &must_free_buffers );

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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Method:    scmap1l
 * Signature: (II[F[F[F[F[I)V
 */
JNIEXPORT void JNICALL
Java_plplot_core_PLStream_scmap1l__II_3F_3F_3F_3F_3I(
  JNIEnv *env, jobject jthis, jint itype, jint npts, jfloatArray ji,
  jfloatArray jc1, jfloatArray jc2, jfloatArray jc3, jintArray jrev )
{
    int must_free_buffers = 0;
    jfloat *jidata = (*env)->GetFloatArrayElements( env, ji, 0 );
    jfloat *jc1data = (*env)->GetFloatArrayElements( env, jc1, 0 );
    jfloat *jc2data = (*env)->GetFloatArrayElements( env, jc2, 0 );
    jfloat *jc3data = (*env)->GetFloatArrayElements( env, jc3, 0 );
    jint *rev = (*env)->GetIntArrayElements( env, jrev, 0 );
    PLFLT *i, *c1, *c2, *c3;

    setup_array_1d_f( &i, jidata, npts, &must_free_buffers );
    setup_array_1d_f( &c1, jc1data, npts, &must_free_buffers );
    setup_array_1d_f( &c2, jc2data, npts, &must_free_buffers );
    setup_array_1d_f( &c3, jc3data, npts, &must_free_buffers );

    set_PLStream(env,jthis);
    plscmap1l(itype, npts, i, c1, c2, c3, (PLINT *) rev);

    if (must_free_buffers) {
	free(i);
	free(c1);
	free(c2);
	free(c3);
    }

    (*env)->ReleaseFloatArrayElements( env, ji, jidata, 0 );
    (*env)->ReleaseFloatArrayElements( env, jc1, jc1data, 0 );
    (*env)->ReleaseFloatArrayElements( env, jc2, jc2data, 0 );
    (*env)->ReleaseFloatArrayElements( env, jc3, jc3data, 0 );
    (*env)->ReleaseIntArrayElements( env, jrev, rev, 0 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    scmap1l
 * Signature: (II[D[D[D[D[I)V
 */
JNIEXPORT void JNICALL
Java_plplot_core_PLStream_scmap1l__II_3D_3D_3D_3D_3I(
  JNIEnv *env, jobject jthis, jint itype, jint npts, jdoubleArray ji,
  jdoubleArray jc1, jdoubleArray jc2, jdoubleArray jc3, jintArray jrev )
{
    int must_free_buffers = 0;
    jdouble *jidata = (*env)->GetDoubleArrayElements( env, ji, 0 );
    jdouble *jc1data = (*env)->GetDoubleArrayElements( env, jc1, 0 );
    jdouble *jc2data = (*env)->GetDoubleArrayElements( env, jc2, 0 );
    jdouble *jc3data = (*env)->GetDoubleArrayElements( env, jc3, 0 );
    jint *rev = (*env)->GetIntArrayElements( env, jrev, 0 );
    PLFLT *i, *c1, *c2, *c3;

    setup_array_1d_d( &i, jidata, npts, &must_free_buffers );
    setup_array_1d_d( &c1, jc1data, npts, &must_free_buffers );
    setup_array_1d_d( &c2, jc2data, npts, &must_free_buffers );
    setup_array_1d_d( &c3, jc3data, npts, &must_free_buffers );

    set_PLStream(env,jthis);
    plscmap1l(itype, npts, i, c1, c2, c3, (PLINT *) rev);

    if (must_free_buffers) {
	free(i);
	free(c1);
	free(c2);
	free(c3);
    }

    (*env)->ReleaseDoubleArrayElements( env, ji, jidata, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jc1, jc1data, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jc2, jc2data, 0 );
    (*env)->ReleaseDoubleArrayElements( env, jc3, jc3data, 0 );
    (*env)->ReleaseIntArrayElements( env, jrev, rev, 0 );
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
 * Method:    setcontlabelparam
 * Signature: (FFFI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_setcontlabelparam__FFFI( JNIEnv *env, jobject jthis,
                                                   jfloat joffset, jfloat jsize,
                                                   jfloat jspacing, jint active )
{
    PLFLT offset = joffset, size = jsize, spacing = jspacing;

    set_PLStream(env,jthis);
    pl_setcontlabelparam( offset, size, spacing, active );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    setcontlabelparam
 * Signature: (DDDI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_setcontlabelparam__DDDI( JNIEnv *env, jobject jthis,
                                                   jdouble joffset, jdouble jsize,
                                                   jdouble jspacing, jint active )
{
    PLFLT offset = joffset, size = jsize, spacing = jspacing;

    set_PLStream(env,jthis);
    pl_setcontlabelparam( offset, size, spacing, active );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    setcontlabelformat
 * Signature: (II)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_setcontlabelformat( JNIEnv *env, jobject jthis,
                                              jint lexp, jint sigdig )
{
    set_PLStream(env,jthis);
    pl_setcontlabelformat( lexp, sigdig );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    shades
 * Signature: ([[FFFFF[FFFFI)V
 */
JNIEXPORT void JNICALL
Java_plplot_core_PLStream_shades___3_3FFFFF_3FFFFI(
    JNIEnv *env, jobject jthis, jobjectArray ja,
    jfloat jleft, jfloat jright, jfloat jbottom, jfloat jtop,
    jfloatArray jclev, jfloat jfill_width,
    jfloat jcont_color, jfloat jcont_width,
    jint rect )
{
}

/*
 * Class:     plplot_core_PLStream
 * Method:    shades
 * Signature: ([[DDDDD[DDDDI)V
 */
JNIEXPORT void JNICALL
Java_plplot_core_PLStream_shades___3_3DDDDD_3DDDDI(
    JNIEnv *env, jobject jthis, jobjectArray ja,
    jdouble jleft, jdouble jright, jdouble jbottom, jdouble jtop,
    jdoubleArray jclev, jdouble jfill_width,
    jdouble jcont_color, jdouble jcont_width,
    jint rect )
{
}

/*
 * Class:     plplot_core_PLStream
 * Method:    shades
 * Signature: ([[FFFFF[FFFFI[F[FI)V
 */
JNIEXPORT void JNICALL
Java_plplot_core_PLStream_shades___3_3FFFFF_3FFFFI_3F_3FI(
    JNIEnv *env, jobject jthis, jobjectArray ja,
    jfloat jleft, jfloat jright, jfloat jbottom, jfloat jtop,
    jfloatArray jclev, jfloat jfill_width,
    jfloat jcont_color, jfloat jcont_width,
    jint rect, jfloatArray jxg, jfloatArray jyg, jint wrap )
{
}

/*
 * Class:     plplot_core_PLStream
 * Method:    shades
 * Signature: ([[DDDDD[DDDDI[D[DI)V
 */
JNIEXPORT void JNICALL
Java_plplot_core_PLStream_shades___3_3DDDDD_3DDDDI_3D_3DI(
    JNIEnv *env, jobject jthis, jobjectArray ja,
    jdouble jleft, jdouble jright, jdouble jbottom, jdouble jtop,
    jdoubleArray jclev, jdouble jfill_width,
    jdouble jcont_color, jdouble jcont_width,
    jint rect, jdoubleArray jxg, jdoubleArray jyg, jint wrap )
{
}

/*
 * Class:     plplot_core_PLStream
 * Method:    shades
 * Signature: ([[FFFFF[FFFFI[[F[[FI)V
 */
JNIEXPORT void JNICALL
Java_plplot_core_PLStream_shades___3_3FFFFF_3FFFFI_3_3F_3_3FI(
    JNIEnv *env, jobject jthis, jobjectArray ja,
    jfloat jleft, jfloat jright, jfloat jbottom, jfloat jtop,
    jfloatArray jclev, jfloat jfill_width,
    jfloat jcont_color, jfloat jcont_width,
    jint rect, jobjectArray jxg, jobjectArray jyg, jint wrap )
{
}

/*
 * Class:     plplot_core_PLStream
 * Method:    shades
 * Signature: ([[DDDDD[DDDDI[[D[[DI)V
 */
JNIEXPORT void JNICALL
Java_plplot_core_PLStream_shades___3_3DDDDD_3DDDDI_3_3D_3_3DI(
    JNIEnv *env, jobject jthis, jobjectArray ja,
    jdouble jleft, jdouble jright, jdouble jbottom, jdouble jtop,
    jdoubleArray jclev, jdouble jfill_width,
    jdouble jcont_color, jdouble jcont_width,
    jint rect, jobjectArray jxg, jobjectArray jyg, jint wrap )
{
}

/*
 * Class:     plplot_core_PLStream
 * Method:    shade
 * Signature: ([[FFFFFFFIFIIIIII)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_shade___3_3FFFFFFFIFIIIIII(
    JNIEnv *env, jobject jthis,
    jobjectArray ja,
    jfloat jleft, jfloat jright, jfloat jbottom, jfloat jtop,
    jfloat jshade_min, jfloat jshade_max,
    jint sh_cmap, jfloat jsh_color, jint sh_width,
    jint min_color, jint min_width, jint max_color, jint max_width,
    jint rect )
{
    PLFLT left = jleft, right = jright;
    PLFLT bottom = jbottom, top = jtop;
    PLFLT shade_min = jshade_min, shade_max = jshade_max;
    PLFLT sh_color = jsh_color;

    jfloat **adat;
    jobject *ai;
    PLFLT **a;

    int nx = (*env)->GetArrayLength( env, ja );
    int ny = -1;

    int must_free_buffers = 0;
    int i, j;

    ai = (jobject *) malloc( nx * sizeof(jobject) );
    adat = (jfloat **) malloc( nx * sizeof(jfloat *) );

    for( i=0; i < nx; i++ )
    {
        ai[i] = (*env)->GetObjectArrayElement( env, ja, i );
        adat[i] = (*env)->GetFloatArrayElements( env, ai[i], 0 );

        if (ny == -1)
            ny = (*env)->GetArrayLength( env, ai[i] );
        else if (ny != (*env)->GetArrayLength( env, ai[i] )) {
            printf( "Misshapen a array.\n" );
            return;
        }
    }

    setup_array_2d_f( &a, adat, nx, ny, &must_free_buffers );

    set_PLStream(env,jthis);
    plshade( a, nx, ny, NULL,
             left, right, bottom, top,
             shade_min, shade_max,
             sh_cmap, sh_color, sh_width,
             min_color, min_width, max_color, max_width,
             plfill, rect, NULL, NULL );

    if (must_free_buffers) {
        free( a[0] );
        free( a );
    }

    for( i=0; i < nx; i++ )
        (*env)->ReleaseFloatArrayElements( env, ai[i], adat[i], 0 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    shade
 * Signature: ([[DDDDDDDIDIIIIII)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_shade___3_3DDDDDDDIDIIIIII(
    JNIEnv *env, jobject jthis,
    jobjectArray ja,
    jdouble jleft, jdouble jright, jdouble jbottom, jdouble jtop,
    jdouble jshade_min, jdouble jshade_max,
    jint sh_cmap, jdouble jsh_color, jint sh_width,
    jint min_color, jint min_width, jint max_color, jint max_width,
    jint rect )
{
    PLFLT left = jleft, right = jright;
    PLFLT bottom = jbottom, top = jtop;
    PLFLT shade_min = jshade_min, shade_max = jshade_max;
    PLFLT sh_color = jsh_color;

    jdouble **adat;
    jobject *ai;
    PLFLT **a;

    int nx = (*env)->GetArrayLength( env, ja );
    int ny = -1;

    int must_free_buffers = 0;
    int i, j;

    ai = (jobject *) malloc( nx * sizeof(jobject) );
    adat = (jdouble **) malloc( nx * sizeof(jdouble *) );

    for( i=0; i < nx; i++ )
    {
        ai[i] = (*env)->GetObjectArrayElement( env, ja, i );
        adat[i] = (*env)->GetDoubleArrayElements( env, ai[i], 0 );

        if (ny == -1)
            ny = (*env)->GetArrayLength( env, ai[i] );
        else if (ny != (*env)->GetArrayLength( env, ai[i] )) {
            printf( "Misshapen a array.\n" );
            return;
        }
    }

    setup_array_2d_d( &a, adat, nx, ny, &must_free_buffers );

    set_PLStream(env,jthis);
    plshade( a, nx, ny, NULL,
             left, right, bottom, top,
             shade_min, shade_max,
             sh_cmap, sh_color, sh_width,
             min_color, min_width, max_color, max_width,
             plfill, rect, NULL, NULL );

    if (must_free_buffers) {
        free( a[0] );
        free( a );
    }

    for( i=0; i < nx; i++ )
        (*env)->ReleaseDoubleArrayElements( env, ai[i], adat[i], 0 );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    shade
 * Signature: ([[FFFFFFFIFIIIIII[F[FI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_shade___3_3FFFFFFFIFIIIIII_3F_3FI(
    JNIEnv *env, jobject jthis,
    jobjectArray ja,
    jfloat jleft, jfloat jright, jfloat jbottom, jfloat jtop,
    jfloat jshade_min, jfloat jshade_max,
    jint sh_cmap, jfloat jsh_color, jint sh_width,
    jint min_color, jint min_width, jint max_color, jint max_width, jint rect,
    jfloatArray jxg, jfloatArray jyg, jint wrap )
{
}

/*
 * Class:     plplot_core_PLStream
 * Method:    shade
 * Signature: ([[DDDDDDDIDIIIIII[D[DI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_shade___3_3DDDDDDDIDIIIIII_3D_3DI(
    JNIEnv *env, jobject jthis,
    jobjectArray ja,
    jdouble jleft, jdouble jright, jdouble jbottom, jdouble jtop,
    jdouble jshade_min, jdouble jshade_max,
    jint sh_cmap, jdouble jsh_color, jint sh_width,
    jint min_color, jint min_width, jint max_color, jint max_width, jint rect,
    jdoubleArray jxg, jdoubleArray jyg, jint wrap )
{
}

/*
 * Class:     plplot_core_PLStream
 * Method:    shade
 * Signature: ([[FFFFFFFIFIIIIII[[F[[FI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_shade___3_3FFFFFFFIFIIIIII_3_3F_3_3FI(
    JNIEnv *env, jobject jthis,
    jobjectArray ja,
    jfloat jleft, jfloat jright, jfloat jbottom, jfloat jtop,
    jfloat jshade_min, jfloat jshade_max,
    jint sh_cmap, jfloat jsh_color, jint sh_width,
    jint min_color, jint min_width, jint max_color, jint max_width, jint rect,
    jobjectArray jxg, jobjectArray jyg, jint wrap )
{
}

/*
 * Class:     plplot_core_PLStream
 * Method:    shade
 * Signature: ([[DDDDDDDIDIIIIII[[D[[DI)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_shade___3_3DDDDDDDIDIIIIII_3_3D_3_3DI(
    JNIEnv *env, jobject jthis,
    jobjectArray ja,
    jdouble jleft, jdouble jright, jdouble jbottom, jdouble jtop,
    jdouble jshade_min, jdouble jshade_max,
    jint sh_cmap, jdouble jsh_color, jint sh_width,
    jint min_color, jint min_width, jint max_color, jint max_width, jint rect,
    jobjectArray jxg, jobjectArray jyg, jint wrap )
{
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
 * Method:    sxax
 * Signature: (II)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_sxax( JNIEnv *env, jobject jthis,
                                jint digmax, jint digits )
{
    set_PLStream(env,jthis);
    plsxax( digmax, digits );
}

/*
 * Class:     plplot_core_PLStream
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
    jfloat *jxdata = (*env)->GetFloatArrayElements( env, jx, 0 );
    jfloat *jydata = (*env)->GetFloatArrayElements( env, jy, 0 );
    int must_free_buffers = 0;

    setup_array_1d_f( &x, jxdata, n, &must_free_buffers );
    setup_array_1d_f( &y, jydata, n, &must_free_buffers );

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
    jdouble *jxdata = (*env)->GetDoubleArrayElements( env, jx, 0 );
    jdouble *jydata = (*env)->GetDoubleArrayElements( env, jy, 0 );
    int must_free_buffers = 0;

    setup_array_1d_d( &x, jxdata, n, &must_free_buffers );
    setup_array_1d_d( &y, jydata, n, &must_free_buffers );

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
 * Class:     plplot_core_PLStream
 * Method:    szax
 * Signature: (II)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_szax( JNIEnv *env, jobject jthis,
                                jint digmax, jint digits )
{
    set_PLStream(env,jthis);
    plszax( digmax, digits );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    text
 * Signature: ()V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_text( JNIEnv *env, jobject jthis )
{
    set_PLStream(env,jthis);
    pltext();
}

/*
 * Class:     plplot_core_PLStream
 * Method:    vasp
 * Signature: (F)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_vasp__F( JNIEnv *env, jobject jthis, jfloat jaspect )
{
    PLFLT aspect = jaspect;
    set_PLStream(env,jthis);
    plvasp( aspect );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    vasp
 * Signature: (D)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_vasp__D( JNIEnv *env, jobject jthis, jdouble jaspect )
{
    PLFLT aspect = jaspect;
    set_PLStream(env,jthis);
    plvasp( aspect );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    vpas
 * Signature: (FFFFF)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_vpas__FFFFF( JNIEnv *env, jobject jthis,
                                       jfloat jxmin, jfloat jxmax,
                                       jfloat jymin, jfloat jymax,
                                       jfloat jaspect )
{
    PLFLT xmin = jxmin, xmax = jxmax;
    PLFLT ymin = jymin, ymax = jymax;
    PLFLT aspect = jaspect;
    set_PLStream(env,jthis);
    plvpas( xmin, xmax, ymin, ymax, aspect );
}

/*
 * Class:     plplot_core_PLStream
 * Method:    vpas
 * Signature: (DDDDD)V
 */

JNIEXPORT void JNICALL
Java_plplot_core_PLStream_vpas__DDDDD( JNIEnv *env, jobject jthis,
                                       jdouble jxmin, jdouble jxmax,
                                       jdouble jymin, jdouble jymax,
                                       jdouble jaspect )
{
    PLFLT xmin = jxmin, xmax = jxmax;
    PLFLT ymin = jymin, ymax = jymax;
    PLFLT aspect = jaspect;
    set_PLStream(env,jthis);
    plvpas( xmin, xmax, ymin, ymax, aspect );
}

/*
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
 * Class:     plplot_core_PLStream
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
