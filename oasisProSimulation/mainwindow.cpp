#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    // Initializing vector pixmaps
    sessionLength_on.push_back(QPixmap(":/icons/20min_on.png"));
    sessionLength_on.push_back(QPixmap(":/icons/45min_on.png"));
    sessionLength_on.push_back(QPixmap(":/icons/userdesigned_on.png"));
    sessionLength_off.push_back(QPixmap(":/icons/20min_off.png"));
    sessionLength_off.push_back(QPixmap(":/icons/45min_off.png"));
    sessionLength_off.push_back(QPixmap(":/icons/userdesigned_off.png"));

    sessionType_on.push_back(QPixmap(":/icons/subdelta_on.png"));
    sessionType_on.push_back(QPixmap(":/icons/delta_on.png"));
    sessionType_on.push_back(QPixmap(":/icons/theta_on.png"));
    sessionType_on.push_back(QPixmap(":/icons/alpha_on.png"));
    sessionType_off.push_back(QPixmap(":/icons/subdelta_off.png"));
    sessionType_off.push_back(QPixmap(":/icons/delta_off.png"));
    sessionType_off.push_back(QPixmap(":/icons/theta_off.png"));
    sessionType_off.push_back(QPixmap(":/icons/alpha_off.png"));

    sessionNum_on.push_back("QLabel { color: #26ff00 }");
    sessionNum_on.push_back("QLabel { color: #26ff00 }");
    sessionNum_on.push_back("QLabel { color: #26ff00 }");
    sessionNum_on.push_back("QLabel { color: #fff700 }");
    sessionNum_on.push_back("QLabel { color: #fff700 }");
    sessionNum_on.push_back("QLabel { color: #fff700 }");
    sessionNum_on.push_back("QLabel { color: #ff0000 }");
    sessionNum_on.push_back("QLabel { color: #ff0000 }");
    sessionNum_off = "QLabel { color: #000000 }";


    // TODO: need to inicialize the sessionNumLabel and sessionStimLabel to appropriate QPixmap

    // Initialize UI label vector pointers
    sessionLengthLabel.push_back(ui->sessionLength20);
    sessionLengthLabel.push_back(ui->sessionLength45);
    sessionLengthLabel.push_back(ui->sessionLengthCustom);

    sessionTypeLabel.push_back(ui->sessionSubDelta);
    sessionTypeLabel.push_back(ui->sessionDelta);
    sessionTypeLabel.push_back(ui->sessionTheta);
    sessionTypeLabel.push_back(ui->sessionAlpha);


    sessionNumLabel.push_back(ui->session1);
    sessionNumLabel.push_back(ui->session2);
    sessionNumLabel.push_back(ui->session3);
    sessionNumLabel.push_back(ui->session4);
    sessionNumLabel.push_back(ui->session5);
    sessionNumLabel.push_back(ui->session6);
    sessionNumLabel.push_back(ui->session7);
    sessionNumLabel.push_back(ui->session8);

    sessionStimLabel.push_back(ui->dcStimLevel1);
    sessionStimLabel.push_back(ui->dcStimLevel2);
    sessionStimLabel.push_back(ui->dcStimLevel3);
    sessionStimLabel.push_back(ui->dcStimLevel4);
    sessionStimLabel.push_back(ui->dcStimLevel5);
    sessionStimLabel.push_back(ui->dcStimLevel6);
    sessionStimLabel.push_back(ui->dcStimUserDesigned1);
    sessionStimLabel.push_back(ui->dcStimUserDesigned1);

    // Initializing pixmaps
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
        switchGroups();
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
        // TODO: add check to see what device state is in
        switchType(1);
    }
}

void MainWindow::downButtonPress()
{
    // TODO: add handling
    if (powerOn)
    {
        std::cout << "Down button pressed" << std::endl;
        // TODO: add check to see what device state is in
        switchType(-1);
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
    // display device power level
    displayBatteryLevel();
    // light up default value labels
    sessionLengthLabel[0]->setPixmap(sessionLength_on[0]);
    sessionTypeLabel[0]->setPixmap(sessionType_on[0]);

    // TODO: might want to find a way to turn LED on after 2 secs have passed (before button is let go) since that's what they have in the demo
    ui->ledLabel->setStyleSheet("background-color:green");
    ui->cesLeftLabel->setPixmap(L_on);
    ui->cesRightLabel->setPixmap(R_on);
    ui->cesShortLabel->setPixmap(shortCES_on);
    ui->cesDutyLabel->setPixmap(dutyCES_on);
}

void MainWindow::handlePowerOff()
{
    for (std::size_t i = 0; i < sessionLengthLabel.size(); i++) {
        sessionLengthLabel[i]->setPixmap(sessionLength_off[i]);
    }
    for (std::size_t i = 0; i < sessionTypeLabel.size(); i++) {
        sessionTypeLabel[i]->setPixmap(sessionType_off[i]);
    }
    for (std::size_t i = 0; i < sessionNumLabel.size(); i++) {
        sessionNumLabel[i]->setPixmap(sessionNum_off);
    }

    ui->ledLabel->setStyleSheet("background-color:white");
    ui->cesLeftLabel->setPixmap(L_off);
    ui->cesRightLabel->setPixmap(R_off);
    ui->cesShortLabel->setPixmap(shortCES_off);
    ui->cesDutyLabel->setPixmap(dutyCES_off);
}

void MainWindow::displayBatteryLevel() {
    int levelToDisplay = (batteryLvl * 0.8)/10; // convert battery level to be out of 8 instead of 100, since we have 8 lights to show level
    std::cout << levelToDisplay << std::endl;
    for (int i = 0; i < levelToDisplay; i++) {
        sessionNumLabel[i]->setStyleSheet(sessionNum_on[i]);
    }
    for (std::size_t i = levelToDisplay; i < sessionNumLabel.size(); i++) {
        sessionNumLabel[i]->setStyleSheet(sessionNum_off);
    }
}

void MainWindow::switchGroups() {
    // start by turning all appropriate labels "off"
    for (std::size_t i = 0; i < sessionLengthLabel.size(); i++) {
        sessionLengthLabel[i]->setPixmap(sessionLength_off[i]);
    }
    lengthPosition++;
    // for groups, every iteration just increments by 1, until end reached, then return to beginning
    if (lengthPosition > (int)sessionLengthLabel.size() - 1) {
        lengthPosition = 0;
    }
    // turn "on" selected group label
    sessionLengthLabel[lengthPosition]->setPixmap(sessionLength_on[lengthPosition]);
}

void MainWindow::switchType(int direction) {
    // start by turning all appropriate labels "off"
    for (std::size_t i = 0; i < sessionTypeLabel.size(); i++) {
        sessionTypeLabel[i]->setPixmap(sessionType_off[i]);
    }
    typePosition += direction;
    // for types, every iteration is either +1 or -1, if out of bounds, wrap around.
    if (typePosition > (int)sessionTypeLabel.size() - 1) {
        typePosition = 0;
    } else if (typePosition < 0) {
        typePosition = sessionTypeLabel.size() - 1;
    }
    // turn "on" selected type label
    sessionTypeLabel[typePosition]->setPixmap(sessionType_on[typePosition]);
}
