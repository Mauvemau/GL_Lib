#pragma once
#include "base_game.h"

#include <iostream>

#include "collisionManager.h"

using namespace std;

class Game : public gllib::BaseGame
{
private:
    bool rockColliding = false;
    bool isAttacking = false;
    gllib::Sprite* background;
    gllib::Sprite* idle;
    gllib::Sprite* rock;
    gllib::Animation* moving;
    gllib::Animation* colliding;
    gllib::Animation* attack;
    gllib::Rectangle* floorCollision;

    gllib::collisionManager* collisionManager;

    void moveRectangle(float speed);
    void UpdatePlayerMirror(bool cond);
    void checkInputs(gllib::Animation* player);

protected:
    void init() override;
    void gravity(gllib::Entity* player);
    void drawObjects();
    void UpdatePlayerPosition();
    void update() override;
    void uninit() override;

public:
    Game();
    ~Game() override;
};
