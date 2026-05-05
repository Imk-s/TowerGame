#include "Enemy.h"
#include <queue>
#include <cstring>

Enemy::Enemy(std::vector<QPointF> path)
{

}
void Enemy::move(int row,int col)
{
    if (TargetIndex >= (int)waypoints.size()) return;

    QPointF targetPoint = waypoints[TargetIndex];
    double dx = targetPoint.x() - currentPos.x();
    double dy = targetPoint.y() - currentPos.y();
    double dist = std::sqrt(dx * dx + dy * dy);
    if (dist < speed) {
        currentPos = targetPoint;
        TargetIndex++;
    } else {
        currentPos.setX(currentPos.x() + (dx / dist) * speed);
        currentPos.setY(currentPos.y() + (dy / dist) * speed);
    }
}
