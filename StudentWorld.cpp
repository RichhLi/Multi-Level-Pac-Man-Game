#include "StudentWorld.h"
#include "Actor.h"
#include "GraphObject.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>

using namespace std;

GameWorld* createStudentWorld(string assetDir) {
    return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
    : GameWorld(assetDir),firstTick(true), ticksSinceLast(0), aliveProtesters(0), m_player(nullptr), oilLeft(0) {}


void StudentWorld::addProtesters() {
    
    int P = fmin(15, 2 + getLevel() * 1.5); // fmin
    int T = fmax(25, 200 - getLevel()); //(int), max
    
    int probabilityHardcore = fmin(90, getLevel() * 10 + 30); // min, (int)
    
    if (firstTick || (ticksSinceLast > T && aliveProtesters < P)) {
        
        if (rand() % 100 + 1 < probabilityHardcore) {
            addActor(new HardcoreProtester(this));
        }
        else {
            addActor(new RegularProtester(this));
        } // change back to regular, testing
        
        ticksSinceLast = 0;
        aliveProtesters++;
        firstTick = false;
    }
    
    ticksSinceLast++;
}

Protester* StudentWorld::protesterInRadius(Actor* actor, int radius) {
    
    vector<Actor*>::iterator it;
    
    for (it = m_actors.begin(); it != m_actors.end(); it++) {
        
        if ((*it)->m_getID() == "regularProtester" || (*it)->m_getID() == "hardcoreProtester") {
            
            if (eucDistance(actor->getX(), actor->getY(), (*it)->getX(), (*it)->getY(), radius)) {
                return dynamic_cast<Protester*> (*it);
            }
        }
        
    }
    
    return nullptr;
}


GraphObject::Direction StudentWorld::trackSignal(Protester* pro, int M) {
    
    
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            maze[i][j] = 0;
        }
    }
    
    int a = pro->getX();
    int b = pro->getY();
    
    queue<Grid> g;
    g.push(Grid(getPlayer()->getX(),getPlayer()->getY()));
    maze[getPlayer()->getX()][getPlayer()->getY()] = 1;
    
    while (!g.empty()) {
        
        Grid c = g.front();
        g.pop();
        
        int x = c.x;
        int y = c.y;
        
        
        if (canMoveInDirection(x, y, GraphObject::left) && maze[x - 1][y] == 0) {
            g.push(Grid(x - 1, y));
            maze[x - 1][y] = maze[x][y] + 1;
        }
        
        if (canMoveInDirection(x, y, GraphObject::right) && maze[x + 1][y] == 0) {
            g.push(Grid(x + 1, y));
            maze[x + 1][y] = maze[x][y] + 1;
        }
        
        if (canMoveInDirection(x, y, GraphObject::up) && maze[x][y + 1] == 0) {
            g.push(Grid(x, y + 1));
            maze[x][y + 1] = maze[x][y] + 1;
        }
        
        if (canMoveInDirection(x, y, GraphObject::down) && maze[x][y - 1] == 0) {
            g.push(Grid(x, y - 1));
            maze[x][y - 1] = maze[x][y] + 1;
        }
    }
    
    if (maze[a][b] <= M + 1) {
        
        if (canMoveInDirection(a, b, GraphObject::left) && maze[a - 1][b] < maze[a][b]) {
            return GraphObject::left;
        }
        
        if (canMoveInDirection(a, b, GraphObject::right) && maze[a + 1][b] < maze[a][b]) {
            return GraphObject::right;
        }
        
        if (canMoveInDirection(a, b, GraphObject::up) && maze[a][b + 1] < maze[a][b]) {
            return GraphObject::up;
        }
        
        if (canMoveInDirection(a, b, GraphObject::down) && maze[a][b - 1] < maze[a][b]) {
            return GraphObject::down;
        }
        
    }
    
    return GraphObject::none;
     
}




