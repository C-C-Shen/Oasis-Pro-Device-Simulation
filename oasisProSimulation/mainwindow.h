#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

        // test for timing button presses
        QElapsedTimer elapsedTimer;

        // objects
        Ui::MainWindow *ui;
        // TODO: might be better way to implement powerHoldTimer (internal power button timer?)
        QTimer powerHoldTimer; // timer that keeps track of how long power button has been held
        QTimer currentSessionTimer; // timer that calls timeout() to reduce battery level and check if session time is up
        Session* currentSession; // current session being used
        std::vector<Session*> sessions; // all possible sessions

        // functions
        /// Initialize sessions
        void initSessions();

        /// Toggle powerOn bool true/false
        void togglePower();

        /// Charge batteryLvl to full
        void chargeBattery();

        /// Deplete batteryLvl by a variable level depending on length, intensity, and skin connection
        void depleteBattery();

        /// Notify user when batteryLvl is low
        void onBatteryLow();

        /// Set currentSession and start currentSessionTimer
        void startSession(Session* s);

        /// End currentSession and currentSessionTimer
        void endSession();

        /// Record last session
        void recordSession();

        /// Set skinConnection bool
        void connectToSkin(bool skinConnected);

        /// Adjust intensityLvl to newIntensity
        void adjustIntensity(int newIntensity);

        /// Perform connection test
        void testConnection();

    private slots:
        // button handling
        void powerButtonPress();
        void powerButtonRelease();

   /* TODO: implement slots
    * private slots:
        void on_powerButton_press();
        void on_powerButton_release();*/
};
#endif // MAINWINDOW_H
