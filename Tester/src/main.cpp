#include "base_game.h"

#include <iostream>

using namespace std;

class Game : public gllib::BaseGame {
private:
    gllib::Triangle* triangle;
    gllib::Sprite* sprite;
    gllib::Sprite* coin;

    void moveRectangle(float speed);

protected:
    void init() override;
    void update() override;
    void uninit() override;

public:
    Game();
    ~Game() override;
};

Game::Game() {
    window->setVsyncEnabled(true);
    cout << "Game created!\n";

    gllib::Transform trs;
    trs.position = { 100.0f, 100.0f, 0.0f };
    trs.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs.scale = { 57.74f, 50.0f, 0.0f };
    triangle = new gllib::Triangle(trs, { 0.85f, 0.2f, 0.4f, 1.0f });

    gllib::Transform trs2;
    trs2.position = { 400.0f, 400.0f, 0.0f };
    trs2.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs2.scale = { 100.0f, 100.0f, 0.0f };
    sprite = new gllib::Sprite(trs2, { 1.0f, 1.0f, 1.0f, 1.0f });
    coin = new gllib::Sprite(trs2, { 1.0f, 1.0f, 1.0f, 1.0f });

    sprite->addTexture("sus.png");
    unsigned int coinTex = gllib::Loader::loadTexture("coin.png");
    coin->addTexture(coinTex, 0, 0, 15, 16);
}

Game::~Game() {
    cout << "Game destroyed!\n";
}

void Game::init() {
    cout << "External init!!!!\n";

    srand(time(nullptr));
    window->setTitle("Example game lmao");
}

void Game::update() {
    // Update

    if(Input::getKeyPressed(Keys::Key_A))
    {
        cout << "A key is pressed!\n";
    }

    if(Input::getKeyReleased(Key_A))
    {
        cout << "A key is released!\n";
        coin->setNextTexture();
    }
    gllib::Quaternion rot = triangle->getRotationQuat();
    rot.z += gllib::LibTime::getDeltaTime() * 30.0f;
    triangle->setRotationQuat(rot);

    moveRectangle(100);

    // Draw
    gllib::Renderer::clear();

    gllib::Shader::useShaderProgram(shaderProgramSolidColor);

    triangle->draw();

    gllib::Shader::useShaderProgram(shaderProgramTexture);
    sprite->draw();
    coin->draw();
}

static int x = 1;
static int y = 1;

void Game::moveRectangle(float speed) {
    if (sprite->getPosition().x - (sprite->getScale().x * .5) <= 0) {
        x = 1;
    }
    if (sprite->getPosition().x + (sprite->getScale().x * .5) >= window->getWidth()) {
        x = -1;
    }

    if (sprite->getPosition().y - (sprite->getScale().y * .5) <= 0) {
        y = 1;
    }
    if (sprite->getPosition().y + (sprite->getScale().y * .5) >= window->getHeight()) {
        y = -1;
    }
    
    sprite->move({static_cast<float>(x * (speed * gllib::LibTime::getDeltaTime())), 
                  static_cast<float>(y * (speed * gllib::LibTime::getDeltaTime())), 
                  0.0f});
    sprite->rotate({ 0.0f, 0.0f, static_cast<float>(gllib::LibTime::getDeltaTime() * -60.0f) });
}


void Game::uninit() {
    cout << "External uninit!!!\n";
    delete triangle;
    delete sprite;
    delete coin;
}

int main() {
    Game game;
    game.start();
}