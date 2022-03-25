#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->powerButton, SIGNAL(pressed()), this, SLOT(powerButtonPress()));
    connect(ui->powerButton, SIGNAL(released()), this, SLOT(powerButtonRelease()));
    connect(ui->chargeBatteryButton, SIGNAL(pressed()), this, SLOT(chargeBattery()));

    powerOn = false;
    skinConnection = false;
    batteryLvl = 100.00;
    intensityLvl = 0;
    sessionLength = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::powerButtonPress() {
    this->elapsedTimer.start();
}

void MainWindow::powerButtonRelease() {
    int elapsed = this->elapsedTimer.elapsed();
    // for now, "long" press is anything over 2 seconds
    if (elapsed >= 2000) {
        // TODO: is there any other "long" press functionality needed for power button?

        // toggle device power
        if (this->powerOn) {
            std::cout << "Powering Off" << std::endl;
            this->powerOn = false;
            handlePowerOff();
        } else {
            std::cout << "Powering On" << std::endl;
            this->powerOn = true;
            handlePowerOn();
        }
        std::cout << "Device power state: " << this->powerOn << std::endl;
    } else if (this->powerOn) {
        std::cout << "Normal Press" << std::endl;
        // TODO: call function relating to a simple press
    } else {
        // this is just here for testing purpose, can be removed later
        std::cout << "Long press to turn on" << std::endl;
    }
}

void MainWindow::handlePowerOn()
{
    // TODO: modify widgets to display their "on" state icons
}

void MainWindow::handlePowerOff()
{
    // TODO: modify widgets to display their "off" state icons
}

void MainWindow::chargeBattery()
{
    batteryLvl = 100.00;
}
