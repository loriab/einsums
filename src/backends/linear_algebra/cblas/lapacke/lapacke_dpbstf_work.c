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
* Contents: Native middle-level C interface to LAPACK function dpbstf
* Author: Intel Corporation
*****************************************************************************/

#include "lapacke_utils.h"

lapack_int LAPACKE_dpbstf_work(int matrix_layout, char uplo, lapack_int n, lapack_int kb, double *bb, lapack_int ldbb) {
    lapack_int info = 0;
    if (matrix_layout == LAPACK_COL_MAJOR) {
        /* Call LAPACK function and adjust info */
        LAPACK_dpbstf(&uplo, &n, &kb, bb, &ldbb, &info);
        if (info < 0) {
            info = info - 1;
        }
    } else if (matrix_layout == LAPACK_ROW_MAJOR) {
        lapack_int ldbb_t = MAX(1, kb + 1);
        double *bb_t = NULL;
        /* Check leading dimension(s) */
        if (ldbb < n) {
            info = -6;
            LAPACKE_xerbla("LAPACKE_dpbstf_work", info);
            return info;
        }
        /* Allocate memory for temporary array(s) */
        bb_t = (double *)LAPACKE_malloc(sizeof(double) * ldbb_t * MAX(1, n));
        if (bb_t == NULL) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_0;
        }
        /* Transpose input matrices */
        LAPACKE_dpb_trans(matrix_layout, uplo, n, kb, bb, ldbb, bb_t, ldbb_t);
        /* Call LAPACK function and adjust info */
        LAPACK_dpbstf(&uplo, &n, &kb, bb_t, &ldbb_t, &info);
        if (info < 0) {
            info = info - 1;
        }
        /* Transpose output matrices */
        LAPACKE_dpb_trans(LAPACK_COL_MAJOR, uplo, n, kb, bb_t, ldbb_t, bb, ldbb);
        /* Release memory and exit */
        LAPACKE_free(bb_t);
    exit_level_0:
        if (info == LAPACK_TRANSPOSE_MEMORY_ERROR) {
            LAPACKE_xerbla("LAPACKE_dpbstf_work", info);
        }
    } else {
        info = -1;
        LAPACKE_xerbla("LAPACKE_dpbstf_work", info);
    }
    return info;
}
