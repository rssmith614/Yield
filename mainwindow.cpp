#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // define which preset each road made in the ui should have
    ui->RoadA->setPreset(Road::A);
    ui->RoadB->setPreset(Road::B);
    ui->Road1->setPreset(Road::C);
    ui->Road2->setPreset(Road::DISABLED);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Space) {
        ui->Road1->toggleStop();
        ui->Road2->toggleStop();
    }
}

