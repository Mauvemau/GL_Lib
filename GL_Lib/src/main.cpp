#include "base_game.h"

#include <iostream>

using namespace std;

class Game : public gllib::BaseGame {
private:
    gllib::Box* box;
    gllib::Animation* coin;
    bool wireframeMode = true;

    gllib::Vector3 velocity = {1.5f, 1.25f, 0.0f};
    float myTime = 0;

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
    gllib::Renderer::setLazyWireframeMode(wireframeMode);

    gllib::Transform trs;
    trs.position = { 0.0f, 0.0f, 0.0f };
    trs.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs.scale = { 1.0f, 1.0f, 1.0f };
    box = new gllib::Box(trs, { 0.85f, 0.2f, 0.4f, 1.0f });

    gllib::Transform trs2;
    trs2.position = { 0.0f, 0.0f, 0.0f };
    trs2.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs2.scale = { 0.5f, 0.5f, 0.0f };
    coin = new gllib::Animation(trs2, { 1.0f, 1.0f, 1.0f, 1.0f });
    int textureWidth = 16;
    unsigned int coinTex = gllib::Loader::loadTexture("coin.png", true);
    coin->addFrames(coinTex, textureWidth, 16, 8, 1);
    coin->setCurrentFrame(0);
    coin->setDurationInSecs(.6);
}

Game::~Game() {
    cout << "Game destroyed!\n";
}

void Game::init() {
    cout << "External init!!!!\n";

    srand(time(nullptr));
    window->setTitle("Cool rotating box");
}

void Game::update() {
    // Update
    if (Input::getKeyReleased(Key_W)) {
        wireframeMode = !wireframeMode;
        gllib::Renderer::setLazyWireframeMode(wireframeMode);
    }

    gllib::Quaternion rot = box->getRotationQuat();
    rot.x += 30.0f * gllib::LibTime::getDeltaTime();
    rot.y += 20.0f * gllib::LibTime::getDeltaTime();
    box->setRotationQuat(rot);

    myTime += gllib::LibTime::getDeltaTime();
    coin->update();
    gllib::Vector3 pos = coin->getPosition();
    pos.x += velocity.x * gllib::LibTime::getDeltaTime();
    pos.y += velocity.y * gllib::LibTime::getDeltaTime();

    if (pos.x < -1.0f || pos.x > 1.0f) {
        velocity.x *= -1.0f;
    }
    if (pos.y < -1.0f || pos.y > 1.0f) {
        velocity.y *= -1.0f;
    }
    float amplitude = 1.0f;
    float speed = 2.0f;

    pos.z = sin(myTime * speed) * amplitude;
    coin->setPosition(pos);

    // Draw
    gllib::Renderer::clear();

    gllib::Shader::useShaderProgram(shaderProgramSolidColor);
    box->draw();
    gllib::Shader::useShaderProgram(shaderProgramTexture);
    coin->draw();
}

void Game::uninit() {
    cout << "External uninit!!!\n";
    delete box;
    delete coin;
}

int main() {
    Game game;
    game.start();
}
