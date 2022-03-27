#ifndef SESSION_H
#define SESSION_H

#include <QString>

class Session
{
    public:
        Session(QString sType, QString freq, int intense);
        ~Session();

        QString getSessionType();
        QString getFrequency();
        int getIntensity();
        void setIntensity(int intense);

    private:
        QString type; // session type (ex. Delta)
        QString frequency; // frequency level (ex. 0.5 Hz)
        int intensity; // intensity level
};

#endif // SESSION_H
