#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // define which preset each road made in the ui should have
    ui->RoadA->setPreset(Road::RoadPreset::A);
    ui->RoadB->setPreset(Road::RoadPreset::B);
}

MainWindow::~MainWindow()
{
    delete ui;
}

