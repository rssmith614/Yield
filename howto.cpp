#include "howto.h"
#include "mainwindow.h"
#include "ui_howto.h"
#include <QPixmap>
#include <QGraphicsView>
#include <QImage>
#include <QDir>
HowTo::HowTo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HowTo)
{
    QDir cwd(QDir::current());
    cwd.cdUp();
    cwd.cdUp();
    cwd.cdUp();
    cwd.cdUp();
    cwd.cd("./Yield");

    ui->setupUi(this);

    QImage left(cwd.path() + "/leftarrowkey.png"); // <- path to image file
    ui->leftarrow->setPixmap(QPixmap::fromImage(left));

    QImage right(cwd.path() + "/rightarrowkey.png");
    ui->rightarrow->setPixmap(QPixmap::fromImage(right));

    QImage space(cwd.path() + "/spacekey.png");
    ui->space->setPixmap(QPixmap::fromImage(space));

    QImage stop(cwd.path()+ "/stopsign.jpg");
    ui->stop->setPixmap(QPixmap::fromImage(stop));

    QImage clickstop(cwd.path()+ "/mouseclick.png");
    ui->clickstop->setPixmap(QPixmap::fromImage(clickstop));

    connect(ui->backToGame, SIGNAL (clicked()), this, SLOT(backToMenu()));
}

HowTo::~HowTo()
{
    delete ui;
}

void HowTo::backToMenu(){

    HowTo::close();

}
