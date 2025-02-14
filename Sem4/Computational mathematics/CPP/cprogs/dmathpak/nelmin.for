        SUBROUTINE NELMIN(FN, N, START, XMIN, YNEWLO, REQMIN, STEP,
     &                    KONVGE, ICOUNT, KCOUNT, NUMRES, IFAULT,
     &                    reltol, abstol)
C
C          The Nelder-Mead Simplex Minimization Procedure.
C
C          *****  Double precision version 1.0  June 1986  *****
C
C          This algorithm is a modified version of ..
C          Algorithm AS 47 Applied Statistics (J. R. Statist. Soc. C),
C          (1971) VOL.20. NO.3
C
C          Written for MATHPAK by P.A. Jacobs and N.J. Lott
C                                 Department of Mechanical Engineering
C                                 University of Queensland
C
C          Version 2.0 march 1989
C
C          Purpose :: To find the minimum value of a user-specified
C                     function.
C
C          Formal parameters ::
C
C              FN :  Input : User specified REAL*8 function to be
C                            minimized.
C               N :  Input : INTEGER. The number of variables over
C                            which we are minimizing. N <= ID (ID is
C                            set in the PARAMETER statement below.)
C           START :  Input : Array (1 .. N) of REAL*8; contains the
C                            coordinates of the starting point.
C                   Output : The values may be over-written.
C            XMIN : Output : Array (1 .. N) of REAL*8; contains the
C                            coordimates of the minimum.
C          YNEWLO : Output : REAL*8. The minimum value of the function.
C          REQMIN :  Input : REAL*8. The terminating limit for the
C                            variance of function values.
C            STEP :  Input : Array (1 .. N) of REAL*8; determines the
C                            size and shape of the initial simplex.
C                            The relative magnitudes of its N elements
C                            should reflect the units of the N
C                            variables.
C          KONVGE :  Input : INTEGER. The convergence check is carried
C                            out every KONVGE iterations.
C          KCOUNT :  Input : INTEGER. Maximum number of function
C                            evaluations.
C          ICOUNT : Output : Function evaluations performed.
C          NUMRES : Output : Number of restarts.
C          IFAULT : Output : 0 No error.
C                            1 REQMIN, N or KONVGE illegal value.
C                            2 Termination because KCOUNT exceeded
C                              before convergence.
C          reltol : Input  : relative tolerance on minimum check
C          abstol : Input  : absolute tolerance on minimum check
C                            (set to zero for well behaved cases
C                            where restarts are not a problem)
C
C          All variables and arrays are to be declared in the calling
C          program as REAL*8.
C
C          The data values of RCOEFF (reflection), ECOEFF (extension),
C          and CCOEFF (contraction) can be changed by rewriting the
C          DATA statement below.  The values currently set are 1.0,
C          2.0 and 0.5 respectively.  These values have been recommended
C          by Nelder and Mead as being best for a general situation.
C
C          The value of REQMIN must be set larger than the rounding
C          error in computing the variance at the minimum.  This holds
C          for both Double and Single precision calculations.
C          Chambers and Ertel recommend that REQMIN be set to a value
C          somewhere between the required accuracy of the function at
C          the minimum and the square of this value.
C
C          Auxiliary Algorithm :: The FUNCTION FN(A)
C          calculates the function value at point A.
C          A is an Array (1 .. N) of REAL*8. The function must be
C          declared EXTERNAL in the calling program as the
C          name of the function is passed as a formal parameter.
C
C          References :: Nelder, J.A. and Mead, R. (1965) A simplex
C                        method for function minimization.
C                        Computer J. 7,308-313
C
C                        O'Neill, R. (1971) Algorithm AS47. Function
C                        minimization using a simplex algorithm.
C                        Appl. Statist. 20,338-345.
C
C                        Chambers, J.M. and Ertel, J.E. (1974)
C                        Remark AS R11.  Appl. Statist. 23,250-251.
C
C                        Olsson, D.M. and Nelson, L.S. (1975) The
C                        Nelder - Mead simplex procedure for function
C                        minimization.  Technometrics 17,45-51.
C                        (Examples of use.)
C
C                        Benyon, P.R. (1976) Remark AS R15. Appl.
C                        Statist. 25,97.
C
C                        Hill, I.D. (1978) Remark AS R28. Appl. Statist.
C                        27,380-382.
C
C***********************************************************************
C
        PARAMETER (ID = 20)
