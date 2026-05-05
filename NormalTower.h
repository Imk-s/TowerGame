#ifndef NORMALTOWER_H
#define NORMALTOWER_H
#include "Tower.h"
#include <QPixmap>

class NormalTower :public Tower
{
public:
    NormalTower(int x,int y);
    void Attack(const std::list<Enemy*>&EnemyList, std::list<FakeBullet>&BulletList) override;
    void draw(QPainter *painter);
    int timecount;
    int spawnrate;
private:
    QPixmap m_texture;
};

#endif // NORMALTOWER_H
