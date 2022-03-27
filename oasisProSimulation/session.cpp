#include "session.h"

Session::Session(QString sType, QString freq, int intense)
{
    type = sType;
    frequency = freq;
    intensity = intense;
}

Session::~Session(){}

QString Session::getSessionType()
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
