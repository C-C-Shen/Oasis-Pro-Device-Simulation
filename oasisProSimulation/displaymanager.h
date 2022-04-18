#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <QLabel>
#include <math.h>
#include <QString>
#include <stdlib.h>
#include <iostream>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class DisplayManager
{
public:
    DisplayManager(Ui::MainWindow *uiToTarget);
    ~DisplayManager();

    // functions
    /// Test Connection
    void testConnectionLights(int num); // 0 = no connection, 1 = good, 2 = ok

    /// Flash connection
    void flashConnection(QString temp, bool connectionLit, bool bad);

    /// Turn off test lights
    void connectionTestOff();

    /// Light up L or R depending on connection
    void connection(bool connected);

    /// Display the current battery level using session number labels
    void displayBatteryLevel(int battery);

    /// Display the current Intensity level using session number labels
    void displayIntensityLevel(int intensity);

    /// Light up session numbers that have available sessions to select
    void displaySessionSelect(int selection, int numGroup, int groupSize);

    /// Helper to display correct bar level on 0 to 8 labels
    void display0To8Level(int levelToDisplay);

    /// Helper to flash the correct value on 0 to 8 labels
    void flash0To8Level(int valueToFlash, bool *flashVal);

    /// Helper to flash the correct two values on 0 to 8 labels
    void flashBatteryLow(int battery,bool *flashVal);

    /// Turn off a specified type light
    void typeOff(int target) { sessionTypeLabel[target]->setPixmap(sessionType_off[target]); }

    /// Turn on a specified type light
    void typeOn(int target) { sessionTypeLabel[target]->setPixmap(sessionType_on[target]); }

    /// Turn off a specified length light
    void lengthOff(int target) { sessionLengthLabel[target]->setPixmap(sessionLength_off[target]); }

    /// Turn on a specified length light
    void lengthOn(int target) { sessionLengthLabel[target]->setPixmap(sessionLength_on[target]); }

    /// Turn off a specified length light
    void numberOff(int target) { sessionNumLabel[target]->setStyleSheet(sessionNum_off); }

    /// Turn on a specified length light
    void numberOn(int target) { sessionNumLabel[target]->setStyleSheet(sessionNum_on[target]); }

    /// Turn all off
    void lightsOff();

    int getLengthLabelSize() { return (int)sessionLengthLabel.size(); }
    int getTypeLabelSize() { return (int)sessionTypeLabel.size(); }
    int getLengthNumSize() { return (int)sessionNumLabel.size(); }

private:
    Ui::MainWindow *ui;

    // vector of QLabel objects, to help iterate over which to turn on and off
    std::vector<QLabel *> sessionLengthLabel;
    std::vector<QLabel *> sessionTypeLabel;
    std::vector<QLabel *> sessionNumLabel;
    std::vector<QLabel *> sessionStimLabel;

    // have the "lights" be stored in vectors corresponding to their groups
    // Positions: 0 -> 20, 1 -> 45, 2 -> user def
    std::vector<QPixmap> sessionLength_on;
    std::vector<QPixmap> sessionLength_off;

    // Positions: 0 -> sub delta, 1 -> delta, 2 -> theta, 3 -> alpha
    std::vector<QPixmap> sessionType_on;
    std::vector<QPixmap> sessionType_off;

    // Positions: 0 -> 1, 1 -> 2, ... 7 -> 8
    std::vector<QString> sessionNum_on;
    QString sessionNum_off = "QLabel { color: #000000 }";

    // Positions: 0 -> 0.25, 1 -> 0.50, ...6 -> +1, 7 -> +2
    std::vector<QPixmap> sessionStim_on;
    std::vector<QPixmap> sessionStim_off;

    QPixmap L_on_green;
    QPixmap L_on_yellow;
    QPixmap L_on_red;
    QPixmap L_off;
    QPixmap R_on_green;
    QPixmap R_on_yellow;
    QPixmap R_on_red;
    QPixmap R_off;
    QPixmap dutyCES_on;
    QPixmap dutyCES_off;
    QPixmap shortCES_on;
    QPixmap shortCES_off;

    // functions
    void softOn();
    void softOff();
    void sessionAnimation();
    void savingAnimation();
};

#endif // DISPLAYMANAGER_H
