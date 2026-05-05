#include "Map.h"
#include <queue>
#include <cstring>

static gridpoint pre[MAP_ROWS+1][MAP_COLS+1];
static bool vis[MAP_ROWS+1][MAP_COLS+1] = {};
static int dx[4] = {1, -1, 0, 0};
static int dy[4] = {0, 0, 1, -1};

void Map::loadMapData(const int gridmap[MAP_ROWS+1][MAP_COLS+1])
{
    for(int i=1;i<=MAP_ROWS;i++)
        for(int j=1;j<=MAP_COLS;j++)
            m_gridmap[i][j]=gridmap[i][j];
}

void Map::getWaypoints(gridpoint start,gridpoint end)
{
    gridPath = BFS(start,end);
    mappath.push_back(gridPathToWaypoints(gridPath));
}
std::vector<gridpoint> Map::BFS(gridpoint start, gridpoint end)
{
    bool found = false;
    memset(vis, false, sizeof(vis));
    std::queue<gridpoint> q;
    q.push(start);
    vis[start.x][start.y] = true;
    while (!q.empty()) {
        gridpoint curpoint = q.front();
        q.pop();
        int cur_x = curpoint.x;
        int cur_y = curpoint.y;
        for (int i = 0; i < 4; i++) {
            int tx = cur_x + dx[i];
            int ty = cur_y + dy[i];
            if (tx >= 1 && tx <= MAP_ROWS && ty >= 1 && ty <= MAP_COLS
                && !vis[tx][ty] && m_gridmap[tx][ty] != 1)
            {
                vis[tx][ty] = true;
                pre[tx][ty] = curpoint;
                if (tx == end.x && ty == end.y) {
                    found = true;
                    break;
                }
                q.push({tx, ty});
            }
        }
        if (found) break;
    }

    std::vector<gridpoint> path;
    if (!found) return path;

    gridpoint cur = end;
    while (!(cur == start)) {
        path.push_back(cur);
        cur = pre[cur.x][cur.y];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<QPointF> Map::gridPathToWaypoints(const std::vector<gridpoint> &path)
{
    std::vector<QPointF> result;
    for (const gridpoint &gp : path) {
        double px = (gp.y - 1) * CELL_SIZE + CELL_SIZE / 2.0;
        double py = (gp.x - 1) * CELL_SIZE + CELL_SIZE / 2.0;
        result.push_back(QPointF(px, py));
    }
    return result;
}