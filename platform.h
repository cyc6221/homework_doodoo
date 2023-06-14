#ifndef PLATFORM_H
#define PLATFORM_H
#include <QGraphicsObject>

class Platform : public QGraphicsObject {
Q_OBJECT
public:
    Platform();
    Platform(int a, int b, int c);
    QRectF boundingRect() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    double X, Y;
    int mode;
    double dir;
    int w, h;
    double down, dis, temp;

public slots:
    void platform_move();
    void slowfall();
    void fall(double a);
    void broken();


signals:
    void outview();
    void view_down_over();

private:
    QPixmap plat;
    QTimer *timer;
};

#endif // PLATFORM_H
