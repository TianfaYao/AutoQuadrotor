/*
 * File: _coder_Height_KF_api.c
 *
 * MATLAB Coder version            : 2.6
 * C/C++ source code generated on  : 27-Apr-2017 00:45:04
 */

/* Include files */
#include "_coder_Height_KF_api.h"

/* Function Declarations */
static float (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *Q, const
  char *identifier))[16];
static float (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[16];
static float (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *H, const
  char *identifier))[8];
static float (*d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[8];
static float (*e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *R, const
  char *identifier))[4];
static float (*f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[4];
static unsigned char (*g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *I,
  const char *identifier))[16];
static unsigned char (*h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId))[16];
static float (*i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *Z, const
  char *identifier))[2];
static float (*j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[2];
static const mxArray *emlrt_marshallOut(const float u);
static float (*k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[16];
static float (*l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[8];
static float (*m_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[4];
static unsigned char (*n_emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *src, const emlrtMsgIdentifier *msgId))[16];
static float (*o_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[2];

/* Function Definitions */

/*
 * Arguments    : emlrtContext *aContext
 * Return Type  : void
 */
void Height_KF_initialize(emlrtContext *aContext)
{
  emlrtStack st = { NULL, NULL, NULL };

  emlrtCreateRootTLS(&emlrtRootTLSGlobal, aContext, NULL, 1);
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, 0);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void Height_KF_terminate(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void Height_KF_atexit(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  emlrtCreateRootTLS(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1);
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  Height_KF_xil_terminate();
}

/*
 * Arguments    : const mxArray *prhs[6]
 *                const mxArray *plhs[3]
 * Return Type  : void
 */
void Height_KF_api(const mxArray *prhs[6], const mxArray *plhs[3])
{
  float (*Q)[16];
  float (*H)[8];
  float (*R)[4];
  float (*F)[16];
  unsigned char (*I)[16];
  float (*Z)[2];
  float a;
  float v;
  float h;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, false, -1);
  prhs[1] = emlrtProtectR2012b(prhs[1], 1, false, -1);
  prhs[2] = emlrtProtectR2012b(prhs[2], 2, false, -1);
  prhs[3] = emlrtProtectR2012b(prhs[3], 3, false, -1);
  prhs[4] = emlrtProtectR2012b(prhs[4], 4, false, -1);
  prhs[5] = emlrtProtectR2012b(prhs[5], 5, false, -1);

  /* Marshall function inputs */
  Q = emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "Q");
  H = c_emlrt_marshallIn(&st, emlrtAlias(prhs[1]), "H");
  R = e_emlrt_marshallIn(&st, emlrtAlias(prhs[2]), "R");
  F = emlrt_marshallIn(&st, emlrtAlias(prhs[3]), "F");
  I = g_emlrt_marshallIn(&st, emlrtAlias(prhs[4]), "I");
  Z = i_emlrt_marshallIn(&st, emlrtAlias(prhs[5]), "Z");

  /* Invoke the target function */
  Height_KF(*Q, *H, *R, *F, *I, *Z, &h, &v, &a);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(h);
  plhs[1] = emlrt_marshallOut(v);
  plhs[2] = emlrt_marshallOut(a);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *Q
 *                const char *identifier
 * Return Type  : float (*)[16]
 */
static float (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *Q, const
  char *identifier))[16]
{
  float (*y)[16];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  y = b_emlrt_marshallIn(sp, emlrtAlias(Q), &thisId);
  emlrtDestroyArray(&Q);
  return y;
}
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : float (*)[16]
 */
  static float (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId))[16]
{
  float (*y)[16];
  y = k_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *H
 *                const char *identifier
 * Return Type  : float (*)[8]
 */
static float (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *H, const
  char *identifier))[8]
{
  float (*y)[8];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  y = d_emlrt_marshallIn(sp, emlrtAlias(H), &thisId);
  emlrtDestroyArray(&H);
  return y;
}
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : float (*)[8]
 */
  static float (*d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId))[8]
{
  float (*y)[8];
  y = l_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *R
 *                const char *identifier
 * Return Type  : float (*)[4]
 */
static float (*e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *R, const
  char *identifier))[4]
{
  float (*y)[4];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  y = f_emlrt_marshallIn(sp, emlrtAlias(R), &thisId);
  emlrtDestroyArray(&R);
  return y;
}
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : float (*)[4]
 */
  static float (*f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId))[4]
{
  float (*y)[4];
  y = m_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *I
 *                const char *identifier
 * Return Type  : unsigned char (*)[16]
 */
static unsigned char (*g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *I,
  const char *identifier))[16]
{
  unsigned char (*y)[16];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  y = h_emlrt_marshallIn(sp, emlrtAlias(I), &thisId);
  emlrtDestroyArray(&I);
  return y;
}
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : unsigned char (*)[16]
 */
  static unsigned char (*h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *
  u, const emlrtMsgIdentifier *parentId))[16]
{
  unsigned char (*y)[16];
  y = n_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *Z
 *                const char *identifier
 * Return Type  : float (*)[2]
 */
static float (*i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *Z, const
  char *identifier))[2]
{
  float (*y)[2];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  y = j_emlrt_marshallIn(sp, emlrtAlias(Z), &thisId);
  emlrtDestroyArray(&Z);
  return y;
}
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : float (*)[2]
 */
  static float (*j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId))[2]
{
  float (*y)[2];
  y = o_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const float u
 * Return Type  : const mxArray *
 */
static const mxArray *emlrt_marshallOut(const float u)
{
  const mxArray *y;
  const mxArray *m0;
  y = NULL;
  m0 = emlrtCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
  *(float *)mxGetData(m0) = u;
  emlrtAssign(&y, m0);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : float (*)[16]
 */
static float (*k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[16]
{
  float (*ret)[16];
  int iv0[2];
  int i;
  for (i = 0; i < 2; i++) {
    iv0[i] = 4;
  }

  emlrtCheckBuiltInR2012b(sp, msgId, src, "single", false, 2U, iv0);
  ret = (float (*)[16])mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : float (*)[8]
 */
  static float (*l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[8]
{
  float (*ret)[8];
  int iv1[2];
  int i0;
  for (i0 = 0; i0 < 2; i0++) {
    iv1[i0] = 2 + (i0 << 1);
  }

  emlrtCheckBuiltInR2012b(sp, msgId, src, "single", false, 2U, iv1);
  ret = (float (*)[8])mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : float (*)[4]
 */
static float (*m_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[4]
{
  float (*ret)[4];
  int iv2[2];
  int i;
  for (i = 0; i < 2; i++) {
    iv2[i] = 2;
  }

  emlrtCheckBuiltInR2012b(sp, msgId, src, "single", false, 2U, iv2);
  ret = (float (*)[4])mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : unsigned char (*)[16]
 */
  static unsigned char (*n_emlrt_marshallIn(const emlrtStack *sp, const mxArray *
  src, const emlrtMsgIdentifier *msgId))[16]
{
  unsigned char (*ret)[16];
  int iv3[2];
  int i;
  for (i = 0; i < 2; i++) {
    iv3[i] = 4;
  }

  emlrtCheckBuiltInR2012b(sp, msgId, src, "uint8", false, 2U, iv3);
  ret = (unsigned char (*)[16])mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : float (*)[2]
 */
static float (*o_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[2]
{
  float (*ret)[2];
  int iv4[1];
  iv4[0] = 2;
  emlrtCheckBuiltInR2012b(sp, msgId, src, "single", false, 1U, iv4);
  ret = (float (*)[2])mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}
/*
 * File trailer for _coder_Height_KF_api.c
 *
 * [EOF]
 */