void StudentWorld::exitRoute(Protester* pro) {
    
    for (int i = 0; i < 64; i++){
        for (int j = 0; j < 64; j++){
            maze[i][j]=0;
        }
    }
    
    int a = pro->getX();
    int b = pro->getY();
    
    queue<Grid> q;
    q.push(Grid(60,60));
    maze[60][60] = 1;
    
    while (!q.empty()) {
        
        Grid c = q.front();
        q.pop();
        
        int x = c.x;
        int y = c.y;

        if (canMoveInDirection(x, y, GraphObject::left) && maze[x-1][y] == 0) {
            q.push(Grid(x - 1,y));
            maze[x-1][y] = maze[x][y] + 1;
        }
        
        if (canMoveInDirection(x, y, GraphObject::right)&& maze[x + 1][y] == 0) {
            q.push(Grid(x + 1, y));
            maze[x + 1][y] = maze[x][y] + 1;
        }
        
        if (canMoveInDirection(x, y, GraphObject::up) && maze[x][y + 1] == 0) {
            q.push(Grid(x, y + 1));
            maze[x][y +1] = maze[x][y] + 1;
        }
        
    
        if (canMoveInDirection(x, y, GraphObject::down) && maze[x][y - 1] == 0) {
            q.push(Grid(x, y-1));
            maze[x][y - 1] = maze[x][y] + 1;
        }
    }
    
    
    if (canMoveInDirection(a, b, GraphObject::left) && maze[a - 1][b] < maze[a][b]) {
        pro->moveInDirection(GraphObject::left);
        return;
    }
        
    if (canMoveInDirection(a, b, GraphObject::right) && maze[a + 1][b] < maze[a][b]) {
        pro->moveInDirection(GraphObject::right);
        return;
    }
 
    if (canMoveInDirection(a, b, GraphObject::up) && maze[a][b + 1] < maze[a][b]) {
        pro->moveInDirection(GraphObject::up);
        return;
    }
    
    if (canMoveInDirection(a, b, GraphObject::down) && maze[a][b - 1] < maze[a][b]) {
        pro->moveInDirection(GraphObject::down);
        return;
    }
    
    return;
}





bool StudentWorld::getEarthVisibility(int x, int y) {
    return earthVisibility[x][y];
}


StudentWorld::~StudentWorld() {
   // cleanUp();
}

TunnelMan* StudentWorld::getPlayer() {
    return m_player;
}


string StudentWorld::displayText(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrels) {
    
    stringstream s;

    s.fill('0');
    s << "Scr: ";
    s << setw(6) << score;

    s.fill(' ');
    s << " Lvl: ";
    s << setw(2) << level;

    s << " Lives: ";
    s << setw(1) << lives;

    s << "  Hlth: ";
    s << setw(3) << health * 10;
    s << '%';

    s << "  Wtr: ";
    s << setw(2) << squirts;

    s << "  Gld: ";
    s << setw(2) << gold;

    s << "  Sonar: ";
    s << setw(2) << sonar;

    s << "  Oil Left: ";
    s << setw(2) << barrels;

    return s.str();
    
}


void StudentWorld::updateDisplayText() {
    
    int level = getLevel();
    int score = getScore();
    int lives = getLives();
    int health = m_player->getHP();
    int sonar = m_player->getSonar();
    int gold = m_player->getGold();
    int squirts = m_player->getWater();

    string s = displayText(score, level, lives, health, squirts, gold, sonar, oilLeft);
    setGameStatText(s);
}


bool StudentWorld::inBoundsMovement(int x, int y) { // keep.
    
    if ((x < 0) || (x > 59) || (y < 0) || (y > 60)) { // y > 60, x > 63
        return false;
    }
    
    return true;
}

bool StudentWorld::digBounds(int x, int y) {
    if ((x < 0) || (x > 63) || (y < 0) || (y > 60)) { // y > 60, x > 63
        return false;
    }
    
    return true;
}


bool StudentWorld::squirtBounds(int x, int y) { // for squirt function
    
    if ((x < 0) || (x > 63) || (y < 0) || (y > 63)) { // y > 60, x > 63
        return false;
    }
    
    return true;
}


bool StudentWorld::digEarth(int x, int y) {
    
    bool clear = false;
    
    
    for (int i = x; i < x + 4; i++) {
        for (int j = y; j < y + 4; j++) {
            
            
            if (digBounds(i, j)) {
         //       if (m_earth[i][j]->isVisible()) {
                if (earthVisibility[i][j]) {
                    
                    m_earth[i][j]->setVisible(false);
                    
                    earthVisibility[i][j] = false;
                    
                    clear = true;
                }
            }
            
        }
    }
    
    
    return clear;
}

