#include "bullet.h"
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <QFrame>
#include <QPainterPath>
#include <QList>
#include <QObject>
#include <unistd.h>

Bullet::Bullet() {
    bullet = QPixmap(":/images/bullet.png");
    X = 500, Y = 500;
    dirX = 0, dirY = -1;
    h = bullet.height(), w = bullet.width();
    setData(999, "bullet");
    timer = new QTimer(this);
    timer->start(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(fly()));
}

Bullet::Bullet(int mouseX, int mouseY, int posX, int posY) {
    bullet = QPixmap(":/images/bullet.png");
    X = posX, Y = posY;
    dirX = mouseX - posX;
    dirY = mouseY - posY;
    h = bullet.height(), w = bullet.width();
    setData(999, "bullet");
    timer = new QTimer(this);
    timer->start(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(fly()));
}

QRectF Bullet::boundingRect() const {
    qreal penWidth = 1;
    return QRectF(X - penWidth / 2, Y - penWidth / 2, w + penWidth, h + penWidth);
}

void Bullet::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(X, Y, w, h, bullet);
}

void Bullet::fly() {
    if(X > 0 && (Y > 0 || Y > 100)) {
        moveBy(dirX, dirY);
        X += dirX;
        Y += dirY;
    }
    else {
        delete this;
    }
}

