      double precision FUNCTION fortFmin(AX,BX,F,TOL)
      double precision AX,BX,F,TOL
C
C     B���C��ET �P�����EH�E X K TO�KE, ��E F �OCT��AET
C     M�H�M�MA HA �HTEPBA�E (AX,BX)
C
C     BXO�HA� �H�OPMA���.
C
C     AX   �EB�� KOHE� �CXO�HO�O �HTEPBA�A
C     BX   �PAB�� KOHE� �CXO�HO�O �HTEPBA�A
C     F    �O��PO�PAMMA-��HK���, KOTOPA� B���C��ET F(X)
C          ��� ���O�O X B �HTEPBA�E (AX,BX)
C    TOL   �E�AEMA� ���HA �HTEPBA�A HEO�PE�E�EHHOCT�
C          KOHE�HO�O PE����TATA (.GE.0.0)
C
C    B�XO�HA� �H�OPMA���.
C
C    FMIN  A�C��CCA, A��POKC�M�P���A� TO�K�,
C          ��E F �OCT��AET M�H�M�MA
C
C       METO� �C�O����ET KOM��HA��� �O�CKA �O�OTO�O CE�EH��
C    � �OC�E�OBATE��HO� �APA�O���ECKO� �HTEP�O�����. CXO��-
C    MOCT� H�KO��A HE ��BAET �HA��TE��HO X��E, �EM ���
C    �O�CKA ���OHA���. EC�� F �MEET HE�PEP�BH�� BTOP��
C    �PO��BO�H��, �O�O��TE��H�� B TO�KE M�H�M�MA (HE
C    COB�A�A��E� H� C AX,H� C BX), TO CXO��MOCT� CBEPX-
C    ��HE�HA� � O���HO �MEET �OP��OK �P�MEPHO 1.324...
C       ��HK��� F H�KO��A HE B���C��ETC� B �B�X TO�KAX,
C    OTCTO���X �P�� OT �P��A MEHEE �EM HA EPS*ABS(X)+(TOL/3),
C    ��E EPS �P������TE��HO PABHO KBA�PATHOM� KOPH� ��
C    OTHOC�TE��HO� MA��HHO� TO�HOCT�. EC�� F-�H�MO�A��HA�
C    ��HK��� � B���C�EHH�E �HA�EH�� F COXPAH��T �H�MO�A��-
C    HOCT� �P� CO����EH�� �KA�AHHO�O �C�OB�� PA��E�EHHOCT�,
C    TO FMIN A��POKC�M�P�ET A�C��CC� ��O�A��HO�O M�H�M�MA F
C    HA �HTEPBA�E (AX,BX) C O���KO�, MEH��E� 3*EPS*ABS(X)+TOL.
C    EC�� F HE �B��ETC� �H�MO�A��HO�, TO FMIN MO�ET C TO� �E
C    TO�HOCT�� A��POKC�M�POBAT� �OKA��H�� M�H�M�M, BO�MO�HO,
C    HE COB�A�A���� C ��O�A��H�M.
C       �TA �O��PO�PAMMA-��HK��� �B��ETC� C�E�KA MO������PO-
C    BAHHO� BEPC�E� A��O� 60-�PO�E��P� LOCALMIN, �P�BE�EHHO�
C    B KH��E RICARD BRENT, ALGORITHMS FOR MINIMIZATION
C    WITHOUT DERIVATIVES, PRENTICE-HALL, INC.(1973).
C
      double precision A,B,C,D,E,EPS,XM,P,Q,R,TOL1,TOL2,U,V,W
      double precision FU,FV,FW,FX,X,ABS,SIGN
C
C     C ECT� BO�BE�EHHA� B KBA�PAT BE����HA,
C     O�PATHA� K �O�OTOM� CE�EH��
C
      C=0.5*(3.0-SQRT(5.0))
C
C     EPS �P������TE��HO PABHO KBA�PATHOM� KOPH� ��
C     OTHOC�TE��HO� MA��HHO� TO�HOCT�
C
      EPS=1.0
   10 EPS=EPS/2.0
      TOL1=1.0+EPS
      IF(TOL1.GT.1.0) GO TO 10
      EPS=SQRT(EPS)
C
C     �P�CBOEH�E HA�A��H�X �HA�EH��
C
      A=AX
      B=BX
      V=A+C*(B-A)
      W=V
      X=V
      E=0.0
      FX=F(X)
      FV=FX
      FW=FX
C
C     ��EC� HA��HAETC� OCHOBHO� ��K�
C
   20 XM=0.5*(A+B)
      TOL1=EPS*ABS(X)+TOL/3.0
      TOL2=2.0*TOL1
C
C     �POBEP�T� KP�TEP�� OKOH�AH��
C
      IF(ABS(X-XM).LE.(TOL2-0.5*(B-A))) GO TO 90
C
C     HEO�XO��MO �� �O�OTOE CE�EH�E
C
      IF(ABS(E).LE.TOL1) GO TO 40
C
C     �OCTPO�T� �APA�O��
C
      R=(X-W)*(FX-FV)
      Q=(X-V)*(FX-FW)
      P=(X-V)*Q-(X-W)*R
      Q=2.0*(Q-R)
      IF(Q.GT.0.0) P=-P
      Q=ABS(Q)
      R=E
      E=D
C
C     �P�EM�EMA �� �APA�O�A
C
   30 IF(ABS(P).GE.ABS(0.5*Q*R)) GO TO 40
      IF(P.LE.Q*(A-X)) GO TO 40
      IF(P.GE.Q*(B-X)) GO TO 40
C
C     �A� �APA�O���ECKO� �HTEP�O�����
C
      D=P/Q
      U=X+D
C
C     F HE C�E��ET B���C��T� C���KOM ����KO K AX ��� BX
C
      IF((U-A).LT.TOL2) D=SIGN(TOL1,XM-X)
      IF((B-U).LT.TOL2) D=SIGN(TOL1,XM-X)
      GO TO 50
C
C     �A� �O�OTO�O CE�EH��
C
   40 IF(X.GE.XM) E=A-X
      IF(X.LT.XM)E=B-X
      D=C*E
C
C     F HE C�E��ET B���C��T� C���KOM ����KO K X
C
   50 IF(ABS(D).GE.TOL1) U=X+D
      IF(ABS(D).LT.TOL1) U=X+SIGN(TOL1,D)
      FU=F(U)
C
C     �P�CBO�T� HOB�E �HA�EH�� �APAMETPAM A,B,V,W � X
C
      IF(FU.GT.FX) GO TO 60
      IF(U.GE.X) A=X
      IF(U.LT.X) B=X
      V=W
      FV=FW
      W=X
      FW=FX
      X=U
      FX=FU
      GO TO 20
C
   60 IF(U.LT.X) A=U
      IF(U.GE.X) B=U
      IF(FU.LE.FW) GO TO 70
      IF(W.EQ.X) GO TO 70
      IF(FU.LE.FV) GO TO 80
      IF(V.EQ.X) GO TO 80
      IF(V.EQ.W) GO TO 80
      GO TO 20
C
   70 V=W
      FV=FW
      W=U
      FW=FU
      GO TO 20
C
   80 V=U
      FV=FU
      GO TO 20
C
C     KOHE� OCHOBHO�O ��K�A
C
   90 FMIN=X
      RETURN
      END