void StudentWorld::addActor(Actor* actor) {
    m_actors.push_back(actor);
}

void StudentWorld::addGoodies() {
    int x, y;
    int G = (int)getLevel() * 25 + 300; // int G = (int)getLevel() * 25 + 300;
    
    if (int(rand() % G) + 1 == 1) { // 1 in G chance
        if (int(rand() % 5) + 1 == 1) { // range of 1 to 5,, % 5
            addActor(new Sonar(this, 0, 60));
        }
        else {
            do {
                x = rand() % 61; // range of 0 to 60
                y = rand() % 61; // check ranges, 0 to 60, 0 to 56
                
    //        } while (!inBounds(x, y) && !isThereEarth(x, y)); // infinite loop, could call spawn location checker here
            
        } while (canDigEarth(x, y) || isThereBoulder(x, y)); // dont want water to spawn under a boulder, or earth
            
            addActor(new Water(this, x, y));
        }
    }
}




void StudentWorld::decreaseOil() {
    oilLeft--;
}

void StudentWorld::decProtester() {
    aliveProtesters--;
}

bool StudentWorld::eucDistance(int x1, int y1, int x2, int y2, int radius) {
    if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) <= radius * radius) { // a^2 + b^2 = c^2
        return true;
    } else {
        return false;
    }
}

bool StudentWorld::ActorsWithInRadius(int x, int y, int radius) {
    vector<Actor*>::iterator it; // earth is not in this vector, so addInitialGameObjects works
    it = m_actors.begin();
    while (it != m_actors.end()) {
        if (eucDistance(x, y, (*it)->getX(), (*it)->getY(), radius)) {
            return true;
        }
        it++;
    }
    return false;
}

void StudentWorld::addInitialGameObjects(int num, char actor) { // addInitialGameObjects
    int x, y;
    for (int i = 0; i < num; i++) {
        do {
            x = rand() % 61; // 0 to 60
            if (actor == 'B') y = rand() % 37 + 20; // 20 to 56
            else    y = rand() % 56 + 1; // 1 to 56
        } while (ActorsWithInRadius(x, y, 6) || (x > 26 && x < 34)); // 0 to 29. was y > 0.
        switch (actor) {
            case 'B':
                addActor(new Boulder(this, x, y));
                break;
            case 'G':
                addActor(new Gold(this, x, y,false,false));
                break;
            case 'L':
                addActor(new Oil(this, x, y));
                oilLeft++;
                break;
        }
    }
}
bool StudentWorld::aboveEarth(int x, int y) {
    for (int i = x; i < x + 4; i++)
    {
 //       if (m_earth[i][y]->isVisible()) // this too, go back to getVisitedEarth[i][j]
        if (earthVisibility[i][y])
        
            return true;
    }
    return false;
}

bool StudentWorld::hasEarth(int x, int y) { // checks if theres earth for 4x4 square. DEBUG. CRUCIAL

    
    for (int i = x; i < x + 4; i++) { // 0 to 4

        for (int j = y; j < y + 4; j++) { // 60 to 64
            
            if (earthVisibility[i][j]) {
                return true;
            }
        }
    }
    
    return false;
    
}

bool StudentWorld::canDigEarth(int x, int y) { // checks if theres earth for 4x4 square. DEBUG. CRUCIAL


    // 
    
    for (int i = x; i < x + 4; i++) { // 0 to 4

        for (int j = y; j < y + 4; j++) { // 60 to 64
            
            if (!digBounds(i, j)) { // it is in bounds. whole function should return false
                return false;
           }
            

       //    if (m_earth[i][j]->isVisible()) // modified this m_earth[i][j]->isVisible
            
            if (earthVisibility[i][j])
                // if it has been visited, there is NO earth
                // is [i][j] is out of bounds, what happens
                return true;
        }
    }
    
    return false;
    
    /*
     
     if (!visitedEarth[i][y]) // modified this m_earth[i][j]->isVisible
         // if it has been visited, there is NO earth
         return true;
 }
}
return false;
     */
    
}

bool StudentWorld::hasEarthLower(int x, int y) { // checks if theres earth for 4x4 square. DEBUG. CRUCIAL

    
    for (int i = x; i < x + 4; i++) { // 0 to 4

        for (int j = y; j > y - 4; j--) { // 60 to 64
 

            if (earthVisibility[i][j]) {
                
                return true;
            }
        }
    }
    
    return false;
    
    /*
     
     if (!visitedEarth[i][y]) // modified this m_earth[i][j]->isVisible
         // if it has been visited, there is NO earth
         return true;
 }
}
return false;
     */
    
}


