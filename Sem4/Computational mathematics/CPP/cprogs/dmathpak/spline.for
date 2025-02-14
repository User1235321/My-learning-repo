      SUBROUTINE SPLINE (N, END1, END2, SLOPE1, SLOPE2,
     &                   X, F, B, C, D)
C
C    PURPOSE:
C        THE COEFFICIENTS B(I),C(I), AND D(I), I = 1,2,..,N ARE
C        COMPUTED FOR A CUBIC INTERPOLATING SPLINE.
C        S(X) = F(I) + B(I)*(X-X(I)) + C(I)*(X-X(I))**2
C               + D(I)*(X-X(I))**3
C        FOR X(I) <= X <= X(I+1).
C
C    ARGUMENT TYPE :
C        N         : INTEGER.
C    END1, END2    : LOGICAL
C    SLOPE1,SLOPE2 : DOUBLE PRECISION
C        X,F,B,C,D : ARRAY (1..N) OF DOUBLE PRECISION.
C
C    INPUT  :
C        N         : NUMBER OF DATA POINTS OR KNOTS.
C    END1, END2    : .TRUE. IF END SLOPES ARE TO BE FORCED
C    SLOPE1,SLOPE2 : VALUES OF THE END SLOPES
C        X         : ABSCISSAS OF KNOTS IN INCREASING ORDER.
C        F         : ORDINATES OF KNOTS.
C
C    OUTPUT :
C        B,C,D     : ARRAYS OF SPLINE COEFFICIENTS AS DEFINED ABOVE.
C                    USING "P" TO DENOTE DIFFERENTIATION -
C                    F(I) = S(X(I))
C                    B(I) = SP(X(I))
C                    C(I) = SPP(X(I))
C                    D(I) = SPPP(X(I))
C
C    REMARKS:
C       DOUBLE PRECISION FUNCTION "SEVAL" CAN BE USED TO EVALUATE THE
C       SPLINE.
C        THIS PROGRAM IS TAKEN FROM "COMPUTER METHODS FOR
C        MATHEMATICAL COMPUTATIONS" BY G.E. FORSYTHE, M.A. MALCOLM,
C        AND C.B. MOLER , PRENTICE-HALL,
C        ENGLEWOOD CLIFFS,N.J. (1977) , P.76.
C        THE END CONDITIONS MATCH THE THIRD DERIVATIVES OF S(X)
C        WITH THE THIRD DERIVATIVES OF THE UNIQUE CUBICS PASSING
C        THROUGH THE FOUR POINTS AT EITHER END.
C
C        MODIFIED TO INCLUDE END SLOPES BY
C        P. JACOBS
C        DEPARTMENT OF MECHANICAL ENGINEERING
C        UNIVERSITY OF QUEENSLAND
C        JUNE 1989
C
      INTEGER N,NM1,IB,I
      DOUBLE PRECISION X(N),F(N),B(N),C(N),D(N),T,T1,T2
      DOUBLE PRECISION SLOPE1, SLOPE2
      LOGICAL END1, END2
C
      NM1 = N - 1
      IF (N .LT. 2) RETURN
      IF (N .LT. 3) GO TO 50
C
C    SET UP TRIDIAGONAL SYSTEM.
C    B = DIAGONAL, D = OFFDIAGONAL, C = RIGHT HAND SIDE.
C
      D(1) = X(2) - X(1)
      C(2) = (F(2) - F(1))/D(1)
      DO 10 I = 2,NM1
        D(I) = X(I+1) - X(I)
        B(I) = 2.0D0*(D(I-1) + D(I))
        C(I+1) = (F(I+1) - F(I))/D(I)
        C(I) = C(I+1) - C(I)
   10 CONTINUE
C
C    DEFAULT END CONDITIONS. THIRD DERIVATIVES AT X(1) AND X(N)
C    OBTAINED FROM DIVIDED DIFFERENCES.
C
      B(1) = -D(1)
      B(N) = -D(N-1)
      C(1) = 0.0
      C(N) = 0.0
      IF (N .EQ. 3) GO TO 15
      T1 = C(3)/(X(4) - X(2))
      T2 = C(2)/(X(3) - X(1))
      C(1) = T1 - T2
      T1 = C(N-1)/(X(N) - X(N-2))
      T2 = C(N-2)/(X(N-1) - X(N-3))
      C(N) = T1 - T2
      C(1) = C(1)*D(1)**2/(X(4) - X(1))
      C(N) = -C(N)*D(N-1)**2/(X(N) - X(N-3))
C
C     ALTERNATIVE END CONDITIONS; KNOWN SLOPES
C
      IF (END1) THEN
         B(1) = 2.0D0 * (X(2) - X(1))
         C(1) = (F(2) - F(1)) / (X(2) - X(1)) - SLOPE1
      ENDIF