C
        REAL*8 START(N), XMIN(N), YNEWLO, REQMIN, STEP(N),
     &         ONE, HALF, ZERO,
     &         P(ID,ID+1), PSTAR(ID), P2STAR(ID), PBAR(ID),
     &         Y(ID+1),
     &         DN, DNP1, Z, SUM, SUMM, YLO, RCOEFF, YSTAR, ECOEFF,
     &         Y2STAR, CCOEFF,
     &         CURMIN, DEL, FN, X, DELTA, YHI
        REAL*8 DSUM, DURMIN
        real*8 reltol, abstol, tol
C
        INTEGER IFAULT, N, KONVGE, ICOUNT, NUMRES, JCOUNT, NP1, I, J,
     &          ILO, IHI, L
C
        LOGICAL OK, REFLOK, EXTNOK, CONTOK, EXIT
C
        EXTERNAL FN
C
        DATA ZERO, HALF, ONE / 0.0D+00, 0.5D+00, 1.0D+00 /
        DATA DELTA / 1.0D-03 /
C
C       Reflection, extension and contraction coefficients.
        DATA RCOEFF, ECOEFF, CCOEFF / 1.0D+00, 2.0D+00, 0.5D+00 /
C
C       Validity checks on input parameters.
C
        IFAULT = 1
        IF ((REQMIN .LE. ZERO) .OR. (N .LT. 1) .OR. (N .GT. ID) .OR.
     &      (KONVGE .LT. 1)) RETURN
        IFAULT = 2
        ICOUNT = 0
        NUMRES = 0
C
        DN = DFLOAT(N)
        NP1 = N + 1
        DNP1 = DFLOAT(NP1)
        DEL = ONE
C
C       Construction of initial simplex.
C
 1001   DO 10 I = 1, N
 10       P(I,NP1) = START(I)
        Z = FN(START)
        Y(NP1) = Z
        DO 30 J = 1, N
          X = START(J)
          START(J) = START(J) + STEP(J) * DEL
          DO 20 I = 1, N
 20         P(I,J) = START(I)
          Z = FN(START)
          Y(J) = Z
 30       START(J) = X
          ICOUNT = ICOUNT + NP1
C
C       Simplex construction complete.
C
 1000   DO 170 JCOUNT = 1, KONVGE
C
C         Find highest and lowest Y values.  YHI ( =Y(IHI) ) indicates
C         the vertex of the simplex to be replaced.
C
          YLO = Y(1)
          YHI = YLO
          ILO = 1
          IHI = 1
          DO 40 I = 2, NP1
            IF(Y(I) .LT. YLO) THEN
              YLO = Y(I)
              ILO = I
            END IF
            IF(Y(I) .GT. YHI) THEN
              YHI = Y(I)
              IHI = I
            END IF
 40         CONTINUE
C
C         Calculate PBAR, the centroid of the simplex vertices
C         excepting that with the Y value YHI. (largest value)
C
          DO 60 I = 1, N
            Z = ZERO
            DO 50 J = 1, NP1
 50           Z = Z + P(I,J)
            Z = Z - P(I,IHI)
 60         PBAR(I) = Z / DN
C
C         Reflection through the centroid.
C
          DO 70 I = 1, N
 70         PSTAR(I) = (ONE + RCOEFF) * PBAR(I) - RCOEFF * P(I,IHI)
          YSTAR = FN(PSTAR)
          ICOUNT = ICOUNT + 1
          REFLOK = (YSTAR .LT. YLO)
C
          IF (REFLOK) THEN
C
C           Successful reflection, so try extension
            DO 80 I = 1, N
 80           P2STAR(I) = ECOEFF * PSTAR(I) + (ONE - ECOEFF) * PBAR(I)
            Y2STAR = FN(P2STAR)
            ICOUNT = ICOUNT + 1
            EXTNOK = (Y2STAR .LT. YSTAR)
C
            IF (EXTNOK) THEN
C
C             Retain extension or contraction.
              DO 90 I = 1, N
 90             P(I,IHI) = P2STAR(I)
              Y(IHI) = Y2STAR
            ELSE
C
C             Retain reflection.
              DO 95 I = 1, N
 95             P(I,IHI) = PSTAR(I)
              Y(IHI) = YSTAR
            END IF
C
          ELSE
