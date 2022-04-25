#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // define which preset each road made in the ui should have
    ui->RoadA->setPreset(Road::A);
//    ui->RoadB->setPreset(Road::B);
    ui->Road1->setPreset(Road::C);
//    ui->Road2->setPreset(Road::A);

    timer = new QTimer(this);
    timer->start(33);

    connect(timer,SIGNAL(timeout()), this, SLOT(updateUI()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateUI() {
    ui->Road1->toggleStop(ui->Stop_Sign->isClicked());
}




