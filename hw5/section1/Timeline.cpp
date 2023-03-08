#include "Timeline.h"

Timeline::Timeline(long tickSize)
{
    Timeline::tick = tickSize;
    Timeline::anchor = {};
    Timeline::isPaused = false;
    Timeline::startTimeStamp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch();
    Timeline::previousTimeStamp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch();
}

Timeline::Timeline(long tickSize, Timeline *anchorTimeline)
{
    Timeline::tick = tickSize;
    Timeline::anchor = anchorTimeline;
    Timeline::isPaused = false;
    Timeline::startTimeStamp = anchorTimeline->getAnchorTime();
    Timeline::previousTimeStamp = anchorTimeline->getAnchorTime();
}

bool Timeline::getIsPaused()
{
    return isPaused;
}
void Timeline::pause()
{
    Timeline::isPaused = true;
}
void Timeline::resume()
{
    if (!Timeline::anchor)
    {
        Timeline::previousTimeStamp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch();
    }
    else
    {
        Timeline::previousTimeStamp = Timeline::anchor->getAnchorTime();
    }
    Timeline::isPaused = false;
}

double Timeline::getTick()
{
    return Timeline::tick;
}
void Timeline::increaseTick(double tickSize)
{
    Timeline::tick += tickSize;
}
void Timeline::decreaseTick(double tickSize)
{
    Timeline::tick -= tickSize;
}

double Timeline::getTime()
{
    if (!Timeline::anchor)
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch() - Timeline::startTimeStamp).count() / Timeline::tick;
    }
    else
    {
        return (Timeline::anchor->getAnchorTime() - Timeline::startTimeStamp).count() / Timeline::tick;
    }
}

double Timeline::getDuration()
{
    if (!Timeline::isPaused)
    {
        if (!Timeline::anchor)
        {
            long duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch() - Timeline::previousTimeStamp).count();
            Timeline::previousTimeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch());
            return duration / Timeline::tick;
        }
        else
        {
            long duration = (Timeline::anchor->getAnchorTime() - Timeline::previousTimeStamp).count();
            Timeline::previousTimeStamp = Timeline::anchor->getAnchorTime();
            return duration / Timeline::tick;
        }
    }
    else
        return 0;
}

std::chrono::milliseconds Timeline::getAnchorTime()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>((std::chrono::steady_clock::now().time_since_epoch() - Timeline::startTimeStamp) / Timeline::tick);
}