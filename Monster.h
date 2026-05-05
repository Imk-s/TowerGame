#ifndef MONSTER_H
#define MONSTER_H
#include "Enemy.h"

class Monster :public Enemy
{
public:
    Monster(std::vector<QPointF> path);
    void move(int row,int col);
    void show_rest_hp() {}
    void showhp(QPainter *painter);
    void draw(QPainter*painter);
private:
    QPixmap m_monsterTexture;
};
#endif // MONSTER_H
