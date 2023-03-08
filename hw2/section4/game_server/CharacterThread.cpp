#include "CharacterThread.h"
#include "MovingPlatformThread.h"
#include <cmath>
#include <iostream>

// Constructor
CharacterThread::CharacterThread(ThreadObject **objs,
                                 int count,
                                 const sf::Vector2f &wSize,
                                 const sf::Vector2f &size,
                                 const sf::Vector2f &position) : ThreadObject(CHARACTER_TYPE, wSize, size, position)
{
    objects = objs;
    objectCount = count;
    isMovingLeft = false;
    isMovingRight = false;
    isJumping = false;
    jumpTime = 0;
    isOnMovingPlatform = false;
    movingPlatformPositionDifference = 0;
    (*this).setFillColor(sf::Color::Red);
}

// Getters
bool CharacterThread::getIsMovingLeft()
{
    return isMovingLeft;
}
bool CharacterThread::getIsMovingRight()
{
    return isMovingRight;
}
bool CharacterThread::getIsJumping()
{
    return isJumping;
}

// Setters
void CharacterThread::setIsMovingLeft(bool isMoving)
{
    isMovingLeft = isMoving;
}
void CharacterThread::setIsMovingRight(bool isMoving)
{
    isMovingRight = isMoving;
}
void CharacterThread::setIsJumping(bool isMoving)
{
    isJumping = isMoving;
}

// Check if any object intersects with the character. Returns the intersecting object's index. If no itersection, return -1
int CharacterThread::isIntercected()
{
    for (int i = 0; i < objectCount; i++)
    {
        if ((*this).getGlobalBounds().intersects((*objects[i]).getGlobalBounds()))
            return i;
    }

    isOnMovingPlatform = false;
    return -1;
}

// Run function tp execute in a thread
void CharacterThread::run()
{
    // Character left movement
    if (isMovingLeft && (*this).getPosition().x >= 0)
    {
        (*this).setPosition((*this).getPosition().x - CHARACTER_MOVEMENT * getDuration() * TIME_RATIO, (*this).getPosition().y);
    }
    // Character right movement
    if (isMovingRight && (*this).getPosition().x + (*this).getSize().x <= getWindowSize().x)
    {
        (*this).setPosition((*this).getPosition().x + CHARACTER_MOVEMENT * getDuration() * TIME_RATIO, (*this).getPosition().y);
    }

    int intersectingObjectIndex = isIntercected();
    // Free falling
    if (!isJumping &&
        intersectingObjectIndex == -1)
    {
        isJumping = true;
        jumpTime = JUMP_FRAME / 2;
    }
    // Character jumping movement
    if (isJumping)
    {
        // Jump logic for smooth jumping with different acceleration
        (*this).move(0, -JUMP_HEIGHT * std::cos(M_PI * jumpTime / JUMP_FRAME) * getDuration() * TIME_RATIO);
        if (jumpTime < JUMP_FRAME)
        {
            jumpTime += 1;
        }

        intersectingObjectIndex = isIntercected();
        if (intersectingObjectIndex != -1)
        {
            // Landing on top of a non-moving platform
            if ((*objects[intersectingObjectIndex]).getThreadName() != ThreadObject::MOVING_PLATFORM_TYPE &&
                (*this).getPosition().y < (*objects[intersectingObjectIndex]).getPosition().y)
            {
                isJumping = false;
                jumpTime = 0;
                (*this).setPosition((*this).getPosition().x, (*objects[intersectingObjectIndex]).getPosition().y - (*this).getSize().y + 0.5); // 0.5 buffer to intersect
            }
            // Landing on top of a moving platform
            else if ((*objects[intersectingObjectIndex]).getThreadName() == ThreadObject::MOVING_PLATFORM_TYPE &&
                     (*this).getPosition().y < (*objects[intersectingObjectIndex]).getPosition().y)
            {
                isOnMovingPlatform = true;
                movingPlatformPositionDifference = (*objects[intersectingObjectIndex]).getPosition().x - (*this).getPosition().x;

                isJumping = false;
                jumpTime = 0;

                (*this).setPosition((*this).getPosition().x, (*objects[intersectingObjectIndex]).getPosition().y - (*this).getSize().y + 0.1); // 0.1 buffer to intersect
            }
            // Hitting the moving platform (ceiling) while jumping
            else if ((*this).getPosition().y > (*objects[intersectingObjectIndex]).getPosition().y)
            {
                jumpTime = JUMP_FRAME / 2 + 5; // falling + bounce out
            }
        }
    }
    // Character on moving platform movement
    if (!isMovingLeft && !isMovingRight &&
        isOnMovingPlatform)
    {
        MovingPlatformThread *movingPlatform = static_cast<MovingPlatformThread *>(objects[intersectingObjectIndex]);
        if ((*movingPlatform).getDirectionFlag())
            (*this).setPosition((*this).getPosition().x + (*movingPlatform).MOVING_PLATFORM_MOVEMENT * getDuration() * TIME_RATIO, (*this).getPosition().y);
        else
            (*this).setPosition((*this).getPosition().x - (*movingPlatform).MOVING_PLATFORM_MOVEMENT * getDuration() * TIME_RATIO, (*this).getPosition().y);
    }
}