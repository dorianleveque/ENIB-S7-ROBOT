//************************************************************************
//   IPS - CALCUL D'UN PI 
//   kerhoas@enib.fr
//***********************************************************************
 
i=0; // index Figures
//-----------------------------------------------------------------------
//    Définition du Gain et de la Constante de Temps en Boucle Ouverte
//    du système à asservir G/(1+to*s)
//-----------------------------------------------------------------------
 
G=2
to=3e-3
 
//-----------------------------------------------------------------------
//              Fonction de Transfert en Boucle Ouverte + Tracé
//-----------------------------------------------------------------------
 
s = poly(0, 's');
FTBO = syslin('c', G/(1+to*s));     
 
 
//i=i+1;
//figure(i)
//t=0:0.0001:1
//plot2d(t,csim('step',t,FTBO))       
//xgrid
//
//title('FTBO - REPONSE A UN ECHELON')
//
//i=i+1;
//figure(i)
//subplot(1,2,1)
//nyquist(FTBO)
//xgrid                               
//title('FTBO - NYQUIST')
//subplot(1,2,2)
//bode(FTBO)
//xgrid                               
//title('FTBO - BODE')
 
//-----------------------------------------------------------------------
//              Définition du Correcteur PI + Tracé
//-----------------------------------------------------------------------
 
ti = 0.1*to // Figé
Kpi = 0.01     // Paramètre à régler
 
s = poly(0, 's');
CORR = syslin('c', (Kpi*(ti*s+1))/(ti*s));  
 
i=i+1;
figure(i)
bode(CORR)
xgrid                  
title('CORRECTEUR PI - BODE')
 
//-----------------------------------------------------------------------
//     Fonction de Transfert EN BO du Système Corrigé + Tracé
//-----------------------------------------------------------------------
 
FTBO_CORR=CORR*FTBO
 
i=i+1;
figure(i)
bode(FTBO_CORR)
 
xgrid
title('FTBO CORRIGE - BODE - MARGE DE PHASE')
 
//-----------------------------------------------------------------------
//     Fonction de Transfert EN BF du Système Corrigé + Tracé
//-----------------------------------------------------------------------
 
FTBF= (CORR*FTBO)/(1+CORR*FTBO)
 
i=i+1;
figure(i)
bode(FTBF)
title('FTBF CORRIGE - BODE')
xgrid                               
i=i+1;
figure(i)
t=0:0.0001:1
plot2d(t,csim('step',t,FTBF))         
xgrid                                 
title('FTBF CORRIGE - REPONSE TEMPORELLE A UN ECHELON')
