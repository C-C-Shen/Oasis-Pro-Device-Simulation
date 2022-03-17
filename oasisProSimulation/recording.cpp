#include "recording.h"

Recording::Recording(int dur, int intensity, Session* session)
{
    duration = dur;
    intensityLvl = intensity;
    savedSession = session;
}

Recording::~Recording(){}

int Recording::getDuration()
{
    return duration;
}

int Recording::getIntensityLvl()
{
    return intensityLvl;
}

Session& Recording::getSavedSession()
{
    return *savedSession;
}
