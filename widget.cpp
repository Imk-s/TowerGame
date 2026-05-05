#include "widget.h"
#include "./ui_widget.h"
#include <QPainter>
#include <cmath>
#include <QPixmap>
#include <QDebug>
#include <QMouseEvent>
#include "Enemy.h"
#include "Monster.h"
#include "NormalTower.h"
#include "CostSystem.h"
#include "Map.h"
#include "Level.h"
#include "Hound.h"
#include "Tank.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    auto loadScaled = [](QPixmap &px, const QString &path, int w, int h) {
        QImage img(path);
        if (img.isNull()) qDebug() << "Failed to load:" << path;
        else px = QPixmap::fromImage(img.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    };
    loadScaled(m_grassTexture,   ":/assets/grass.png",        CELL_SIZE, CELL_SIZE);
    loadScaled(m_wallTexture,    ":/assets/wall.png",         CELL_SIZE, CELL_SIZE);
    loadScaled(m_spawnTexture,   ":/assets/spawnplace.png",   CELL_SIZE, CELL_SIZE);
    loadScaled(m_homeTexture,    ":/assets/home.png",         CELL_SIZE, CELL_SIZE);
    loadScaled(m_towerIconTexture,  ":/assets/NormalTower.png",  45, 45);
    loadScaled(m_cannonIconTexture, ":/assets/cannontower.png",  45, 45);
    loadScaled(m_lightIconTexture,  ":/assets/lighttower.png",   45, 45);

    QImage bgImg(":/assets/background.png");
    if (!bgImg.isNull()) m_menuBackground = QPixmap::fromImage(bgImg);

    Gametimer = new QTimer(this);
    connect(Gametimer, &QTimer::timeout, this, &Widget::Updategamelogic);
    Gametimer->start(16);

    m_towerShopButton = QRectF(10, 10, m_shopButtonSize, m_shopButtonSize);
    setMouseTracking(true);

    m_bgmPlayer = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_bgmPlayer->setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(0.3);
    // TODO: 替换为实际路径，注意使用正斜杠 '/'
    m_bgmPlayer->setSource(QUrl("qrc:/assets/bgm.mp3"));
    m_bgmPlayer->setLoops(QMediaPlayer::Infinite);
    m_bgmPlayer->play();
}

Widget::~Widget()
{
    delete current_level;
    qDeleteAll(EnemyList);
    qDeleteAll(TowerList);
    delete ui;
}

void Widget::bullet_move()
{
    if(BulletList.empty())return;
    auto it=BulletList.begin();
    while(it!=BulletList.end()){
    double dx=it->TargetPos.x()-it->currentPos.x();
    double dy=it->TargetPos.y()-it->currentPos.y();
    double dis=sqrt(dx*dx+dy*dy);
    if(dis<it->b_speed){
        it=BulletList.erase(it);
    }else{
        it->currentPos.setX(it->currentPos.x()+(dx/dis)*it->b_speed);
        it->currentPos.setY(it->currentPos.y()+(dy/dis)*it->b_speed);
        ++it;
    }
    }
}

void Widget::resetGame(int level)
{
    qDeleteAll(EnemyList);
    EnemyList.clear();
    qDeleteAll(TowerList);
    TowerList.clear();

    delete current_level;
    current_level = new Level(level);

    m_map = Map();
    m_map.loadMapData(current_level->l_gridmap);
    for (auto sp : current_level->startpoints)
        m_map.getWaypoints(sp, current_level->endpoint);

    costsystem = CostSystem();
    costsystem.Costs = current_level->initcosts;
    m_initHomehp = current_level->Homehp;

    spawnCounter = 0;
    spawnedCount = 0;
    iswin  = false;
    islose = false;
    m_buildState = None;
}

