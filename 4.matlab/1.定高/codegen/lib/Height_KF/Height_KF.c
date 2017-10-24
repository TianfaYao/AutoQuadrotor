/*
 * File: Height_KF.c
 *
 * MATLAB Coder version            : 2.6
 * C/C++ source code generated on  : 27-Apr-2017 00:45:04
 */

/* Include files */
#include "rt_nonfinite.h"
#include "Height_KF.h"

/* Variable Definitions */
static float X[4];
static float P[16];

/* Function Definitions */

/*
 * ���� ytf
 * ���� 2017��4��27��
 * ״̬ת��Э�������
 * H Ԥ�����
 * R �۲���������
 * F ״̬ת�ƾ���
 * I
 * Z ��������
 * Arguments    : const float Q[16]
 *                const float H[8]
 *                const float R[4]
 *                const float F[16]
 *                const unsigned char I[16]
 *                const float Z[2]
 *                float *h
 *                float *v
 *                float *a
 * Return Type  : void
 */
void Height_KF(const float Q[16], const float H[8], const float R[4], const
               float F[16], const unsigned char I[16], const float Z[2], float
               *h, float *v, float *a)
{
  float B[4];
  int i;
  float b_F[16];
  int r2;
  int k;
  float a21;
  float y[8];
  float K[8];
  float a22;
  float b_Z[2];
  float b_B[16];

  /*  ״̬���� */
  /*  ״̬Э������� */
  for (i = 0; i < 4; i++) {
    B[i] = X[i];
  }

  for (i = 0; i < 4; i++) {
    X[i] = 0.0F;
    for (r2 = 0; r2 < 4; r2++) {
      X[i] += F[i + (r2 << 2)] * B[r2];
    }

    for (r2 = 0; r2 < 4; r2++) {
      b_F[i + (r2 << 2)] = 0.0F;
      for (k = 0; k < 4; k++) {
        b_F[i + (r2 << 2)] += F[i + (k << 2)] * P[k + (r2 << 2)];
      }
    }
  }

  for (i = 0; i < 4; i++) {
    for (r2 = 0; r2 < 4; r2++) {
      a21 = 0.0F;
      for (k = 0; k < 4; k++) {
        a21 += b_F[i + (k << 2)] * F[r2 + (k << 2)];
      }

      P[i + (r2 << 2)] = a21 + Q[i + (r2 << 2)];
    }
  }

  for (i = 0; i < 4; i++) {
    for (r2 = 0; r2 < 2; r2++) {
      y[i + (r2 << 2)] = 0.0F;
      for (k = 0; k < 4; k++) {
        y[i + (r2 << 2)] += P[i + (k << 2)] * H[r2 + (k << 1)];
      }
    }
  }

  for (i = 0; i < 2; i++) {
    for (r2 = 0; r2 < 4; r2++) {
      K[i + (r2 << 1)] = 0.0F;
      for (k = 0; k < 4; k++) {
        K[i + (r2 << 1)] += H[i + (k << 1)] * P[k + (r2 << 2)];
      }
    }
  }

  for (i = 0; i < 2; i++) {
    for (r2 = 0; r2 < 2; r2++) {
      a21 = 0.0F;
      for (k = 0; k < 4; k++) {
        a21 += K[i + (k << 1)] * H[r2 + (k << 1)];
      }

      B[i + (r2 << 1)] = a21 + R[i + (r2 << 1)];
    }
  }

  if ((real32_T)fabs(B[1]) > (real32_T)fabs(B[0])) {
    i = 1;
    r2 = 0;
  } else {
    i = 0;
    r2 = 1;
  }

  a21 = B[r2] / B[i];
  a22 = B[2 + r2] - a21 * B[2 + i];
  for (k = 0; k < 4; k++) {
    K[k + (i << 2)] = y[k] / B[i];
    K[k + (r2 << 2)] = (y[4 + k] - K[k + (i << 2)] * B[2 + i]) / a22;
    K[k + (i << 2)] -= K[k + (r2 << 2)] * a21;
  }

  /* ���� */
  for (i = 0; i < 2; i++) {
    a21 = 0.0F;
    for (r2 = 0; r2 < 4; r2++) {
      a21 += H[i + (r2 << 1)] * X[r2];
    }

    b_Z[i] = Z[i] - a21;
  }

  for (i = 0; i < 4; i++) {
    a21 = 0.0F;
    for (r2 = 0; r2 < 2; r2++) {
      a21 += K[i + (r2 << 2)] * b_Z[r2];
    }

    X[i] += a21;
  }

  memcpy(&b_B[0], &P[0], sizeof(float) << 4);
  for (i = 0; i < 4; i++) {
    for (r2 = 0; r2 < 4; r2++) {
      a21 = 0.0F;
      for (k = 0; k < 2; k++) {
        a21 += K[i + (k << 2)] * H[k + (r2 << 1)];
      }

      b_F[i + (r2 << 2)] = (float)I[i + (r2 << 2)] - a21;
    }
  }

  for (i = 0; i < 4; i++) {
    for (r2 = 0; r2 < 4; r2++) {
      P[i + (r2 << 2)] = 0.0F;
      for (k = 0; k < 4; k++) {
        P[i + (r2 << 2)] += b_F[i + (k << 2)] * b_B[k + (r2 << 2)];
      }
    }
  }

  *h = X[0];
  *v = X[1];
  *a = X[2];
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void Height_KF_init(void)
{
  static const float fv0[16] = { 10.0F, 0.0F, 0.0F, 0.0F, 0.0F, 10.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 10.0F, 0.0F, 0.0F, 0.0F, 0.0F, 100.0F };

  int i;
  memcpy(&P[0], &fv0[0], sizeof(float) << 4);
  for (i = 0; i < 4; i++) {
    X[i] = 0.0F;
  }
}

/*
 * File trailer for Height_KF.c
 *
 * [EOF]
 */
