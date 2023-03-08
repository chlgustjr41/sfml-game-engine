#ifndef TIMELINE_H
#define TIMELINE_H

#include <chrono>

class Timeline
{
public:
    explicit Timeline(long tickSize);
    explicit Timeline(long tickSize, Timeline *anchorTimeline);

    double getTick();
    void increaseTick(double tickSize);
    void decreaseTick(double tickSize);

    bool getIsPaused();
    void pause();
    void resume();

    double getTime();
    double getDuration();
    std::chrono::milliseconds getAnchorTime();

private:
    Timeline *anchor;
    double tick;
    bool isPaused;
    std::chrono::milliseconds startTimeStamp;
    std::chrono::milliseconds previousTimeStamp;
};

#endif