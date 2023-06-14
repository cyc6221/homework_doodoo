#ifndef BULLET_H
#define BULLET_H

#include<QGraphicsObject>

class Bullet : public QGraphicsObject {
    Q_OBJECT

public:
    Bullet();
    Bullet(int mouseX, int mouseY, int posX, int posY);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    double X, Y;
    int w, h;
    double dirX, dirY;

public slots:
    void fly();

signals:

private:
    QPixmap bullet;
    QTimer *timer;

};

#endif // BULLET_H
