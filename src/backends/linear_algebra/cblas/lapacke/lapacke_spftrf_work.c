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
* Contents: Native middle-level C interface to LAPACK function spftrf
* Author: Intel Corporation
*****************************************************************************/

#include "lapacke_utils.h"

lapack_int LAPACKE_spftrf_work(int matrix_layout, char transr, char uplo, lapack_int n, float *a) {
    lapack_int info = 0;
    if (matrix_layout == LAPACK_COL_MAJOR) {
        /* Call LAPACK function and adjust info */
        LAPACK_spftrf(&transr, &uplo, &n, a, &info);
        if (info < 0) {
            info = info - 1;
        }
    } else if (matrix_layout == LAPACK_ROW_MAJOR) {
        float *a_t = NULL;
        /* Allocate memory for temporary array(s) */
        a_t = (float *)LAPACKE_malloc(sizeof(float) * (MAX(1, n) * MAX(2, n + 1)) / 2);
        if (a_t == NULL) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_0;
        }
        /* Transpose input matrices */
        LAPACKE_spf_trans(matrix_layout, transr, uplo, n, a, a_t);
        /* Call LAPACK function and adjust info */
        LAPACK_spftrf(&transr, &uplo, &n, a_t, &info);
        if (info < 0) {
            info = info - 1;
        }
        /* Transpose output matrices */
        LAPACKE_spf_trans(LAPACK_COL_MAJOR, transr, uplo, n, a_t, a);
        /* Release memory and exit */
        LAPACKE_free(a_t);
    exit_level_0:
        if (info == LAPACK_TRANSPOSE_MEMORY_ERROR) {
            LAPACKE_xerbla("LAPACKE_spftrf_work", info);
        }
    } else {
        info = -1;
        LAPACKE_xerbla("LAPACKE_spftrf_work", info);
    }
    return info;
}
