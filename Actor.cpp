#include "Actor.h"
#include "GraphObject.h"
#include "StudentWorld.h"
#include <algorithm>
using namespace std;


// ACTOR ---------------------------------------------------------------------------------------------------

Actor::Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) :GraphObject(imageID, startX, startY, dir, size, depth), m_world(world),m_alive(true) {
    setVisible(true);
}

Actor::~Actor() {
    setVisible(false);
}


StudentWorld* Actor::getWorld() {
    return m_world;
}


void Actor::die() {
    m_alive = false;
}

bool Actor::isAlive() {
    return m_alive;
}

void Actor::moveTo(int x, int y) {
    if (x < 0) x = 0;
    if (x > 60) x = 60; // 60
    if (y < 0) y = 0;
    if (y > 60)  y = 60;
    GraphObject::moveTo(x, y);
}


string Actor::m_getID() {
    return m_ID;
}


void Actor::m_setID(string x) {
    m_ID = x;
}


// HUMAN ---------------------------------------------------------------------------------------------------


Human::Human(StudentWorld* world, int imageID, int startX, int startY, Direction dir, int hp)
    :Actor(world, imageID, startX, startY, dir, 1.0, 0), m_hP(hp) {}

int Human::getHP() {
    return m_hP;
}

void Human::decreaseHP(int points) {
    m_hP -= points;
}


// TUNNELMAN ---------------------------------------------------------------------------------------------------

TunnelMan::TunnelMan(StudentWorld* world)
    :Human(world, TID_PLAYER, 30, 60, right, 10), m_water(5), m_sonar(1), m_gold(0) {} // CHANGE WATER


void TunnelMan::add(string id) {
    
    if (id == "water") {
        m_water +=5;
    }
    
    if (id == "gold") {
        m_gold++;
    }
    
    if (id == "sonar") {
        m_sonar += 2;
    }
    
}

void TunnelMan::doSomething() {
    
    if (!isAlive()) {
        return;
    }
    
    if (getWorld()->digEarth(getX(), getY())) {
        getWorld()->playSound(SOUND_DIG);
    }
    
    int keyInput;
    
    if (getWorld()->getKey(keyInput) == true) {
        
        switch (keyInput) {
                
        case KEY_PRESS_ESCAPE:
            die();
            break;
                
        case KEY_PRESS_LEFT: moveInDirection(left); break;
        case KEY_PRESS_RIGHT:moveInDirection(right); break;
        case KEY_PRESS_UP:   moveInDirection(up); break;
        case KEY_PRESS_DOWN: moveInDirection(down); break;
                
        case KEY_PRESS_SPACE:
                
            if (m_water > 0) {
                m_water--;
                shoot();
            }
            break;
                
        case 'Z':
        case 'z':
                
            if (m_sonar > 0) {
                m_sonar--;
                getWorld()->detectNearActors(getX(), getY(), 12); // make sure doesnt crash when OOB
                getWorld()->playSound(SOUND_SONAR);
            }
            break;
                
        case KEY_PRESS_TAB:
                
            if (m_gold > 0) {
                getWorld()->addActor(new Gold(getWorld(), getX(), getY(), true, true));
                m_gold--;
            }
            break;
        }
    }
}



void TunnelMan::isAnnoyed(int hP) {
    
    decreaseHP(hP);
    if (getHP() <= 0) {
        die();
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
    }
}


