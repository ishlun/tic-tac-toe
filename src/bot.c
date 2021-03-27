#include "bot.h"

/*
 * Потом сделаю(с)
 */

//C - Center
//B - Bottom
//T - Top
//L - Left
//R - Right
//TL - Top Left
//TR - Top Right
//BL - Bottom Left
//BR - Bottom Right

/*Presets is bot is X*/
static XO_StepPreset B1C_P1B_B2BR_PANY = {
    {
        {ANY, ANY, NOTHING},
        {ANY, B, ANY},
        {ANY, P, B},
    },
    (pos2d){2, 0},
    NULL,
    NULL};
static XO_StepPreset B1C_P1B = {
    {
        {NOTHING, NOTHING, NOTHING},
        {NOTHING, B, NOTHING},
        {NOTHING, P, NOTHING},
    },
    (pos2d){2, 2},
    &B1C_P1B_B2BR_PANY,
    NULL};
static XO_StepPreset B1C_P1BR_B2TL = {
    {
        {B, ANY, NOTHING},
        {ANY, B, ANY},
        {ANY, ANY, P},
    },
    (pos2d){2, 0},
    NULL,
    NULL};
static XO_StepPreset B1C_P1BR = {
    {
        {NOTHING, NOTHING, NOTHING},
        {NOTHING, B, NOTHING},
        {NOTHING, NOTHING, P},
    },
    (pos2d){0, 0},
    &B1C_P1BR_B2TL,
    NULL};
static XO_StepPreset B1C = {
    {
        {NOTHING, NOTHING, NOTHING},
        {NOTHING, NOTHING, NOTHING},
        {NOTHING, NOTHING, NOTHING},
    },
    (pos2d){1, 1},
    &B1C_P1B,
    &B1C_P1BR};

bool compareCell(const int cell, const int template, const int bot_cell)
{
    if (template == ANY)
        return true;

    if (template == NOTHING)
    {
        if (cell == VOID_CELL)
            return true;
        else
            return false;
    }

    if (template == B)
    {
        if (bot_cell == cell)
            return true;
        else
            return false;
    }
    if (template == P)
    {
        if (bot_cell == cell)
            return false;
        else
            return true;
    }
}
int compareArena(const int arena[3][3], const int preset[3][3], const int bot_cell)
{
    int same_cnt = 0;
    int arena_copy[3][3];
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            arena_copy[i][j] = arena[i][j];
        }
    }

    for (int rotation = 0; rotation < 4; rotation++)
    {

        rotateArena(arena_copy, rotation);
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (compareCell(arena_copy[i][j], preset[i][j], bot_cell))
                    ++same_cnt;
            }
        }
        if (same_cnt == 9)
            return rotation;
        else
            same_cnt = 0;
    }

    return -1;
}
void rotateArena(int arena[3][3], int rotation)
{
    rotation = rotation < 0 ? abs(4 + rotation) : rotation;
    /*TODO: ПАЧИНИ ЭТУ ЗАЛУПУ БЛЯТЬ ПИЗДЕЦ СУКА НАХУЙ БЛЯТЬ*/
    for (int i = 1; i <= rotation; i++)
    {
        int t1, t2, t3, t4, t5, t6, t7, t8;
        t1 = arena[0][0];
        t2 = arena[2][0];
        t3 = arena[2][2];
        t4 = arena[0][2];
        arena[0][0] = t2;
        arena[2][0] = t3;
        arena[2][2] = t4;
        arena[0][2] = t1;

        t1 = arena[1][0];
        t2 = arena[2][1];
        t3 = arena[1][2];
        t4 = arena[0][1];
        arena[1][0] = t2;
        arena[2][1] = t3;
        arena[1][2] = t4;
        arena[0][1] = t1;
    }
}
pos2d botStep(const int arena[3][3], const int bot_cell)
{
    while (1)
    {
        int x = rand() % 3;
        int y = rand() % 3;
        if (arena[x][y] == VOID_CELL)
            return (pos2d){x, y};
    }
}
