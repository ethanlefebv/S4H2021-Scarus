#ifndef NUT_H
#define NUT_H

const int INVALID_INPUT = 9999;

struct Coord
{
    float x = INVALID_INPUT;
    float y = INVALID_INPUT;
};

struct Nut
{
    Coord coord;
    int type = INVALID_INPUT;
    bool is_valid = false;
};

#endif
