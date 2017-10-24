/*
 * File: Height_KF_initialize.c
 *
 * MATLAB Coder version            : 2.6
 * C/C++ source code generated on  : 27-Apr-2017 00:45:04
 */

/* Include files */
#include "rt_nonfinite.h"
#include "Height_KF.h"
#include "Height_KF_initialize.h"

/* Function Definitions */

/*
 * Arguments    : void
 * Return Type  : void
 */
void Height_KF_initialize(void)
{
  rt_InitInfAndNaN(8U);
  Height_KF_init();
}

/*
 * File trailer for Height_KF_initialize.c
 *
 * [EOF]
 */
