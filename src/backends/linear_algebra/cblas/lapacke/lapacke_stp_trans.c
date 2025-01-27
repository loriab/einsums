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

/* Converts input triangular packed matrix from row-major(C) to
 * column-major(Fortran) layout or vice versa.
 */

void LAPACKE_stp_trans(int matrix_layout, char uplo, char diag, lapack_int n, const float *in, float *out) {
    lapack_int i, j, st;
    lapack_logical colmaj, upper, unit;

    if (in == NULL || out == NULL)
        return;

    colmaj = (matrix_layout == LAPACK_COL_MAJOR);
    upper = LAPACKE_lsame(uplo, 'u');
    unit = LAPACKE_lsame(diag, 'u');

    if ((!colmaj && (matrix_layout != LAPACK_ROW_MAJOR)) || (!upper && !LAPACKE_lsame(uplo, 'l')) || (!unit && !LAPACKE_lsame(diag, 'n'))) {
        /* Just exit if any of input parameters are wrong */
        return;
    }
    if (unit) {
        /* If unit, then don't touch diagonal, start from 1st column or row */
        st = 1;
    } else {
        /* If non-unit, then check diagonal also, starting from [0,0] */
        st = 0;
    }

    /* Perform conversion:
     * Since col_major upper and row_major lower are equal,
     * and col_major lower and row_major upper are equals too -
     * using one code for equal cases. XOR( colmaj, upper )
     */
    if (!(colmaj || upper) || (colmaj && upper)) {
        for (j = st; j < n; j++) {
            for (i = 0; i < j + 1 - st; i++) {
                out[j - i + (i * (2 * n - i + 1)) / 2] = in[((j + 1) * j) / 2 + i];
            }
        }
    } else {
        for (j = 0; j < n - st; j++) {
            for (i = j + st; i < n; i++) {
                out[j + ((i + 1) * i) / 2] = in[(j * (2 * n - j + 1)) / 2 + i - j];
            }
        }
    }
}
