#ifndef HEART_H
#define HEART_H

#include<QGraphicsObject>

class Heart : public QGraphicsObject {
    Q_OBJECT

public:
    Heart();
    Heart(int i);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    double X, Y;
    int w, h;
    int index;

public slots:
    void hurtHP(int i);

signals:

private:
    QPixmap heart;

};

#endif // HEART_H
