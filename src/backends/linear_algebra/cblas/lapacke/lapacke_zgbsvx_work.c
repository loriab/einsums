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
* Contents: Native middle-level C interface to LAPACK function zgbsvx
* Author: Intel Corporation
*****************************************************************************/

#include "lapacke_utils.h"

lapack_int LAPACKE_zgbsvx_work(int matrix_layout, char fact, char trans, lapack_int n, lapack_int kl, lapack_int ku, lapack_int nrhs,
                               lapack_complex_double *ab, lapack_int ldab, lapack_complex_double *afb, lapack_int ldafb, lapack_int *ipiv,
                               char *equed, double *r, double *c, lapack_complex_double *b, lapack_int ldb, lapack_complex_double *x,
                               lapack_int ldx, double *rcond, double *ferr, double *berr, lapack_complex_double *work, double *rwork) {
    lapack_int info = 0;
    if (matrix_layout == LAPACK_COL_MAJOR) {
        /* Call LAPACK function and adjust info */
        LAPACK_zgbsvx(&fact, &trans, &n, &kl, &ku, &nrhs, ab, &ldab, afb, &ldafb, ipiv, equed, r, c, b, &ldb, x, &ldx, rcond, ferr, berr,
                      work, rwork, &info);
        if (info < 0) {
            info = info - 1;
        }
    } else if (matrix_layout == LAPACK_ROW_MAJOR) {
        lapack_int ldab_t = MAX(1, kl + ku + 1);
        lapack_int ldafb_t = MAX(1, 2 * kl + ku + 1);
        lapack_int ldb_t = MAX(1, n);
        lapack_int ldx_t = MAX(1, n);
        lapack_complex_double *ab_t = NULL;
        lapack_complex_double *afb_t = NULL;
        lapack_complex_double *b_t = NULL;
        lapack_complex_double *x_t = NULL;
        /* Check leading dimension(s) */
        if (ldab < n) {
            info = -9;
            LAPACKE_xerbla("LAPACKE_zgbsvx_work", info);
            return info;
        }
        if (ldafb < n) {
            info = -11;
            LAPACKE_xerbla("LAPACKE_zgbsvx_work", info);
            return info;
        }
        if (ldb < nrhs) {
            info = -17;
            LAPACKE_xerbla("LAPACKE_zgbsvx_work", info);
            return info;
        }
        if (ldx < nrhs) {
            info = -19;
            LAPACKE_xerbla("LAPACKE_zgbsvx_work", info);
            return info;
        }
        /* Allocate memory for temporary array(s) */
        ab_t = (lapack_complex_double *)LAPACKE_malloc(sizeof(lapack_complex_double) * ldab_t * MAX(1, n));
        if (ab_t == NULL) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_0;
        }
        afb_t = (lapack_complex_double *)LAPACKE_malloc(sizeof(lapack_complex_double) * ldafb_t * MAX(1, n));
        if (afb_t == NULL) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_1;
        }
        b_t = (lapack_complex_double *)LAPACKE_malloc(sizeof(lapack_complex_double) * ldb_t * MAX(1, nrhs));
        if (b_t == NULL) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_2;
        }
        x_t = (lapack_complex_double *)LAPACKE_malloc(sizeof(lapack_complex_double) * ldx_t * MAX(1, nrhs));
        if (x_t == NULL) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_3;
        }
        /* Transpose input matrices */
        LAPACKE_zgb_trans(matrix_layout, n, n, kl, ku, ab, ldab, ab_t, ldab_t);
        if (LAPACKE_lsame(fact, 'f')) {
            LAPACKE_zgb_trans(matrix_layout, n, n, kl, kl + ku, afb, ldafb, afb_t, ldafb_t);
        }
        LAPACKE_zge_trans(matrix_layout, n, nrhs, b, ldb, b_t, ldb_t);
        /* Call LAPACK function and adjust info */
        LAPACK_zgbsvx(&fact, &trans, &n, &kl, &ku, &nrhs, ab_t, &ldab_t, afb_t, &ldafb_t, ipiv, equed, r, c, b_t, &ldb_t, x_t, &ldx_t,
                      rcond, ferr, berr, work, rwork, &info);
        if (info < 0) {
            info = info - 1;
        }
        /* Transpose output matrices */
        if (LAPACKE_lsame(fact, 'e') && (LAPACKE_lsame(*equed, 'b') || LAPACKE_lsame(*equed, 'c') || LAPACKE_lsame(*equed, 'r'))) {
            LAPACKE_zgb_trans(LAPACK_COL_MAJOR, n, n, kl, ku, ab_t, ldab_t, ab, ldab);
        }
        if (LAPACKE_lsame(fact, 'e') || LAPACKE_lsame(fact, 'n')) {
            LAPACKE_zgb_trans(LAPACK_COL_MAJOR, n, n, kl, kl + ku, afb_t, ldafb_t, afb, ldafb);
        }
        if (LAPACKE_lsame(fact, 'f') && (LAPACKE_lsame(*equed, 'b') || LAPACKE_lsame(*equed, 'c') || LAPACKE_lsame(*equed, 'r'))) {
            LAPACKE_zge_trans(LAPACK_COL_MAJOR, n, nrhs, b_t, ldb_t, b, ldb);
        }
        LAPACKE_zge_trans(LAPACK_COL_MAJOR, n, nrhs, x_t, ldx_t, x, ldx);
        /* Release memory and exit */
        LAPACKE_free(x_t);
    exit_level_3:
        LAPACKE_free(b_t);
    exit_level_2:
        LAPACKE_free(afb_t);
    exit_level_1:
        LAPACKE_free(ab_t);
    exit_level_0:
        if (info == LAPACK_TRANSPOSE_MEMORY_ERROR) {
            LAPACKE_xerbla("LAPACKE_zgbsvx_work", info);
        }
    } else {
        info = -1;
        LAPACKE_xerbla("LAPACKE_zgbsvx_work", info);
    }
    return info;
}
