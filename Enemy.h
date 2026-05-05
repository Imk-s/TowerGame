#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include <utility>
#include <algorithm>
#include <list>
#include <QPoint>
#include <QPainter>
#include "Map.h"

class Enemy
{
public:
    Enemy(std::vector<QPointF> path);
    virtual void move(int row,int col);
    virtual void show_rest_hp() {}
    virtual void showhp(QPainter *painter){}
    virtual void draw (QPainter*painter) {}
    virtual ~Enemy() = default;
    bool isarrive = false;
    bool isdead = false;
    QPointF currentPos;
    double hp;
protected:
    double speed;
    int TargetIndex;
    std::vector<QPointF> waypoints;
};

#endif // ENEMY_H
