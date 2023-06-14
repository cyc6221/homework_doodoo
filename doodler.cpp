#include "doodler.h"
#include <unistd.h>
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QFrame>
#include <QPainterPath>
#include <QList>
#include <QObject>
#include <QTimer>

Doodler::Doodler() {
    setFlags(QGraphicsItem::ItemIsFocusable);;
    X = 200, Y = 600;
    mode = 0;
    heart = 3;
    speed = 1;
    score = 0;
    doo = QPixmap(":/images/doodleL.png");
    w = doo.width(), h = doo.height();
    timer = new QTimer(this);
    timer->start(15);
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
}

Doodler::Doodler(int c) {
    setFlags(QGraphicsItem::ItemIsFocusable);;
    X = 200, Y = 600;
    //W = 2*x, H = 2*y;
    mode = c;
    heart = 3;
    speed = 1;
    score = 0;
    if (mode == 0) doo = QPixmap(":/images/doodleL.png");
    if (mode == 1) doo = QPixmap(":/images/doodle_pL.png");
    if (mode == 2) doo = QPixmap(":/images/doodle_jL.png");
    w = doo.width(), h = doo.height();

    timer = new QTimer(this);
    timer->start(15);
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));

    //timer_monster = new QTimer(this);
    //timer_monster->start(15);
}

QRectF Doodler::boundingRect() const {
    qreal penWidth = 1;
    return QRectF(X - penWidth / 2, Y - penWidth / 2, w + penWidth, h + penWidth);
}

void Doodler::paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(X,Y,w,h,doo);
}

void Doodler::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_Left:
        if(event->isAutoRepeat()) return;
        if (mode == 0) doo=QPixmap(":/images/doodleL.png");
        if (mode == 1) doo=QPixmap(":/images/doodle_pL.png");
        if (mode == 2) doo=QPixmap(":/images/doodle_jL.png");
        moveBy(-20,0);
        X-=20;
        break;

    case Qt::Key_Right:
        if(event->isAutoRepeat()) return;
        if (mode == 0) doo=QPixmap(":/images/doodleR.png");
        if (mode == 1) doo=QPixmap(":/images/doodle_pR.png");
        if (mode == 2) doo=QPixmap(":/images/doodle_jR.png");
        moveBy(20,0);
        X+=20;
        break;
    }
}

QPainterPath Doodler::shape() const {
    QPainterPath path;
    path.addRect(X+w/3, Y+h-10, w/3, 10);
    return path;
}

void Doodler::move() {
    this->setFocus();
    score = totalH;
    moveBy(0, speed);
    Y += speed;
    speed += 0.02;

    if (Y > 750) {
        emit ggwp();
    }
    if (speed > 0){
        if(mode != 0) {
            mode = 0;
            doo = QPixmap(":/images/doodleL.png");
        }

        collisions = collidingItems();
        foreach (QGraphicsItem *collidingItem, collisions) {
            if(this->scene() == NULL || collidingItem->scene() == NULL) continue;
            //***monster part***//
            if(collidingItem->data(19) == "monsterGr") {
                emit hurt(heart);
                emit touch_monster(1);
                heart--;
                if(heart == 0) emit ggwp();
                break;
            }
            if(collidingItem->data(28) == "monsterFly") {
                emit hurt(heart);
                emit touch_monster(2);
                heart--;
                if(heart == 0) emit ggwp();
                break;
            }
            if(collidingItem->data(37) == "monsterDb") {
                emit hurt(heart);
                emit touch_monster(3);
                heart--;
                if(heart == 0) emit ggwp();
                break;
            }
            //***monster part***//
            //***platform part***//
            if(collidingItem->data(336) == "platB" && speed > 0) {
                //broken platform
                emit touch_broken_plat();
                //break;
            }
            if(collidingItem->data(234) == "platBL" && speed > 0) {
                //moving platform
                speed = -gravity;
                break;
            }
            if(collidingItem->data(485) == "platN" && speed > 0) {
                //invisible platform
                speed = -gravity;
                collidingItem->hide();
                break;
            }
            if(collidingItem->data(196) == "plat" && speed > 0) {
                //normal platform
                speed = -gravity;
                break;
            }
            //***platform part***//
            //***item part***//
            if(collidingItem->data(1357) == "spring" && speed > 0) {
                speed = -gravity * 2;
                emit touchitem(79);
                break;
            }
            if(collidingItem->data(2468) == "trampoline" && speed > 0) {
                speed = -gravity * 3;
                emit touchitem(89);
                break;
            }
            if(collidingItem->data(3579) == "propeller" && speed > 0) {
                speed = -gravity * 4;
                emit touchitem(99);
                doo = QPixmap(":/images/doodle_pL.png");
                mode = 1;
                break;
            }
            if(collidingItem->data(4680) == "jetpack" && speed > 0) {
                speed = -gravity * 5;
                emit touchitem(107);
                doo = QPixmap(":/images/doodle_jL.png");
                mode = 2;
                break;
            }
            //***item part***//
        }
    }

    if (Y < 500 && speed < 0) {
        double dis = -speed * 25;
        speed = 0;
        moveBy(0, 490-Y);
        Y += 490 - Y;

        totalH += dis * 3;
        stop_timer();
        emit view_down(dis);
    }

}

void Doodler::mid_fall() {
    if(speed == 0) {
        speed += 0.02;
        timer->start(15);
    }
}

void Doodler::startTimer(int interval) {
    timer->start(interval);
}

void Doodler::stop_timer() {
    timer->stop();
}