void TunnelMan::shoot() { // SHOOT WATER
    switch (getDirection()) {
    case left:
        if (!getWorld()->canDigEarth(getX() - 4, getY()) && !getWorld()->isThereBoulder(getX() - 4, getY()) &&
            getWorld()->inBoundsMovement(getX() - 4, getY())) { //in bounds should return false,
            getWorld()->addActor(new Squirt(getWorld(), getX() - 4, getY(), getDirection()));
        }
        break;
    case right:
        if (!getWorld()->canDigEarth(getX() + 4, getY()) && !getWorld()->isThereBoulder(getX() + 4, getY()) &&
            getWorld()->inBoundsMovement(getX() + 4, getY())) {
            getWorld()->addActor(new Squirt(getWorld(), getX() + 4, getY(), getDirection()));
        }
        break;
    case down: // this
        if (!getWorld()->canDigEarth(getX() , getY() - 4) && !getWorld()->isThereBoulder(getX(), getY() - 4)
            && getWorld()->squirtBounds(getX(), getY() - 4)) { // has extra bound checker because the squirt spawns in 4 spaces out. for top and bottom, we need the extra check.
            getWorld()->addActor(new Squirt(getWorld(), getX(), getY() - 4, getDirection()));
        }
        break;
    case up: // this
        if (!getWorld()->canDigEarth(getX(), getY() + 4) && !getWorld()->isThereBoulder(getX(), getY() + 4)
            && getWorld()->squirtBounds(getX(), getY() + 7)) { // has extra bound checker because the squirt spawns in 4 spaces out. for top and bottom, we need the extra check.
            getWorld()->addActor(new Squirt(getWorld(), getX(), getY() + 4, getDirection()));
        }
        break;
    case none: return;
    }
    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
}


void TunnelMan::moveInDirection(Direction dir) {
    
    int x = getX();
    int y = getY();
    
    switch (dir) {
    case left:
        if (getDirection() == left) {
            if (!getWorld()->isThereBoulder(x - 1, y) && getWorld()->digBounds(x-1, y)) // inbounds
                moveTo(x - 1, y);
            else return;
        }
        else setDirection(left);
        break;
    case right:
        if (getDirection() == right) {
            if (!getWorld()->isThereBoulder(x + 1, y) && getWorld()->digBounds(x+1, y))
                moveTo(x + 1, y);
            else return;
        }
        else setDirection(right);
        break;
    case up:
        if (getDirection() == up) {
            if (!getWorld()->isThereBoulder(x, y + 1) && getWorld()->digBounds(x, y+1)) // has different bound checker because of bug when moving on the right side.
                moveTo(x, y + 1);
            else return;
        }
        else setDirection(up);
        break;
    case down:
        if (getDirection() == down) {
            if (!getWorld()->isThereBoulder(x, y - 1) && getWorld()->digBounds(x, y-1)) // has different bound checker because of bug when moving on the right side
                moveTo(x, y - 1);
            else return;
        }
        else setDirection(down);
        break;
    case none: return;
    }
}

int TunnelMan::getWater() {
    return m_water;
}

int TunnelMan::getSonar() {
    return m_sonar;
}

int TunnelMan::getGold() {
    return m_gold;
}


// EARTH ---------------------------------------------------------------------------------------------------


Earth::Earth(StudentWorld* world, int startX, int startY)
:Actor(world, TID_EARTH, startX, startY, right, 0.25, 3) {}


void Earth::doSomething() {}


// BOULDER ---------------------------------------------------------------------------------------------------

Boulder::Boulder(StudentWorld* world, int startX, int startY)
    : Actor(world, TID_BOULDER, startX, startY, down, 1.0, 1),isStable(true), m_ticks(0) {
        
    world->digEarth(startX, startY);
    m_setID("boulder");
}

void Boulder::doSomething() {
    
    if (!isAlive()) {
        return;
    }
    
    if (isStable) {
        if (getWorld()->aboveEarth(getX(), getY() - 1))
            return;
        else isStable = false;
    }
    
    if (m_ticks == 30) { // WAITING STATE
        isFalling = true;
        getWorld()->playSound(SOUND_FALLING_ROCK);
    }
    m_ticks++;
    
    if (isFalling) {
        if (getWorld()->aboveEarth(getX(), getY()-1)|| getWorld()->isThereBoulder(getX(), getY()-4, 0)
            || getY() == 0) {
            
            die();
        }
        else {
            moveTo(getX(), getY() - 1); // go down
        }
        
        annoyHuman(); // checks if in radius of tunnelman or protester
    }
}