void Widget::Updategamelogic()
{
    if (m_gameState != Playing) {
        update();
        return;
    }

    if (iswin) { m_gameState = GameOver_Win;  update(); return; }
    if (islose){ m_gameState = GameOver_Lose; update(); return; }
    if(!current_level->spawnQueue.empty()){
        spawnCounter++;
        if(spawnCounter>=60){
            spawnedCount++;
            spawnCounter=0;      
            SpawnEvent event=current_level->spawnQueue.front();
            current_level->spawnQueue.pop();
            if(event.enemyID==1){
                Monster* newmonster=new Monster(m_map.mappath[event.startID]);
                EnemyList.push_back(newmonster);
            }
            else if(event.enemyID==2){
                Hound* newhound=new Hound(m_map.mappath[event.startID]);
                EnemyList.push_back(newhound);
            }
            else {
                Tank* newtank=new Tank(m_map.mappath[event.startID]);
                EnemyList.push_back(newtank);
            }
        }
    }
    costsystem.cost_time_count++;
    if(costsystem.cost_time_count>=60*2){
        costsystem.Costs+=costsystem.time_addcost;
        costsystem.cost_time_count=0;
    }
    if(current_level->spawnQueue.empty()&&EnemyList.empty()){
        iswin=true;
    }
    auto it=EnemyList.begin();
    while(!EnemyList.empty()&&it!=EnemyList.end()){
        if((*it)->isdead||(*it)->isarrive){
            if((*it)->isdead)costsystem.Costs+=costsystem.enemy_beats_addcost;
            if((*it)->isarrive){
                current_level->Homehp--;
                if(current_level->Homehp<=0){
                    current_level->Homehp=0;
                    islose=true;
                }
            }
            delete *it;
            it=EnemyList.erase(it);
        }else{
            (*it)->move(current_level->endpoint.x,current_level->endpoint.y);
            ++it;
        }
    }
    for(auto tower:TowerList){
        tower->Attack(EnemyList, BulletList);
    }
    bullet_move();
    update();
}

void Widget::drawButton(QPainter &painter, const QRectF &rect, const QString &text)
{
    painter.save();
    painter.setBrush(QColor(50, 50, 50, 200));
    painter.setPen(QPen(Qt::white, 2));
    painter.drawRoundedRect(rect, 8, 8);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(rect, Qt::AlignCenter, text);
    painter.restore();
}

void Widget::paintMainMenu(QPainter &painter)
{
    if (!m_menuBackground.isNull())
        painter.drawPixmap(rect(), m_menuBackground);
    else
        painter.fillRect(rect(), QColor(30, 30, 60));

    double cx = width() / 2.0;
    double cy = height() / 2.0;

    painter.save();
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 36, QFont::Bold));
    painter.drawText(QRectF(cx - 200, cy - 120, 400, 80), Qt::AlignCenter, "Tower Defense");
    painter.restore();

    m_btnStart = QRectF(cx - 100, cy, 200, 50);
    m_btnExit  = QRectF(cx - 100, cy + 70, 200, 50);
    drawButton(painter, m_btnStart, "Start Game");
    drawButton(painter, m_btnExit,  "Exit");
}

