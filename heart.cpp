#include "heart.h"
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <QFrame>
#include <QPainterPath>
#include <QList>
#include <QObject>
#include <unistd.h>

Heart::Heart() {
    heart = QPixmap(":/images/heart.png");
    index = 1;
    X = 0, Y = 10 * index;
    h = heart.height(), w = heart.width();
}

Heart::Heart(int i) {
    heart = QPixmap(":/images/heart.png");
    index = i;
    X = 50 + index * 30, Y = 0;
    h = heart.height(), w = heart.width();
}

QRectF Heart::boundingRect() const {
    qreal penWidth = 1;
    return QRectF(X - penWidth / 2, Y - penWidth / 2, 30 + penWidth, 30 + penWidth);
}

void Heart::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(X, Y, 30, 30, heart);
}

void Heart::hurtHP(int i) {
    if(index == i) delete this;
}
