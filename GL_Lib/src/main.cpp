#include "base_game.h"

#include <iostream>

using namespace std;

class Game : public gllib::BaseGame {
private:
    gllib::Triangle* triangle;
protected:
    void init() override;
    void update() override;
    void uninit() override;
public:
    Game();
    ~Game() override;
};

Game::Game() {
    cout << "Game created!\n";
    gllib::Transform trf;
    trf.position = { 100.0f, 100.0f, 0.0f };
    trf.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trf.scale = { 57.74f, 50.0f, 0.0f };
    triangle = new gllib::Triangle(trf, {0.85f, 0.2f, 0.4f, 1.0f});
}

Game::~Game() {
    cout << "Game destroyed!\n";
}

void Game::init() {
    cout << "External init!!!!\n";
    window->setTitle("Example game lmao");
}

void Game::update() {
    // Update
    gllib::Quaternion rot = triangle->getRotationQuat();
    rot.z += gllib::LibTime::getDeltaTime() * 30.0f;
    triangle->setRotationQuat(rot);

    // Draw
    gllib::Renderer::clear();

    triangle->draw();
}

void Game::uninit() {
    cout << "External uninit!!!\n";
    delete triangle;
}

int main() {
    Game game;
    game.start();
}
