#include "base_game.h"

#include <iostream>

using namespace std;

class Game : public gllib::BaseGame {
private:
    gllib::Triangle* triangle;
    gllib::Sprite* sprite;
    gllib::Animation* coin;
    float animSpeed, nextFrame;

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
    window->setVsyncEnabled(false);
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
    coin = new gllib::Animation(trs2, { 1.0f, 1.0f, 1.0f, 1.0f });

    sprite->addTexture("sus.png", true);
    sprite->setMirroredX(true);
    int textureWidth = 16;
    unsigned int coinTex = gllib::Loader::loadTexture("coin.png", true);
    coin->addFrames(coinTex, textureWidth, 16, 8, 1);
    
    coin->setCurrentFrame(0);

    coin->setDurationInSecs(.6);

    animSpeed = .075f;
    nextFrame = 0;
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

    coin->update();

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
        sprite->setMirroredX(true);
    }
    if (sprite->getPosition().x + (sprite->getScale().x * .5) >= window->getWidth()) {
        x = -1;
        sprite->setMirroredX(false);
        gllib::Vector3 scale = coin->getScale();
    }

    if (sprite->getPosition().y - (sprite->getScale().y * .5) <= 0) {
        y = 1;
        sprite->setMirroredY(true);
    }
    if (sprite->getPosition().y + (sprite->getScale().y * .5) >= window->getHeight()) {
        y = -1;
        sprite->setMirroredY(false);
    }
    
    gllib::Vector3 scale = coin->getScale();
    scale.x += (25.0f * gllib::LibTime::getDeltaTime()) * x;
    scale.y += (25.0f * gllib::LibTime::getDeltaTime()) * x;
    coin->setScale(scale);

    sprite->move({static_cast<float>(x * (speed * gllib::LibTime::getDeltaTime())), 
                  static_cast<float>(y * (speed * gllib::LibTime::getDeltaTime())), 
                  0.0f});
    //sprite->rotate({ 0.0f, 0.0f, static_cast<float>(gllib::LibTime::getDeltaTime() * -60.0f) });
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