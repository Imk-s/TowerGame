#ifndef TOWER_H
#define TOWER_H
#include<vector>
#include<QPointF>
#include<queue>
#include<utility>
#include<cmath>
#include<algorithm>
#include<list>
#include<QPainter>
#include<QSoundEffect>
#include"Enemy.h"
const int GRID_SIZE=60;

struct FakeBullet{
    QPointF currentPos;
    QPointF TargetPos;
    QPixmap texture;
    double b_speed;
};

class Tower
{
public:
    Tower(int x,int y);
    virtual double calculate_distant(const Enemy* enemy);
    virtual Enemy* findminDistant(const std::list<Enemy*>EnemyList);
    virtual void draw(QPainter *painter) {}
    virtual void Attack(const std::list<Enemy*>&EnemyList, std::list<FakeBullet>&BulletList){}
    virtual ~ Tower()=default;
    double getCost()  const { return t_cost; }
    double getRange() const { return t_range; }
protected:
    double t_hp;
    double t_cost;
    double t_attack;
    double t_range;
    int t_firerate;
    double pos_x;
    double pos_y;
    int grid_x;
    int grid_y;
    QSoundEffect* m_fireSound;
};

#endif // TOWER_H
