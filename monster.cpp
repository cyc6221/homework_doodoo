#include "monster.h"
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <QFrame>
#include <QPainterPath>
#include <QList>
#include <QObject>
#include <unistd.h>

Monster::Monster() {
    X = 500, Y = 500;
    mode = 1;
    setData(19,"monsterGr");
    monster = QPixmap(":/images/monster1.png");
    h = monster.height(), w = monster.width();
    timer = new QTimer(this);
    timer->start(10);
}

Monster::Monster(int a, int b, int c) {
    X = a, Y = b;
    mode = c;
    timer = new QTimer(this);
    timer->start(10);
    if(mode == 1) {
        setData(19, "monsterGr");
        monster = QPixmap(":/images/monster1.png");
        h = monster.height(), w = monster.width();
        X -= w/5, Y -= h;
    }
    else if(mode == 2) {
        setData(28, "monsterFly");
        monster = QPixmap(":/images/monster2.png");
        h = monster.height(), w = monster.width();
        X -= w/5, Y -= h;
    }
    else if(mode == 3) {
        setData(37, "monsterDb");
        monster = QPixmap(":/images/monster3.png");
        h = monster.height(), w = monster.width();
        X -= w/5, Y -= h;
    }
}

QRectF Monster::boundingRect() const {
    qreal penWidth = 1;
    return QRectF(X - penWidth / 2, Y - penWidth / 2, w + penWidth, h + penWidth);
}

void Monster::paint(QPainter *painter,
                     const QStyleOptionGraphicsItem *option,
                     QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(X, Y, w, h, monster);
}

void Monster::fall(double a){
    down = this->Y+a;
    dis = a;
    timer->start(a/10);
    connect(timer, SIGNAL(timeout()), this, SLOT(slowfall()));
}

void Monster::slowfall() {
    if(Y > down) {
        disconnect(timer, SIGNAL(timeout()), this, SLOT(slowfall()));
        return;
    }
    moveBy(0, this->dis/50);
    Y += this->dis/50;
}

void Monster::touch(int index) {
    if(mode == index) delete this;
}
