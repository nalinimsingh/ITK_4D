/* lapack/complex16/zgecon.f -- translated by f2c (version 20090411).
   You must link the resulting object file with libf2c:
        on Microsoft Windows system, link with libf2c.lib;
        on Linux or Unix systems, link with .../path/to/libf2c.a -lm
        or, if you install libf2c.a in a standard place, with -lf2c -lm
        -- in that order, at the end of the command line, as in
                cc *.o -lf2c -lm
        Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

                http://www.netlib.org/f2c/libf2c.zip
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "v3p_netlib.h"

/* Table of constant values */

static integer c__1 = 1;

/*<    >*/
/* Subroutine */ int zgecon_(char *norm, integer *n, doublecomplex *a,
        integer *lda, doublereal *anorm, doublereal *rcond, doublecomplex *
        work, doublereal *rwork, integer *info, ftnlen norm_len)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1;
    doublereal d__1, d__2;

    /* Builtin functions */
    double d_imag(doublecomplex *);

    /* Local variables */
    doublereal sl;
    integer ix;
    doublereal su;
    integer kase, kase1;
    doublereal scale;
    extern logical lsame_(const char *, const char *, ftnlen, ftnlen);
    integer isave[3];
    extern /* Subroutine */ int zlacn2_(integer *, doublecomplex *,
            doublecomplex *, doublereal *, integer *, integer *);
    extern doublereal dlamch_(char *, ftnlen);
    extern /* Subroutine */ int xerbla_(char *, integer *, ftnlen);
    doublereal ainvnm;
    extern integer izamax_(integer *, doublecomplex *, integer *);
    logical onenrm;
    extern /* Subroutine */ int zdrscl_(integer *, doublereal *,
            doublecomplex *, integer *);
    char normin[1];
    doublereal smlnum;
    extern /* Subroutine */ int zlatrs_(char *, char *, char *, char *,
            integer *, doublecomplex *, integer *, doublecomplex *,
            doublereal *, doublereal *, integer *, ftnlen, ftnlen, ftnlen,
            ftnlen);
    (void)norm_len;

/*  -- LAPACK routine (version 3.2) -- */
/*  -- LAPACK is a software package provided by Univ. of Tennessee,    -- */
/*  -- Univ. of California Berkeley, Univ. of Colorado Denver and NAG Ltd..-- */
/*     November 2006 */

/*     Modified to call ZLACN2 in place of ZLACON, 10 Feb 03, SJH. */

/*     .. Scalar Arguments .. */
/*<       CHARACTER          NORM >*/
/*<       INTEGER            INFO, LDA, N >*/
/*<       DOUBLE PRECISION   ANORM, RCOND >*/
/*     .. */
/*     .. Array Arguments .. */
/*<       DOUBLE PRECISION   RWORK( * ) >*/
/*<       COMPLEX*16         A( LDA, * ), WORK( * ) >*/
/*     .. */

/*  Purpose */
/*  ======= */

/*  ZGECON estimates the reciprocal of the condition number of a general */
/*  complex matrix A, in either the 1-norm or the infinity-norm, using */
/*  the LU factorization computed by ZGETRF. */

/*  An estimate is obtained for norm(inv(A)), and the reciprocal of the */
/*  condition number is computed as */
/*     RCOND = 1 / ( norm(A) * norm(inv(A)) ). */

/*  Arguments */
/*  ========= */

/*  NORM    (input) CHARACTER*1 */
/*          Specifies whether the 1-norm condition number or the */
/*          infinity-norm condition number is required: */
/*          = '1' or 'O':  1-norm; */
/*          = 'I':         Infinity-norm. */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0. */

/*  A       (input) COMPLEX*16 array, dimension (LDA,N) */
/*          The factors L and U from the factorization A = P*L*U */
/*          as computed by ZGETRF. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A.  LDA >= max(1,N). */

/*  ANORM   (input) DOUBLE PRECISION */
/*          If NORM = '1' or 'O', the 1-norm of the original matrix A. */
/*          If NORM = 'I', the infinity-norm of the original matrix A. */

