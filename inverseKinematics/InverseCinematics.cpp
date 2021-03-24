#include "InverseCinematics.h"
#include <Arduino.h>

void multiply(float opGauche[4][4], float opDroit[4], float nouvelleMatrice[4]){
      for (int j = 0; j < 4; j++){
          for (int k = 0; k < 4; k++){
              nouvelleMatrice[j] += opGauche[j][k] * opDroit[k];
          }
      }
}

void inverseKinematics(float x, float y, float THETA[4]){
  
    float Ln = 0.192;
    float La = 0.192;
    float Lb = 0.192;
    float Lc = 0.288;
    float Ld = 0.288;

  float F[4] = {
    Ln+La*sin(THETA[0])+Lc*sin(THETA[2])-Lb*sin(THETA[1])-Ld*sin(THETA[3]),
    Lb*cos(THETA[1])+Ld*cos(THETA[3])-La*cos(THETA[0])-Lc*cos(THETA[2]),
    -x-La*sin(THETA[0])-Lc*sin(THETA[2]),
    La*cos(THETA[0])+Lc*cos(THETA[2])-y,
  };
  float FINV[4][4] = {
   {0,0,-(125*sin(THETA[2]))/(24*(cos(THETA[0])*sin(THETA[2]) - cos(THETA[2])*sin(THETA[0]))),(125*cos(THETA[2]))/(24*(cos(THETA[0])*sin(THETA[2]) - cos(THETA[2])*sin(THETA[0])))},
   {-(125*sin(THETA[3]))/(24*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1]))),(125*cos(THETA[3]))/(24*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1]))), -(125*sin(THETA[3]))/(24*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1]))),  (125*cos(THETA[3]))/(24*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1])))},
   {0,0,  (125*sin(THETA[0]))/(36*(cos(THETA[0])*sin(THETA[2]) - cos(THETA[2])*sin(THETA[0]))), -(125*cos(THETA[0]))/(36*(cos(THETA[0])*sin(THETA[2]) - cos(THETA[2])*sin(THETA[0])))},
   {(125*sin(THETA[1]))/(36*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1]))), -(125*cos(THETA[1]))/(36*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1]))),  (125*sin(THETA[1]))/(36*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1]))), -(125*cos(THETA[1]))/(36*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1])))},
  };

  float theMatrix[4] = {};
  
  for( int i = 0; i<30; i++){

    F[0] = Ln+La*sin(THETA[0])+Lc*sin(THETA[2])-Lb*sin(THETA[1])-Ld*sin(THETA[3]);
    F[1] = Lb*cos(THETA[1])+Ld*cos(THETA[3])-La*cos(THETA[0])-Lc*cos(THETA[2]);
    F[2] = -x-La*sin(THETA[0])-Lc*sin(THETA[2]);
    F[3] = La*cos(THETA[0])+Lc*cos(THETA[2])-y;

    FINV[0][0] = 0;
    FINV[0][1] = 0;
    FINV[0][2] = -(125*sin(THETA[2]))/(24*(cos(THETA[0])*sin(THETA[2]) - cos(THETA[2])*sin(THETA[0])));
    FINV[0][3] = (125*cos(THETA[2]))/(24*(cos(THETA[0])*sin(THETA[2]) - cos(THETA[2])*sin(THETA[0])));
    FINV[1][0] = -(125*sin(THETA[3]))/(24*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1])));
    FINV[1][1] = (125*cos(THETA[3]))/(24*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1])));
    FINV[1][2] = -(125*sin(THETA[3]))/(24*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1])));
    FINV[1][3] = (125*cos(THETA[3]))/(24*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1])));
    FINV[2][0] = 0;
    FINV[2][1] = 0;
    FINV[2][2] = (125*sin(THETA[0]))/(36*(cos(THETA[0])*sin(THETA[2]) - cos(THETA[2])*sin(THETA[0])));
    FINV[2][3] = -(125*cos(THETA[0]))/(36*(cos(THETA[0])*sin(THETA[2]) - cos(THETA[2])*sin(THETA[0])));
    FINV[3][0] = (125*sin(THETA[1]))/(36*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1])));
    FINV[3][1] = -(125*cos(THETA[1]))/(36*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1])));
    FINV[3][2] = (125*sin(THETA[1]))/(36*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1])));
    FINV[3][3] = -(125*cos(THETA[1]))/(36*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1])));
    
    theMatrix[0] = 0;
    theMatrix[1] = 0;
    theMatrix[2] = 0;
    theMatrix[3] = 0;

    multiply(FINV, F, theMatrix);

    for( int j = 0; j<4; j++){
      THETA[j] = THETA[j] - theMatrix[j];
    }
  }

  THETA[0] = (atan2(sin(THETA[0]),cos(THETA[0])))*180/M_PI;
  THETA[1] = (atan2(sin(THETA[1]),cos(THETA[1])))*180/M_PI;
  THETA[2] = (atan2(sin(THETA[2]),cos(THETA[2])))*180/M_PI;
  THETA[3] = (atan2(sin(THETA[3]),cos(THETA[3])))*180/M_PI;

}