void Widget::paintPlaying(QPainter &painter)
{
    for (int i = 1; i <= MAP_ROWS; i++) {
        for (int j = 1; j <= MAP_COLS; j++) {
            double x = (j - 1) * CELL_SIZE;
            double y = (i - 1) * CELL_SIZE;
            int cell = m_map.m_gridmap[i][j];
            if (cell == 1)
                painter.drawPixmap(x, y, CELL_SIZE, CELL_SIZE, m_wallTexture);
            else if (cell == 3) {
                painter.drawPixmap(x, y, CELL_SIZE, CELL_SIZE, m_grassTexture);
                painter.drawPixmap(x, y, CELL_SIZE, CELL_SIZE, m_spawnTexture);
            } else if (cell == 4) {
                painter.drawPixmap(x, y, CELL_SIZE, CELL_SIZE, m_grassTexture);
                painter.drawPixmap(x, y, CELL_SIZE, CELL_SIZE, m_homeTexture);
            } else if (cell == 5) {
                painter.drawPixmap(x, y, CELL_SIZE, CELL_SIZE, m_wallTexture);
            } else {
                painter.drawPixmap(x, y, CELL_SIZE, CELL_SIZE, m_grassTexture);
            }
        }
    }

    for (auto enemy : EnemyList)
        enemy->draw(&painter);
    for (Tower* tower : TowerList)
        tower->draw(&painter);

    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(160, 160, 160));
    for (const auto &bullet : BulletList)
        painter.drawEllipse(bullet.currentPos, 10.0, 10.0);
    painter.restore();

    const double slotSize = CELL_SIZE;
    const double slotY    = 0;
    const double slotW    = 160.0;
    const double iconSize = 45.0;
    const double topRowH  = slotSize * 0.6;
    const double botRowH  = slotSize * 0.4;

    auto drawTowerSlot = [&](QRectF &btn, double slotX, const QPixmap &icon,
                              const QString &name, double cost, bool canAfford) {
        btn = QRectF(slotX, slotY, slotW, slotSize);
        QColor textColor = canAfford ? Qt::white : QColor(80, 80, 80, 200);
        painter.save();
        double iconY = slotY + (topRowH - iconSize) / 2.0;
        if (!icon.isNull())
            painter.drawPixmap(QPointF(slotX + 5, iconY), icon);
        QFont nameFont("SimHei", 16, QFont::Bold);
        painter.setFont(nameFont);
        painter.setPen(textColor);
        painter.drawText(QRectF(slotX + 55, slotY, slotW - 60, topRowH),
                         Qt::AlignVCenter | Qt::AlignLeft, name);
        QFont costFont("SimHei", 13);
        painter.setFont(costFont);
        painter.setPen(textColor);
        painter.drawText(QRectF(slotX + 5, slotY + topRowH, slotW - 10, botRowH),
                         Qt::AlignVCenter | Qt::AlignLeft, QString("费用:%1").arg((int)cost));
        painter.restore();
    };

    drawTowerSlot(m_towerShopButton,  0,        m_towerIconTexture,  "防御塔",
                  NormalTower(0,0).getCost(),  costsystem.Costs >= NormalTower(0,0).getCost());
    drawTowerSlot(m_cannonShopButton, slotW,    m_cannonIconTexture, "炮塔",
                  CannonTower(0,0).getCost(),  costsystem.Costs >= CannonTower(0,0).getCost());
    drawTowerSlot(m_lightShopButton,  slotW*2,  m_lightIconTexture,  "激光塔",
                  LightTower(0,0).getCost(),   costsystem.Costs >= LightTower(0,0).getCost());

    painter.save();
    QFont infoFont("SimHei", 32, QFont::Bold);
    painter.setFont(infoFont);
    painter.setPen(Qt::red);
    int remaining = (current_level->MAX_enemies - spawnedCount) + (int)EnemyList.size();
    if (remaining < 0) remaining = 0;
    QString enemyStr = QString("剩余敌人：%1/%2").arg(remaining).arg(current_level->MAX_enemies);
    QString hpStr    = QString("生命值：%1/%2").arg(current_level->Homehp).arg(m_initHomehp);
    painter.drawText(QRectF(width() - 980, slotY, 480, slotSize), Qt::AlignVCenter | Qt::AlignLeft, enemyStr);
    painter.drawText(QRectF(width() - 560, slotY, 250, slotSize), Qt::AlignVCenter | Qt::AlignLeft, hpStr);
    painter.restore();

    painter.save();
    QFont costFont("SimHei", 32, QFont::Bold);
    painter.setFont(costFont);
    painter.setPen(Qt::yellow);
    QString costStr = QString("费用：%1").arg((int)costsystem.Costs);
    painter.drawText(QRectF(width() - 300, slotY, 295, slotSize), Qt::AlignVCenter | Qt::AlignRight, costStr);
    painter.restore();

    // if (m_buildState == PreparingBuild) {
    //     painter.save();
    //     painter.setBrush(Qt::NoBrush);
    //     painter.setPen(QPen(Qt::yellow, 1, Qt::DashLine));
    //     painter.drawEllipse(m_mousePos, NormalTower(0,0).getRange(), NormalTower(0,0).getRange());
    //     painter.setBrush(QColor(100, 100, 100, 100));
    //     painter.setPen(Qt::NoPen);
    //     painter.drawEllipse(m_mousePos, 20, 20);
    //     painter.restore();
    // }
    // 1. 普通防御塔的预览
    if (m_buildState == PreparingBuild) {
        painter.save();
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(Qt::yellow, 1, Qt::DashLine));
        // 获取普通塔的攻击范围
        painter.drawEllipse(m_mousePos, NormalTower(0,0).getRange(), NormalTower(0,0).getRange());
        painter.setBrush(QColor(100, 100, 100, 100));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(m_mousePos, 20, 20); // 塔身占位符
        painter.restore();
    }
    // 2. 炮塔的预览 (对应 m_buildState == PreparingBuildCannon)
    else if (m_buildState == PreparingBuildCannon) {
        painter.save();
        painter.setBrush(Qt::NoBrush);
        // 炮塔可以换个颜色的虚线，比如红色，方便区分
        painter.setPen(QPen(Qt::red, 1, Qt::DashLine));
        painter.drawEllipse(m_mousePos, CannonTower(0,0).getRange(), CannonTower(0,0).getRange());
        painter.setBrush(QColor(100, 100, 100, 100));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(m_mousePos, 20, 20);
        painter.restore();
    }
    // 3. 激光塔的预览 (对应 m_buildState == PreparingBuildLight)
    else if (m_buildState == PreparingBuildLight) {
        painter.save();
        painter.setBrush(Qt::NoBrush);
        // 激光塔可以用蓝色的虚线
        painter.setPen(QPen(Qt::blue, 1, Qt::DashLine));
        painter.drawEllipse(m_mousePos, LightTower(0,0).getRange(), LightTower(0,0).getRange());
        painter.setBrush(QColor(100, 100, 100, 100));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(m_mousePos, 20, 20);
        painter.restore();
    }

}