/*  RCOND   (output) DOUBLE PRECISION */
/*          The reciprocal of the condition number of the matrix A, */
/*          computed as RCOND = 1/(norm(A) * norm(inv(A))). */

/*  WORK    (workspace) COMPLEX*16 array, dimension (2*N) */

/*  RWORK   (workspace) DOUBLE PRECISION array, dimension (2*N) */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */

/*  ===================================================================== */

/*     .. Parameters .. */
/*<       DOUBLE PRECISION   ONE, ZERO >*/
/*<       PARAMETER          ( ONE = 1.0D+0, ZERO = 0.0D+0 ) >*/
/*     .. */
/*     .. Local Scalars .. */
/*<       LOGICAL            ONENRM >*/
/*<       CHARACTER          NORMIN >*/
/*<       INTEGER            IX, KASE, KASE1 >*/
/*<       DOUBLE PRECISION   AINVNM, SCALE, SL, SMLNUM, SU >*/
/*<       COMPLEX*16         ZDUM >*/
/*     .. */
/*     .. Local Arrays .. */
/*<       INTEGER            ISAVE( 3 ) >*/
/*     .. */
/*     .. External Functions .. */
/*<       LOGICAL            LSAME >*/
/*<       INTEGER            IZAMAX >*/
/*<       DOUBLE PRECISION   DLAMCH >*/
/*<       EXTERNAL           LSAME, IZAMAX, DLAMCH >*/
/*     .. */
/*     .. External Subroutines .. */
/*<       EXTERNAL           XERBLA, ZDRSCL, ZLACN2, ZLATRS >*/
/*     .. */
/*     .. Intrinsic Functions .. */
/*<       INTRINSIC          ABS, DBLE, DIMAG, MAX >*/
/*     .. */
/*     .. Statement Functions .. */
/*<       DOUBLE PRECISION   CABS1 >*/
/*     .. */
/*     .. Statement Function definitions .. */
/*<       CABS1( ZDUM ) = ABS( DBLE( ZDUM ) ) + ABS( DIMAG( ZDUM ) ) >*/
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

/*<       INFO = 0 >*/
    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --work;
    --rwork;

    /* Function Body */
    *info = 0;
/*<       ONENRM = NORM.EQ.'1' .OR. LSAME( NORM, 'O' ) >*/
    onenrm = *(unsigned char *)norm == '1' || lsame_(norm, "O", (ftnlen)1, (
            ftnlen)1);
/*<       IF( .NOT.ONENRM .AND. .NOT.LSAME( NORM, 'I' ) ) THEN >*/
    if (! onenrm && ! lsame_(norm, "I", (ftnlen)1, (ftnlen)1)) {
/*<          INFO = -1 >*/
        *info = -1;
/*<       ELSE IF( N.LT.0 ) THEN >*/
    } else if (*n < 0) {
/*<          INFO = -2 >*/
        *info = -2;
/*<       ELSE IF( LDA.LT.MAX( 1, N ) ) THEN >*/
    } else if (*lda < max(1,*n)) {
/*<          INFO = -4 >*/
        *info = -4;
/*<       ELSE IF( ANORM.LT.ZERO ) THEN >*/
    } else if (*anorm < 0.) {
/*<          INFO = -5 >*/
        *info = -5;
/*<       END IF >*/
    }
/*<       IF( INFO.NE.0 ) THEN >*/
    if (*info != 0) {
/*<          CALL XERBLA( 'ZGECON', -INFO ) >*/
        i__1 = -(*info);
        xerbla_("ZGECON", &i__1, (ftnlen)6);
/*<          RETURN >*/
        return 0;
/*<       END IF >*/
    }

/*     Quick return if possible */

/*<       RCOND = ZERO >*/
    *rcond = 0.;
/*<       IF( N.EQ.0 ) THEN >*/
    if (*n == 0) {
/*<          RCOND = ONE >*/
        *rcond = 1.;
/*<          RETURN >*/
        return 0;
/*<       ELSE IF( ANORM.EQ.ZERO ) THEN >*/
    } else if (*anorm == 0.) {
/*<          RETURN >*/
        return 0;
/*<       END IF >*/
    }

