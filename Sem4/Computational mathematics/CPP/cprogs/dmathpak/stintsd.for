C
C    SAMPLE DRIVER FOR STINT
C
      PROGRAM STINTT
      DOUBLE PRECISION T,TOUT,HI,ERROR,Y,Y0,YDOT,SAVE,RJ,YMAX,RW
      DIMENSION Y(3),Y0(96),YDOT(12),SAVE(39),RJ(9),
     &      YMAX(3),RW(9),IPIV(3)
      COMMON/STAT/NSTEP,NFE,NJE,NINVS
      EXTERNAL DIFFUN,PEDERV
      DATA N,N0,MF/2,3,1/
      HI = 1.0E-5
      ERROR = 1.0E-6
      Y(1) = 0.0
      Y(2) = 0.0
      NSTEP = 0
      NFE = 0
      NJE = 0
      NINVS = 0
      WRITE(*,*) '*** TEST FOR STINT ***'
      WRITE(*,10)
   10 FORMAT(11X,'T',20X,'Y')
      T = 0.0
      TOUT = 10.0
      CALL STINT1(N,N0,T,TOUT,HI,ERROR,MF,Y,Y0,YDOT,SAVE,
     &             RJ,YMAX,RW,IPIV,KFLAG,DIFFUN,PEDERV)
      IF (KFLAG .LT. 0) STOP
      DO 20 J = 1,N
        WRITE(*,30) T,Y(J)
   20 CONTINUE
      write (*,*) 'nstep=',nstep,'  nfe=',nfe,'  nje=',nje,'  ninvs=',
     &            ninvs
   30 FORMAT(5X,F10.2,10X,F10.2)
      WRITE(*,*) 'CORRECT ANSWER IS 1.0 & 2.0'
      END
      SUBROUTINE DIFFUN(N,T,Y,YDOT)
      DOUBLE PRECISION Y(N),YDOT(N),T,A,B
      A = 1.0
      B = 1.0E+3
      YDOT(1) = A - A*Y(1)**2
      YDOT(2) = A + B - (A - B)*Y(1)**2 - B*Y(2)**2
     &          + 2.0*B*Y(1)*(Y(2) - Y(1))
      RETURN
      END
      SUBROUTINE PEDERV(N,T,Y,P0,N0)
      DOUBLE PRECISION P0(2,2),Y(N),T,A,B
      A = 1.0
      B = 1.0E+3
      P0(1,1) = -2.0*A*Y(1)
      P0(1,2) = 0.0
      P0(2,1) = -2.0*(A - B)*Y(1) + 2.0*B*(Y(2) - Y(1))
     &          - 2.0*B*Y(1)
      P0(2,2) = 2.0*B*(Y(1) - Y(2))
      RETURN
      END