void Widget::paintGameOverWin(QPainter &painter)
{
    paintPlaying(painter);

    painter.fillRect(rect(), QColor(0, 0, 0, 140));

    double cx = width() / 2.0;
    double cy = height() / 2.0;

    painter.save();
    painter.setPen(QColor(255, 215, 0));
    painter.setFont(QFont("Arial", 48, QFont::Bold));
    painter.drawText(QRectF(cx - 200, cy - 100, 400, 100), Qt::AlignCenter, "YOU WIN!");
    painter.restore();

    if (current_level && current_level->levelID < 5) {
        m_btnNextLevel   = QRectF(cx - 110, cy + 30,  220, 50);
        m_btnMenuFromWin = QRectF(cx - 110, cy + 100, 220, 50);
        drawButton(painter, m_btnNextLevel, "Next Level");
        drawButton(painter, m_btnMenuFromWin, "Return to Menu");
    } else {
        m_btnMenuFromWin = QRectF(cx - 110, cy + 30, 220, 50);
        drawButton(painter, m_btnMenuFromWin, "Return to Menu");
    }
}

void Widget::paintGameOverLose(QPainter &painter)
{
    paintPlaying(painter);

    painter.fillRect(rect(), QColor(0, 0, 0, 140));

    double cx = width() / 2.0;
    double cy = height() / 2.0;

    painter.save();
    painter.setPen(Qt::red);
    painter.setFont(QFont("Arial", 48, QFont::Bold));
    painter.drawText(QRectF(cx - 200, cy - 100, 400, 100), Qt::AlignCenter, "GAME OVER");
    painter.restore();

    m_btnRetry        = QRectF(cx - 110, cy + 30,  220, 50);
    m_btnMenuFromLose = QRectF(cx - 110, cy + 100, 220, 50);
    drawButton(painter, m_btnRetry,        "Retry");
    drawButton(painter, m_btnMenuFromLose, "Return to Menu");
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    switch (m_gameState) {
    case MainMenu:     paintMainMenu(painter);     break;
    case Playing:      paintPlaying(painter);      break;
    case GameOver_Win: paintGameOverWin(painter);  break;
    case GameOver_Lose:paintGameOverLose(painter); break;
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    switch (m_gameState) {
    case MainMenu:
        if (m_btnStart.contains(event->pos())) {
            resetGame();
            m_gameState = Playing;
        } else if (m_btnExit.contains(event->pos())) {
            this->close();
        }
        break;

    case Playing:
        if (m_towerShopButton.contains(event->pos())) {
            if (costsystem.Costs >= NormalTower(0, 0).getCost())
                m_buildState = PreparingBuild;
            return;
        }
        if (m_cannonShopButton.contains(event->pos())) {
            if (costsystem.Costs >= CannonTower(0, 0).getCost())
                m_buildState = PreparingBuildCannon;
            return;
        }
        if (m_lightShopButton.contains(event->pos())) {
            if (costsystem.Costs >= LightTower(0, 0).getCost())
                m_buildState = PreparingBuildLight;
            return;
        }
        if (m_buildState == PreparingBuild) {
            int gridX = event->pos().y() / CELL_SIZE + 1;
            int gridY = event->pos().x() / CELL_SIZE + 1;
            if (gridX >= 1 && gridX <= MAP_ROWS && gridY >= 1 && gridY <= MAP_COLS && m_map.m_gridmap[gridX][gridY] == 1) {
                Tower* newTower = new NormalTower(gridX, gridY);
                TowerList.push_back(newTower);
                m_map.m_gridmap[gridX][gridY] = 5;
                costsystem.Costs -= newTower->getCost();
            }
            m_buildState = None;
        } else if (m_buildState == PreparingBuildCannon) {
            int gridX = event->pos().y() / CELL_SIZE + 1;
            int gridY = event->pos().x() / CELL_SIZE + 1;
            if (gridX >= 1 && gridX <= MAP_ROWS && gridY >= 1 && gridY <= MAP_COLS && m_map.m_gridmap[gridX][gridY] == 1) {
                Tower* newTower = new CannonTower(gridX, gridY);
                TowerList.push_back(newTower);
                m_map.m_gridmap[gridX][gridY] = 5;
                costsystem.Costs -= newTower->getCost();
            }
            m_buildState = None;
        } else if (m_buildState == PreparingBuildLight) {
            int gridX = event->pos().y() / CELL_SIZE + 1;
            int gridY = event->pos().x() / CELL_SIZE + 1;
            if (gridX >= 1 && gridX <= MAP_ROWS && gridY >= 1 && gridY <= MAP_COLS && m_map.m_gridmap[gridX][gridY] == 1) {
                Tower* newTower = new LightTower(gridX, gridY);
                TowerList.push_back(newTower);
                m_map.m_gridmap[gridX][gridY] = 5;
                costsystem.Costs -= newTower->getCost();
            }
            m_buildState = None;
        }
        break;

    case GameOver_Win:
        if (current_level && current_level->levelID < 5 && m_btnNextLevel.contains(event->pos())) {
            resetGame(current_level->levelID + 1);
            m_gameState = Playing;
        } else if (m_btnMenuFromWin.contains(event->pos())) {
            iswin = false;
            m_gameState = MainMenu;
        }
        break;

    case GameOver_Lose:
        if (m_btnRetry.contains(event->pos())) {
            resetGame(current_level ? current_level->levelID : 1);
            m_gameState = Playing;
        } else if (m_btnMenuFromLose.contains(event->pos())) {
            islose = false;
            m_gameState = MainMenu;
        }
        break;
    }

    QWidget::mousePressEvent(event);
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    // 只要不是 None 状态（即处于任何一种建塔状态），就实时追踪鼠标并刷新
    if (m_buildState != None) {
        m_mousePos = event->pos();
        update();
    }
    QWidget::mouseMoveEvent(event);
}
