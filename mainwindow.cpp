#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // define which preset each road made in the ui should have
    ui->RoadA->setPreset(Road::A);
    ui->RoadB->setPreset(Road::RoadPreset::B);
    ui->Road1->setPreset(Road::C);
}

MainWindow::~MainWindow()
{
    delete ui;
}

