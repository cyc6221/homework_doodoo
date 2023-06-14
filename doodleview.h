#ifndef DOODLEVIEW_H
#define DOODLEVIEW_H

//#include <QGraphicsView>
#include <QTimer>
#include "doodler.h"

class DoodleView : public QGraphicsView {
Q_OBJECT
public:
    explicit DoodleView(QWidget *parent = 0);

public slots:
    void start_game();
    void game_over();
    void back_to_menu();
    void pause_game();
    void return_game();
    void addNew(double a);
    void update_score();
    void out_view();
    //void create_bullet(double mouse_x, double mouse_y, double doo_x, double doo_y);

private:
    Doodler *doodoo;
    QPixmap bg = QPixmap(":/images/bg.png");
    int h = bg.height(), w = bg.width();
    int initbottom = 2*h;
    int SCORE;
    int ran = 69;

    //text
    QGraphicsTextItem *game_score;
    QGraphicsTextItem *welcome_text;
    QGraphicsTextItem *pause_text;
    QGraphicsTextItem *game_over_text;

    //buttons
    QGraphicsWidget *start_button;
    QGraphicsWidget *exit_button;
    QGraphicsWidget *pause_button;   
    QGraphicsWidget *return_button;    
    QGraphicsWidget *backmenu_button;

    //shade for special use
    QGraphicsWidget *shade_widget;

    void init_view();
    void init_game();

private:
    QTimer *timer2;

signals:

protected:
    //void mousePressEvent(QMouseEvent *event);

};

#endif // DOODLEVIEW_H
