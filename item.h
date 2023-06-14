#ifndef ITEM_H
#define ITEM_H

#include<QGraphicsObject>

class Item : public QGraphicsObject {
    Q_OBJECT

public:
    Item();
    Item(int x, int y, int which);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    double X, Y;
    int w, h;
    int mode;
    double down, dis;

public slots:
    void fall(double a);
    void slowfall();
    void release(int mode);

signals:

private:
    QPixmap item;
    QTimer *timer;

};

#endif // ITEM_H
