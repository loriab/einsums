/*****************************************************************************
  Copyright (c) 2014, Intel Corp.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************
* Contents: Native high-level C interface to LAPACK function sggsvp3
* Author: Intel Corporation
*****************************************************************************/

#include "lapacke_utils.h"

lapack_int LAPACKE_sggsvp3(int matrix_layout, char jobu, char jobv, char jobq, lapack_int m, lapack_int p, lapack_int n, float *a,
                           lapack_int lda, float *b, lapack_int ldb, float tola, float tolb, lapack_int *k, lapack_int *l, float *u,
                           lapack_int ldu, float *v, lapack_int ldv, float *q, lapack_int ldq) {
    lapack_int info = 0;
    lapack_int *iwork = NULL;
    float *tau = NULL;
    float *work = NULL;
    lapack_int lwork = -1;
    float work_query;
    if (matrix_layout != LAPACK_COL_MAJOR && matrix_layout != LAPACK_ROW_MAJOR) {
        LAPACKE_xerbla("LAPACKE_sggsvp3", -1);
        return -1;
    }
#ifndef LAPACK_DISABLE_NAN_CHECK
    if (LAPACKE_get_nancheck()) {
        /* Optionally check input matrices for NaNs */
        if (LAPACKE_sge_nancheck(matrix_layout, m, n, a, lda)) {
            return -8;
        }
        if (LAPACKE_sge_nancheck(matrix_layout, p, n, b, ldb)) {
            return -10;
        }
        if (LAPACKE_s_nancheck(1, &tola, 1)) {
            return -12;
        }
        if (LAPACKE_s_nancheck(1, &tolb, 1)) {
            return -13;
        }
    }
#endif
    /* Query optimal size for working array */
    info = LAPACKE_sggsvp3_work(matrix_layout, jobu, jobv, jobq, m, p, n, a, lda, b, ldb, tola, tolb, k, l, u, ldu, v, ldv, q, ldq, iwork,
                                tau, &work_query, lwork);
    if (info != 0)
        goto exit_level_0;
    lwork = work_query;
    /* Allocate memory for working array(s) */
    iwork = (lapack_int *)LAPACKE_malloc(sizeof(lapack_int) * MAX(1, n));
    if (iwork == NULL) {
        info = LAPACK_WORK_MEMORY_ERROR;
        goto exit_level_0;
    }
    tau = (float *)LAPACKE_malloc(sizeof(float) * MAX(1, n));
    if (tau == NULL) {
        info = LAPACK_WORK_MEMORY_ERROR;
        goto exit_level_1;
    }
    work = (float *)LAPACKE_malloc(sizeof(float) * lwork);
    if (work == NULL) {
        info = LAPACK_WORK_MEMORY_ERROR;
        goto exit_level_2;
    }
    /* Call middle-level interface */
    info = LAPACKE_sggsvp3_work(matrix_layout, jobu, jobv, jobq, m, p, n, a, lda, b, ldb, tola, tolb, k, l, u, ldu, v, ldv, q, ldq, iwork,
                                tau, work, lwork);
    /* Release memory and exit */
    LAPACKE_free(work);
exit_level_2:
    LAPACKE_free(tau);
exit_level_1:
    LAPACKE_free(iwork);
exit_level_0:
    if (info == LAPACK_WORK_MEMORY_ERROR) {
        LAPACKE_xerbla("LAPACKE_sggsvp3", info);
    }
    return info;
}