void Boulder::annoyHuman() {
    
    if (getWorld()->playerInRadius(this, 3)) {
        getWorld()->getPlayer()->isAnnoyed(100);
    }
    
    Protester * p = getWorld()->protesterInRadius(this, 3);
    
    if (p != nullptr) {
        p->isAnnoyed(100); // add 100 points
    }
}



// GOODIES ---------------------------------------------------------------------------------------------------


Goodies::Goodies(StudentWorld* world, int imageID, int startX, int startY)
    :Actor(world, imageID, startX, startY, right, 1.0, 2) {}


void Goodies::m_setVisible(bool x) { // since we can't use the one in graphobject
    m_isVisible = x;
}

bool Goodies::m_getVisible() { // since we can't use the one in graphobject
    return m_isVisible;
}


void Goodies::disappearIn(int ticks) {
    if (m_tick == ticks) {
        die();
    } else {
        m_tick++;
    }
}


// OIL ---------------------------------------------------------------------------------------------------


Oil::Oil(StudentWorld* world, int startX, int startY)
    :Goodies(world, TID_BARREL, startX, startY) {
    setVisible(false);
    m_setVisible(false);
    m_setID("oil");
}


void Oil::doSomething() {
    
    if (!isAlive()) {
        return;
    }
    
    
    if (!m_getVisible() && getWorld()->playerInRadius(this, 4)) { // player walked near
        setVisible(true);
        m_setVisible(true);
        return;
    }
  
    
    if (getWorld()->playerInRadius(this, 3)) { // played picked up
        die();
        getWorld()->increaseScore(1000);
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->decreaseOil();
        return;
    }
}


// GOLD ---------------------------------------------------------------------------------------------------


Gold::Gold(StudentWorld* world, int startX, int startY, bool isVisible, bool dropped)
    :Goodies(world, TID_GOLD, startX, startY), isDropped(dropped) {
    setVisible(isVisible);
    m_setVisible(isVisible);
    m_setID("gold");

}

void Gold::doSomething() {
    
    if (!isAlive()) {
        return;
    }


    if (!m_getVisible() && getWorld()->playerInRadius(this, 4)) {
        setVisible(true);
        m_setVisible(true);
        return;
    }
  
    
    if (!isDropped && getWorld()->playerInRadius(this, 3)) {
        die();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->getPlayer()->add(m_getID());
        getWorld()->increaseScore(10);
        return;
    }
    
    if (isDropped) { // if dropped, and protester comes near
        Protester* p = (getWorld()->protesterInRadius(this, 3));
        if (p != nullptr) {
            die();
            p->getBribed();
        }
        disappearIn(100); // lifetime of 100 ticks
    }
}


// LATERGOODIES ---------------------------------------------------------------------------------------------------

// sonar and water

LaterGoodies::LaterGoodies(StudentWorld* world, int imageID, int startX, int startY)
    :Goodies(world, imageID, startX, startY) {}

void LaterGoodies::doSomething() {
    
    if (!isAlive()) {
        return;
    }
    
    if (getWorld()->playerInRadius(this, 3)) {
        die();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->getPlayer()->add(m_getID());
        
        if (m_getID() == "water") {
            getWorld()->increaseScore(100);
        }
        
        
        if (m_getID() == "sonar") {
            getWorld()->increaseScore(75);
        }
                
        return;
    }
    
    disappearIn(max(100, 300 - 10 * (int)getWorld()->getLevel())); // from spec
}


// SONAR ---------------------------------------------------------------------------------------------------

Sonar::Sonar(StudentWorld* world, int startX, int startY)
    :LaterGoodies(world, TID_SONAR, startX, startY) {
        m_setID("sonar");
    }


// WATER ---------------------------------------------------------------------------------------------------

Water::Water(StudentWorld* world, int startX, int startY)
    :LaterGoodies(world, TID_WATER_POOL, startX, startY) {
        m_setID("water");
    }


