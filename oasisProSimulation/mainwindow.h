#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QElapsedTimer>
#include <QString>
#include <QVector>
#include <QPair>
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <unistd.h>

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
        bool badConnection;
        bool connectionButtonsLit;
        bool recordingPending; // if true, a session needs to be recorded
        double batteryLvl; // battery level ranging from 0-100
        float intensityLvl; // current session intensity level
        int sessionLength; // total current session length (in minutes)
        int elaspedTime; // elasped time for the current session, increases by one on each timeout call by currentSessionTimer

        // TODO: currentState -> track device state, (0 = just powered on, 1 = session running, etc), more may be needed later

        // variables holding specific positions for vectors
        int lengthPosition = 0;
        int typePosition = 0;

        QElapsedTimer elapsedTimer;
        QElapsedTimer elapsedTimerConfirm;        //For the Confirm Button to save or start session
        Ui::MainWindow *ui;
        QTimer* currentSessionTimer; // timer that calls timeout() to reduce battery level and check if session time is up
        QTimer* testConnectionTimer;
        Session* currentSession; // current session being used

        // vector of pairs of strings to session pointer vectors
        /* General Structure
         * Outer vector stores pairs, 0 = "20" pair, 1 = "45" pair, 2 = "User" pair
         * Each pair associates a group type to a vector of sessions with that group type (ie, "20", "45", "User")
         * Inner vector stores pointers to sessions within the pairs
         * In this case, sessions found under "20" and "45" are the pre-defined default sessions, "User" is empty to start
         */
        QVector<QPair<QString, QVector<Session *>>> sessions; // all possible sessions

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

        /// Display the current battery level using session number labels
        void displayBatteryLevel();

        /// Display the current Intensity level using session number labels
        void displayIntensityLevel();

        /// Light up session numbers that have available sessions to select
        void displaySessionSelect();

        /// Notify user when batteryLvl is low
        void onBatteryLow();

        /// Set currentSession and start currentSessionTimer
        void startSession(Session* s);

        /// End currentSession and currentSessionTimer
        void endSession();

        /// Record last session
        void recordSession();

        /// Adjust intensityLvl to newIntensity             (could remove maybe idk - Gurpiar)
        void adjustIntensity(int newIntensity);

        /// Perform connection test
        bool testConnection();

        /// Switch between different session groups (ie, lengths)
        void switchGroups();

        /// Switch between different session types
        void switchType(int direction);

        /// Starts Timer and Calls depleteBattery();
        void initializeTimer();

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
        void confirmButtonRelease();
        void recordButtonPress();
        void loadUserDesignedButtonPress();

        /// Deplete batteryLvl by a variable level depending on length, intensity, and skin connection
        /// Here for now, needs to be a slot for timeout
        void depleteBattery();

        /// Handles connection status blinking
        void flashConnection();

        /// Finishes connection test
        void stopConnectionTest();
};
#endif // MAINWINDOW_H
