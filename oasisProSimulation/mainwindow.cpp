#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdlib.h>
#include <time.h>

// helper function to nicely record the recorded session to a file.
void recordToFile(Session * sToRecord);

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
    badConnection = false;
    batteryLvl = 100.00;
    intensityLvl = 0.0;
    sessionLength = 0;
    currentSessionTimer = new QTimer(this);
    testConnectionTimer = new QTimer(this);
    currentSession = NULL;
    elaspedTime = 0;

    // Initializing connections
    connect(ui->powerButton, SIGNAL(pressed()), this, SLOT(powerButtonPress()));
    connect(ui->powerButton, SIGNAL(released()), this, SLOT(powerButtonRelease()));
    connect(ui->chargeBatteryButton, SIGNAL(pressed()), this, SLOT(chargeBattery()));
    connect(ui->connectToSkinButton, SIGNAL(pressed()), this, SLOT(connectToSkin()));
    connect(ui->upButton, SIGNAL(pressed()), this, SLOT(upButtonPress()));
    connect(ui->downButton, SIGNAL(pressed()), this, SLOT(downButtonPress()));
    connect(ui->confirmButton, SIGNAL(pressed()), this, SLOT(confirmButtonPress()));
    connect(ui->confirmButton, SIGNAL(released()), this, SLOT(confirmButtonRelease()));
    connect(ui->recordButton, SIGNAL(pressed()), this, SLOT(recordButtonPress()));
    connect(ui->loadUserDesignedButton, SIGNAL(pressed()), this, SLOT(loadUserDesignedButtonPress()));
    connect(testConnectionTimer, SIGNAL(timeout()), this, SLOT(flashConnection()));
    connect(currentSessionTimer, SIGNAL(timeout()), this, SLOT(depleteBattery()));

    // Initialize default sessions
    QVector<QString> g = {"20", "45", "User"}; // lengths are actually just groups
    QVector<QString> t = {"Sub Delta", "Delta", "Alpha", "Theta"};
    QVector<QString> f = {"1.75", "3.75", "7", "10"}; // types are actually just sessions (sub delta, delta, etc)

    // -1 since last group is user defined, which will be given nothing seperatly
    for (int i = 0; i < g.size() - 1; i++) {
        QVector<Session *> newVector;
        for (int j = 0 ; j < f.size(); j++) {
            Session *newS = new Session(g[i],t[j],f[j],0);
            newVector.push_back(newS);
        }
        sessions.push_back(newVector);
    }

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
        // toggle device power
        if (this->powerOn) {
            // if the device is powered off before a session completes, record first
            // TODO: should we also record if the session was interupted or not?
            if (recordingPending) {
                recordToFile(currentSession);
                recordingPending = false;
            }

            std::cout << "Powering Off" << std::endl;
            this->powerOn = false;
            handlePowerOff();
        } else if (batteryLvl > 0) {
            std::cout << "Powering On" << std::endl;
            this->powerOn = true;
            handlePowerOn();
            displaySessionSelect();
        }
        std::cout << "Device power state: " << this->powerOn << std::endl;
    } else if (this->powerOn && !testConnectionTimer->isActive()) {
        std::cout << "Normal Press" << std::endl;
        switchGroups();
        displaySessionSelect();
    } else {
        // indicate the need for power on first
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
    if (powerOn && !testConnectionTimer->isActive())
    {
        std::cout << "Up button pressed" << std::endl;

        if (currentSession != NULL)
        {
            if(intensityLvl<8)
                intensityLvl +=0.5;
            else
                intensityLvl =8;

            currentSession->setIntensity(intensityLvl);
            displayIntensityLevel();
        }
        else
        {
            switchType(1);
        }
    }
}

void MainWindow::downButtonPress()
{
    if (powerOn && !testConnectionTimer->isActive())
    {
        std::cout << "Down button pressed" << std::endl;

        if (currentSession != NULL && !testConnectionTimer->isActive())
        {
            if(intensityLvl>0)
                intensityLvl -=0.5;
            else
                intensityLvl =0;

            currentSession->setIntensity(intensityLvl);
            displayIntensityLevel();
        }
        else
        {
            switchType(-1);
        }
    }
}

