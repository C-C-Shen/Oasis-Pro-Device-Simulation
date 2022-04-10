#include "mainwindow.h"
#include "ui_mainwindow.h"

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
//    flashIntensity = false;
    flashValue = false;
    batteryLvl = 100.00;
    elaspedTime = 0;
    numToFlash = 0;
    softAnimation = 0;

    currentSessionTimer = new QTimer(this);
    testConnectionTimer = new QTimer(this);
    softAnimationtTimer = new QTimer(this);
    currentSession = NULL;

    recorder.setFile(fileName);

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
    connect(ui->printRecordedSessions, SIGNAL(pressed()), this, SLOT(printRecordedButtonPress()));
    connect(testConnectionTimer, SIGNAL(timeout()), this, SLOT(flashConnection()));
    connect(currentSessionTimer, SIGNAL(timeout()), this, SLOT(updateTime()));
    connect(softAnimationtTimer, SIGNAL(timeout()), this, SLOT(blinkNum()));

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
    L_on_green = QPixmap(":/icons/L_on_green.png");
    L_on_yellow = QPixmap(":/icons/L_on_yellow.png");
    L_on_red = QPixmap(":/icons/L_on_red.png");
    L_off = QPixmap(":/icons/L_off.png");
    R_on_green = QPixmap(":/icons/R_on_green.png");
    R_on_yellow = QPixmap(":/icons/R_on_yellow.png");
    R_on_red = QPixmap(":/icons/R_on_red.png");
    R_off = QPixmap(":/icons/R_off.png");
    dutyCES_on = QPixmap(":/icons/dutyCES_on.png");
    dutyCES_off = QPixmap(":/icons/dutyCES_off.png");
    shortCES_on = QPixmap(":/icons/shortCES_on.png");
    shortCES_off = QPixmap(":/icons/shortCES_off.png");
}

MainWindow::~MainWindow()
{
    delete currentSessionTimer;
    delete testConnectionTimer;
    delete softAnimationtTimer;
    delete currentSession;

    for (int i = 0; i < sessions.size(); i++)
    {
        for (int j = 0; j < sessions[i].size(); j++)
        {
            delete sessions[i][j];
        }
    }

    for (int i = 0; i < (int)sessionLengthLabel.size(); i++)
    {
        delete sessionLengthLabel[i];
    }

    for (int i = 0; i < (int)sessionTypeLabel.size(); i++)
    {
        delete sessionTypeLabel[i];
    }

    for (int i = 0; i < (int)sessionNumLabel.size(); i++)
    {
        delete sessionNumLabel[i];
    }

    for (int i = 0; i < (int)sessionStimLabel.size(); i++)
    {
        delete sessionStimLabel[i];
    }

    delete ui;
}

void MainWindow::powerButtonPress()
{
    this->elapsedTimer.start();
}

