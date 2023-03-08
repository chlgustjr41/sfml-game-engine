#ifndef EVENTOBJECT_H
#define EVENTOBJECT_H

#include <string>

class EventObject
{
public:
    static const std::string CHARACTER_INPUT;
    static const std::string CHARACTER_DEATH;
    static const std::string CHARACTER_SPAWN;
    static const std::string CHARACTER_PLATFORM_COLLIDE;

    explicit EventObject(std::string raisedEventType,
                         int eventPriority,
                         std::string sourceObjectHeader,
                         std::string eventContent);

    std::string getEventType();
    int getPriority();
    std::string getSoruceHeader();
    std::string getContent();

private:
    std::string eventType;
    int priority;
    std::string sourceHeader;
    std::string content;
};

#endif