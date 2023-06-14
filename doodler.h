#ifndef DOODLER_H
#define DOODLER_H
#include <QGraphicsObject>
#include <QGraphicsView>

class Doodler : public QGraphicsObject {
Q_OBJECT
public:
    Doodler();
    Doodler(int c);
    QRectF boundingRect() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QPainterPath shape() const;
    double X, Y;
    int mode = 0;
    int heart;
    double speed;
    double totalH = 0;
    int score = 0;
    int w, h;

    void startTimer(int interval);
    void stop_timer();

    QList<QGraphicsItem*> collisions;

public slots:
    void move();
    void mid_fall();

signals:
    void view_down(double a);
    void ggwp();
    void touchitem(int mode);
    void hurt(int HP);
    void touch_monster(int mode);
    void touch_broken_plat();

private:
    double gravity = 4.5;
    QTimer *timer;
    QPixmap doo;


protected:
    void keyPressEvent(QKeyEvent *event);

};

#endif // DOODLER_H
