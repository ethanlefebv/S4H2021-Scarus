/* 
 * Auteur: Alec Gagnon     
 * date: Fevrier 2021
*/
/*------------------------------ Librairies ---------------------------------*/
#include "InverseCinematics.h";
#include <math.h>
/*------------------------- FUNCTIONS PROTOTYPES -------------------------*/
/*---------------------------- fonctions "Main" -----------------------------*/
void setup() {
  Serial.begin(9600);
  while(!Serial.available() ){} // juste faire attendre input pour starter le program
  int allo = Serial.read();

  float x = 0.096;
  float y = 0.462;  
  
  float THETA1 = 80.0;
  float THETA2 = 80.0;
  float THETA3 = -130.0;
  float THETA4 = 150.0;
  float THETA[4] = {THETA1*M_PI/180,THETA2*M_PI/180,THETA3*M_PI/180,THETA4*M_PI/180};
  
  inverseKinematics( x, y, THETA);

  Serial.println(THETA[0],4);
  Serial.println(THETA[1],4);
  Serial.println(THETA[2],4);
  Serial.println(THETA[3],4);

}
void loop() {
  delay(500);
}

/*---------------------------Definition de fonctions ------------------------*/
