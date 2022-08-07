#ifndef ACTOR_H_
#define ACTOR_H_

#include "GameConstants.h"
#include "GraphObject.h"
#include "StudentWorld.h"
#include <string>

// MOST RECENT VERSION

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;


// ACTOR ------------------------------------------------------------------------------------------------

class Actor : public GraphObject {
    
public:
    
    Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
    virtual ~Actor();
    StudentWorld* getWorld();
    bool isAlive();
    void die();
    void moveTo(int x, int y);
    virtual void doSomething() = 0;
    virtual void isAnnoyed(int hP) {}
    std::string m_getID();
    void m_setID(std::string x);

private:
    StudentWorld* m_world;
    bool m_alive;
    std::string m_ID;

};

// HUMAN ------------------------------------------------------------------------------------------------

class Human : public Actor {
    
public:
    
    Human(StudentWorld* world, int imageID, int startX, int startY, Direction dir, int hp);
    int getHP();
    void decreaseHP(int hitPoints);
    virtual void moveInDirection(Direction direction) = 0;
    virtual void isAnnoyed(int hp) = 0;
    
private:
    
    int m_hP;
    
};


// TUNNELMAN ------------------------------------------------------------------------------------------------

class TunnelMan : public Human {
    
public:
    TunnelMan(StudentWorld* world);
    
  //  void add(int id);
    void add(std::string id);
    
    int getWater();
    int getSonar();
    int getGold();
    virtual void doSomething();
    void shoot();
    virtual void isAnnoyed(int hP);
    virtual void moveInDirection(Direction direction);
    
private:
    
    int m_gold;
    int m_water;
    int m_sonar;
};


// PROTESTER ------------------------------------------------------------------------------------------------

class Protester : public Human {
    
public:
    
    Protester(StudentWorld* world, int imageID,int hP);
    virtual void doSomething();
    virtual void getBribed();
    void moveInDirection(Direction direction);
    void randomSpaceToMove();
    virtual void isAnnoyed(int hP);
    void getSplashed();
    bool isFacingPlayer();
    Direction pickRandomDirection();
    Direction findDirectionToPlayer();
    void pickSuccessfulDirection();
    bool hasStraightPath(Direction direction);
    bool atIntersection();

private:
    
    bool leaveState;
    int numSquaresToMoveInCurrentDirection;
    int ticksWithoutShout;
    int ticksSinceLastTurn;
    int ticksToWaitBetweenMoves;
    
};

// REGULAR PROTESTER --------------------------------------------------------------------------------------------

class RegularProtester : public Protester {
    
public:
    RegularProtester(StudentWorld* world);
    
};


// HARDCORE PROTESTER --------------------------------------------------------------------------------------------

class HardcoreProtester : public Protester {
    
public:
    HardcoreProtester(StudentWorld* world);
    
};


// EARTH --------------------------------------------------------------------------------------------


class Earth : public Actor {
    
public:
    Earth(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    
};


// BOULDER --------------------------------------------------------------------------------------------


class Boulder : public Actor {
    
public:
    Boulder(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    void annoyHuman();
    
private:
    bool isStable;
    bool isFalling;
    int m_ticks;

};


// GOODIES --------------------------------------------------------------------------------------------


class Goodies : public Actor { // INITIAL GOODIES (OIL AND GOLD)
    
public:
    Goodies(StudentWorld* world, int imageID, int startX, int startY);
    virtual void doSomething() = 0;
    void disappearIn(int ticks);
    bool m_getVisible();
    void m_setVisible(bool x);
    
    
private:
    int m_tick;
    bool m_isVisible;

};


// OIL --------------------------------------------------------------------------------------------


class Oil : public Goodies {
    
public:
    Oil(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    
};


// GOLD --------------------------------------------------------------------------------------------


class Gold : public Goodies {
    
public:
    Gold(StudentWorld* world, int startX, int startY, bool visible,bool dropped);
    virtual void doSomething();
    
private:
    bool isDropped;
    
};

// LATER GOODIES --------------------------------------------------------------------------------------------


class LaterGoodies : public Goodies {
    
public:
    
    LaterGoodies(StudentWorld* world, int imageID, int startX, int startY);
    virtual void doSomething();
    
};


// SONAR --------------------------------------------------------------------------------------------

class Sonar : public LaterGoodies {
    
public:
    Sonar(StudentWorld* world, int startX, int startY);
    
};


// WATER --------------------------------------------------------------------------------------------


class Water: public LaterGoodies {
    
public:
    Water(StudentWorld* world, int startX, int startY);
    
};


// SQUIRT --------------------------------------------------------------------------------------------


class Squirt : public Actor {
    
public:
    Squirt(StudentWorld* world, int startX, int startY, Direction dir);
    virtual void doSomething();
    bool annoyProtesters();
    
private:
    int m_travel;
    
};



#endif // ACTOR_H_
