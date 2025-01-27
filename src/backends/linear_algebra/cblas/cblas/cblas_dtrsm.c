/*
 *
 * cblas_dtrsm.c
 * This program is a C interface to dtrsm.
 * Written by Keita Teranishi
 * 4/6/1998
 *
 */

#include "cblas.h"
#include "cblas_f77.h"
void cblas_dtrsm(const CBLAS_LAYOUT layout, const CBLAS_SIDE Side, const CBLAS_UPLO Uplo, const CBLAS_TRANSPOSE TransA,
                 const CBLAS_DIAG Diag, const CBLAS_INT M, const CBLAS_INT N, const double alpha, const double *A, const CBLAS_INT lda,
                 double *B, const CBLAS_INT ldb)

{
    char UL, TA, SD, DI;
#ifdef F77_CHAR
    F77_CHAR F77_TA, F77_UL, F77_SD, F77_DI;
#else
#define F77_TA &TA
#define F77_UL &UL
#define F77_SD &SD
#define F77_DI &DI
#endif

#ifdef F77_INT
    F77_INT F77_M = M, F77_N = N, F77_lda = lda, F77_ldb = ldb;
#else
#define F77_M M
#define F77_N N
#define F77_lda lda
#define F77_ldb ldb
#endif

    extern int CBLAS_CallFromC;
    extern int RowMajorStrg;
    RowMajorStrg = 0;
    CBLAS_CallFromC = 1;

    if (layout == CblasColMajor) {
        if (Side == CblasRight)
            SD = 'R';
        else if (Side == CblasLeft)
            SD = 'L';
        else {
            cblas_xerbla(2, "cblas_dtrsm", "Illegal Side setting, %d\n", Side);
            CBLAS_CallFromC = 0;
            RowMajorStrg = 0;
            return;
        }
        if (Uplo == CblasUpper)
            UL = 'U';
        else if (Uplo == CblasLower)
            UL = 'L';
        else {
            cblas_xerbla(3, "cblas_dtrsm", "Illegal Uplo setting, %d\n", Uplo);
            CBLAS_CallFromC = 0;
            RowMajorStrg = 0;
            return;
        }

        if (TransA == CblasTrans)
            TA = 'T';
        else if (TransA == CblasConjTrans)
            TA = 'C';
        else if (TransA == CblasNoTrans)
            TA = 'N';
        else {
            cblas_xerbla(4, "cblas_dtrsm", "Illegal Trans setting, %d\n", TransA);
            CBLAS_CallFromC = 0;
            RowMajorStrg = 0;
            return;
        }

        if (Diag == CblasUnit)
            DI = 'U';
        else if (Diag == CblasNonUnit)
            DI = 'N';
        else {
            cblas_xerbla(5, "cblas_dtrsm", "Illegal Diag setting, %d\n", Diag);
            CBLAS_CallFromC = 0;
            RowMajorStrg = 0;
            return;
        }

#ifdef F77_CHAR
        F77_UL = C2F_CHAR(&UL);
        F77_TA = C2F_CHAR(&TA);
        F77_SD = C2F_CHAR(&SD);
        F77_DI = C2F_CHAR(&DI);
#endif

        F77_dtrsm(F77_SD, F77_UL, F77_TA, F77_DI, &F77_M, &F77_N, &alpha, A, &F77_lda, B, &F77_ldb);
    } else if (layout == CblasRowMajor) {
        RowMajorStrg = 1;
        if (Side == CblasRight)
            SD = 'L';
        else if (Side == CblasLeft)
            SD = 'R';
        else {
            cblas_xerbla(2, "cblas_dtrsm", "Illegal Side setting, %d\n", Side);
            CBLAS_CallFromC = 0;
            RowMajorStrg = 0;
            return;
        }

        if (Uplo == CblasUpper)
            UL = 'L';
        else if (Uplo == CblasLower)
            UL = 'U';
        else {
            cblas_xerbla(3, "cblas_dtrsm", "Illegal Uplo setting, %d\n", Uplo);
            CBLAS_CallFromC = 0;
            RowMajorStrg = 0;
            return;
        }

        if (TransA == CblasTrans)
            TA = 'T';
        else if (TransA == CblasConjTrans)
            TA = 'C';
        else if (TransA == CblasNoTrans)
            TA = 'N';
        else {
            cblas_xerbla(4, "cblas_dtrsm", "Illegal Trans setting, %d\n", TransA);
            CBLAS_CallFromC = 0;
            RowMajorStrg = 0;
            return;
        }

        if (Diag == CblasUnit)
            DI = 'U';
        else if (Diag == CblasNonUnit)
            DI = 'N';
        else {
            cblas_xerbla(5, "cblas_dtrsm", "Illegal Diag setting, %d\n", Diag);
            CBLAS_CallFromC = 0;
            RowMajorStrg = 0;
            return;
        }

#ifdef F77_CHAR
        F77_UL = C2F_CHAR(&UL);
        F77_TA = C2F_CHAR(&TA);
        F77_SD = C2F_CHAR(&SD);
        F77_DI = C2F_CHAR(&DI);
#endif

        F77_dtrsm(F77_SD, F77_UL, F77_TA, F77_DI, &F77_N, &F77_M, &alpha, A, &F77_lda, B, &F77_ldb);
    } else
        cblas_xerbla(1, "cblas_dtrsm", "Illegal layout setting, %d\n", layout);
    CBLAS_CallFromC = 0;
    RowMajorStrg = 0;
    return;
}
