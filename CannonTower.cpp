#include "CannonTower.h"
#include <QPainter>
#include<algorithm>
CannonTower::CannonTower(int x,int y):Tower(x,y)
{
    t_hp=100.0;
    t_attack=120.0;
    t_cost=200.0;
    t_range=220.0;
    t_firerate=2;
    grid_x=x;
    grid_y=y;
    timecount=0;
    spawnrate=1;
    
    m_fireSound = new QSoundEffect();
    // TODO: 替换为实际路径，注意使用正斜杠 '/'
    m_fireSound->setSource(QUrl("qrc:/assets/cannonfire.wav"));
    m_fireSound->setVolume(0.5);

    QImage img(":/assets/cannontower.png");
    if (!img.isNull())
        m_texture = QPixmap::fromImage(img.scaled(GRID_SIZE, GRID_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void CannonTower::Attack(const std::list<Enemy*>&EnemyList, std::list<FakeBullet>&BulletList)
{
    timecount++;
    if(timecount>=t_firerate*60){
        std::vector<std::pair<double,Enemy*>>targetEnemies;
        for(auto enemy:EnemyList){
            double curdis=calculate_distant(enemy);
            if(curdis<t_range){
                targetEnemies.push_back({curdis,enemy});
            }
        }
        sort(targetEnemies.begin(),targetEnemies.end());
        int attackcnt=std::min(3,(int)targetEnemies.size()); //最多aoe攻击3个
        double px = (grid_y - 1) * GRID_SIZE + GRID_SIZE/2.0;
        double py = (grid_x - 1) * GRID_SIZE + GRID_SIZE/2.0;

        for(int i=0;i<attackcnt;i++){
            FakeBullet bullet;
            Enemy* Target=targetEnemies[i].second;
            if(Target==NULL)return;
            Target->hp-=t_attack;
            if(Target->hp<=0){
                Target->hp=0;
                Target->isdead=true;
            }
            bullet.currentPos=QPointF(px,py);
            bullet.TargetPos=Target->currentPos;
            bullet.b_speed=25.0;
            BulletList.push_back(bullet);
        }
        if (attackcnt > 0) {
            m_fireSound->play();
        }
        timecount=0;
    }
}

void CannonTower::draw(QPainter *painter)
{
    painter->save();
    double px = (grid_y - 1) * GRID_SIZE + GRID_SIZE/2.0;
    double py = (grid_x - 1) * GRID_SIZE + GRID_SIZE/2.0;
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::yellow, 1, Qt::DashLine));
    painter->drawEllipse(QPointF(px, py), t_range, t_range);
    if (!m_texture.isNull()) {
        painter->drawPixmap((grid_y - 1) * GRID_SIZE, (grid_x - 1) * GRID_SIZE, m_texture);
    } else {
        painter->setBrush(Qt::green);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(QPointF(px, py), 15, 15);
    }
    painter->restore();
}