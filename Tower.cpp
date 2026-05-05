#include "Tower.h"

Tower::Tower(int x,int y)
{
    t_hp=100.0;
    t_attack=50.0;
    t_cost=10.0;
    t_range=400.0;
    t_firerate=1;
    grid_x=x;
    grid_y=y;
    pos_x = (y - 1) * GRID_SIZE + (GRID_SIZE / 2.0);
    pos_y = (x - 1) * GRID_SIZE + (GRID_SIZE / 2.0);
}

double Tower::calculate_distant(const Enemy* enemy)
{
    return sqrt((enemy->currentPos.x()-pos_x)*(enemy->currentPos.x()-pos_x)+
                      (enemy->currentPos.y()-pos_y)*(enemy->currentPos.y()-pos_y));
}

Enemy* Tower::findminDistant(const std::list<Enemy*>EnemyList)
{
    Enemy* Target=NULL;
    double minDis=t_range;
    for(auto enemy:EnemyList)
    {
        double currentDis=calculate_distant(enemy);
        if(currentDis<minDis){
            minDis=currentDis;
            Target=enemy;
        }
    }
    return Target;
}