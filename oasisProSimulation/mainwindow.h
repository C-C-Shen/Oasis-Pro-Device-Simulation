#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QElapsedTimer>
#include <iostream>
#include <vector>

#include "recording.h"
#include "session.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        // variables
        bool powerOn;
        bool skinConnection;
        double batteryLvl; // battery level ranging from 0-100
        int intensityLvl; // current session intensity level
        int sessionLength; // total current session length (in minutes)

        // variables holding specific positions for vectors
        int lengthPosition = 0;
        int typePosition = 0;

        QElapsedTimer elapsedTimer;
        Ui::MainWindow *ui;
        QTimer currentSessionTimer; // timer that calls timeout() to reduce battery level and check if session time is up
        Session* currentSession; // current session being used
        std::vector<Session*> sessions; // all possible sessions

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
        QString sessionNum_off;

        // Positions: 0 -> 0.25, 1 -> 0.50, ...6 -> +1, 7 -> +2
        std::vector<QPixmap> sessionStim_on;
        std::vector<QPixmap> sessionStim_off;

        QPixmap L_on;
        QPixmap L_off;
        QPixmap R_on;
        QPixmap R_off;
        QPixmap dutyCES_on;
        QPixmap dutyCES_off;
        QPixmap shortCES_on;
        QPixmap shortCES_off;

        // functions
        /// Initialize sessions
        void initSessions();

        /// Handle power on
        void handlePowerOn();

        /// Handle power off
        void handlePowerOff();

        /// Deplete batteryLvl by a variable level depending on length, intensity, and skin connection
        void depleteBattery();

        /// Display the current battery level using session number labels
        void displayBatteryLevel();

        /// Notify user when batteryLvl is low
        void onBatteryLow();

        /// Set currentSession and start currentSessionTimer
        void startSession(Session* s);

        /// End currentSession and currentSessionTimer
        void endSession();

        /// Record last session
        void recordSession();

        /// Adjust intensityLvl to newIntensity
        void adjustIntensity(int newIntensity);

        /// Perform connection test
        void testConnection();

        /// Switch between different session groups (ie, lengths)
        void switchGroups();

        /// Switch between different session types
        void switchType(int direction);

    private slots:
        // button handling
        void powerButtonPress();
        void powerButtonRelease();

        /// Charge batteryLvl to full
        void chargeBattery();

        /// Toggle skinConnection bool
        void connectToSkin();

        void upButtonPress();
        void downButtonPress();
        void confirmButtonPress();
};
#endif // MAINWINDOW_H
