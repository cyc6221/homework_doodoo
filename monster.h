#ifndef MONSTER_H
#define MONSTER_H

#include <QGraphicsObject>

class Monster : public QGraphicsObject {
    Q_OBJECT
public:
    Monster();
    Monster(int a, int b, int c);
    QRectF boundingRect() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    double X, Y;
    int w, h;
    int mode;
    double down, dis;

public slots:
    void fall(double a);
    void slowfall();
    void touch(int index);

signals:

private:
    QPixmap monster;
    QTimer *timer;
};

#endif // MONSTER_H
