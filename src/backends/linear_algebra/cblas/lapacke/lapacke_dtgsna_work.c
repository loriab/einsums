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
* Contents: Native middle-level C interface to LAPACK function dtgsna
* Author: Intel Corporation
*****************************************************************************/

#include "lapacke_utils.h"

lapack_int LAPACKE_dtgsna_work(int matrix_layout, char job, char howmny, const lapack_logical *select, lapack_int n, const double *a,
                               lapack_int lda, const double *b, lapack_int ldb, const double *vl, lapack_int ldvl, const double *vr,
                               lapack_int ldvr, double *s, double *dif, lapack_int mm, lapack_int *m, double *work, lapack_int lwork,
                               lapack_int *iwork) {
    lapack_int info = 0;
    if (matrix_layout == LAPACK_COL_MAJOR) {
        /* Call LAPACK function and adjust info */
        LAPACK_dtgsna(&job, &howmny, select, &n, a, &lda, b, &ldb, vl, &ldvl, vr, &ldvr, s, dif, &mm, m, work, &lwork, iwork, &info);
        if (info < 0) {
            info = info - 1;
        }
    } else if (matrix_layout == LAPACK_ROW_MAJOR) {
        lapack_int lda_t = MAX(1, n);
        lapack_int ldb_t = MAX(1, n);
        lapack_int ldvl_t = MAX(1, n);
        lapack_int ldvr_t = MAX(1, n);
        double *a_t = NULL;
        double *b_t = NULL;
        double *vl_t = NULL;
        double *vr_t = NULL;
        /* Check leading dimension(s) */
        if (lda < n) {
            info = -7;
            LAPACKE_xerbla("LAPACKE_dtgsna_work", info);
            return info;
        }
        if (ldb < n) {
            info = -9;
            LAPACKE_xerbla("LAPACKE_dtgsna_work", info);
            return info;
        }
        if (ldvl < mm) {
            info = -11;
            LAPACKE_xerbla("LAPACKE_dtgsna_work", info);
            return info;
        }
        if (ldvr < mm) {
            info = -13;
            LAPACKE_xerbla("LAPACKE_dtgsna_work", info);
            return info;
        }
        /* Query optimal working array(s) size if requested */
        if (lwork == -1) {
            LAPACK_dtgsna(&job, &howmny, select, &n, a, &lda_t, b, &ldb_t, vl, &ldvl_t, vr, &ldvr_t, s, dif, &mm, m, work, &lwork, iwork,
                          &info);
            return (info < 0) ? (info - 1) : info;
        }
        /* Allocate memory for temporary array(s) */
        a_t = (double *)LAPACKE_malloc(sizeof(double) * lda_t * MAX(1, n));
        if (a_t == NULL) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_0;
        }
        b_t = (double *)LAPACKE_malloc(sizeof(double) * ldb_t * MAX(1, n));
        if (b_t == NULL) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_1;
        }
        if (LAPACKE_lsame(job, 'b') || LAPACKE_lsame(job, 'e')) {
            vl_t = (double *)LAPACKE_malloc(sizeof(double) * ldvl_t * MAX(1, mm));
            if (vl_t == NULL) {
                info = LAPACK_TRANSPOSE_MEMORY_ERROR;
                goto exit_level_2;
            }
        }
        if (LAPACKE_lsame(job, 'b') || LAPACKE_lsame(job, 'e')) {
            vr_t = (double *)LAPACKE_malloc(sizeof(double) * ldvr_t * MAX(1, mm));
            if (vr_t == NULL) {
                info = LAPACK_TRANSPOSE_MEMORY_ERROR;
                goto exit_level_3;
            }
        }
        /* Transpose input matrices */
        LAPACKE_dge_trans(matrix_layout, n, n, a, lda, a_t, lda_t);
        LAPACKE_dge_trans(matrix_layout, n, n, b, ldb, b_t, ldb_t);
        if (LAPACKE_lsame(job, 'b') || LAPACKE_lsame(job, 'e')) {
            LAPACKE_dge_trans(matrix_layout, n, mm, vl, ldvl, vl_t, ldvl_t);
        }
        if (LAPACKE_lsame(job, 'b') || LAPACKE_lsame(job, 'e')) {
            LAPACKE_dge_trans(matrix_layout, n, mm, vr, ldvr, vr_t, ldvr_t);
        }
        /* Call LAPACK function and adjust info */
        LAPACK_dtgsna(&job, &howmny, select, &n, a_t, &lda_t, b_t, &ldb_t, vl_t, &ldvl_t, vr_t, &ldvr_t, s, dif, &mm, m, work, &lwork,
                      iwork, &info);
        if (info < 0) {
            info = info - 1;
        }
        /* Release memory and exit */
        if (LAPACKE_lsame(job, 'b') || LAPACKE_lsame(job, 'e')) {
            LAPACKE_free(vr_t);
        }
    exit_level_3:
        if (LAPACKE_lsame(job, 'b') || LAPACKE_lsame(job, 'e')) {
            LAPACKE_free(vl_t);
        }
    exit_level_2:
        LAPACKE_free(b_t);
    exit_level_1:
        LAPACKE_free(a_t);
    exit_level_0:
        if (info == LAPACK_TRANSPOSE_MEMORY_ERROR) {
            LAPACKE_xerbla("LAPACKE_dtgsna_work", info);
        }
    } else {
        info = -1;
        LAPACKE_xerbla("LAPACKE_dtgsna_work", info);
    }
    return info;
}
