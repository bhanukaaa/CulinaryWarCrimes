#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <thread>
#include <queue>
#include <iostream>

void cameraMovement(short, short);
void place(short, short);
void blockPlacement();
void boxPlacement();
void blockDeletion();
void npcPlacement();
void npcDeletion();
void objectPlacement(int&);
void objectDeletion();
void selectionChange(short&, int&);
void drawMainUI(short&, int&);

#endif