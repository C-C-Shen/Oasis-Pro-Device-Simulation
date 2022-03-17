#ifndef SESSION_H
#define SESSION_H

#include <QString>

class Session
{
    public:
        Session(QString sType, QString freq);
        ~Session();

        QString getSessionType();
        QString getFrequency();

    private:
        QString type; // session type (ex. Delta)
        QString frequency; // frequency level (ex. 0.5 Hz)
};

#endif // SESSION_H