void MainWindow::powerButtonRelease()
{
    int elapsed = this->elapsedTimer.elapsed();

    // for now, "long" press is anything over 2 seconds
    if (elapsed >= 2000)
    {
        // toggle device power
        if (this->powerOn)
        {
            // if the device is powered off before a session completes, record first
            // TODO: should we also record if the session was interupted or not?
            if(recorder.getPending())
            {
                recorder.recordSession(currentSession, false);
            }

            std::cout << "Powering Off" << std::endl;
            this->powerOn = false;
            handlePowerOff();
        }
        else if (batteryLvl > 0)
        {
            std::cout << "Powering On" << std::endl;
            this->powerOn = true;
            createUserDesignated = 0;
            customLength = 0;
            handlePowerOn();

            // on power on, flash initial session selected
            if (lengthPosition != 2 || sessions.size() == 3) {
                softAnimation = 4;
                softAnimationtTimer->start(250);
            } else {
                softAnimationtTimer->stop();
            }
            displaySessionSelect();
        }

        std::cout << "Device power state: " << this->powerOn << std::endl;
    }
    else if (this->powerOn && !testConnectionTimer->isActive() && !createUserDesignated)
    {
        std::cout << "Normal Press" << std::endl;
        switchGroups();
        displaySessionSelect();
    }
    else
    {
        if (!this->powerOn) {
            std::cout << "Long press to turn on" << std::endl;
        } else {
            std::cout << "Cannot change group in when in user designated mode" << std::endl;
        }
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

    // Pause timer
    if (currentSessionTimer->isActive() && currentSession != NULL)
    {
        if(!skinConnection)
        {
            ui->cesLeftLabel->setPixmap(L_on_red);
            ui->cesRightLabel->setPixmap(R_on_red);
            currentSessionTimer->stop();
        }
    }

    if (!currentSessionTimer->isActive() && currentSession != NULL)
    {
        if(skinConnection)
        {
            ui->cesLeftLabel->setPixmap(L_on_green);
            ui->cesRightLabel->setPixmap(R_on_green);
            currentSessionTimer->start(1000);
        }
    }
}

void MainWindow::upButtonPress()
{
    if (powerOn && !testConnectionTimer->isActive())
    {
        std::cout << "Up button pressed" << std::endl;

        if (softAnimation == 1)
        {
            std::cout<<"Interrupt!"<<std::endl;
        }

        if (currentSession != NULL)
        {
            if (currentSession->getIntensity() < 8)
            {
                currentSession->setIntensity(currentSession->getIntensity() + 0.5);
            }
            else
            {
                currentSession->setIntensity(8);
            }

            softAnimation = 3;
            softAnimationtTimer->start(250);
            displayIntensityLevel();
        }
        else if (createUserDesignated == 2)
        {
            // change custom length for user designated session
            if (customLength < 8) {
                customLength++;
            }
            display0To8Level(customLength);
        }
        else
        {
            switchType(1);
            // only flash sessions if there are available ones that are currently selected
            if (lengthPosition != 2 || sessions.size() == 3) {
                softAnimation = 4;
                softAnimationtTimer->start(250);
            }
            displaySessionSelect();
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
            if (currentSession->getIntensity() > 0)
            {
                currentSession->setIntensity(currentSession->getIntensity() - 0.5);
            }
            else
            {
                currentSession->setIntensity(0);
            }

            softAnimation = 3;
            softAnimationtTimer->start(250);
            displayIntensityLevel();
        }
        else if (createUserDesignated == 2)
        {
            // change custom length for user designated session
            if (customLength > 0) {
                customLength--;
            }
            display0To8Level(customLength);
        }
        else
        {
            switchType(-1);
            // only flash sessions if there are available ones that are currently selected
            if (lengthPosition != 2 || sessions.size() == 3) {
                softAnimation = 4;
                softAnimationtTimer->start(250);
            }
            displaySessionSelect();
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
    {
        return;
    }

    if (elapsed >= 1000 && (currentSession != NULL))
    {
        // Long Press to save preference
        std::cout << typePosition << std::endl;

        if (lengthPosition == 2 && sessions[2].size() == 1) {
            typePosition = 0;
        }

        sessions[lengthPosition][typePosition]->setIntensity(currentSession->getIntensity());
        std::cout << "Intensity Saved: " <<sessions[lengthPosition][typePosition]->getIntensity()<< std::endl;
    }
    else if (elapsed >= 1000 && createUserDesignated)
    {
        std::cout << "Exiting User Designated Mode" << std::endl;
        createUserDesignated = 0;
        customLength = 0;
        displaySessionSelect();
    }
    else if (elapsed >= 1000 && lengthPosition == 2)
    {
        // Long Press while selecting user designated to create a user designated session

        // TODO: should there be a way to overwrite/delet old sessions if we want new ones but already have 8 slots filled
        if (sessions.size() == 3) {
            if (sessions[2].size() >= 8) {
                std::cout << "No more slots left for user designated sessions" << std::endl;
            }
        }
        std::cout << "Entering User Designated Mode" << std::endl;
        createUserDesignated = 1;
    }
    else if (createUserDesignated == 1)
    {
        // Type for the user designated session has been set
        std::cout << "User Designated Type Set: " << typePosition << std::endl;
        display0To8Level(0);
        createUserDesignated = 2;
    }
    else if (powerOn && (currentSession == NULL))
    {
        // 0, 1 mean pre-defined groups or 2 for saved user designated sessions
        if (createUserDesignated == 2)
            {
                // Length (duration) for the user designated session has been set
                std::cout << "Created User Designated Session" << std::endl;

                // TODO: set the new session
                QVector<QString> t = {"Sub Delta", "Delta", "Alpha", "Theta"};
                QVector<QString> f = {"1.75", "3.75", "7", "10"}; // types are actually just sessions (sub delta, delta, etc)

                // if this is the first custom user session, create a new vector
                if (sessions.size() < 3) {
                    QVector<Session *> newVector;
                    sessions.push_back(newVector);
                }

                // minimum length is 20, and increases in 20 minute intervals
                Session *newS = new Session(QString::number(20+customLength*20),t[typePosition],f[typePosition],0);
                sessions[2].push_back(newS);

                currentSession = new Session(newS);

                createUserDesignated = 0;
                customLength = 0;
            }
        else if (lengthPosition != 2 || sessions.size() == 3)
        {
            std::cout << "Session selected" << std::endl;
            currentSession = new Session(sessions[lengthPosition][typePosition]);
        }
        else
        {
            std::cout << "No user designated sessions" << std::endl;
            return;
        }

        startSession();
    }
    else if (currentSession != NULL)
    {
        // disable regular press during session
        return;
    }
    else
    {
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
        recorder.setPending();
    }
    else
    {
        std::cout << "No session to record" << std::endl;
    }
}

void MainWindow::printRecordedButtonPress()
{
    recorder.print();
}

void MainWindow::startSession()
{
    if (testConnection())
    {
        currentSession->print();
    }
}

void MainWindow::endSession()
{
    std::cout << "Session End" << std::endl;

    // record session
    if (recorder.getPending())
    {
        recorder.recordSession(currentSession, true);
    }

    // pause timer
    currentSessionTimer->stop();

    // remove current session
    delete currentSession;
    currentSession = NULL;

    skinConnection = false;
    elaspedTime = 0;

}

void MainWindow::initializeTimer()
{
    if (skinConnection)
    {
        currentSessionTimer->start(1000);
    }
}

void MainWindow::updateTime()
{
    depleteBattery();

    if(elaspedTime == currentSession->getSessionLength().toInt())
    {
        this->powerOn = false;
        handlePowerOff();
        return;
    }

    if ((elaspedTime % 5) == 0)
    {
        stopFlashing();
        displayBatteryLevel();
    }
    else
    {
        displayIntensityLevel();
    }
}

void MainWindow::depleteBattery()
{
    elaspedTime += 1;
    batteryLvl -= (currentSession->getIntensity() > 1) ? (0.2+(currentSession->getIntensity() / 10)) : (0.2);

    std::cout << "Battery Level: " << batteryLvl << std::endl;

    if (batteryLvl == 0)
    {
        powerOn = false;
        handlePowerOff();
    }
}

void MainWindow::displayBatteryLevel()
{
    // convert battery level to be out of 8 instead of 100, since we have 8 lights to show level
    // needed since otherwise when battery below ~12% it would otherwise show no bars
    int levelToDisplay = (batteryLvl * 0.8)/10;

    if (levelToDisplay < 1)
    {
        levelToDisplay = 1;
    }

    display0To8Level(levelToDisplay);
}

void MainWindow::displayIntensityLevel()
{
    int levelToDisplay = floor(currentSession->getIntensity()); // round intensity level down (6.5 displays 6, 5.5, displays 5, etc.)

    display0To8Level(levelToDisplay);
}

void MainWindow::displaySessionSelect()
{
    int availableToSelect = 0;

    // if user defined sessions is empty, sessions.size() will possibly be 1 less than lengthPosition
    if (sessions.size() > lengthPosition)
    {
        availableToSelect = sessions[lengthPosition].size();
    }
    for (int i = 0; i < availableToSelect; i++)
    {
        sessionNumLabel[i]->setStyleSheet(sessionNum_on[i]);
    }
    for (std::size_t i = availableToSelect; i < 8; i++)
    {
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

    // light up LED strip
    ui->ledLabel->setStyleSheet("background-color:green");

    // force the battery level to display for 1 second
    QWidget::repaint();
    sleep(1);
}

void MainWindow::handlePowerOff()
{
    if (currentSession != NULL)
    {
        endSession();
    }

    softAnimation = 2;
    numToFlash = 7;
    softAnimationtTimer->start(250);

    //TODO: Reset menu select options. When the powered up again the menu jumps to the a select since no reset.

    // update interface
    for (std::size_t i = 0; i < sessionLengthLabel.size(); i++)
    {
        sessionLengthLabel[i]->setPixmap(sessionLength_off[i]);
    }
    for (std::size_t i = 0; i < sessionTypeLabel.size(); i++)
    {
        sessionTypeLabel[i]->setPixmap(sessionType_off[i]);
    }
    for (std::size_t i = 0; i < sessionNumLabel.size(); i++)
    {
        sessionNumLabel[i]->setStyleSheet(sessionNum_off);
    }

    ui->ledLabel->setStyleSheet("background-color:white");
    ui->cesLeftLabel->setPixmap(L_off);
    ui->cesRightLabel->setPixmap(R_off);
    ui->cesShortLabel->setPixmap(shortCES_off);
    ui->cesDutyLabel->setPixmap(dutyCES_off);
}

void MainWindow::switchGroups()
{
    // start by turning all appropriate labels "off"
    for (std::size_t i = 0; i < sessionLengthLabel.size(); i++)
    {
        sessionLengthLabel[i]->setPixmap(sessionLength_off[i]);
    }

    lengthPosition++;

    // for groups, every iteration just increments by 1, until end reached, then return to beginning
    if (lengthPosition > (int)sessionLengthLabel.size() - 1)
    {
        lengthPosition = 0;
    }

    // turn "on" selected group label
    sessionLengthLabel[lengthPosition]->setPixmap(sessionLength_on[lengthPosition]);

    // on switching group, go to default type/session position and flash selection
    typePosition = 0;
    if (lengthPosition != 2 || sessions.size() == 3) {
        softAnimation = 4;
        softAnimationtTimer->start(250);
    } else {
        softAnimationtTimer->stop();
    }
}

void MainWindow::switchType(int direction)
{
    // start by turning all appropriate labels "off"
    for (std::size_t i = 0; i < sessionTypeLabel.size(); i++)
    {
        sessionTypeLabel[i]->setPixmap(sessionType_off[i]);
    }

    typePosition += direction;

    // turn "on" selected type label
    if (lengthPosition == 2 && sessions.size() == 3 && createUserDesignated == 0) {
        // custom for displaying the user designated ones

        // for types, every iteration is either +1 or -1, if out of bounds, wrap around.
        if (typePosition > (int)sessions[2].size() - 1)
        {
            typePosition = 0;
        }
        else if (typePosition < 0)
        {
            typePosition = sessions[2].size() - 1;
        }

        QVector<QString> t = {"Sub Delta", "Delta", "Alpha", "Theta"};
        QString type = sessions[2][typePosition]->getType();
        for (int i = 0; i < t.size(); i++) {
            if (type == t[i]) {
                sessionTypeLabel[i]->setPixmap(sessionType_on[i]);
            }
        }
    } else {
        // for types, every iteration is either +1 or -1, if out of bounds, wrap around.
        if (typePosition > (int)sessionTypeLabel.size() - 1)
        {
            typePosition = 0;
        }
        else if (typePosition < 0)
        {
            typePosition = sessionTypeLabel.size() - 1;
        }
        sessionTypeLabel[typePosition]->setPixmap(sessionType_on[typePosition]);
    }
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
            ui->cesLeftLabel->setPixmap(L_on_green);
            ui->cesRightLabel->setPixmap(R_on_green);
        }
        else
        {
            // okay connection
            sessionNumLabel[3]->setStyleSheet(sessionNum_on[3]);
            sessionNumLabel[4]->setStyleSheet(sessionNum_on[4]);
            sessionNumLabel[5]->setStyleSheet(sessionNum_on[5]);
            ui->cesLeftLabel->setPixmap(L_on_yellow);
            ui->cesRightLabel->setPixmap(R_on_yellow);
        }

        badConnection = false;
        return true;
    }
    else
    {
        // no connection
        badConnection = true;
        ui->cesLeftLabel->setPixmap(L_on_red);
        ui->cesRightLabel->setPixmap(R_on_red);
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

    softAnimation = 1;
    numToFlash = 0;
    softAnimationtTimer->start(250);
    displayIntensityLevel();
    initializeTimer();
}

void MainWindow::blinkNum()
{
    if (softAnimation == 1)
    {
        softOn();
    }
    else if (softAnimation == 2)
    {
        softOff();
    }
    else if (softAnimation == 3)
    {
        savingAnimation();
    }
    else if (softAnimation == 4)
    {
        sessionAnimation();
    }
}

void MainWindow::softOn()
{
    if (numToFlash < 8)
    {
        for (int i = 0; i < 8; i++)
        {
            sessionNumLabel[i]->setStyleSheet(sessionNum_off);
        }

        sessionNumLabel[numToFlash]->setStyleSheet(sessionNum_on[numToFlash]);
    }
    else
    {
        softAnimationtTimer->stop();
        softAnimation = 0;
    }

    numToFlash++;
}

void MainWindow::softOff()
{
    if (numToFlash > -1)
    {
        for (int i = 7; i > 0; i--)
        {
            sessionNumLabel[i]->setStyleSheet(sessionNum_off);
        }

        sessionNumLabel[numToFlash]->setStyleSheet(sessionNum_on[numToFlash]);
    }
    else
    {
        softAnimationtTimer->stop();
        softAnimation = 0;
        sessionNumLabel[0]->setStyleSheet(sessionNum_off);
    }

    numToFlash--;
}

void MainWindow::savingAnimation()
{
//    std::cout<<"Intensity: " << currentSession->getIntensity() << " : " << floor(currentSession->getIntensity()) << std::endl;

    int leveltoFlash = floor(currentSession->getIntensity()) - 1;

    if (leveltoFlash < 0)
    {
        return;
    }

    flash0To8Level(leveltoFlash);
}

void MainWindow::sessionAnimation()
{
    flash0To8Level(typePosition);
}

void MainWindow::stopFlashing()
{
    softAnimationtTimer->stop();
}

void MainWindow::display0To8Level(int levelToDisplay) {
    for (int i = 0; i < levelToDisplay; i++) {
        sessionNumLabel[i]->setStyleSheet(sessionNum_on[i]);
    }
    for (std::size_t i = levelToDisplay; i < sessionNumLabel.size(); i++) {
        sessionNumLabel[i]->setStyleSheet(sessionNum_off);
    }
}

void MainWindow::flash0To8Level(int valueToFlash) {
    if (flashValue)
    {
        sessionNumLabel[valueToFlash]->setStyleSheet(sessionNum_on[valueToFlash]);
    }
    else
    {
        sessionNumLabel[valueToFlash]->setStyleSheet(sessionNum_off);
    }

    flashValue = !flashValue;
}