C
C           Reflection has not been successful.  YSTAR is the function
C           value at the extended point.  Now look at the other points
C           of the simplex ...
C           if there are none > YSTAR then contract to a point
C                                          within the simplex
C           if there is one   > YSTAR then reduce the size of the
C                                          extension
C           if there are many > YSTAR then retain the reflection as is
C
            L = 0
            DO 100 I = 1, NP1
              IF (Y(I) .GT. YSTAR) L = L + 1
 100          CONTINUE
C
            IF (L .EQ. 1) THEN
C
C             Reduce the extension by
C             contracting on the reflection side of the centriod.
              DO 110 I = 1, N
 110            P(I,IHI) = PSTAR(I)
              Y(IHI) = YSTAR
            END IF
C
            IF (L .EQ. 0) THEN
C
C             Contraction on the Y(IHI) side of the centroid.
              DO 120 I = 1, N
 120            P2STAR(I) = CCOEFF * P(I,IHI) + (ONE - CCOEFF) *
     &                      PBAR(I)
              Y2STAR = FN(P2STAR)
              ICOUNT = ICOUNT + 1
              CONTOK = (Y2STAR .LE. Y(IHI))
C
              IF (CONTOK) THEN
C
C               Retain contraction
                DO 130 I = 1, N
 130              P(I,IHI) = P2STAR(I)
                Y(IHI) = Y2STAR
C
              ELSE
C
C               Contract whole simplex.
                DO 150 J = 1, NP1
                  DO 140 I = 1, N
                    P(I,J) = (P(I,J) + P(I,ILO)) * HALF
 140                XMIN(I) = P(I,J)
 150              Y(J) = FN(XMIN)
                ICOUNT = ICOUNT + NP1
              END IF
C
            ELSE
C
C             Retain reflection. (L > 1)
              DO 160 I = 1, N
 160            P(I,IHI) = PSTAR(I)
              Y(IHI) = YSTAR
            END IF
C
          END IF
C
 170      CONTINUE
C
C       Are we over the limit for number of function evaluations ?
        EXIT = (ICOUNT .GT. KCOUNT)
C
        IF (.NOT. EXIT) THEN
C
C         Check to see if minimum reached.
C         Calculation of the variance must be done in double
C         precision even if the rest of the code is in REAL*4.
          SUM = ZERO
          DO 180 I = 1, NP1
 180        SUM = SUM + Y(I)
          DSUM = DBLE(SUM) / DBLE(DNP1)
          DURMIN = 0.0D0
          DO 190 I = 1, NP1
 190        DURMIN = DURMIN + (DBLE(Y(I)) - DSUM)**2
          CURMIN = DURMIN / DN
C         CURMIN = SNGL(DURMIN) / DN  for single precision.
C
C         CURMIN is the variance of the N+1 FN values at the vertices.
C         If we haven't reached the minimum to the required accuracy
C         then take a few more steps.
          IF (CURMIN .GE. REQMIN) GO TO 1000
        END IF
C
C       Save the current minimum
C
        IF (Y(IHI) .GT. Y(ILO)) IHI = ILO
        DO 200 I = 1, N
 200      XMIN(I) = P(I,IHI)
        YNEWLO = Y(IHI)
C
C       **** Bail out ****
        IF (EXIT) RETURN
C
C       Check around the currently selected point to see if it is
C       a local minimum.
        tol = abstol + reltol * abs(ynewlo)
        OK = .TRUE.
        DO 210 I = 1, N
          IF (OK) THEN
            DEL = STEP(I) * DELTA
            XMIN(I) = XMIN(I) + DEL
            Z = FN(XMIN)
            ICOUNT = ICOUNT+1
            IF ((YNEWLO - Z) .gt. tol) OK = .FALSE.
            XMIN(I) = XMIN(I) - DEL - DEL
            Z = FN(XMIN)
            ICOUNT = ICOUNT+1
            IF ((YNEWLO - Z) .gt. tol) OK = .FALSE.
            XMIN(I) = XMIN(I) + DEL
          END IF
  210     CONTINUE
C
C       Return on finding a local minimum to the desired accuracy.
C
        IF (OK) THEN
          IFAULT = 0
          RETURN
        END IF
C
C       Restart procedure, and reduce the size of the simplex.
C
        DO 220 I = 1, N
 220      START(I) = XMIN(I)
        DEL = DELTA
        NUMRES = NUMRES + 1
        GO TO 1001
        END
