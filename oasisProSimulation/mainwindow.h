#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QElapsedTimer>
#include <QString>
#include <QVector>
#include <iostream>
#include <vector>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "recording.h"
#include "session.h"
#include "displaymanager.h"

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
        bool flashValue;
        double batteryLvl; // battery level ranging from 0-100
        int elaspedTime; // elasped time for the current session, increases by one on each timeout call by currentSessionTimer
        int numToFlash;
        int softAnimation;
        int createUserDesignated; // 0 = not in mode, 1 = setting type, 2 = setting duration.
        int customLength; // used to set user designated session length

        // variables holding specific positions for vectors
        int lengthPosition = 0;
        int typePosition = 0;

        QString fileName = "treatment_history.txt";

        Recording recorder;
        DisplayManager *dm;

        QElapsedTimer elapsedTimer;
        QElapsedTimer elapsedTimerConfirm;        //For the Confirm Button to save or start session
        Ui::MainWindow *ui;
        QTimer* currentSessionTimer; // timer that calls timeout() to reduce battery level and check if session time is up
        QTimer* testConnectionTimer;
        QTimer* softAnimationtTimer;

        Session* currentSession; // current session being used

        // vector of pairs of strings to session pointer vectors
        /* General Structure
         * Outer vector stores vector grouped by lengths
         * In this case, sessions found under 0 = "20" and 1 = "45" are the pre-defined default sessions, 2 = "User" is empty to start
         */
        QVector<QVector<Session *>> sessions; // all possible sessions

        // functions
        /// Handle power on
        void handlePowerOn();

        /// Handle power off
        void handlePowerOff();

        /// Helper function to call the actual display function with correct parameters
        void displaySessionSelect();

        /// Notify user when batteryLvl is low
        void onBatteryLow();

        /// Set currentSession and start currentSessionTimer
        void startSession();

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

        /// Deplete batteryLvl by a variable level depending on length, intensity, and skin connection
        void depleteBattery();

        void softOn();
        void softOff();
        void savingAnimation();

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
        void printRecordedButtonPress();

        /// drains battery and updates GUI based on elaspedTime
        void updateTime();

        /// Handles connection status blinking
        void flashConnection();

        /// Finishes connection test
        void stopConnectionTest();

        void blinkNum();
        void stopFlashing();
};
#endif // MAINWINDOW_H