// SQUIRT ---------------------------------------------------------------------------------------------------

Squirt::Squirt(StudentWorld* world, int startX, int startY, Direction dir)
    :Actor(world, TID_WATER_SPURT, startX, startY, dir, 1.0, 1),m_travel(0) {}

void Squirt::doSomething() { // DEBUG
    
    if (!isAlive()) {
        return;
    }
    
    if (annoyProtesters() || m_travel == 4 ) { // if hit protester or moved 4 spaces
        die();
        return;
    }
    
    switch (getDirection()) {
            
    case left:
            
        if (getWorld()->canDigEarth(getX() - 4, getY()) || getWorld()->isThereBoulder(getX() - 4, getY())
            || !getWorld()->squirtBounds(getX() - 1, getY())) {
            die();
            return;
        }
        else {
            moveTo(getX() - 1, getY());
        }
        break;
            
    case right:
            
        if (getWorld()->canDigEarth(getX() + 4, getY()) || getWorld()->isThereBoulder(getX() + 4, getY())
            || !getWorld()->squirtBounds(getX() + 1, getY())) {
            
            die();
            return;
        }
        else {
            moveTo(getX() + 1, getY());
        }
        break;
            
    case up:
            
        if (getWorld()->canDigEarth(getX(), getY() + 4) || getWorld()->isThereBoulder(getX(), getY() + 4)
            || !getWorld()->squirtBounds(getX(), getY() + 1)) {
            
            die();
            return;
        }
        else {
            moveTo(getX(), getY() + 1);
        }
        break;
            
    case down:
            
        if (getWorld()->canDigEarth(getX(), getY() - 4) || getWorld()->isThereBoulder(getX(), getY() - 4)
            || !getWorld()->squirtBounds(getX(), getY() - 1)) {
            die();
            return;
        }
        else {
            moveTo(getX(), getY() - 1);
        }
        break;
            
    case none : return;
            
    }
    m_travel++; // moved a space
}

bool Squirt::annoyProtesters() {
    
    Protester * p= getWorld()->protesterInRadius(this,3);
    
    if (p != nullptr) {
        p->isAnnoyed(2); // lose 2 hp
       return true;
    }
    
    return false;
}

// PROTESTER ---------------------------------------------------------------------------------------------------

Protester::Protester(StudentWorld* world, int imageID, int hP)
    : Human(world, imageID, 60, 60, left, hP), leaveState(false), ticksSinceLastTurn(200), ticksWithoutShout(15) {
    randomSpaceToMove();
    ticksToWaitBetweenMoves = max(0, 3 - (int)getWorld()->getLevel() / 4);
}

void Protester::randomSpaceToMove() {
    numSquaresToMoveInCurrentDirection = rand() % 53 + 8; // 8 to 60
}

void Protester::doSomething() {
    
    if (!isAlive()) {
        return;
    }
    
    if (ticksToWaitBetweenMoves > 0) {
        ticksToWaitBetweenMoves--;
        return;
    }
    
    else {
        
        if (!leaveState) {
        ticksToWaitBetweenMoves = max(0, 3 - (int)getWorld()->getLevel() / 4);
        ticksSinceLastTurn++;
        ticksWithoutShout++;
        }
    }
    
    if (leaveState) {
        if (getX() == 60 && getY() == 60) {
            die();
            getWorld()->decProtester();
            return;
        }
        
        
        getWorld()->exitRoute(this);
        return;
    }


    if (getWorld()->playerInRadius(this, 4) && isFacingPlayer()) {
        
        if (ticksWithoutShout > 15) {
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            getWorld()->getPlayer()->isAnnoyed(2);
            ticksWithoutShout = 0;
            return;
        }
        return;
        
    }
    
    if (m_getID() == "hardcoreProtester") {
        
        int M = 16 + int(getWorld()->getLevel() * 2);
        Direction s = getWorld()->trackSignal(this, M);
        
        if (s != none) {
            moveInDirection(s);
            return;
        }
    }
    
    
    // DEBUG THIS
    
    Direction d = findDirectionToPlayer(); // maybe
    
    if (d != none && hasStraightPath(d) && (!getWorld()->playerInRadius(this, 4))) {
        setDirection(d);
        moveInDirection(d);
        numSquaresToMoveInCurrentDirection = 0;
        return;
    }
    
    numSquaresToMoveInCurrentDirection--;
    
    if (numSquaresToMoveInCurrentDirection <= 0) {
        Direction d2;
        while (true) {
            d2 = pickRandomDirection();
            if (getWorld()->canMoveInDirection(getX(),getY(), d2)) break;
        }
        setDirection(d2);
        randomSpaceToMove();
    } else if (atIntersection() && ticksSinceLastTurn > 200) { // DEBUG??
        pickSuccessfulDirection(); // sets direction in void function
        ticksSinceLastTurn = 0;
        randomSpaceToMove(); // sets number to move
    }
    

    moveInDirection(getDirection());
    
    if (!getWorld()->canMoveInDirection(getX(),getY(),getDirection())) {
        numSquaresToMoveInCurrentDirection = 0;
    }
}


