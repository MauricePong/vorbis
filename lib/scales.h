/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2001             *
 * by the XIPHOPHORUS Company http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

 function: linear scale -> dB, Bark and Mel scales
 last mod: $Id: scales.h,v 1.21 2002/03/17 19:50:47 xiphmont Exp $

 ********************************************************************/

#ifndef _V_SCALES_H_
#define _V_SCALES_H_

#include <math.h>
#include "os.h"

/* 20log10(x) */
#ifdef VORBIS_IEEE_FLOAT32

static float unitnorm(float x){
  ogg_uint32_t *ix=(ogg_uint32_t *)&x;
  *ix=(*ix&0x80000000UL)|(0x3f800000UL);
  return(x);
}

static float FABS(float x){
  ogg_uint32_t *ix=(ogg_uint32_t *)&x;
  *ix&=0x7fffffffUL;
  return(x);
}

static float todB_LOOKUP[256]={
        -140.277330f, -139.633636f, -139.034372f, -138.473797f, 
        -137.450747f, -136.535597f, -135.707743f, -134.951972f, 
        -134.256730f, -133.613036f, -133.013772f, -132.453198f, 
        -131.430147f, -130.514997f, -129.687144f, -128.931372f, 
        -128.236130f, -127.592437f, -126.993172f, -126.432598f, 
        -125.409547f, -124.494397f, -123.666544f, -122.910772f, 
        -122.215530f, -121.571837f, -120.972572f, -120.411998f, 
        -119.388947f, -118.473797f, -117.645944f, -116.890173f, 
        -116.194930f, -115.551237f, -114.951972f, -114.391398f, 
        -113.368347f, -112.453198f, -111.625344f, -110.869573f, 
        -110.174331f, -109.530637f, -108.931372f, -108.370798f, 
        -107.347748f, -106.432598f, -105.604744f, -104.848973f, 
        -104.153731f, -103.510037f, -102.910773f, -102.350198f, 
        -101.327148f, -100.411998f,  -99.584144f,  -98.828373f, 
         -98.133131f,  -97.489437f,  -96.890173f,  -96.329598f, 
         -95.306548f,  -94.391398f,  -93.563544f,  -92.807773f, 
         -92.112531f,  -91.468837f,  -90.869573f,  -90.308998f, 
         -89.285948f,  -88.370798f,  -87.542944f,  -86.787173f, 
         -86.091931f,  -85.448237f,  -84.848973f,  -84.288398f, 
         -83.265348f,  -82.350198f,  -81.522344f,  -80.766573f, 
         -80.071331f,  -79.427637f,  -78.828373f,  -78.267799f, 
         -77.244748f,  -76.329598f,  -75.501745f,  -74.745973f, 
         -74.050731f,  -73.407038f,  -72.807773f,  -72.247199f, 
         -71.224148f,  -70.308998f,  -69.481145f,  -68.725373f, 
         -68.030131f,  -67.386438f,  -66.787173f,  -66.226599f, 
         -65.203548f,  -64.288399f,  -63.460545f,  -62.704774f, 
         -62.009531f,  -61.365838f,  -60.766573f,  -60.205999f, 
         -59.182948f,  -58.267799f,  -57.439945f,  -56.684174f, 
         -55.988932f,  -55.345238f,  -54.745973f,  -54.185399f, 
         -53.162349f,  -52.247199f,  -51.419345f,  -50.663574f, 
         -49.968332f,  -49.324638f,  -48.725374f,  -48.164799f, 
         -47.141749f,  -46.226599f,  -45.398745f,  -44.642974f, 
         -43.947732f,  -43.304038f,  -42.704774f,  -42.144199f, 
         -41.121149f,  -40.205999f,  -39.378145f,  -38.622374f, 
         -37.927132f,  -37.283438f,  -36.684174f,  -36.123599f, 
         -35.100549f,  -34.185399f,  -33.357545f,  -32.601774f, 
         -31.906532f,  -31.262838f,  -30.663574f,  -30.102999f, 
         -29.079949f,  -28.164799f,  -27.336945f,  -26.581174f, 
         -25.885932f,  -25.242238f,  -24.642974f,  -24.082400f, 
         -23.059349f,  -22.144199f,  -21.316346f,  -20.560574f, 
         -19.865332f,  -19.221639f,  -18.622374f,  -18.061800f, 
         -17.038749f,  -16.123599f,  -15.295746f,  -14.539974f, 
         -13.844732f,  -13.201039f,  -12.601774f,  -12.041200f, 
         -11.018149f,  -10.103000f,   -9.275146f,   -8.519375f, 
          -7.824132f,   -7.180439f,   -6.581174f,   -6.020600f, 
          -4.997549f,   -4.082400f,   -3.254546f,   -2.498775f, 
          -1.803533f,   -1.159839f,   -0.560574f,    0.000000f, 
           1.023050f,    1.938200f,    2.766054f,    3.521825f, 
	   4.217067f,    4.860761f,    5.460025f,    6.020600f, 
           7.043650f,    7.958800f,    8.786654f,    9.542425f, 
          10.237667f,   10.881361f,   11.480625f,   12.041200f, 
          13.064250f,   13.979400f,   14.807254f,   15.563025f, 
          16.258267f,   16.901961f,   17.501225f,   18.061800f, 
          19.084850f,   20.000000f,   20.827854f,   21.583625f, 
          22.278867f,   22.922561f,   23.521825f,   24.082400f, 
          25.105450f,   26.020600f,   26.848453f,   27.604225f, 
          28.299467f,   28.943161f,   29.542425f,   30.102999f, 
          31.126050f,   32.041200f,   32.869053f,   33.624825f, 
          34.320067f,   34.963760f,   35.563025f,   36.123599f, 
          37.146650f,   38.061800f,   38.889653f,   39.645424f, 
          40.340667f,   40.984360f,   41.583625f,   42.144199f, 
          43.167250f,   44.082399f,   44.910253f,   45.666024f, 
          46.361266f,   47.004960f,   47.604225f,   48.164799f, 
          49.187850f,   50.102999f,   50.930853f,   51.686624f
};