void MainWindow::confirmButtonPress()
{
    // TODO: long press should save current intensity value to the session being run
    if (powerOn)
    {
        this->elapsedTimerConfirm.start();
    }
}

void MainWindow::confirmButtonRelease()
{
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
        QString setLen;
        QString setType;
        QString setFreq;
        int setInt;
        // 0, 1 mean pre-defined groups
        if (lengthPosition != 2) {
            std::cout << lengthPosition << " " << typePosition << std::endl;
            setLen = sessions[lengthPosition][typePosition]->getSessionLength();
            setType = sessions[lengthPosition][typePosition]->getType();
            setFreq = sessions[lengthPosition][typePosition]->getFrequency();
            setInt = sessions[lengthPosition][typePosition]->getIntensity();
        } else {
            // TODO: select user defined session
            // these are place holder values for now
            setLen = "20";
            setFreq = "5";
            setInt = 0;
        }
        // TODO: instead of making a new session, just point to one of the stored ones
        Session* s = new Session(setLen,setType,setFreq,setInt);
        intensityLvl = 0.0; // as specified in manual, intensity will always start at 0
        currentSession = s;
        startSession(s);

    } else {
        std::cout << "Turn on device" << std::endl;
    }
}

void MainWindow::recordButtonPress()
{
    // TODO: implement recording
    std::cout << "Record button pressed" << std::endl;

    if (currentSession != NULL)
    {
        // recordToFile(currentSession);
        // indicated that we need to record a session
        recordingPending = true;
    }
    else
    {
        std::cout << "No session to record" << std::endl;
    }
}

void MainWindow::loadUserDesignedButtonPress()
{
    std::cout << "Load user designed button pressed" << std::endl;
}

void MainWindow::startSession(Session *s)
{
    //a test session created in confirmButtonRelase()

    //Initial connection test, see manual for more functionality

    if (testConnection())
    {
        currentSession->print();
    }
}

void MainWindow::endSession()
{
    std::cout << "Session End" << std::endl;
    recordToFile(currentSession);
    recordingPending = false;
}

void MainWindow::initializeTimer()
{
    if(skinConnection){
        currentSessionTimer->start(1000);
    }
}

void MainWindow::depleteBattery()
{
    elaspedTime += 1;
    batteryLvl -= 1;
    std::cout << "Battery Level: " << batteryLvl << std::endl;

    if (batteryLvl == 0)
    {
        powerOn = false;
        handlePowerOff();
    }

    if ((elaspedTime % 5) == 0)
    {
        displayBatteryLevel();
    }
    else
    {
        displayIntensityLevel();
    }

}

void MainWindow::displayBatteryLevel()
{
    int levelToDisplay = (batteryLvl * 0.8)/10; // convert battery level to be out of 8 instead of 100, since we have 8 lights to show level
    // needed since otherwise when battery below ~12% it would otherwise show no bars
    if (levelToDisplay < 1) {
        levelToDisplay = 1;
    }
    // std::cout << levelToDisplay << std::endl;
    for (int i = 0; i < levelToDisplay; i++) {
        sessionNumLabel[i]->setStyleSheet(sessionNum_on[i]);
    }
    for (std::size_t i = levelToDisplay; i < sessionNumLabel.size(); i++) {
        sessionNumLabel[i]->setStyleSheet(sessionNum_off);
    }
}

void MainWindow::displayIntensityLevel()
{
    int levelToDisplay = floor(intensityLvl); // round intensity level down (6.5 displays 6, 5.5, displays 5, etc.)

    for (int i = 0; i < levelToDisplay; i++) {
        sessionNumLabel[i]->setStyleSheet(sessionNum_on[i]);
    }
    for (std::size_t i = levelToDisplay; i < sessionNumLabel.size(); i++) {
        sessionNumLabel[i]->setStyleSheet(sessionNum_off);
    }
}

