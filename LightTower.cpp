#include "LightTower.h"
#include <QPainter>
LightTower::LightTower(int x,int y):Tower(x,y)
{
    t_hp=100.0;
    t_attack=0.9;
    t_cost=160.0;
    t_range=220.0;
    t_firerate=1;
    grid_x=x;
    grid_y=y;
    timecount=0;
    spawnrate=1;
    
    m_fireSound = new QSoundEffect();
    // TODO: 替换为实际路径，注意使用正斜杠 '/'
    m_fireSound->setSource(QUrl("qrc:/assets/lightfire.wav"));
    m_fireSound->setVolume(0.5);

    QImage img(":/assets/lighttower.png");
    if (!img.isNull())
        m_texture = QPixmap::fromImage(img.scaled(GRID_SIZE, GRID_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void LightTower::Attack(const std::list<Enemy*>&EnemyList, std::list<FakeBullet>&BulletList)
{
    Enemy* Target=findminDistant(EnemyList);
    if(Target==NULL){
        m_isAttacking=false;
        return;
    }
    if (!m_isAttacking) {
        m_fireSound->play();
    }
    m_isAttacking=true;
    m_laserTarget=Target->currentPos;
    Target->hp-=t_attack;
    if(Target->hp<=0){
        Target->hp=0;
        Target->isdead=true;
    }
}

void LightTower::draw(QPainter *painter)
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
    if (m_isAttacking) {
        painter->setPen(QPen(Qt::yellow, 3));
        painter->drawLine(QPointF(px, py), m_laserTarget);
    }
    painter->restore();
}