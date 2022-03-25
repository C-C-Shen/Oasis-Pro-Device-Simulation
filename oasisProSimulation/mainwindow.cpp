#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initializing connections
    connect(ui->powerButton, SIGNAL(pressed()), this, SLOT(powerButtonPress()));
    connect(ui->powerButton, SIGNAL(released()), this, SLOT(powerButtonRelease()));
    connect(ui->chargeBatteryButton, SIGNAL(pressed()), this, SLOT(chargeBattery()));
    connect(ui->connectToSkinButton, SIGNAL(pressed()), this, SLOT(connectToSkin()));
    connect(ui->upButton, SIGNAL(pressed()), this, SLOT(upButtonPress()));
    connect(ui->downButton, SIGNAL(pressed()), this, SLOT(downButtonPress()));
    connect(ui->confirmButton, SIGNAL(pressed()), this, SLOT(confirmButtonPress()));

    // Initializing pixmaps
    sessionLength20_on = QPixmap(":/icons/20min_on.png");
    sessionLength20_off = QPixmap(":/icons/20min_off.png");
    sessionLength45_on = QPixmap(":/icons/45min_on.png");
    sessionLength45_off = QPixmap(":/icons/45min_off.png");
    userDesigned_on = QPixmap(":/icons/userdesigned_on.png");
    userDesigned_off = QPixmap(":/icons/userdesigned_off.png");
    theta_on = QPixmap(":/icons/theta_on.png");
    theta_off = QPixmap(":/icons/theta_off.png");
    alpha_on = QPixmap(":/icons/alpha_on.png");
    alpha_off = QPixmap(":/icons/alpha_off.png");
    delta_on = QPixmap(":/icons/delta_on.png");
    delta_off = QPixmap(":/icons/delta_off.png");
    subDelta_on = QPixmap(":/icons/subdelta_on.png");
    subDelta_off = QPixmap(":/icons/subdelta_off.png");
    L_on = QPixmap(":/icons/L_on.png");
    L_off = QPixmap(":/icons/L_off.png");
    R_on = QPixmap(":/icons/R_on.png");
    R_off = QPixmap(":/icons/R_off.png");
    dutyCES_on = QPixmap(":/icons/dutyCES_on.png");
    dutyCES_off = QPixmap(":/icons/dutyCES_off.png");
    shortCES_on = QPixmap(":/icons/shortCES_on.png");
    shortCES_off = QPixmap(":/icons/shortCES_off.png");

    // Initializing variables
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

void MainWindow::chargeBattery()
{
    batteryLvl = 100.00;
    std::cout << "Battery Level: " << batteryLvl << std::endl;
}

void MainWindow::connectToSkin()
{
    skinConnection = !skinConnection;
    std::cout << "Skin connection: " << skinConnection << std::endl;
}

void MainWindow::upButtonPress()
{
    // TODO: add handling
    if (powerOn)
    {
        std::cout << "Up button pressed" << std::endl;
    }
}

void MainWindow::downButtonPress()
{
    // TODO: add handling
    if (powerOn)
    {
        std::cout << "Down button pressed" << std::endl;
    }
}

void MainWindow::confirmButtonPress()
{
    // TODO: add handling
    if (powerOn)
    {
        std::cout << "Confirm button pressed" << std::endl;
    }
}

void MainWindow::handlePowerOn()
{
    // TODO: might want to find a way to turn LED on after 2 secs have passed (before button is let go) since that's what they have in the demo
    ui->ledLabel->setStyleSheet("background-color:green");
    ui->sessionLength20->setPixmap(sessionLength20_on);
    ui->sessionLength45->setPixmap(sessionLength45_on);
    ui->sessionLengthCustom->setPixmap(userDesigned_on);
    ui->sessionTheta->setPixmap(theta_on);
    ui->sessionAlpha->setPixmap(alpha_on);
    ui->sessionDelta->setPixmap(delta_on);
    ui->sessionSubDelta->setPixmap(subDelta_on);
    ui->cesLeftLabel->setPixmap(L_on);
    ui->cesRightLabel->setPixmap(R_on);
    ui->cesShortLabel->setPixmap(shortCES_on);
    ui->cesDutyLabel->setPixmap(dutyCES_on);
}

void MainWindow::handlePowerOff()
{
    ui->ledLabel->setStyleSheet("background-color:white");
    ui->sessionLength20->setPixmap(sessionLength20_off);
    ui->sessionLength45->setPixmap(sessionLength45_off);
    ui->sessionLengthCustom->setPixmap(userDesigned_off);
    ui->sessionTheta->setPixmap(theta_off);
    ui->sessionAlpha->setPixmap(alpha_off);
    ui->sessionDelta->setPixmap(delta_off);
    ui->sessionSubDelta->setPixmap(subDelta_off);
    ui->cesLeftLabel->setPixmap(L_off);
    ui->cesRightLabel->setPixmap(R_off);
    ui->cesShortLabel->setPixmap(shortCES_off);
    ui->cesDutyLabel->setPixmap(dutyCES_off);
}
