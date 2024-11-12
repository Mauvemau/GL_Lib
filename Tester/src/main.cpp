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
    void movement(gllib::Animation* player);

protected:
    void init() override;
    void drawObjects();
    void UpdatePlayerPosition();
    void update() override;
    void uninit() override;

public:
    Game();
    ~Game() override;
};

Game::Game()
{
    window->setVsyncEnabled(false);
    cout << "Game created!\n";

    gllib::Transform trs2;
    trs2.rotationQuat = {0.0f, 0.0f, 0.0f, 0.0f};
    trs2.scale = {100.0f, 100.0f, 0.0f};
    trs2.position = {window->getWidth() * .5f, window->getHeight() * .5f, 0.0f};
    moving = new gllib::Animation(trs2, {1.0f, 1.0f, 1.0f, 1.0f});
    idle = new gllib::Sprite(trs2, {1.0f, 1.0f, 1.0f, 1.0f});
    colliding = new gllib::Animation(trs2, {1.0f, 1.0f, 1.0f, 1.0f});
    attack = new gllib::Animation(trs2, {1.0f, 1.0f, 1.0f, 1.0f});

    gllib::Transform trs3;
    trs3.position = {window->getWidth() * .5f, window->getHeight() * .5f, 0.0f};
    trs3.rotationQuat = {0.0f, 0.0f, 0.0f, 0.0f};
    trs3.scale = {640.0f, 480.0f, 0.0f};
    background = new gllib::Sprite(trs3, {1.0f, 1.0f, 1.0f, 1.0f});
    background->addTexture("background.png", true);

    gllib::Transform trs4;
    trs4.position = {window->getWidth() * .5f, window->getHeight() * .95f, 0};
    trs4.rotationQuat = {0.0f, 0.0f, 0.0f, 0.0f};
    trs4.scale = {static_cast<float>(window->getWidth()), 80, 0};
    floorCollision = new gllib::Rectangle(trs4, {0.8f, 0.0f, 1.0f, 0.5f});

    collisionManager = new gllib::collisionManager({static_cast<gllib::Entity*>(floorCollision)});

    unsigned int rockTex = gllib::Loader::loadTexture("Rock.jpg", false);
    gllib::Transform trs5;
    trs5.position = {window->getWidth() * .7f, window->getHeight() * .75f, 0.0f};
    trs5.rotationQuat = {0.0f, 0.0f, 0.0f, 0.0f};
    trs5.scale = {100.0f, 100.0f, 0.0f};

    rock = new gllib::Sprite(trs5, {1.0f, 1.0f, 1.0f, 1.0f});

    rock->addFrame(rockTex, 74, 21, 84, 56);

    unsigned int knucklesSpriteSheet = gllib::Loader::loadTexture("Knuckles_Sprite_Sheet.png", true);
    idle->addFrame(knucklesSpriteSheet, 0, 3, 31, 39);
    moving->addFrame(knucklesSpriteSheet, 3, 45, 27, 36);
    moving->addFrame(knucklesSpriteSheet, 35, 46, 31, 37);
    moving->addFrame(knucklesSpriteSheet, 76, 45, 41, 39);
    moving->addFrame(knucklesSpriteSheet, 125, 44, 37, 38);
    moving->addFrame(knucklesSpriteSheet, 175, 45, 27, 36);
    moving->addFrame(knucklesSpriteSheet, 209, 45, 29, 37);
    moving->addFrame(knucklesSpriteSheet, 247, 44, 40, 38);
    moving->addFrame(knucklesSpriteSheet, 293, 47, 36, 36);
    //player->addFrame(knucklesSpriteSheet, 340, 49, 32, 36);
    //player->addFrame(knucklesSpriteSheet, 380, 50, 32, 35);
    //player->addFrame(knucklesSpriteSheet, 420, 49, 32, 36);
    //player->addFrame(knucklesSpriteSheet, 460, 49, 32, 34);

    colliding->addFrame(knucklesSpriteSheet, 427, 97, 31, 34);
    colliding->addFrame(knucklesSpriteSheet, 466, 96, 26, 35);
    colliding->addFrame(knucklesSpriteSheet, 500, 97, 28, 34);
    colliding->addFrame(knucklesSpriteSheet, 533, 96, 27, 35);

    attack->addFramesFromAtlas(knucklesSpriteSheet, 228, 131, 33, 28, 4, 1);

    moving->setCurrentFrame(0);
    moving->setDurationInSecs(0.9f);
    colliding->setCurrentFrame(0);
    colliding->setDurationInSecs(1.f);
    attack->setCurrentFrame(0);
    attack->setDurationInSecs(.3f);
}

