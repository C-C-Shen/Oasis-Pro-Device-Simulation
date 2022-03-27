#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdlib.h>
#include <time.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand(time(NULL));

    // Initializing variables
    powerOn = false;
    skinConnection = false;
    connectionButtonsLit = false;
    batteryLvl = 100.00;
    intensityLvl = 1;
    sessionLength = 0;
    currentSessionTimer = new QTimer(this);
    noConnectionTimer = new QTimer(this);
    currentSession = NULL;

    // Initializing connections
    connect(ui->powerButton, SIGNAL(pressed()), this, SLOT(powerButtonPress()));
    connect(ui->powerButton, SIGNAL(released()), this, SLOT(powerButtonRelease()));
    connect(ui->chargeBatteryButton, SIGNAL(pressed()), this, SLOT(chargeBattery()));
    connect(ui->connectToSkinButton, SIGNAL(pressed()), this, SLOT(connectToSkin()));
    connect(ui->upButton, SIGNAL(pressed()), this, SLOT(upButtonPress()));
    connect(ui->downButton, SIGNAL(pressed()), this, SLOT(downButtonPress()));
    connect(ui->confirmButton, SIGNAL(pressed()), this, SLOT(confirmButtonPress()));
    connect(ui->confirmButton, SIGNAL(released()), this, SLOT(confirmButtonRelease()));
    connect(noConnectionTimer, SIGNAL(timeout()), this, SLOT(flashConnection()));
    connect(currentSessionTimer, SIGNAL(timeout()), this, SLOT(depleteBattery()));

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
            currentSession = NULL;
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

    //Pause timer
    if (currentSessionTimer->isActive() && currentSession != NULL){
        if(!skinConnection){
            currentSessionTimer->stop();
        }
    }
    if (!currentSessionTimer->isActive() && currentSession != NULL){
        if(skinConnection){
            currentSessionTimer->start(1000);
        }
    }
}

void MainWindow::upButtonPress()
{
    if (powerOn)
    {
        std::cout << "Up button pressed" << std::endl;

        if (currentSession != NULL)
        {
            // TODO: adjust intensity
        }
        else
        {
            switchType(1);
        }
    }
}

void MainWindow::downButtonPress()
{
    if (powerOn)
    {
        std::cout << "Down button pressed" << std::endl;

        if (currentSession != NULL)
        {
            // TODO: adjust intensity
        }
        else
        {
            switchType(-1);
        }
    }
}

void MainWindow::confirmButtonPress()
{
    if (powerOn)
    {
        this->elapsedTimerConfirm.start();
    }
}

void MainWindow::confirmButtonRelease() {
    int elapsed = this->elapsedTimerConfirm.elapsed();

    if (!powerOn)
           return;

    if (currentSession != NULL)
            return;

    if (elapsed >= 2000) {
        //TODO: Long Press to switch to save mode later on

        std::cout << "Long Confirm Pressed" << std::endl;

    } else if (powerOn) {
        std::cout << "Normal Press" << std::endl;
        //testing battery depletion, can remove
        Session* s = new Session("25","10");
        startSession(s);

    } else {
        std::cout << "Turn on device" << std::endl;
    }
}

void MainWindow::startSession(Session *s)
{
    //a test session created in confirmButtonRelase()

    //Initial connection test, see manual for more functionality

    if(testConnection())
    {
        initializeTimer();
        currentSession = s;
        std::cout << "Session Start" << std::endl;
    }
    else
    {
        std::cout << "sKiN iS dETaCheD" << std::endl;
    }
}

void MainWindow::endSession()
{
    std::cout << "Session End" << std::endl;
}


void MainWindow::initializeTimer()
{
    if(skinConnection){
        currentSessionTimer->start(1000);
    }
}

void MainWindow::depleteBattery(){
    batteryLvl-=1;
    std::cout << "Battery Level: "<<batteryLvl << std::endl;
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
    //pause timer
    currentSessionTimer->stop();

    //update interface
    for (std::size_t i = 0; i < sessionLengthLabel.size(); i++) {
        sessionLengthLabel[i]->setPixmap(sessionLength_off[i]);
    }
    for (std::size_t i = 0; i < sessionTypeLabel.size(); i++) {
        sessionTypeLabel[i]->setPixmap(sessionType_off[i]);
    }
    for (std::size_t i = 0; i < sessionNumLabel.size(); i++) {
        sessionNumLabel[i]->setStyleSheet(sessionNum_off);
    }

    ui->ledLabel->setStyleSheet("background-color:white");
    ui->cesLeftLabel->setPixmap(L_off);
    ui->cesRightLabel->setPixmap(R_off);
    ui->cesShortLabel->setPixmap(shortCES_off);
    ui->cesDutyLabel->setPixmap(dutyCES_off);
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

bool MainWindow::testConnection()
{
    // TODO: implement CES mode light blinking
    for (int i = 0; i < (int)sessionNumLabel.size(); i++)
    {
        sessionNumLabel[i]->setStyleSheet(sessionNum_off);
    }

    if (skinConnection)
    {
        // randomize either okay or excellent connection
        if ((rand() % 2) == 1)
        {
            // excellent connection
            sessionNumLabel[0]->setStyleSheet(sessionNum_on[0]);
            sessionNumLabel[1]->setStyleSheet(sessionNum_on[1]);
            sessionNumLabel[2]->setStyleSheet(sessionNum_on[2]);
        }
        else
        {
            // okay connection
            sessionNumLabel[3]->setStyleSheet(sessionNum_on[3]);
            sessionNumLabel[4]->setStyleSheet(sessionNum_on[4]);
            sessionNumLabel[5]->setStyleSheet(sessionNum_on[5]);
        }

        return true;
    }
    else
    {
        // no connection
        noConnectionTimer->start(1000);
        return false;
    }
}

void MainWindow::flashConnection()
{
    // TODO: implement graph scrolling up/down after some seconds?

    // stop timer if power is turned off
    if (!powerOn)
    {
        noConnectionTimer->stop();
        connectionButtonsLit = false;
        return;
    }
    else if (skinConnection)
    {
        noConnectionTimer->stop();
        connectionButtonsLit = false;
        // start session if skin is connected
        startSession(currentSession);
        return;
    }

    // flash connection
    if (connectionButtonsLit)
    {
        sessionNumLabel[6]->setStyleSheet(sessionNum_off);
        sessionNumLabel[7]->setStyleSheet(sessionNum_off);
    }
    else
    {
        sessionNumLabel[6]->setStyleSheet(sessionNum_on[6]);
        sessionNumLabel[7]->setStyleSheet(sessionNum_on[7]);
    }

    connectionButtonsLit = !connectionButtonsLit;
}
