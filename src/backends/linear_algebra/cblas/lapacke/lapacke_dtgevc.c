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
* Contents: Native high-level C interface to LAPACK function dtgevc
* Author: Intel Corporation
*****************************************************************************/

#include "lapacke_utils.h"

lapack_int LAPACKE_dtgevc(int matrix_layout, char side, char howmny, const lapack_logical *select, lapack_int n, const double *s,
                          lapack_int lds, const double *p, lapack_int ldp, double *vl, lapack_int ldvl, double *vr, lapack_int ldvr,
                          lapack_int mm, lapack_int *m) {
    lapack_int info = 0;
    double *work = NULL;
    if (matrix_layout != LAPACK_COL_MAJOR && matrix_layout != LAPACK_ROW_MAJOR) {
        LAPACKE_xerbla("LAPACKE_dtgevc", -1);
        return -1;
    }
#ifndef LAPACK_DISABLE_NAN_CHECK
    if (LAPACKE_get_nancheck()) {
        /* Optionally check input matrices for NaNs */
        if (LAPACKE_dge_nancheck(matrix_layout, n, n, p, ldp)) {
            return -8;
        }
        if (LAPACKE_dge_nancheck(matrix_layout, n, n, s, lds)) {
            return -6;
        }
        if (LAPACKE_lsame(side, 'b') || LAPACKE_lsame(side, 'l')) {
            if (LAPACKE_dge_nancheck(matrix_layout, n, mm, vl, ldvl)) {
                return -10;
            }
        }
        if (LAPACKE_lsame(side, 'b') || LAPACKE_lsame(side, 'r')) {
            if (LAPACKE_dge_nancheck(matrix_layout, n, mm, vr, ldvr)) {
                return -12;
            }
        }
    }
#endif
    /* Allocate memory for working array(s) */
    work = (double *)LAPACKE_malloc(sizeof(double) * MAX(1, 6 * n));
    if (work == NULL) {
        info = LAPACK_WORK_MEMORY_ERROR;
        goto exit_level_0;
    }
    /* Call middle-level interface */
    info = LAPACKE_dtgevc_work(matrix_layout, side, howmny, select, n, s, lds, p, ldp, vl, ldvl, vr, ldvr, mm, m, work);
    /* Release memory and exit */
    LAPACKE_free(work);
exit_level_0:
    if (info == LAPACK_WORK_MEMORY_ERROR) {
        LAPACKE_xerbla("LAPACKE_dtgevc", info);
    }
    return info;
}
