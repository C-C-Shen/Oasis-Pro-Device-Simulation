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

    dm = new DisplayManager(ui);
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

    delete dm;
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
            dm->connection(false);
            currentSessionTimer->stop();
        }
    }

    if (!currentSessionTimer->isActive() && currentSession != NULL)
    {
        if(skinConnection)
        {
            dm->connection(true);
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
            dm->displayIntensityLevel(currentSession->getIntensity());
        }
        else if (createUserDesignated == 2)
        {
            // change custom length for user designated session
            if (customLength < 8) {
                customLength++;
            }
            dm->display0To8Level(customLength);
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
            dm->displayIntensityLevel(currentSession->getIntensity());
        }
        else if (createUserDesignated == 2)
        {
            // change custom length for user designated session
            if (customLength > 0) {
                customLength--;
            }
            dm->display0To8Level(customLength);
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
        dm->display0To8Level(0);
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

        softAnimationtTimer->stop();
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
        dm->displayBatteryLevel(batteryLvl);
    }
    else
    {
        dm->displayIntensityLevel(currentSession->getIntensity());
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

void MainWindow::displaySessionSelect()
{
    if (sessions.size() > lengthPosition) {
        dm->displaySessionSelect(lengthPosition, sessions.size(), sessions[lengthPosition].size());
    } else {
        dm->displaySessionSelect(lengthPosition, sessions.size(), 0);
    }
}

void MainWindow::handlePowerOn()
{
    // display device power level
    dm->displayBatteryLevel(batteryLvl);

    // light up default value labels
    dm->lengthOn(lengthPosition);
    dm->typeOn(typePosition);

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

    // update interface
    dm->lightsOff();
}

void MainWindow::switchGroups()
{
    // start by turning all appropriate labels "off"
    for (int i = 0; i < dm->getLengthLabelSize(); i++)
    {
        dm->lengthOff(i);
    }

    lengthPosition++;

    // for groups, every iteration just increments by 1, until end reached, then return to beginning
    if (lengthPosition > dm->getLengthLabelSize() - 1)
    {
        lengthPosition = 0;
    }

    // turn "on" selected group label
    dm->lengthOn(lengthPosition);

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
    for (int i = 0; i < dm->getLengthLabelSize(); i++)
    {
        dm->typeOff(i);
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
                dm->typeOn(i);
            }
        }
    } else {
        // for types, every iteration is either +1 or -1, if out of bounds, wrap around.
        if (typePosition > dm->getLengthLabelSize() - 1)
        {
            typePosition = 0;
        }
        else if (typePosition < 0)
        {
            typePosition = dm->getLengthLabelSize() - 1;
        }
        dm->typeOn(typePosition);
    }
}

bool MainWindow::testConnection()
{
    // Start connection test and end it after 5 seconds
    testConnectionTimer->start(1000);
    testConnectionTimer->singleShot(5000, this, SLOT(stopConnectionTest()));

    for (int i = 0; i < dm->getLengthNumSize(); i++)
    {
        dm->numberOff(i);
    }

    if (skinConnection)
    {
        // randomize either okay or excellent connection if skin is connected
        if ((rand() % 2) == 1)
        {
            // excellent connection
            dm->testConnectionLights(1);
        }
        else
        {
            // okay connection
            dm->testConnectionLights(2);
        }

        badConnection = false;
        return true;
    }
    else
    {
        // no connection
        badConnection = true;
        dm->testConnectionLights(0);
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

    dm->flashConnection(tempFreq, connectionButtonsLit, badConnection);
    connectionButtonsLit = !connectionButtonsLit;
}

void MainWindow::stopConnectionTest()
{
    // Stop connection test, reset vars, and attempt to initialize timer
    testConnectionTimer->stop();
    connectionButtonsLit = false;
    dm->connectionTestOff();

    softAnimation = 1;
    numToFlash = 0;
    softAnimationtTimer->start(250);
    dm->displayIntensityLevel(currentSession->getIntensity());
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
        dm->flash0To8Level(typePosition, &flashValue);
    }
}

void MainWindow::softOn()
{
    if (numToFlash < 8)
    {
        for (int i = 0; i < 8; i++)
        {
            dm->numberOff(i);
        }

        dm->numberOn(numToFlash);
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
            dm->numberOff(i);
        }

        dm->numberOn(numToFlash);
    }
    else
    {
        softAnimationtTimer->stop();
        softAnimation = 0;
        dm->numberOff(0);
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

    dm->flash0To8Level(leveltoFlash, &flashValue);
}

void MainWindow::stopFlashing()
{
    softAnimationtTimer->stop();
}
