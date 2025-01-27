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
* Contents: Native high-level C interface to LAPACK function chegv
* Author: Intel Corporation
*****************************************************************************/

#include "lapacke_utils.h"

lapack_int LAPACKE_chegv(int matrix_layout, lapack_int itype, char jobz, char uplo, lapack_int n, lapack_complex_float *a, lapack_int lda,
                         lapack_complex_float *b, lapack_int ldb, float *w) {
    lapack_int info = 0;
    lapack_int lwork = -1;
    float *rwork = NULL;
    lapack_complex_float *work = NULL;
    lapack_complex_float work_query;
    if (matrix_layout != LAPACK_COL_MAJOR && matrix_layout != LAPACK_ROW_MAJOR) {
        LAPACKE_xerbla("LAPACKE_chegv", -1);
        return -1;
    }
#ifndef LAPACK_DISABLE_NAN_CHECK
    if (LAPACKE_get_nancheck()) {
        /* Optionally check input matrices for NaNs */
        if (LAPACKE_che_nancheck(matrix_layout, uplo, n, a, lda)) {
            return -6;
        }
        if (LAPACKE_che_nancheck(matrix_layout, uplo, n, b, ldb)) {
            return -8;
        }
    }
#endif
    /* Allocate memory for working array(s) */
    rwork = (float *)LAPACKE_malloc(sizeof(float) * MAX(1, 3 * n - 2));
    if (rwork == NULL) {
        info = LAPACK_WORK_MEMORY_ERROR;
        goto exit_level_0;
    }
    /* Query optimal working array(s) size */
    info = LAPACKE_chegv_work(matrix_layout, itype, jobz, uplo, n, a, lda, b, ldb, w, &work_query, lwork, rwork);
    if (info != 0) {
        goto exit_level_1;
    }
    lwork = LAPACK_C2INT(work_query);
    /* Allocate memory for work arrays */
    work = (lapack_complex_float *)LAPACKE_malloc(sizeof(lapack_complex_float) * lwork);
    if (work == NULL) {
        info = LAPACK_WORK_MEMORY_ERROR;
        goto exit_level_1;
    }
    /* Call middle-level interface */
    info = LAPACKE_chegv_work(matrix_layout, itype, jobz, uplo, n, a, lda, b, ldb, w, work, lwork, rwork);
    /* Release memory and exit */
    LAPACKE_free(work);
exit_level_1:
    LAPACKE_free(rwork);
exit_level_0:
    if (info == LAPACK_WORK_MEMORY_ERROR) {
        LAPACKE_xerbla("LAPACKE_chegv", info);
    }
    return info;
}
