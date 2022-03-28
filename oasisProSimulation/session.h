#ifndef SESSION_H
#define SESSION_H

#include <QString>
#include <iostream>

class Session
{
    public:
        Session(QString sLength, QString freq, int intense);
        ~Session();

        QString getSessionLength();
        QString getFrequency();
        int getIntensity();
        void setIntensity(int intense);
        void print() { std::cout << "Len. " << length.toStdString() << " Freq. " << frequency.toStdString() << " int. " << intensity << std::endl; }

    private:
        QString length; // session length/group (ex. 20 min)
        QString frequency; // frequency level (ex. 0.5 Hz) <- this is also type (ex. 6-8 Hz = Theta)
        int intensity; // intensity level
};

#endif // SESSION_H
