#include "platform.h"
#include "random.h"
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <QFrame>
#include <QPainterPath>
#include <QList>
#include <QObject>
#include <unistd.h>

Platform::Platform() {
    X = 500, Y = 500;
    mode = 1;
    dir = 1;
    setData(196,"plat");
    plat=QPixmap(":/images/platform.png");
    h = plat.height(), w = plat.width();
    timer = new QTimer(this);
    timer->start(10);
}

Platform::Platform(int a, int b, int c) {
    X = a, Y = b;
    mode = c;
    dir = 1;
    down = Y;

    timer = new QTimer(this);
    timer->start(10);

    if(mode == 33) {
        plat=QPixmap(":/images/platformBL.png");
        setData(234, "platBL");
        connect(timer, SIGNAL(timeout()), this, SLOT(platform_move()));
    }
    else if(mode % 97 == 0) {
        plat=QPixmap(":/images/platformB.png");
        setData(336, "platB");
    }
    else if(mode % 101 == 0) {
        plat=QPixmap(":/images/platformN.png");
        setData(485, "platN");
    }
    else {
        plat=QPixmap(":/images/platform.png");
        setData(196, "plat");
    }
    h = plat.height(), w = plat.width();
}

QRectF Platform::boundingRect() const {
    qreal penWidth = 1;
    return QRectF(X - penWidth / 2, Y - penWidth / 2, w + penWidth, h + penWidth);
}

void Platform::paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(X, Y, w, h, plat);
}

void Platform::fall(double a){
    if(a == 0) return;
    down = this->Y+a;
    dis = a;
    timer->start(a/10);
    connect(timer, SIGNAL(timeout()), this, SLOT(slowfall()));
}

void Platform::slowfall() {
    if(Y > down) {
        disconnect(timer, SIGNAL(timeout()), this, SLOT(slowfall()));
        emit view_down_over();
        return;
    }
    moveBy(0, this->dis/50);
    Y += this->dis/50;
    if(this->mode == 888) {
        if(Y - temp > 30) delete this;
        else if(Y - temp > 20) plat = QPixmap(":/images/platformB4.png");
        else if(Y - temp > 10) plat = QPixmap(":/images/platformB3.png");
        else if(Y - temp > 0) plat = QPixmap(":/images/platformB2.png");
    }
}

void Platform::platform_move() {
    X += dir*2;
    this->setPos(X, 0);
    if(X >= 220) dir = -1;
    if(X <= 0) dir = 1;
}

void Platform::broken() {
    temp = Y;
    down = Y + 100;
    dis = 100;
    timer->start(dis/10);
    mode = 888;
    connect(timer, SIGNAL(timeout()), this, SLOT(slowfall()));
}
