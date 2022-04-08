#ifndef RECORDING_H
#define RECORDING_H

#include "session.h"
#include <string>
#include <fstream>

class Recording
{
    public:
        Recording();
        ~Recording();

        bool getPending();
        void setFile(QString f) { fileName = f; }
        void setPending() { recordingPending = true; }
        void recordSession(Session *toRecord, bool complete);
        void print(); // to help view the recorded sessions in console

    private:
        bool recordingPending;
        QString fileName;
};

#endif // RECORDING_H