void MainWindow::displaySessionSelect()
{
    int availableToSelect = 0;
    // if user defined sessions is empty, sessions.size() will possibly be 1 less than lengthPosition
    if (sessions.size() > lengthPosition) {
        availableToSelect = sessions[lengthPosition].size();
    }
    for (int i = 0; i < availableToSelect; i++) {
        sessionNumLabel[i]->setStyleSheet(sessionNum_on[i]);
    }
    for (std::size_t i = availableToSelect; i < 8; i++) {
        sessionNumLabel[i]->setStyleSheet(sessionNum_off);
    }

    // TODO: on selecting a session, flash the number to indicate selection before confirmation
}

void MainWindow::handlePowerOn()
{
    // display device power level
    displayBatteryLevel();

    // light up default value labels
    sessionLengthLabel[0]->setPixmap(sessionLength_on[0]);
    sessionTypeLabel[0]->setPixmap(sessionType_on[0]);

    ui->ledLabel->setStyleSheet("background-color:green");
}

void MainWindow::handlePowerOff()
{
    //pause timer
    currentSessionTimer->stop();

    // remove current session
    currentSession = NULL;

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
    // Start connection test and end it after 5 seconds
    testConnectionTimer->start(1000);
    testConnectionTimer->singleShot(5000, this, SLOT(stopConnectionTest()));

    for (int i = 0; i < (int)sessionNumLabel.size(); i++)
    {
        sessionNumLabel[i]->setStyleSheet(sessionNum_off);
    }

    if (skinConnection)
    {
        // randomize either okay or excellent connection if skin is connected
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

        badConnection = false;
        return true;
    }
    else
    {
        // no connection
        badConnection = true;
        return false;
    }
}

void MainWindow::flashConnection()
{   
    // stop timer if power is turned off
    if (!powerOn)
    {
        testConnectionTimer->stop();
        connectionButtonsLit = false;
        badConnection = false;
        return;
    }

    QString tempFreq = currentSession->getFrequency();

    // Flash duty or short CES depending on the select session
    if (tempFreq == "1.75")
    {
        if (connectionButtonsLit)
        {
            ui->cesDutyLabel->setPixmap(dutyCES_off);
        }
        else
        {
            ui->cesDutyLabel->setPixmap(dutyCES_on);
        }
    }
    else
    {
        if (connectionButtonsLit)
        {
            ui->cesShortLabel->setPixmap(shortCES_off);
        }
        else
        {
            ui->cesShortLabel->setPixmap(shortCES_on);
        }
    }

    if (badConnection)
    {
        // flash bad connection on/off
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
    }

    connectionButtonsLit = !connectionButtonsLit;
}

void MainWindow::stopConnectionTest()
{
    // Stop connection test, reset vars, and attempt to initialize timer
    testConnectionTimer->stop();
    connectionButtonsLit = false;
    ui->cesDutyLabel->setPixmap(dutyCES_off);
    ui->cesShortLabel->setPixmap(shortCES_off);

    displayIntensityLevel();
    initializeTimer();
}

// Should this maybe be done by a seperate recording class?
void recordToFile(Session * sToRecord) {
    std::ofstream recordFile;
    // TODO: do we need to change the output directory to the non-debug folder?
    recordFile.open("treatment_history.txt", std::fstream::app);

    // to check where the .txt will be created/looked for
    char cwd[256];
    getcwd(cwd, 256);
    std::cout << cwd << std::endl;

    if (!recordFile) {
        std::cout << "Could not opening/making file" << std::endl;
        return;
    }

    recordFile.seekp(0, std::ios::end);

    recordFile << "Session Type: " << sToRecord->getType().toStdString() << ", Frequency: " << sToRecord->getFrequency().toStdString()
               << "\nTotal Duration: " << sToRecord->getSessionLength().toStdString()
               << "\nIntensity: " << sToRecord->getIntensity() << "\n\n";

    std::cout << "Recorded session - type: " << sToRecord->getType().toStdString()
              << ", freq: " << sToRecord->getFrequency().toStdString() << ", intensity: "
              << sToRecord->getIntensity()<< ", length: " << sToRecord->getSessionLength().toStdString() << std::endl;

    recordFile.close();
}
