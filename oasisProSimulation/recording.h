#ifndef RECORDING_H
#define RECORDING_H

#include "session.h"

class Recording
{
    public:
        Recording(int dur, int intensity, Session* session);
        ~Recording();

        int getDuration();
        int getIntensityLvl();
        Session& getSavedSession();

    private:
        int duration;
        int intensityLvl;
        Session* savedSession;
};

#endif // RECORDING_H