/*<       SMLNUM = DLAMCH( 'Safe minimum' ) >*/
    smlnum = dlamch_("Safe minimum", (ftnlen)12);

/*     Estimate the norm of inv(A). */

/*<       AINVNM = ZERO >*/
    ainvnm = 0.;
/*<       NORMIN = 'N' >*/
    *(unsigned char *)normin = 'N';
/*<       IF( ONENRM ) THEN >*/
    if (onenrm) {
/*<          KASE1 = 1 >*/
        kase1 = 1;
/*<       ELSE >*/
    } else {
/*<          KASE1 = 2 >*/
        kase1 = 2;
/*<       END IF >*/
    }
/*<       KASE = 0 >*/
    kase = 0;
/*<    10 CONTINUE >*/
L10:
/*<       CALL ZLACN2( N, WORK( N+1 ), WORK, AINVNM, KASE, ISAVE ) >*/
    zlacn2_(n, &work[*n + 1], &work[1], &ainvnm, &kase, isave);
/*<       IF( KASE.NE.0 ) THEN >*/
    if (kase != 0) {
/*<          IF( KASE.EQ.KASE1 ) THEN >*/
        if (kase == kase1) {

/*           Multiply by inv(L). */

/*<    >*/
            zlatrs_("Lower", "No transpose", "Unit", normin, n, &a[a_offset],
                    lda, &work[1], &sl, &rwork[1], info, (ftnlen)5, (ftnlen)
                    12, (ftnlen)4, (ftnlen)1);

/*           Multiply by inv(U). */

/*<    >*/
            zlatrs_("Upper", "No transpose", "Non-unit", normin, n, &a[
                    a_offset], lda, &work[1], &su, &rwork[*n + 1], info, (
                    ftnlen)5, (ftnlen)12, (ftnlen)8, (ftnlen)1);
/*<          ELSE >*/
        } else {

/*           Multiply by inv(U'). */

/*<    >*/
            zlatrs_("Upper", "Conjugate transpose", "Non-unit", normin, n, &a[
                    a_offset], lda, &work[1], &su, &rwork[*n + 1], info, (
                    ftnlen)5, (ftnlen)19, (ftnlen)8, (ftnlen)1);

/*           Multiply by inv(L'). */

/*<    >*/
            zlatrs_("Lower", "Conjugate transpose", "Unit", normin, n, &a[
                    a_offset], lda, &work[1], &sl, &rwork[1], info, (ftnlen)5,
                     (ftnlen)19, (ftnlen)4, (ftnlen)1);
/*<          END IF >*/
        }

/*        Divide X by 1/(SL*SU) if doing so will not cause overflow. */

/*<          SCALE = SL*SU >*/
        scale = sl * su;
/*<          NORMIN = 'Y' >*/
        *(unsigned char *)normin = 'Y';
/*<          IF( SCALE.NE.ONE ) THEN >*/
        if (scale != 1.) {
/*<             IX = IZAMAX( N, WORK, 1 ) >*/
            ix = izamax_(n, &work[1], &c__1);
/*<    >*/
            i__1 = ix;
            if (scale < ((d__1 = work[i__1].r, abs(d__1)) + (d__2 = d_imag(&
                    work[ix]), abs(d__2))) * smlnum || scale == 0.) {
                goto L20;
            }
/*<             CALL ZDRSCL( N, SCALE, WORK, 1 ) >*/
            zdrscl_(n, &scale, &work[1], &c__1);
/*<          END IF >*/
        }
/*<          GO TO 10 >*/
        goto L10;
/*<       END IF >*/
    }

/*     Compute the estimate of the reciprocal condition number. */

/*<    >*/
    if (ainvnm != 0.) {
        *rcond = 1. / ainvnm / *anorm;
    }

/*<    20 CONTINUE >*/
L20:
/*<       RETURN >*/
    return 0;

/*     End of ZGECON */

/*<       END >*/
} /* zgecon_ */

#ifdef __cplusplus
        }
#endif
