#include "recording.h"

Recording::Recording()
{
    recordingPending = false;
}

Recording::~Recording() {}

bool Recording::getPending()
{
    return recordingPending;
}

void Recording::recordSession(Session *toRecord, bool completed)
{
    std::ofstream recordFile;
    // TODO: do we need to change the output directory to the non-debug folder?
    recordFile.open(fileName.toStdString(), std::fstream::app);

    // to check where the .txt will be created/looked for
    // char cwd[256];
    // getcwd(cwd, 256);
    // std::cout << cwd << std::endl;

    if (!recordFile)
    {
        std::cout << "Could not open/make file " << fileName.toStdString() << std::endl;
        return;
    }

    recordFile.seekp(0, std::ios::end);

    if (completed)
    {
        recordFile << "[Completed]\n";
    }
    else
    {
        recordFile << "[Inturrupted]\n";
    }

    recordFile << "Session Type: " << toRecord->getType().toStdString() << ", Frequency: " << toRecord->getFrequency().toStdString()
               << "\nTotal Duration: " << toRecord->getSessionLength().toStdString()
               << "\nIntensity: " << toRecord->getIntensity() << "\n\n";

    // std::cout << "Recorded session - type: " << toRecord->getType().toStdString()
        // << ", freq: " << toRecord->getFrequency().toStdString() << ", intensity: "
        // << toRecord->getIntensity()<< ", length: " << toRecord->getSessionLength().toStdString() << std::endl;

    recordFile.close();
    recordingPending = false;
}

void Recording::print()
{
    std::cout << "\n[Printout of recorded sessions]" << std::endl;

    std::ifstream inFile(fileName.toStdString());

    if (!inFile)
    {
        std::cout << "Could not open/make file " << fileName.toStdString() << std::endl;
        return;
    }

    std::string nextLine;
    while (std::getline(inFile, nextLine))
    {
        std::cout << nextLine << std::endl;
    }

    inFile.close();
}