Game::~Game()
{
    cout << "Game destroyed!\n";
}

void Game::init()
{
    cout << "External init!!!!\n";

    srand(time(nullptr));
    window->setTitle("Segundo Parcial Barra Santiago");
}


void Game::update()
{
    // Update
    movement(moving);

    // Draw

    drawObjects();
}


void Game::drawObjects()
{
    gllib::Renderer::clear();

    gllib::Shader::useShaderProgram(shaderProgramTexture);
    background->draw();
    if (!Input::isAnyKeyPressed())
    {
        idle->draw();
    }
    else
    {
        if (rockColliding)
        {
            colliding->draw();
        }
        else if (isAttacking)
        {
            attack->draw();    
        }
        else
        {
            moving->draw();
        }
    }
    rock->draw();
}

void Game::UpdatePlayerPosition()
{
    idle->setPosition(moving->getPosition());
    colliding->setPosition(moving->getPosition());
    attack->setPosition(moving->getPosition());
}

void Game::UpdatePlayerMirror(bool cond)
{
    moving->setMirroredX(cond);
    idle->setMirroredX(cond);
    colliding->setMirroredX(cond);
    attack->setMirroredX(cond);
}

void Game::movement(gllib::Animation* player)
{
    rockColliding = false;
    isAttacking = false;
    gllib::Transform transform2 = player->getTransform();
    transform2.position.y += 1.f;
    float speed = 200 * gllib::LibTime::getDeltaTime();
    float gravity = 89 * gllib::LibTime::getDeltaTime();
    
    if (!collisionManager->checkCollision(transform2))
    {
        player->move({0.f, gravity, 0});
        UpdatePlayerPosition();
    }
    
    if (Input::getKeyPressed(Key_F))
    {
        attack->update();
        isAttacking = true;
    }
    
    if (!Input::isAnyKeyPressed())
    {
        player->reset();
        return;
    }
    gllib::Transform transform = player->getTransform();

    if (Input::getKeyPressed(Key_D))
    {
        // D
        transform.position.x += 2.0f;
        
        UpdatePlayerMirror(false);
        if (!collisionManager->checkCollision(transform))
        {
            player->move({speed, 0.f, 0.f});
        }
        if (player->isColliding(rock->getTransform()))
        {
            rock->move({speed, 0, 0});
            colliding->update();
            rockColliding = true;
        }
    }

    if (Input::getKeyPressed(Key_A))
    {
        // A
        transform.position.x -= 2.0f;
        UpdatePlayerMirror(true);
        
        if (!collisionManager->checkCollision(transform))
        {
            player->move({-speed, 0.f, 0.f});
        }
        if (player->isColliding(rock->getTransform()))
        {
            rock->move({-speed, 0, 0});
            colliding->update();
            rockColliding = true;
        }
    }

    if (Input::getKeyPressed(Key_W))
    {
        // W
        transform.position.y -= 2.0f;
        if (!collisionManager->checkCollision(transform))
        {
            player->move({0.f, -speed, 0.f});
        }
    }

    if (Input::getKeyPressed(Key_S))
    {
        // S
        transform.position.y += 2.0f;
        if (!collisionManager->checkCollision(transform))
        {
            player->move({0.f, speed, 0.f});
        }
    }
    player->update();
    UpdatePlayerPosition();
}

void Game::uninit()
{
    cout << "External uninit!!!\n";
    delete background;
    delete idle;
    delete rock;
    delete moving;
    delete colliding;
    delete floorCollision;
    delete collisionManager;
}

int main()
{
    Game game;
    game.start();
}
