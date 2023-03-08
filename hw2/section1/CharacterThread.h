#include "ThreadObject.h"

#ifndef CHARACTERTHREAD_H 
#define CHARACTERTHREAD_H

class CharacterThread: public ThreadObject
{
    public :
        static const int CHARACTER_MOVEMENT = 10;
        static const int JUMP_HEIGHT = 15;
        static const int JUMP_FRAME = 60;

        // Constructor
        explicit CharacterThread(ThreadObject **objs = NULL,
                                    int count = 0,
                                    const sf::Vector2f &wSize = sf::Vector2f(0.f, 0.f),
                                    const sf::Vector2f &size = sf::Vector2f(0.f, 0.f), 
                                    const sf::Vector2f &position = sf::Vector2f(0.f, 0.f));
        // Getter
        bool getIsMovingLeft();
        bool getIsMovingRight();
        bool getIsJumping();

        // Setter
        void setIsMovingLeft(bool isMoving);
        void setIsMovingRight(bool isMoving);
        void setIsJumping(bool isMoving);

        // Run function tp execute in a thread
        void run();
    private:
        bool isMovingLeft;
        bool isMovingRight;
        bool isJumping;
        int jumpTime;
        bool isOnMovingPlatform;
        int movingPlatformPositionDifference;

        ThreadObject **objects;
        int objectCount;

        int isIntercected();
};

#endif