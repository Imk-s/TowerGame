#ifndef LEVEL_H
#define LEVEL_H
#include "./ui_widget.h"
#include <QPainter>
#include <cmath>
#include <QPixmap>
#include <QDebug>
#include <QMouseEvent>
#include "Enemy.h"
#include "Monster.h"
#include "NormalTower.h"
#include "CostSystem.h"
#include "Map.h"

struct SpawnEvent{
    int enemyID;
    int startID;
};

class Level{
public:
    int LevelNum;
    int levelID;
    Level(int ID);
    int initcosts;
    std::vector<gridpoint> startpoints;
    gridpoint endpoint;
    int Homehp;
    int MAX_enemies;
    int l_gridmap[MAP_ROWS+1][MAP_COLS+1];
    std::queue<SpawnEvent> spawnQueue;
};

#endif // LEVEL_H
