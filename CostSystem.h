#ifndef COSTSYSTEM_H
#define COSTSYSTEM_H
#include "Enemy.h"
#include "Map.h"
#include "Tower.h"

class CostSystem{
public:
    CostSystem();
    int enemy_beats_addcost;
    int time_addcost;
    int Costs;
    int cost_time_count;
    ~CostSystem(){}
};
#endif // COSTSYSTEM_H
