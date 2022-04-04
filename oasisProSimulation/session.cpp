#include "session.h"

Session::Session(QString sLength, QString sType, QString freq, int intense)
{
    length = sLength;
    type = sType;
    frequency = freq;
    intensity = intense;
}

Session::~Session(){}

QString Session::getSessionLength()
{
    return length;
}

QString Session::getType()
{
    return type;
}

QString Session::getFrequency()
{
    return frequency;
}

int Session::getIntensity()
{
    return intensity;
}

void Session::setIntensity(int intense)
{
    if (intense >= 0 && intense <= 7) {
        intensity = intense;
    } else {
        std::cout << "Invalid intensity setting" << std::endl;
    }
}
