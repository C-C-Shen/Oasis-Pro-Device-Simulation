#include "session.h"

Session::Session(QString sLength, QString sType, QString freq, int intense)
{
    length = sLength;
    type = sType;
    frequency = freq;
    intensity = intense;
}

Session::~Session() {}

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

double Session::getIntensity()
{
    return intensity;
}

void Session::setIntensity(double i)
{
    if (i >= 0 && i <= 8)
    {
        intensity = i;
    }
    else
    {
        std::cout << "Invalid intensity setting" << std::endl;
    }
}
