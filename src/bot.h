#pragma once
#include "main.h"

//const B1P

enum MovesNum{
    NOTHING,
    B, P,
    ANY
};

typedef struct XO_StepPreset
{
    int arena[3][3];
    pos2d win_move;
    struct XO_StepPreset *AVariant;
    struct XO_StepPreset *BVariant;
}XO_StepPreset;


bool  compareCell(const int cell, const int template, const int bot_cell);
int   compareArena(const int arena[3][3], const int preset[3][3], const int bot_cell);
void  rotateArena(int arena[3][3], const int rotation);
pos2d botFindWin(const int arena[3][3], const int bot_cell);
pos2d botDefence(const int arena[3][3], const int bot_cell);
pos2d botStep(const int arena[3][3], const int bot_cell);