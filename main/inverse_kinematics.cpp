#include "inverse_kinematics.h"
#include <Arduino.h>

void multiply(float matrix1[4][4], float matrix2[4], float result[4])
{
    for (int j = 0; j < 4; j++)
    {
        for (int k = 0; k < 4; k++)
        {
            result[j] += matrix1[j][k] * matrix2[k];
        }
    }
}

void inverse_kinematics(float x, float y, float THETA[4])
{
    // Makes sure the input coordinates are reachable with the current arm segment lengths
    if ((x >= -0.24 && x <= 0.45) && (y >= -0.46 && y <= 0.46))
    {
        for (int i = 0; i < 4; i++)
        {
            THETA[i] = THETA[i] * M_PI / 180;
        }
        float Ln = 0.192;
        float La = 0.192;
        float Lb = 0.192;
        float Lc = 0.288;
        float Ld = 0.288;
    
        float F[4] = {
            Ln+La*sin(THETA[0])+Lc*sin(THETA[2])-Lb*sin(THETA[1])-Ld*sin(THETA[3]),
            Lb*cos(THETA[1])+Ld*cos(THETA[3])-La*cos(THETA[0])-Lc*cos(THETA[2]),
            -x-La*sin(THETA[0])-Lc*sin(THETA[2]),
            La*cos(THETA[0])+Lc*cos(THETA[2])-y};
        float FINV[4][4] = {
            {0,0,-(125*sin(THETA[2]))/(24*(cos(THETA[0])*sin(THETA[2]) - cos(THETA[2])*sin(THETA[0]))),(125*cos(THETA[2]))/(24*(cos(THETA[0])*sin(THETA[2]) - cos(THETA[2])*sin(THETA[0])))},
            {-(125*sin(THETA[3]))/(24*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1]))),(125*cos(THETA[3]))/(24*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1]))), -(125*sin(THETA[3]))/(24*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1]))),  (125*cos(THETA[3]))/(24*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1])))},
            {0,0,  (125*sin(THETA[0]))/(36*(cos(THETA[0])*sin(THETA[2]) - cos(THETA[2])*sin(THETA[0]))), -(125*cos(THETA[0]))/(36*(cos(THETA[0])*sin(THETA[2]) - cos(THETA[2])*sin(THETA[0])))},
            {(125*sin(THETA[1]))/(36*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1]))), -(125*cos(THETA[1]))/(36*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1]))),  (125*sin(THETA[1]))/(36*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1]))), -(125*cos(THETA[1]))/(36*(cos(THETA[1])*sin(THETA[3]) - cos(THETA[3])*sin(THETA[1])))}};
    
        float the_matrix[4] = {};
        for (int i = 0; i < 30; i++)
        {
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
    
            the_matrix[0] = 0;
            the_matrix[1] = 0;
            the_matrix[2] = 0;
            the_matrix[3] = 0;
    
            multiply(FINV, F, the_matrix);
    
            for (int j = 0; j < 4; j++)
            {
                THETA[j] = THETA[j] - the_matrix[j];
            }
        }
    
        THETA[0] = (atan2(sin(THETA[0]),cos(THETA[0])))*180/M_PI;
        THETA[1] = (atan2(sin(THETA[1]),cos(THETA[1])))*180/M_PI;
        THETA[2] = (atan2(sin(THETA[2]),cos(THETA[2])))*180/M_PI;
        THETA[3] = (atan2(sin(THETA[3]),cos(THETA[3])))*180/M_PI;

        // Makes sure there won't be a collision with the calculated angles
        // Danger zone = (-THETA[0] (Quadrant 3) and +THETA[1] (Quadrant 1))
        if (THETA[0]-THETA[1] < -48)
        {
            //Serial.println("Error: Collision avoided");
            for (int i = 0; i < sizeof(HOMEANGLES)/sizeof(HOMEANGLES[0]); ++i)
            {
                THETA[i] = HOMEANGLES[i];
            }
        }
    }
    else
    {
        //Serial.println("Error: Unattainable angles");
        for (int i = 0; i < sizeof(HOMEANGLES)/sizeof(HOMEANGLES[0]); ++i)
        {
            THETA[i] = HOMEANGLES[i];
        }
    }
}
