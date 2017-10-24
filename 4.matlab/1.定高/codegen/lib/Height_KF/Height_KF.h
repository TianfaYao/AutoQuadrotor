/*
 * File: Height_KF.h
 *
 * MATLAB Coder version            : 2.6
 * C/C++ source code generated on  : 27-Apr-2017 00:45:04
 */

#ifndef __HEIGHT_KF_H__
#define __HEIGHT_KF_H__

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "Height_KF_types.h"

/* Function Declarations */
extern void Height_KF(const float Q[16], const float H[8], const float R[4],
                      const float F[16], const unsigned char I[16], const float
                      Z[2], float *h, float *v, float *a);
extern void Height_KF_init(void);

#endif

/*
 * File trailer for Height_KF.h
 *
 * [EOF]
 */
