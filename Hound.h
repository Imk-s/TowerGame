#ifndef HOUND_H
#define HOUND_H
#include "Enemy.h"

class Hound :public Enemy
{
public:
    Hound(std::vector<QPointF> path);
    void move(int row,int col);
    void show_rest_hp() {}
    void showhp(QPainter *painter);
    void draw(QPainter*painter);
private:
    QPixmap m_monsterTexture;
};
#endif // HOUND_H
