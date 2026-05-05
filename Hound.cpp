#include "Hound.h"
#include <QDebug>
const double GRID_SIZE=60.0;
Hound::Hound(std::vector<QPointF> path):Enemy(path)
{
    this->waypoints=path;
    this->hp=100.0;
    this->speed=2.5;
    this->TargetIndex=waypoints.size()>=2?1:0;
    this->currentPos=waypoints.size()>=2?waypoints[0]:QPointF(0, 0);
    QImage img(":/assets/hound.png");
    if (img.isNull()) qDebug() << "Failed to load hound texture!";
    else m_monsterTexture = QPixmap::fromImage(img.scaled(35, 35, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Hound::draw(QPainter*painter)
{
    if (m_monsterTexture.isNull()) {
        // 贴图加载失败，绘制一个蓝色圆形作为备用
        painter->setBrush(Qt::blue);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(currentPos, 8.0, 8.0);
    } else {
        // 计算左上角坐标，使图片中心对齐 currentPos
        const QPointF offset(m_monsterTexture.width() / 2.0, m_monsterTexture.height() / 2.0);
        painter->drawPixmap(currentPos - offset, m_monsterTexture);
    }
    showhp(painter);
}

void Hound::showhp(QPainter *painter)
{
    // 血条总宽度
    const int barWidth = 35;
    // 血条高度
    const int barHeight = 5;
    // 血条y轴偏移量
    const int yOffset = 20;

    // 计算血条位置
    QPointF barPos = currentPos - QPointF(barWidth / 2.0, yOffset);

    double hpRatio = hp / 100.0;
    double redWidth = barWidth * hpRatio;

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    painter->drawRect(QRectF(barPos, QSizeF(redWidth, barHeight)));

    painter->setBrush(Qt::gray);
    painter->drawRect(QRectF(barPos + QPointF(redWidth, 0), QSizeF(barWidth - redWidth, barHeight)));
}

void Hound::move(int row,int col)
{
    if (TargetIndex >= (int)waypoints.size()) {
        isarrive = true;   // 走完路径就到达
        return;
    }
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
    if((currentPos.x()==(row- 1) * GRID_SIZE + GRID_SIZE/2.0)&&(currentPos.y()==(col- 1) * GRID_SIZE + GRID_SIZE/2.0))isarrive=true;
}