void Protester::moveInDirection(Direction dir) { // DEBUG
    
    switch (dir) {
            
    case left:
            
        if (getDirection() == left) {
            
            if (getX() == 0) {
                setDirection(right);
            }
                moveTo(getX() - 1, getY());
        }
            
        else  {
            setDirection(left);
        }
        break;
            
    case right:
            
        if (getDirection() == right) {
            if (getX() == 60) { // 60
                
                if (getY() != 0 && !getWorld()->getEarthVisibility(60,59)) { // this extra stuff is to prevent a bug where after facing the tunnelan on the right side, it gets stuck and tries to move into the wall on the left side. it picks an up or down direction instead.
                    setDirection(down);
                    moveTo(getX(), getY() - 1);
                } else {
                    setDirection(up);
                    moveTo(getX(), getY() + 1);
                }
            }
            moveTo(getX() + 1, getY());
        } else  {
            setDirection(right);
        }
        break;
  
            
    case up:
            
        if (getDirection() == up) {
            if (getY() == 60) {
                setDirection(down);
            }
            moveTo(getX(), getY() + 1);
        }
        else { setDirection(up);
        }
        break;
            
            
    case down:
            
        if (getDirection() == down) {
            if (getY() == 0) {
                setDirection(up);
            }
            moveTo(getX(), getY() - 1);
        }
        else {
            setDirection(down);
        }
        break;
            
            
    case none: return;
            
    }
}


bool Protester::hasStraightPath(Direction dir) {
    
    int pX = getWorld()->getPlayer()->getX();
    int pY = getWorld()->getPlayer()->getX();
    
    switch (dir) {
            
    case left:
        for (int i = getX(); i >= pX; i--) {
            if (getWorld()->canDigEarth(i, getY()) || getWorld()->isThereBoulder(i, getY())) {
                return false;
            }
        }
        return true;
        break;
            
            
    case right:
        for (int i = getX(); i <= pX; i++) {
            if (getWorld()->canDigEarth(i, getY()) || getWorld()->isThereBoulder(i, getY())) {
                return false;
            }
        }
        return true;
        break;
            
            
    case up:
        for (int j = getY(); j <= pY; j++) {
            if (getWorld()->canDigEarth(getX(), j) || getWorld()->isThereBoulder(getX(), j)) {
                return false;
            }
        }
        return true;
        break;
            
            
    case down:
            
        for (int j = getY(); j >= pY; j--) {
        //    bool x = true;
            if (getWorld()->hasEarthLower(getX(), j) || getWorld()->isThereBoulder(getX(), j)) {
                return false;
            }
        }
        return true;
        break;
            
            
    case none:
        return false;
    }
}