static float todB(const float *x){
  ogg_int32_t *i=(ogg_int32_t *)x;
  ogg_int32_t temp=((*i&0x7fffffff)-0x33c7ffff)>>20;
  if(temp<0)return -400.f;
  return(todB_LOOKUP[temp]);
}

#define todB_nn(x) todB(x)

#else

static float unitnorm(float x){
  if(x<0)return(-1.f);
  return(1.f);
}

#define FABS(x) fabs(*(x))

#define todB(x)   (*(x)==0?-400.f:log(*(x)**(x))*4.34294480f)
#define todB_nn(x)   (*(x)==0.f?-400.f:log(*(x))*8.6858896f)

#endif 

#define fromdB(x) (exp((x)*.11512925f))  

/* The bark scale equations are approximations, since the original
   table was somewhat hand rolled.  The below are chosen to have the
   best possible fit to the rolled tables, thus their somewhat odd
   appearance (these are more accurate and over a longer range than
   the oft-quoted bark equations found in the texts I have).  The
   approximations are valid from 0 - 30kHz (nyquist) or so.

   all f in Hz, z in Bark */

#define toBARK(n)   (13.1f*atan(.00074f*(n))+2.24f*atan((n)*(n)*1.85e-8f)+1e-4f*(n))
#define fromBARK(z) (102.f*(z)-2.f*pow(z,2.f)+.4f*pow(z,3.f)+pow(1.46f,z)-1.f)
#define toMEL(n)    (log(1.f+(n)*.001f)*1442.695f)
#define fromMEL(m)  (1000.f*exp((m)/1442.695f)-1000.f)

/* Frequency to octave.  We arbitrarily declare 63.5 Hz to be octave
   0.0 */

#define toOC(n)     (log(n)*1.442695f-5.965784f)
#define fromOC(o)   (exp(((o)+5.965784f)*.693147f))

#endif

