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

        QElapsedTimer elapsedTimer;
        Ui::MainWindow *ui;
        QTimer currentSessionTimer; // timer that calls timeout() to reduce battery level and check if session time is up
        Session* currentSession; // current session being used
        std::vector<Session*> sessions; // all possible sessions

        QPixmap sessionLength20_on;
        QPixmap sessionLength20_off;
        QPixmap sessionLength45_on;
        QPixmap sessionLength45_off;
        QPixmap userDesigned_on;
        QPixmap userDesigned_off;
        QPixmap theta_on;
        QPixmap theta_off;
        QPixmap alpha_on;
        QPixmap alpha_off;
        QPixmap delta_on;
        QPixmap delta_off;
        QPixmap subDelta_on;
        QPixmap subDelta_off;
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
