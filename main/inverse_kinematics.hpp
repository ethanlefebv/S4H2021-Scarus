#ifndef inverse_kinematics_h
#define inverse_kinematics_h

const float HOMEANGLES[4] = {65.2, -65.2, -70.17, 70.17};

void inverse_kinematics(float x /*meters*/, float y /*meters*/, float THETA[4] /*degrees*/);

#endif
