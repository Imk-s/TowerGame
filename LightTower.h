#ifndef LIGHTTOWER_H
#define LIGHTTOWER_H
#include "Tower.h"
#include <QPixmap>
#include <QPointF>

class LightTower :public Tower
{
public:
    LightTower(int x,int y);
    void Attack(const std::list<Enemy*>&EnemyList, std::list<FakeBullet>&BulletList) override;
    void draw(QPainter *painter);
    int timecount;
    int spawnrate;
    bool m_isAttacking = false;
    QPointF m_laserTarget;
private:
    QPixmap m_texture;
};
#endif // LIGHTTOWER_H