bool StudentWorld::isThereBoulder(int x, int y, int radius) {
    vector<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++) {
        if ((*it)->m_getID() == "boulder" && eucDistance(x, y, (*it)->getX(), (*it)->getY(), radius))
            return true;
    }
    return false;
}

bool StudentWorld::canMoveInDirection(int x, int y, GraphObject::Direction d) { // DEBUG, glitchy
    
    switch (d) {
            
    case GraphObject::left:
        return (inBoundsMovement(x - 1, y) && !hasEarth(x - 1, y) && !isThereBoulder(x - 1, y));
            
    case GraphObject::right: // right side is fucked up. inbounds???
        return (digBounds(x + 1, y) && !canDigEarth(x + 1, y) && !isThereBoulder(x + 1, y));
            
    case GraphObject::up:
        return (inBoundsMovement(x, y + 1) && !hasEarth(x, y + 1) && !isThereBoulder(x, y + 1));
            
    case GraphObject::down:
        return (inBoundsMovement(x, y - 1) && !hasEarth(x, y - 1) && !isThereBoulder(x, y - 1));
            
    case GraphObject::none:
            return false;
    }
    
    return false;
}

bool StudentWorld::playerInRadius(Actor *actor, int radius) {
    
    return eucDistance(actor->getX(), actor->getY(), m_player->getX(), m_player->getY(), radius);
}


void StudentWorld::detectNearActors(int x, int y, int radius) {
    
    int a, b;
    vector<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++) {
        if ((*it)->m_getID() == "oil" || (*it)->m_getID() == "gold") {
            a = (*it)->getX(); b = (*it)->getY();
            if ((x - a)*(x - a) + (y - b)*(y - b) <= radius * radius) {
                (*it)->setVisible(true);
            }
        }
    }
}


int StudentWorld::init() {
    
    firstTick = true;
    oilLeft = 0;
    aliveProtesters = 0;
    ticksSinceLast = 0;


    //initiate full 64 squares to avoid bad access issues
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            m_earth[i][j] = new Earth(this, i, j);
            earthVisibility[i][j] = true;
        }
    }
    
    // manually set top to false, so we have a 60 square grid and not 64
    for (int i = 0; i < 64; i++) {
        for (int j = 60; j < 64; j++) {
            m_earth[i][j]->setVisible(false);
            earthVisibility[i][j] = false;

        }
    }
    
    // manually set middle shaft to false
    for (int column = 30; column < 34; column++) {
        for (int row = 4; row < 60; row++) { // hold on
            m_earth[column][row]->setVisible(false);
            earthVisibility[column][row] = false;

        }
    }

    
    m_player = new TunnelMan(this);
    
    int B = min( (int)getLevel() / 2 + 2, 9);
    int G = max((int)getLevel() / 2, 2);
    int L = min(2 + (int)getLevel(), 21);
    
    addInitialGameObjects(B, 'B');
    addInitialGameObjects(L, 'L');
    addInitialGameObjects(G, 'G');

    return GWSTATUS_CONTINUE_GAME;
    
}

int StudentWorld::move() {
   
    updateDisplayText();

    vector<Actor*>::iterator it;
    
    for (it = m_actors.begin(); it!=m_actors.end(); it++) {
        if ((*it)->isAlive()) {
            (*it)->doSomething();
        }
        if (!m_player->isAlive()) {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if (oilLeft == 0) {
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    
    m_player->doSomething();

    addGoodies();
    addProtesters();

    for (it = m_actors.begin(); it != m_actors.end();) {
        
        if (!(*it)->isAlive()) {
            delete *it;
            it = m_actors.erase(it);
        }
        else it++;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    
    for (int x = 0; x < 64; x++) {
        
        for (int y = 0; y < 60; y++) {
            delete m_earth[x][y];
        }
        
    }
    
    vector<Actor*>::iterator it;
    
    for (it = m_actors.begin(); it != m_actors.end(); it++) {
        delete *it;
    }
    
    m_actors.clear();
    delete m_player;

}
