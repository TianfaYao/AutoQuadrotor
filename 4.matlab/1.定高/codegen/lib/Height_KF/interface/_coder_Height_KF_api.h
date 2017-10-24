/* 
 * File: _coder_Height_KF_api.h 
 *  
 * MATLAB Coder version            : 2.6 
 * C/C++ source code generated on  : 27-Apr-2017 00:45:04 
 */

#ifndef ___CODER_HEIGHT_KF_API_H__
#define ___CODER_HEIGHT_KF_API_H__
/* Include files */
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"

/* Function Declarations */
extern void Height_KF_initialize(emlrtContext *aContext);
extern void Height_KF_terminate(void);
extern void Height_KF_atexit(void);
extern void Height_KF_api(const mxArray *prhs[6], const mxArray *plhs[3]);
extern void Height_KF(float Q[16], float H[8], float R[4], float F[16], unsigned char I[16], float Z[2], float *h, float *v, float *a);
extern void Height_KF_xil_terminate(void);

#endif
/* 
 * File trailer for _coder_Height_KF_api.h 
 *  
 * [EOF] 
 */
