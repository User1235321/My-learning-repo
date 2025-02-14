C
C    SAMPLE DRIVER FOR SPLINE
C
      PROGRAM SPLINT
      DOUBLE PRECISION X(5), Y(5), B(5), C(5), D(5)
      DOUBLE PRECISION SLOPE1, SLOPE2
      DOUBLE PRECISION U, S, SEVAL
      INTEGER N
      LOGICAL END1, END2
      N    = 5
      X(1) = 0.0D0
      X(2) = 1.0D0
      X(3) = 3.0D0
      X(4) = 4.0D0
      X(5) = 5.0D0
      DO 10 I = 1,N
        Y(I) = X(I)**2
   10 CONTINUE
      END1   = .FALSE.
      SLOPE1 = 0.0D0
      END2   = .FALSE.
      SLOPE2 = 0.0D0
      WRITE(*,*) '*** TEST FOR SPLINE ***'
      CALL SPLINE (N, END1, END2, SLOPE1, SLOPE2, X, Y, B, C, D)
      WRITE (*,*) '... SPLINE FITTED ...'
      U = 2.0D0
      S = SEVAL(N, U, X, Y, B, C, D)
      WRITE(*,40) U,S
   40 FORMAT(5X,'X=',F5.2,5X,'F(X)=',F5.2)
      WRITE(*,*) 'CORRECT ANSWER IS 4.0'
      END
