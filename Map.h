#ifndef MAP_H
#define MAP_H
#include "QPointF"
#include <vector>

static const int MAP_ROWS = 18;
static const int MAP_COLS = 32;
static const double CELL_SIZE = 60.0;

struct gridpoint {
    int x, y;
    bool operator==(const gridpoint p) const {
        return x == p.x && y == p.y;
    }
};

class Map{
public:
    Map(){}
    void loadMapData(const int gridmap[MAP_ROWS+1][MAP_COLS+1]);
    void getWaypoints(gridpoint start,gridpoint end);
    std::vector<std::vector<QPointF>> mappath;
    int m_gridmap[MAP_ROWS+1][MAP_COLS+1]={};
    std::vector<gridpoint> gridPath;
    ~Map(){}
private:
    std::vector<QPointF> gridPathToWaypoints(const std::vector<gridpoint> &path);
    std::vector<gridpoint> BFS(gridpoint start, gridpoint end);
};

#endif // MAP_H
