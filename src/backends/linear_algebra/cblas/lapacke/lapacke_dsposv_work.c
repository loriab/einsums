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
* Contents: Native middle-level C interface to LAPACK function dsposv
* Author: Intel Corporation
*****************************************************************************/

#include "lapacke_utils.h"

lapack_int LAPACKE_dsposv_work(int matrix_layout, char uplo, lapack_int n, lapack_int nrhs, double *a, lapack_int lda, double *b,
                               lapack_int ldb, double *x, lapack_int ldx, double *work, float *swork, lapack_int *iter) {
    lapack_int info = 0;
    if (matrix_layout == LAPACK_COL_MAJOR) {
        /* Call LAPACK function and adjust info */
        LAPACK_dsposv(&uplo, &n, &nrhs, a, &lda, b, &ldb, x, &ldx, work, swork, iter, &info);
        if (info < 0) {
            info = info - 1;
        }
    } else if (matrix_layout == LAPACK_ROW_MAJOR) {
        lapack_int lda_t = MAX(1, n);
        lapack_int ldb_t = MAX(1, n);
        lapack_int ldx_t = MAX(1, n);
        double *a_t = NULL;
        double *b_t = NULL;
        double *x_t = NULL;
        /* Check leading dimension(s) */
        if (lda < n) {
            info = -6;
            LAPACKE_xerbla("LAPACKE_dsposv_work", info);
            return info;
        }
        if (ldb < nrhs) {
            info = -8;
            LAPACKE_xerbla("LAPACKE_dsposv_work", info);
            return info;
        }
        if (ldx < nrhs) {
            info = -10;
            LAPACKE_xerbla("LAPACKE_dsposv_work", info);
            return info;
        }
        /* Allocate memory for temporary array(s) */
        a_t = (double *)LAPACKE_malloc(sizeof(double) * lda_t * MAX(1, n));
        if (a_t == NULL) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_0;
        }
        b_t = (double *)LAPACKE_malloc(sizeof(double) * ldb_t * MAX(1, nrhs));
        if (b_t == NULL) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_1;
        }
        x_t = (double *)LAPACKE_malloc(sizeof(double) * ldx_t * MAX(1, nrhs));
        if (x_t == NULL) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_2;
        }
        /* Transpose input matrices */
        LAPACKE_dpo_trans(matrix_layout, uplo, n, a, lda, a_t, lda_t);
        LAPACKE_dge_trans(matrix_layout, n, nrhs, b, ldb, b_t, ldb_t);
        /* Call LAPACK function and adjust info */
        LAPACK_dsposv(&uplo, &n, &nrhs, a_t, &lda_t, b_t, &ldb_t, x_t, &ldx_t, work, swork, iter, &info);
        if (info < 0) {
            info = info - 1;
        }
        /* Transpose output matrices */
        LAPACKE_dpo_trans(LAPACK_COL_MAJOR, uplo, n, a_t, lda_t, a, lda);
        LAPACKE_dge_trans(LAPACK_COL_MAJOR, n, nrhs, b_t, ldb_t, b, ldb);
        LAPACKE_dge_trans(LAPACK_COL_MAJOR, n, nrhs, x_t, ldx_t, x, ldx);
        /* Release memory and exit */
        LAPACKE_free(x_t);
    exit_level_2:
        LAPACKE_free(b_t);
    exit_level_1:
        LAPACKE_free(a_t);
    exit_level_0:
        if (info == LAPACK_TRANSPOSE_MEMORY_ERROR) {
            LAPACKE_xerbla("LAPACKE_dsposv_work", info);
        }
    } else {
        info = -1;
        LAPACKE_xerbla("LAPACKE_dsposv_work", info);
    }
    return info;
}
