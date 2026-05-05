#ifndef TANK_H
#define TANK_H
#include "Enemy.h"

class Tank :public Enemy
{
public:
    Tank(std::vector<QPointF> path);
    void move(int row,int col);
    void show_rest_hp() {}
    void showhp(QPainter *painter);
    void draw(QPainter*painter);
private:
    QPixmap m_monsterTexture;
};
#endif // TANK_H