GraphObject::Direction Protester::findDirectionToPlayer() {
    
    int pX = getWorld()->getPlayer()->getX();
    int pY = getWorld()->getPlayer()->getY();
    
    if (getX() == pX && getY() == pY) {
        return getDirection();
    }
    
    if (getX() == pX) { // vertical line of sight
        if (getY() < pY)
            return up;
        if (getY() > pY)
            return down;
    }
    
    if (getY() == pY) { // horizontal line of sight
        if (getX() > pX)
            return left;
        if (getY() < pY)
            return right;
    }
    
    return none;   // not in line of sight
}



GraphObject::Direction Protester::pickRandomDirection() {
    
    int x = rand() % 4; // 0 to 3
    
    switch (x) {
        case 0: return left;
        case 1: return right;
        case 2: return up;
        case 3: return down;
    }
    
    return none;
}


bool Protester::atIntersection() {
    
    if (getDirection() == up || getDirection() == down) {
        
        return (getWorld()->canMoveInDirection(getX(),getY(), left) || getWorld()->canMoveInDirection(getX(),getY(), right));
    }
    else {
        return (getWorld()->canMoveInDirection(getX(),getY(), up) || getWorld()->canMoveInDirection(getX(),getY(), down));
    }
}

void Protester::pickSuccessfulDirection() {
    
    if (getDirection() == up || getDirection() == down) {
        
        if (!getWorld()->canMoveInDirection(getX(),getY(), left)) {
            setDirection(right);
        } else if (!getWorld()->canMoveInDirection(getX(),getY(), right)) {
            setDirection(left);
        }
        else {
            switch (rand() % 2) { // 0 to 1
            case 0: setDirection(left); // left
            case 1: setDirection(right); // right
            }
        }
    }
    
    else {
        if (!getWorld()->canMoveInDirection(getX(),getY(), up)) {
            setDirection(down);
        } else if (!getWorld()->canMoveInDirection(getX(),getY(), down)) {
            setDirection(up);
        }
        else {
            switch (rand() % 2) {
            case 0: setDirection(up);
            case 1: setDirection(down);
            }
        }
    }
}

void Protester::isAnnoyed(int hP) {
    
    if (leaveState) {
        return;
    }
    
    decreaseHP(hP);
    getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
    getSplashed();
    
    if (getHP() <= 0) {
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        leaveState = true;
        ticksToWaitBetweenMoves = 0;
        
        if (hP == 100) {
            getWorld()->increaseScore(500); // hit by boulder
        } else if (m_getID() == "regularProtester") {
            getWorld()->increaseScore(100);
        } else {
            getWorld()->increaseScore(250); // squirted
        }
    }
}

void Protester::getSplashed() {
    ticksToWaitBetweenMoves = max(50, 100 - (int)getWorld()->getLevel()*10);
}


void Protester::getBribed() {
    
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    
    if (m_getID() == "regularProtester") {
        getWorld()->increaseScore(25); // regular gets 25
        leaveState = true;
    } else {
        getWorld()->increaseScore(50); // hardcore gets 50
        ticksToWaitBetweenMoves = max(50,100- int(getWorld()->getLevel())*10);
    }
}

bool Protester::isFacingPlayer() {
    
    switch (getDirection()) {
        case left:
            return getWorld()->getPlayer()->getX() <= getX();
        case right:
            return getWorld()->getPlayer()->getX() >= getX();
        case up:
            return getWorld()->getPlayer()->getY() >= getY();
        case down:
            return getWorld()->getPlayer()->getY() <= getY();
        case none:
            return false;
    }
    return false;
}


// REGULAR PROTESTER --------------------------------------------------------------------------------

RegularProtester::RegularProtester(StudentWorld* world)
    : Protester(world, TID_PROTESTER, 5) {
        m_setID("regularProtester");
    }


// HARDCORE PROTESTER --------------------------------------------------------------------------------

HardcoreProtester::HardcoreProtester(StudentWorld* world)
    : Protester(world, TID_HARD_CORE_PROTESTER, 20) {
        m_setID("hardcoreProtester");
    }

