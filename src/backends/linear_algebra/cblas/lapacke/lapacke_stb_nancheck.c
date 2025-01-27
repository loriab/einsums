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
******************************************************************************
* Contents: Native C interface to LAPACK utility function
* Author: Intel Corporation
*****************************************************************************/
#include "lapacke_utils.h"

/* Check a matrix for NaN entries. */

lapack_logical LAPACKE_stb_nancheck(int matrix_layout, char uplo, char diag, lapack_int n, lapack_int kd, const float *ab,
                                    lapack_int ldab) {
    lapack_logical colmaj, upper, unit;

    if (ab == NULL)
        return (lapack_logical)0;

    colmaj = (matrix_layout == LAPACK_COL_MAJOR);
    upper = LAPACKE_lsame(uplo, 'u');
    unit = LAPACKE_lsame(diag, 'u');

    if ((!colmaj && (matrix_layout != LAPACK_ROW_MAJOR)) || (!upper && !LAPACKE_lsame(uplo, 'l')) || (!unit && !LAPACKE_lsame(diag, 'n'))) {
        /* Just exit if any of input parameters are wrong */
        return (lapack_logical)0;
    }

    if (unit) {
        /* Unit case, diagonal should be excluded from the check for NaN. */
        if (colmaj) {
            if (upper) {
                return LAPACKE_sgb_nancheck(matrix_layout, n - 1, n - 1, 0, kd - 1, &ab[ldab], ldab);
            } else {
                return LAPACKE_sgb_nancheck(matrix_layout, n - 1, n - 1, kd - 1, 0, &ab[1], ldab);
            }
        } else {
            if (upper) {
                return LAPACKE_sgb_nancheck(matrix_layout, n - 1, n - 1, 0, kd - 1, &ab[1], ldab);
            } else {
                return LAPACKE_sgb_nancheck(matrix_layout, n - 1, n - 1, kd - 1, 0, &ab[ldab], ldab);
            }
        }
    } else {
        /* Non-unit case */
        if (upper) {
            return LAPACKE_sgb_nancheck(matrix_layout, n, n, 0, kd, ab, ldab);
        } else {
            return LAPACKE_sgb_nancheck(matrix_layout, n, n, kd, 0, ab, ldab);
        }
    }
}
