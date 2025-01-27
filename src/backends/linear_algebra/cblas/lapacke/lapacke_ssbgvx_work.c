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
* Contents: Native middle-level C interface to LAPACK function ssbgvx
* Author: Intel Corporation
*****************************************************************************/

#include "lapacke_utils.h"

lapack_int LAPACKE_ssbgvx_work(int matrix_layout, char jobz, char range, char uplo, lapack_int n, lapack_int ka, lapack_int kb, float *ab,
                               lapack_int ldab, float *bb, lapack_int ldbb, float *q, lapack_int ldq, float vl, float vu, lapack_int il,
                               lapack_int iu, float abstol, lapack_int *m, float *w, float *z, lapack_int ldz, float *work,
                               lapack_int *iwork, lapack_int *ifail) {
    lapack_int info = 0;
    if (matrix_layout == LAPACK_COL_MAJOR) {
        /* Call LAPACK function and adjust info */
        LAPACK_ssbgvx(&jobz, &range, &uplo, &n, &ka, &kb, ab, &ldab, bb, &ldbb, q, &ldq, &vl, &vu, &il, &iu, &abstol, m, w, z, &ldz, work,
                      iwork, ifail, &info);
        if (info < 0) {
            info = info - 1;
        }
    } else if (matrix_layout == LAPACK_ROW_MAJOR) {
        lapack_int ldab_t = MAX(1, ka + 1);
        lapack_int ldbb_t = MAX(1, kb + 1);
        lapack_int ldq_t = MAX(1, n);
        lapack_int ldz_t = MAX(1, n);
        float *ab_t = NULL;
        float *bb_t = NULL;
        float *q_t = NULL;
        float *z_t = NULL;
        /* Check leading dimension(s) */
        if (ldab < n) {
            info = -9;
            LAPACKE_xerbla("LAPACKE_ssbgvx_work", info);
            return info;
        }
        if (ldbb < n) {
            info = -11;
            LAPACKE_xerbla("LAPACKE_ssbgvx_work", info);
            return info;
        }
        if (ldq < n) {
            info = -13;
            LAPACKE_xerbla("LAPACKE_ssbgvx_work", info);
            return info;
        }
        if (ldz < n) {
            info = -22;
            LAPACKE_xerbla("LAPACKE_ssbgvx_work", info);
            return info;
        }
        /* Allocate memory for temporary array(s) */
        ab_t = (float *)LAPACKE_malloc(sizeof(float) * ldab_t * MAX(1, n));
        if (ab_t == NULL) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_0;
        }
        bb_t = (float *)LAPACKE_malloc(sizeof(float) * ldbb_t * MAX(1, n));
        if (bb_t == NULL) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_1;
        }
        if (LAPACKE_lsame(jobz, 'v')) {
            q_t = (float *)LAPACKE_malloc(sizeof(float) * ldq_t * MAX(1, n));
            if (q_t == NULL) {
                info = LAPACK_TRANSPOSE_MEMORY_ERROR;
                goto exit_level_2;
            }
        }
        if (LAPACKE_lsame(jobz, 'v')) {
            z_t = (float *)LAPACKE_malloc(sizeof(float) * ldz_t * MAX(1, n));
            if (z_t == NULL) {
                info = LAPACK_TRANSPOSE_MEMORY_ERROR;
                goto exit_level_3;
            }
        }
        /* Transpose input matrices */
        LAPACKE_ssb_trans(matrix_layout, uplo, n, ka, ab, ldab, ab_t, ldab_t);
        LAPACKE_ssb_trans(matrix_layout, uplo, n, kb, bb, ldbb, bb_t, ldbb_t);
        /* Call LAPACK function and adjust info */
        LAPACK_ssbgvx(&jobz, &range, &uplo, &n, &ka, &kb, ab_t, &ldab_t, bb_t, &ldbb_t, q_t, &ldq_t, &vl, &vu, &il, &iu, &abstol, m, w, z_t,
                      &ldz_t, work, iwork, ifail, &info);
        if (info < 0) {
            info = info - 1;
        }
        /* Transpose output matrices */
        LAPACKE_ssb_trans(LAPACK_COL_MAJOR, uplo, n, ka, ab_t, ldab_t, ab, ldab);
        LAPACKE_ssb_trans(LAPACK_COL_MAJOR, uplo, n, kb, bb_t, ldbb_t, bb, ldbb);
        if (LAPACKE_lsame(jobz, 'v')) {
            LAPACKE_sge_trans(LAPACK_COL_MAJOR, n, n, q_t, ldq_t, q, ldq);
        }
        if (LAPACKE_lsame(jobz, 'v')) {
            LAPACKE_sge_trans(LAPACK_COL_MAJOR, n, n, z_t, ldz_t, z, ldz);
        }
        /* Release memory and exit */
        if (LAPACKE_lsame(jobz, 'v')) {
            LAPACKE_free(z_t);
        }
    exit_level_3:
        if (LAPACKE_lsame(jobz, 'v')) {
            LAPACKE_free(q_t);
        }
    exit_level_2:
        LAPACKE_free(bb_t);
    exit_level_1:
        LAPACKE_free(ab_t);
    exit_level_0:
        if (info == LAPACK_TRANSPOSE_MEMORY_ERROR) {
            LAPACKE_xerbla("LAPACKE_ssbgvx_work", info);
        }
    } else {
        info = -1;
        LAPACKE_xerbla("LAPACKE_ssbgvx_work", info);
    }
    return info;
}
