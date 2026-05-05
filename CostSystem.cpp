#include "CostSystem.h"
#include <QPainter>
#include <cmath>
#include <QPixmap>
#include <QDebug>
#include <QMouseEvent>

CostSystem::CostSystem()
{
    enemy_beats_addcost=10;
    time_addcost=20;
    Costs=0;
    cost_time_count=0;
}
