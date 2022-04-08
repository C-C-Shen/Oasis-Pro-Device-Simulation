#ifndef SESSION_H
#define SESSION_H

#include <QString>
#include <iostream>

class Session
{
    public:
        Session(QString sLength, QString sType, QString freq, int intense);
        Session(Session* s);
        ~Session();

        QString getSessionLength();
        QString getType();
        QString getFrequency();
        double getIntensity();
        void setIntensity(double i);
        void print() { std::cout << "Len. " << length.toStdString() << " Freq. " << frequency.toStdString() << " int. " << intensity << std::endl; }

    private:
        QString length; // session length/group (ex. 20 min)
        QString type; // the string version of the session type (Sub Delta, Delta, etc)
        QString frequency; // frequency level (ex. 0.5 Hz) <- this is also type (ex. 6-8 Hz = Theta)
        double intensity; // intensity level
};

#endif // SESSION_H
