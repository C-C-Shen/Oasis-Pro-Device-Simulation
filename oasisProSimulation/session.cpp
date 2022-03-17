#include "session.h"

Session::Session(QString sType, QString freq)
{
    type = sType;
    frequency = freq;
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
