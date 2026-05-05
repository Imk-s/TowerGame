#ifndef CANNONTOWER_H
#define CANNONTOWER_H
#include "Tower.h"
#include <QPixmap>
class CannonTower :public Tower
{
public:
    CannonTower(int x,int y);
    void Attack(const std::list<Enemy*>&EnemyList, std::list<FakeBullet>&BulletList) override;
    void draw(QPainter *painter) override;
    int timecount;
    int spawnrate;
private:
    QPixmap m_texture;
};
#endif // CANNONTOWER_H
