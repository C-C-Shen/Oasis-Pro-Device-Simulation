#include "displaymanager.h"
#include "ui_mainwindow.h"

DisplayManager::DisplayManager(Ui::MainWindow *uiToTarget)
{
    ui = uiToTarget;

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

DisplayManager::~DisplayManager()
{

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
}

void DisplayManager::testConnectionLights(int num)
{
    if (num == 1)
    {
        sessionNumLabel[0]->setStyleSheet(sessionNum_on[0]);
        sessionNumLabel[1]->setStyleSheet(sessionNum_on[1]);
        sessionNumLabel[2]->setStyleSheet(sessionNum_on[2]);
        ui->cesLeftLabel->setPixmap(L_on_green);
        ui->cesRightLabel->setPixmap(R_on_green);
    }
    else if (num == 2)
    {
        sessionNumLabel[3]->setStyleSheet(sessionNum_on[3]);
        sessionNumLabel[4]->setStyleSheet(sessionNum_on[4]);
        sessionNumLabel[5]->setStyleSheet(sessionNum_on[5]);
        ui->cesLeftLabel->setPixmap(L_on_yellow);
        ui->cesRightLabel->setPixmap(R_on_yellow);
    }
    else
    {
        ui->cesLeftLabel->setPixmap(L_on_red);
        ui->cesRightLabel->setPixmap(R_on_red);
    }
}

void DisplayManager::flashConnection(QString temp, bool connectionLit, bool bad)
{
    // Flash duty or short CES depending on the select session
    if (temp == "1.75")
    {
        if (connectionLit)
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
        if (connectionLit)
        {
            ui->cesShortLabel->setPixmap(shortCES_off);
        }
        else
        {
            ui->cesShortLabel->setPixmap(shortCES_on);
        }
    }

    if (bad)
    {
        // flash bad connection on/off
        if (connectionLit)
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
}

void DisplayManager::connectionTestOff()
{
    ui->cesDutyLabel->setPixmap(dutyCES_off);
    ui->cesShortLabel->setPixmap(shortCES_off);
}

void DisplayManager::connection(bool connected)
{
    if(connected)
    {
        ui->cesLeftLabel->setPixmap(L_on_green);
        ui->cesRightLabel->setPixmap(R_on_green);

    }
    else
    {
        ui->cesLeftLabel->setPixmap(L_on_red);
    }
}

void DisplayManager::displayBatteryLevel(int battery)
{
    // convert battery level to be out of 8 instead of 100, since we have 8 lights to show level
    // needed since otherwise when battery below ~12% it would otherwise show no bars
    int levelToDisplay = (battery * 0.8)/10;

    if (levelToDisplay < 1)
    {
        levelToDisplay = 1;
    }

    display0To8Level(levelToDisplay);
}

void DisplayManager::displayIntensityLevel(int intensity)
{
    int levelToDisplay = floor(intensity); // round intensity level down (6.5 displays 6, 5.5, displays 5, etc.)

    display0To8Level(levelToDisplay);
}

void DisplayManager::displaySessionSelect(int selection, int numGroups, int groupSize)
{
    int availableToSelect = 0;

    // if user defined sessions is empty, sessions.size() will possibly be 1 less than lengthPosition
    if (numGroups > selection)
    {
        availableToSelect = groupSize;
    }
    for (int i = 0; i < availableToSelect; i++)
    {
        sessionNumLabel[i]->setStyleSheet(sessionNum_on[i]);
    }
    for (std::size_t i = availableToSelect; i < 8; i++)
    {
        sessionNumLabel[i]->setStyleSheet(sessionNum_off);
    }
}

void DisplayManager::display0To8Level(int levelToDisplay)
{
    for (int i = 0; i < levelToDisplay; i++) {
        sessionNumLabel[i]->setStyleSheet(sessionNum_on[i]);
    }
    for (std::size_t i = levelToDisplay; i < sessionNumLabel.size(); i++) {
        sessionNumLabel[i]->setStyleSheet(sessionNum_off);
    }
}

void DisplayManager::flash0To8Level(int valueToFlash, bool *flashVal)
{
    if (*flashVal)
    {
        sessionNumLabel[valueToFlash]->setStyleSheet(sessionNum_on[valueToFlash]);
    }
    else
    {
        sessionNumLabel[valueToFlash]->setStyleSheet(sessionNum_off);
    }

    *flashVal = !*flashVal;
}

void DisplayManager::lightsOff()
{
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
