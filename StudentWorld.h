#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameConstants.h"
#include "Actor.h"
#include "GameWorld.h"
#include "GraphObject.h"
#include <string>
#include <queue>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


class GraphObject;
class Actor;
class TunnelMan;
class Protester;
class Earth;

class StudentWorld : public GameWorld {
    
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();

    virtual int init();
    virtual int move();
    virtual void cleanUp();

    TunnelMan* getPlayer();
    void updateDisplayText();
    std::string displayText(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrels);
    
    void detectNearActors(int x, int y, int radius);
    bool ActorsWithInRadius(int x, int y, int radius);

    bool digEarth(int x, int y);
    
    void addActor(Actor* actor);
    void addProtesters();
    void addInitialGameObjects(int num, char actor);
    void addGoodies();
    
    void decreaseOil();
    void decProtester();
    
    bool eucDistance(int x1, int y1, int x2, int y2, int radius);
    
    
    bool aboveEarth(int x, int y);
    
    bool hasEarth(int x, int y); // keep
    bool canDigEarth(int x, int y); // keep 2
    bool hasEarthLower(int x, int y); // keep
    bool isThereBoulder(int x, int y, int radius = 3);
    
    bool canMoveInDirection(int x, int y, GraphObject::Direction direction);
    bool playerInRadius(Actor* actor, int radius);
    
    
    Protester* protesterInRadius(Actor* actor, int radius);
    GraphObject::Direction trackSignal(Protester* pro, int M);
    void exitRoute(Protester* pro);
    
    
    bool inBoundsMovement(int x, int y); // keep
    bool digBounds(int x, int y); // keep
    bool squirtBounds(int x, int y); // keep

    bool getEarthVisibility(int x, int y); // since we cant use isVisible
                 
private:
    
    bool firstTick;
    int ticksSinceLast;
    int aliveProtesters;
    int oilLeft;
    
    Earth* m_earth[64][64];
    bool earthVisibility[64][64];
    
    TunnelMan* m_player;
    
    std::vector<Actor*> m_actors;
    
    int maze[64][64];
    
    struct Grid {
        int x;
        int y;
        Grid(int a, int b): x(a),y(b) {}
    };
    
};

#endif // STUDENTWORLD_H_
