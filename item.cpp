#include "item.h"
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <QFrame>
#include <QPainterPath>
#include <QList>
#include <QObject>
#include <unistd.h>

Item::Item() {
    item = QPixmap(":/images/spring1.png");
    X = 500, Y = 500;
    h = item.height(), w = item.width();
    mode = 0;
    setData(1357, "spring");
    timer = new QTimer(this);
    timer->start(10);
}

Item::Item(int a, int b, int c) {
    mode = c;
    timer = new QTimer(this);
    timer->start(10);
    if(mode % 79 == 0) {
        item = QPixmap(":/images/spring1.png");
        h = item.height(), w = item.width();
        X = a+1, Y = b-h;
        setData(1357, "spring");
    }
    else if(mode % 89 == 0) {
        item = QPixmap(":/images/trampoline1.png");
        h = item.height(), w = item.width();
        X = a+w/3, Y = b-h;
        setData(2468, "trampoline");
    }
    else if(mode % 99 == 0) {
        item = QPixmap(":/images/propeller.png");
        h = item.height(), w = item.width();
        X = a+w/3, Y = b-h;
        setData(3579, "propeller");
    }
    else if(mode % 107 == 0) {
        item = QPixmap(":/images/jetpack.png");
        h = item.height(), w = item.width();
        X = a+w*3/4, Y = b-h;
        setData(4680, "jetpack");
    }
    else {
        return;
    }
}

QRectF Item::boundingRect() const {
    qreal penWidth = 1;
    return QRectF(X - penWidth / 2, Y - penWidth / 2, w + penWidth, h + penWidth);
}

void Item::paint(QPainter *painter,
                     const QStyleOptionGraphicsItem *option,
                     QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(X, Y, w, h, item);
}

void Item::fall(double a){
    down = this->Y+a;
    dis = a;
    timer->start(a/10);
    connect(timer, SIGNAL(timeout()), this, SLOT(slowfall()));
}

void Item::slowfall() {
    if(Y > down) {
        disconnect(timer, SIGNAL(timeout()), this, SLOT(slowfall()));
        return;
    }
    moveBy(0, this->dis/50);
    Y += this->dis/50;
}

void Item::release(int x) {
    if(this->mode == x && x == 79) {
        this->Y += h;
        item = QPixmap(":/images/spring2.png");
        h = item.height(), w = item.width();
        this->Y = this->Y-h;
    }
    else if(this->mode == x && x == 89) {
        item = QPixmap(":/images/trampoline2.png");
    }
    else if(this->mode == x && x == 99) {
        delete this;
    }
    else if(this->mode == x && x == 107) {
        delete this;
    }
}
