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
* Contents: Native high-level C interface to LAPACK function cstein
* Author: Intel Corporation
*****************************************************************************/

#include "lapacke_utils.h"

lapack_int LAPACKE_cstein(int matrix_layout, lapack_int n, const float *d, const float *e, lapack_int m, const float *w,
                          const lapack_int *iblock, const lapack_int *isplit, lapack_complex_float *z, lapack_int ldz, lapack_int *ifailv) {
    lapack_int info = 0;
    lapack_int *iwork = NULL;
    float *work = NULL;
    if (matrix_layout != LAPACK_COL_MAJOR && matrix_layout != LAPACK_ROW_MAJOR) {
        LAPACKE_xerbla("LAPACKE_cstein", -1);
        return -1;
    }
#ifndef LAPACK_DISABLE_NAN_CHECK
    if (LAPACKE_get_nancheck()) {
        /* Optionally check input matrices for NaNs */
        if (LAPACKE_s_nancheck(n, d, 1)) {
            return -3;
        }
        if (LAPACKE_s_nancheck(n - 1, e, 1)) {
            return -4;
        }
        if (LAPACKE_s_nancheck(n, w, 1)) {
            return -6;
        }
    }
#endif
    /* Allocate memory for working array(s) */
    iwork = (lapack_int *)LAPACKE_malloc(sizeof(lapack_int) * MAX(1, n));
    if (iwork == NULL) {
        info = LAPACK_WORK_MEMORY_ERROR;
        goto exit_level_0;
    }
    work = (float *)LAPACKE_malloc(sizeof(float) * MAX(1, 5 * n));
    if (work == NULL) {
        info = LAPACK_WORK_MEMORY_ERROR;
        goto exit_level_1;
    }
    /* Call middle-level interface */
    info = LAPACKE_cstein_work(matrix_layout, n, d, e, m, w, iblock, isplit, z, ldz, work, iwork, ifailv);
    /* Release memory and exit */
    LAPACKE_free(work);
exit_level_1:
    LAPACKE_free(iwork);
exit_level_0:
    if (info == LAPACK_WORK_MEMORY_ERROR) {
        LAPACKE_xerbla("LAPACKE_cstein", info);
    }
    return info;
}
