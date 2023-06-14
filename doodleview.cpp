#include "doodleview.h"
#include "doodler.h"
#include "platform.h"
#include "random.h"
#include "item.h"
#include "heart.h"
#include "monster.h"
#include "bullet.h"

#include <QIcon>
#include <QPropertyAnimation>
#include <QGraphicsBlurEffect>
#include <QTimer>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QApplication>
#include <QLabel>
#include <QFileInfo>
#include <QMouseEvent>

DoodleView::DoodleView(QWidget *parent) :
    QGraphicsView(parent) {
    init_view();
}

void DoodleView::init_view() {
    setWindowTitle("Doodle Jump");
    setWindowIcon(QIcon(":/images/doo.ico"));

    setFixedSize(w, h);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->setSceneRect(0, 0, w-5, h-5);
    scene->setBackgroundBrush(bg);
    setScene(scene);

    //text information: score of the game
    game_score = new QGraphicsTextItem();
    scene->addItem(game_score);
    game_score->setFont(QFont("Times", 20, QFont::Bold));
    game_score->setPos(0, 0);
    game_score->hide();

    //shade
    QWidget *shade = new QWidget;
    shade->setAutoFillBackground(true);
    shade->setPalette(QPalette(QColor(0, 0, 0, 50)));
    shade->setFixedSize(w, h);
    shade_widget = scene->addWidget(shade);
    shade_widget->setPos(0, 0);
    shade_widget->setZValue(1);
    shade_widget->hide();

    //welcome text & title | set content & font & position & depth
    welcome_text = new QGraphicsTextItem();
    scene->addItem(welcome_text);
    welcome_text->setHtml("<font color=black>Doodle Jump</font>");
    welcome_text->setFont(QFont("Times", 40, QFont::Bold));
    welcome_text->setPos(w/4, h/6);
    welcome_text->setZValue(2);

    //pause inform | set content & font & position & depth
    pause_text = new QGraphicsTextItem();
    scene->addItem(pause_text);
    pause_text->setHtml("<font color=black>Paused</font>");
    pause_text->setFont(QFont("Times", 40, QFont::Bold));
    pause_text->setPos(w/3, h/6);
    pause_text->setZValue(2);
    pause_text->hide();

    //finish inform | set content & font & position & depth
    game_over_text = new QGraphicsTextItem();
    scene->addItem(game_over_text);
    game_over_text->setHtml("<font color=black>Game Over！</font>");
    game_over_text->setFont(QFont("Times", 40, QFont::Bold));
    game_over_text->setPos(w/4, h/6);
    game_over_text->setZValue(2);
    game_over_text->hide();

    //buttons used in the game and menu
    QPushButton *button1 = new QPushButton("Start");
    connect(button1, SIGNAL(clicked()), this, SLOT(start_game()));
    start_button = scene->addWidget(button1);
    start_button->setPos(w/2-20, h/3);
    start_button->setZValue(2);

    QPushButton *button4 = new QPushButton("Quit");
    connect(button4, SIGNAL(clicked()), qApp, SLOT(quit()));
    exit_button = scene->addWidget(button4);
    exit_button->setPos(w/2-20, h/3+100);
    exit_button->setZValue(2);

    QPushButton *button6 = new QPushButton("Pause");
    connect(button6, SIGNAL(clicked()), this, SLOT(pause_game()));
    pause_button = scene->addWidget(button6);
    pause_button->setPos(w/2, 0);
    pause_button->setZValue(2);

    QPushButton *button7 = new QPushButton("Return");
    connect(button7, SIGNAL(clicked()), this, SLOT(return_game()));
    return_button = scene->addWidget(button7);
    return_button->setPos(w/2-20, h/3);
    return_button->setZValue(2);

    QPushButton *button8 = new QPushButton("Menu");
    connect(button8, SIGNAL(clicked()), this, SLOT(back_to_menu()));
    backmenu_button = scene->addWidget(button8);
    backmenu_button->setPos(w/2+100, 0);
    backmenu_button->setZValue(2);

    pause_button->hide();
    backmenu_button->hide();
    return_button->hide();
}

void DoodleView::start_game() {
    welcome_text->hide();
    start_button->hide();
    exit_button->hide();
    init_game();
}

void DoodleView::init_game() {
    doodoo = new Doodler(0);
    doodoo->setFocus();
    scene()->addItem(doodoo);
    doodoo->show();
    SCORE = 0;

    //***HP***//
    Heart *h1 = new Heart(1);
    Heart *h2 = new Heart(2);
    Heart *h3 = new Heart(3);
    connect(doodoo, SIGNAL(hurt(int)), h1, SLOT(hurtHP(int)));
    connect(doodoo, SIGNAL(hurt(int)), h2, SLOT(hurtHP(int)));
    connect(doodoo, SIGNAL(hurt(int)), h3, SLOT(hurtHP(int)));
    scene()->addItem(h1);
    scene()->addItem(h2);
    scene()->addItem(h3);
    h1->show();
    h2->show();
    h3->show();
    //***HP***//

    game_score->setHtml("<font color = red >Score: 0</font>");
    game_score->show();
    pause_button->show();
    backmenu_button->show();

    connect(doodoo, SIGNAL(ggwp()),this,SLOT(game_over()));
    connect(doodoo, SIGNAL(view_down(double)), this, SLOT(addNew(double)));

    initbottom = h;
    emit doodoo->view_down(0);
    emit doodoo->view_down(1);

    timer2 = new QTimer;
    timer2->start(5);
    connect(timer2,SIGNAL(timeout()),this,SLOT(update_score()));
    connect(timer2, SIGNAL(timeout()), this, SLOT(out_view()));
}