C
      IF (END2) THEN
         B(N) = 2.0 * (X(N) - X(N-1))
         C(N) = SLOPE2 - (F(N) - F(N-1)) / (X(N) - X(N-1))
      ENDIF
C
C
C    FORWARD ELIMINATION.
C
   15 DO 20 I = 2,N
        T = D(I-1)/B(I-1)
        B(I) = B(I) - T*D(I-1)
        C(I) = C(I) - T*C(I-1)
   20 CONTINUE
C
C    BACK SUBSTITUTION.
C
      C(N) = C(N)/B(N)
      DO 30 IB = 1,NM1
        I = N - IB
        C(I) = (C(I) - D(I)*C(I+1))/B(I)
   30 CONTINUE
C
C    C(I) IS NOW THE SIGMA(I) OF THE TEXT.
C    COMPUTE POLYNOMIAL COEFFICIENTS.
C
      B(N) = (F(N) - F(NM1))/D(NM1) + D(NM1)*(C(NM1) + 2.0D0*C(N))
      DO 40 I = 1,NM1
        B(I) = (F(I+1) - F(I))/D(I) - D(I)*(C(I+1) + 2.0D0*C(I))
        D(I) = (C(I+1) - C(I))/D(I)
        C(I) = 3.0D0*C(I)
   40 CONTINUE
      C(N) = 3.0D0*C(N)
      D(N) = D(N-1)
      RETURN
C
C    LINEAR INTERPOLATION.
C
   50 B(1) = (F(2) - F(1))/(X(2) - X(1))
      C(1) = 0.0
      D(1) = 0.0
      B(2) = B(1)
      C(2) = 0.0
      D(2) = 0.0
      RETURN
      END
C
C
C
C
C
      DOUBLE PRECISION FUNCTION SEVAL(N,XX,X,F,B,C,D)
C
C    PURPOSE:
C        EVALUATES CUBIC SPLINE FUNCTION , USING HORNER'S RULE.
C
C    ARGUMENT TYPE :
C        N         : INTEGER.
C        XX        : DOUBLE PRECISION.
C        X,F,B,C,D : ARRAY (1..N) OF DOUBLE PRECISION.
C
C    INPUT  :
C        N         : NUMBER OF DATA POINTS.
C        XX        : ABSCISSA AT WHICH SPLINE IS TO BE EVALUATED.
C        X,F       : ARRAYS OF DATA ABSCISSAS AND ORDINATES.
C        B,C,D     : ARRAYS OF SPLINE COEFFICIENTS COMPUTED
C                    BY SUBROUTINE "SPLINE".
C
      INTEGER N,I,J,K
      DOUBLE PRECISION X(N),F(N),B(N),C(N),D(N),XX,DX
      DATA I/1/
      IF (I .GE. N) I = 1
      IF (XX .LT. X(I)) GO TO 10
      IF (XX .LE. X(I+1)) GO TO 30
C
C    BINARY SEARCH.
C
   10 I = 1
      J = N + 1
   20 K = (I + J)/2
      IF (XX .LT. X(K)) J = K
      IF (XX .GE. X(K)) I = K
      IF (J .GT. (I+1)) GO TO 20
C
C    EVALUATE SPLINE.
C
   30 DX = XX - X(I)
      SEVAL = F(I) + DX*(B(I) + DX*(C(I) + DX*D(I)))
      RETURN
      END
C
C
C
C
      DOUBLE PRECISION function deriv (N,XX,X,F,B,C,D)
C
C    PURPOSE:
C        EVALUATES the derivative of the CUBIC SPLINE FUNCTION.
C
C    ARGUMENT TYPE :
C        N         : INTEGER.
C        XX        : DOUBLE PRECISION.
C        X,F,B,C,D : ARRAY (1..N) OF DOUBLE PRECISION.
C
C    INPUT  :
C        N         : NUMBER OF DATA POINTS.
C        XX        : ABSCISSA AT WHICH SPLINE IS TO BE EVALUATED.
C        X,F       : ARRAYS OF DATA ABSCISSAS AND ORDINATES.
C        B,C,D     : ARRAYS OF SPLINE COEFFICIENTS COMPUTED
C                    BY SUBROUTINE "SPLINE".
C
      INTEGER N,I,J,K
      DOUBLE PRECISION X(N),F(N),B(N),C(N),D(N),XX,DX
      DATA I/1/
      IF (I .GE. N) I = 1
      IF (XX .LT. X(I)) GO TO 10
      IF (XX .LE. X(I+1)) GO TO 30
C
C    BINARY SEARCH.
C
   10 I = 1
      J = N + 1
   20 K = (I + J)/2
      IF (XX .LT. X(K)) J = K
      IF (XX .GE. X(K)) I = K
      IF (J .GT. (I+1)) GO TO 20
C
C    EVALUATE SPLINE derivative.
C
   30 DX = XX - X(I)
      deriv = B(I) + dx * (2.0d0 * C(I) + 3.0d0 * DX * D(I))
      RETURN
      END
