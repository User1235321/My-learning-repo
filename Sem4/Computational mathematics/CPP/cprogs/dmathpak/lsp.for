        SUBROUTINE LSP(N,M,X,Y,C,WORK,SHIFT,RELERR,RESID,IERR)
C
C       PURPOSE         :GIVEN A SET OF M DATA POINTS X AND Y,SUCH THAT
C                        Y=Y(X),THE N COEFFICIENTS C OF A LEAST
C                        SQUARES POLYNOMIAL ARE COMPUTED.THE POLYNOMIAL IS
C                        Y=C(1)+C(2)*Z+C(3)*Z**2+..+C(N)*Z**(N-1)
C                        WHERE Z = X - SHIFT.
C
C       DATE            :AUGUST 1983.
C
C       WRITTEN BY      :JOHN D. DAY
C
C       VERSION         :1
C
C       PARAMETERS      :N      :INTEGER
C                        M      :INTEGER
C                        X      :ARRAY(1..M) OF DOUBLE PRECISION
C                        Y      :ARRAY(1..M) OF DOUBLE PRECISION
C                        C      :ARRAY(1..N) OF DOUBLE PRECISION
C                        WORK   :ARRAY(1..3*M*N+2*N) OF DOUBLE PRECISION
C                        SHIFT  :DOUBLE PRECISION
C                        RELERR :DOUBLE PRECISION
C                        IERR   :INTEGER
C
C       INPUT           :N      :NUMBER OF COEFFICIENTS IN LEAST SQUARES
C                                POLYNOMIAL.THE ORDER OF THE POLYNOMIAL
C                                IS (N-1).
C                        M      :NUMBER OF DATA POINTS.
C                        X      :DATA POINTS ,X COORDINATE.
C                        Y      :DATA POINTS ,Y COORDINATE.
C                        WORK   :WORK SPACE.NO INPUT REQUIRED.
C                        SHIFT  :ORIGIN SHIFT FOR X.A SUITABLE CENTERING
C                                VALUE CAN IMPROVE THE ACCURACY OF THE
C                                RESULT.
C                        RELERR :RELATIVE ERROR OF THE DATA.(E.G) IF THE
C                                DATA IS CORRECT TO 3 SIGNIFICANT FIGURES
C                                THEN SET RELERR=0.001. IF DATA IS EXACT,
C                                SET RELERR=0.0.
C
C       OUTPUT          :C      :COEFFICIENTS OF SHIFTED LEAST SQUARES
C                                POLYNOMIAL.
C                        RESID  :SQUARE ROOT OF THE SUM OF SQUARES OF
C                                THE RESIDUALS.
C                        IERR   :ERROR  =0,NORMAL RETURN
C                                       =K,IF K TH SINGULAR VALUE
C                                          HAS NOT BEEN COMPUTED
C                                          IN 30 ITERATIONS.
C
        INTEGER N,M,IERR,N1,N2,N3,N4
        DOUBLE PRECISION X(M),Y(M),C(N),WORK(1),SHIFT,RELERR,RESID,T
C
C       CHECK RELERR FOR MINIMUM ALLOWABLE VALUE .
C
        T = N*1.0E-16
        RELERR = MAX(RELERR,T)
C
C       INTERFACE ROUTINE FOR SPLITTING UP WORK ARRAY.
C
        N1 = M*N + 1
        N2 = M*N + N1
        N3 = M*N + N2
        N4 = N3 + N
        CALL LSP1(N,M,X,Y,C,WORK(1),WORK(N1),WORK(N2),WORK(N3),WORK(N4),
     #            SHIFT,RELERR,RESID,IERR)
        RETURN
        END
        SUBROUTINE LSP1(N,M,X,Y,C,A,U,V,SIGMA,VWORK,SHIFT,RELERR,
     #                  RESID,IERR)
C
C       INTERFACING ROUTINE WITH LSP.
C
        INTEGER N,M,IERR,I,J
        DOUBLE PRECISION X(M),Y(M),C(N),A(M,N),U(M,N),V(M,N),SIGMA(N),
     #                   VWORK(N),
     #          SHIFT,RELERR,RESID,SIGMAX,TAU,T,TT
C
C       SET UP DESIGN MATRIX.
C
        DO 20 I = 1,M
          A(I,1) = 1.0
          DO 10 J = 2,N
            A(I,J) = (X(I) - SHIFT)*A(I,J-1)
   10     CONTINUE
   20   CONTINUE
C
C       DECOMPOSITION.
C
        CALL SVD(M,M,N,A,SIGMA,.TRUE.,U,.TRUE.,V,IERR,VWORK)
        IF (IERR .NE. 0) RETURN
C
C       FIND LARGEST SINGULAR VALUE,RESET COEFFICIENTS.
C
        SIGMAX = 0.0
        DO 30 J = 1,N
          C(J) = 0.0
          SIGMAX = MAX(SIGMAX,SIGMA(J))
   30   CONTINUE
C
C       ABSOLUTE ERROR TOLERANCE.
C
        TAU = RELERR*SIGMAX
C
C       FIND COEFFICIENTS.
C
        DO 60 J = 1,N
          IF (SIGMA(J) .GT. TAU) THEN
            T = 0.0
            DO 40 I = 1,M
              T = T + U(I,J)*Y(I)
   40       CONTINUE
            T = T/SIGMA(J)
            DO 50 I = 1,N
              C(I) = C(I) + T*V(I,J)
   50       CONTINUE
          END IF
   60   CONTINUE
C
C       FIND SQUARE ROOT OF SUM OF SQUARES OF RESIDUALS.
C
        T = 0.0
        DO 80 I = 1,M
          TT = 0.0
          DO 70 J = 1,N
            TT = TT + C(J)*A(I,J)
   70     CONTINUE
          T = T + (TT - Y(I))**2
   80   CONTINUE
        RESID = SQRT(T)
        RETURN
        END
        DOUBLE PRECISION FUNCTION LSPVAL(XX,SHIFT,C,N)
C
C       PURPOSE         :EVALUATES THE N-1 TH ORDER POLYNOMIAL
C                        YY=C(1)+C(2)*X+C(3)*X**2+..+C(N)*X**(N-1).
C                               WHERE X = XX-SHIFT.
C
C       DATE            :AUGUST 1983
C
C       WRITTEN         :JOHN D. DAY
C
C       VERSION         :1
C
C       PARAMETERS      :XX     :DOUBLE PRECISION.
C                        SHIFT  :DOUBLE PRECISION
C                        C      :ARRAY(1..N) OF DOUBLE PRECISION.
C                        N      :INTEGER.
C
C       INPUT           :XX,    :VALUE OF X AT WHICH POLYNOMIAL Y(X) IS
C                        SHIFT      TO BE EVALUATED.X=XX-SHIFT.
C                        C      :COEFFICIENTS OF POLYNOMIAL.
C                        N      :NUMBER OF COEFFICIENTS.
C
        DOUBLE PRECISION C(N),XX,SHIFT,T
        INTEGER I,J
        T = 0.0
        DO 10 I = 1,N
          J = N + 1 - I
          T = (XX - SHIFT)*T + C(J)
   10   CONTINUE
        LSPVAL = T
        RETURN
        END