void DoodleView::update_score() {
    game_score->setHtml(tr("<font color = red>%1</font>").arg(doodoo->score));
    game_score->show();
    SCORE = doodoo->score;
}

void DoodleView::game_over() {
    timer2->stop();
    doodoo->stop_timer();
    delete doodoo;
    pause_button->hide();
    game_over_text->show();
    backmenu_button->setPos(w/2-20, h/3);
    exit_button->show();
    shade_widget->show();
}

void DoodleView::pause_game() {
    timer2->stop();
    doodoo->stop_timer();
    pause_button->hide();
    backmenu_button->hide();
    shade_widget->show();
    pause_text->show();
    return_button->show();
}

void DoodleView::return_game() {
    timer2->start(5);
    doodoo->startTimer(25);
    doodoo->setFocus();
    return_button->hide();
    pause_text->hide();
    shade_widget->hide();
    pause_button->show();
    backmenu_button->show();
}

void DoodleView::back_to_menu() {
    game_over_text->hide();
    game_score->hide();
    QList<QGraphicsItem *> itemList = scene()->items(-w, -h*2, w*3, h*4, Qt::ContainsItemShape, Qt::AscendingOrder);
    foreach (QGraphicsItem *item, itemList) {
        if(item->scene()) scene()->removeItem(item);
    }
    init_view();
}

void DoodleView::out_view() {
    QList<QGraphicsItem *> itemList = scene()->items(-w, h+50, w*3, h*2, Qt::ContainsItemShape, Qt::AscendingOrder);
    foreach (QGraphicsItem *item, itemList) {
        if(item->scene()) scene()->removeItem(item);
    }
}
/*
void DoodleView::create_bullet(double mouse_x, double mouse_y, double doo_x, double doo_y) {
    Bullet *bullet = new Bullet(mouse_x, mouse_y, doo_x, doo_y);
    scene()->addItem(bullet);
    bullet->show();
}
*/
/*
void DoodleView::mousePressEvent(QMouseEvent *event) {
    if(bnt) return;
    if(event->button()==Qt::RightButton) {
        qDebug()<<"mouse clicked "<<event->pos();
        //if(doodoo->scene() != NULL) qDebug()<<"doodoo position "<<doodoo->pos();
    }
}
*/

void DoodleView::addNew(double dis) {
    initbottom += dis*3/2;
    SCORE = doodoo->score;
    //qDebug() << "init " << initbottom;
    //qDebug() << "dis " << dis;
    //qDebug() << "score " << SCORE;
    int a, b, c, temp;
    for (int i=0; i<100; i++) {
        if(initbottom < -h*5/4) break;
        a = randomInteger(0, w-120);
        b = randomInteger(20, 25);
        c = ran;
        ran+=2;
        if(ran > 110) ran = 3;
        temp = initbottom;

        //***platform part***//
        //moving platform: c == 33
        //broken platform: c % 97 == 0
        //invisible platform: c % 101 == 0
        //normal platform: else
        if(c == 33 && SCORE < 6000) c = 3; //moving plat after 6000
        if(c == 101 && SCORE < 9000) c = 3; //invisible plat after 9000
        Platform *platform = new Platform(a, temp, c);
        //qDebug() << "I am ccc " << c;
        connect(doodoo, SIGNAL(view_down(double)), platform, SLOT(fall(double)));
        connect(platform, SIGNAL(view_down_over()), doodoo, SLOT(mid_fall()));
        if(c == 97) connect(doodoo, SIGNAL(touch_broken_plat()), platform, SLOT(broken()));
        initbottom -= b * 5;
        scene()->addItem(platform);
        platform->show();
        //***platform part***//

        //***item part***//
        //spring : c == 79
        //trampoline : c == 89
        //propeller : c == 103
        //jetpack : c == 107
        if(c == 79 && SCORE < 3000) c = 3;
        if(c == 89 && SCORE < 6000) c = 3;
        if(c == 99 && SCORE < 9000) c = 3;
        if(c == 107 && SCORE < 12000) c = 3;
        if(c == 79 || c == 89 || c == 99 || c == 107) {
            //qDebug() << "I want Item " << c;
            Item *item = new Item(a, temp, c);
            connect(doodoo, SIGNAL(view_down(double)), item, SLOT(fall(double)));
            connect(doodoo, SIGNAL(touchitem(int)), item, SLOT(release(int)));
            scene()->addItem(item);
            item->show();
        }
        //***item part***//

        //***monster part***//
        //monster Green : c == 49
        //monster Fly : c == 59
        //monster Double : c == 69
        if(c == 49 && SCORE > 1000) {
            Monster *monster = new Monster(a, temp, 1);
            connect(doodoo, SIGNAL(view_down(double)), monster, SLOT(fall(double)));
            connect(doodoo, SIGNAL(touch_monster(int)), monster, SLOT(touch(int)));
            scene()->addItem(monster);
            monster->show();
        }
        if(c == 59 && SCORE > 1000) {
            Monster *monster = new Monster(a, temp, 2);
            connect(doodoo, SIGNAL(view_down(double)), monster, SLOT(fall(double)));
            connect(doodoo, SIGNAL(touch_monster(int)), monster, SLOT(touch(int)));
            scene()->addItem(monster);
            monster->show();
        }
        if(c == 69 && SCORE > 1000) {
            Monster *monster = new Monster(a, temp, 3);
            connect(doodoo, SIGNAL(view_down(double)), monster, SLOT(fall(double)));
            connect(doodoo, SIGNAL(touch_monster(int)), monster, SLOT(touch(int)));
            scene()->addItem(monster);
            monster->show();
        }
        //***monster part***//
    }
}
