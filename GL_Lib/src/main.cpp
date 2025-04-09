#include "base_game.h"

#include <iostream>

using namespace std;

class Game : public gllib::BaseGame {
private:
    gllib::Box* box;
    bool wireframeMode = true;

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
    if (Input::getKeyReleased(Key_W)) {
        wireframeMode = !wireframeMode;
        gllib::Renderer::setLazyWireframeMode(wireframeMode);
    }

    gllib::Quaternion rot = box->getRotationQuat();
    rot.x += gllib::LibTime::getDeltaTime() * 30.0f;
    rot.y += gllib::LibTime::getDeltaTime() * 20.0f;
    box->setRotationQuat(rot);

    // Draw
    gllib::Renderer::clear();

    box->draw();
}

void Game::uninit() {
    cout << "External uninit!!!\n";
    delete box;
}

int main() {
    Game game;
    game.start();
}
