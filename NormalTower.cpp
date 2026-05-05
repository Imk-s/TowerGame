#include "NormalTower.h"
#include <QPainter>
NormalTower::NormalTower(int x,int y):Tower(x,y)
{
    t_hp=100.0;
    t_attack=50.0;
    t_cost=100.0;
    t_range=220.0;
    t_firerate=1;
    grid_x=x;
    grid_y=y;
    timecount=0;
    spawnrate=1;
    
    m_fireSound = new QSoundEffect();
    // TODO: 替换为实际路径，注意使用正斜杠 '/'
    m_fireSound->setSource(QUrl("qrc:/assets/fire.wav"));
    m_fireSound->setVolume(0.5);

    QImage img(":/assets/NormalTower.png");
    if (!img.isNull())
        m_texture = QPixmap::fromImage(img.scaled(GRID_SIZE, GRID_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void NormalTower::Attack(const std::list<Enemy*>&EnemyList, std::list<FakeBullet>&BulletList)
{
    timecount++;
    if(timecount>=t_firerate*60){
        Enemy* Target=findminDistant(EnemyList);
        if(Target==NULL)return;
        Target->hp-=t_attack;
        double px = (grid_y - 1) * GRID_SIZE + GRID_SIZE/2.0;
        double py = (grid_x - 1) * GRID_SIZE + GRID_SIZE/2.0;
        FakeBullet bullet;
        bullet.currentPos=QPointF(px,py);
        bullet.TargetPos=Target->currentPos;
        bullet.b_speed=25.0;
        BulletList.push_back(bullet);
        
        m_fireSound->play();

        if(Target->hp<=0){
            Target->hp=0;
            Target->isdead=true;
        }
        timecount=0;
    }
}

void NormalTower::draw(QPainter *painter)
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
