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
* Contents: Native middle-level C interface to LAPACK function clag2z
* Author: Intel Corporation
*****************************************************************************/

#include "lapacke_utils.h"

lapack_int LAPACKE_clag2z_work(int matrix_layout, lapack_int m, lapack_int n, const lapack_complex_float *sa, lapack_int ldsa,
                               lapack_complex_double *a, lapack_int lda) {
    lapack_int info = 0;
    if (matrix_layout == LAPACK_COL_MAJOR) {
        /* Call LAPACK function and adjust info */
        LAPACK_clag2z(&m, &n, sa, &ldsa, a, &lda, &info);
        if (info < 0) {
            info = info - 1;
        }
    } else if (matrix_layout == LAPACK_ROW_MAJOR) {
        lapack_int lda_t = MAX(1, m);
        lapack_int ldsa_t = MAX(1, m);
        lapack_complex_float *sa_t = NULL;
        lapack_complex_double *a_t = NULL;
        /* Check leading dimension(s) */
        if (lda < n) {
            info = -7;
            LAPACKE_xerbla("LAPACKE_clag2z_work", info);
            return info;
        }
        if (ldsa < n) {
            info = -5;
            LAPACKE_xerbla("LAPACKE_clag2z_work", info);
            return info;
        }
        /* Allocate memory for temporary array(s) */
        sa_t = (lapack_complex_float *)LAPACKE_malloc(sizeof(lapack_complex_float) * ldsa_t * MAX(1, n));
        if (sa_t == NULL) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_0;
        }
        a_t = (lapack_complex_double *)LAPACKE_malloc(sizeof(lapack_complex_double) * lda_t * MAX(1, n));
        if (a_t == NULL) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_1;
        }
        /* Transpose input matrices */
        LAPACKE_cge_trans(matrix_layout, m, n, sa, ldsa, sa_t, ldsa_t);
        /* Call LAPACK function and adjust info */
        LAPACK_clag2z(&m, &n, sa_t, &ldsa_t, a_t, &lda_t, &info);
        if (info < 0) {
            info = info - 1;
        }
        /* Transpose output matrices */
        LAPACKE_zge_trans(LAPACK_COL_MAJOR, m, n, a_t, lda_t, a, lda);
        /* Release memory and exit */
        LAPACKE_free(a_t);
    exit_level_1:
        LAPACKE_free(sa_t);
    exit_level_0:
        if (info == LAPACK_TRANSPOSE_MEMORY_ERROR) {
            LAPACKE_xerbla("LAPACKE_clag2z_work", info);
        }
    } else {
        info = -1;
        LAPACKE_xerbla("LAPACKE_clag2z_work", info);
    }
    return info;
}
