#include "base_game.h"

#include <iostream>

using namespace std;

class Game : public gllib::BaseGame {
private:
    gllib::Rectangle* rectangle;

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
    trs.position = { 0.0f, 200.0f, 0.0f };
    trs.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs.scale = { 1000.0f, 1000.0f, 0.0f };
    rectangle = new gllib::Rectangle(trs, { 0.0f, 0.0f, 1.0f, 1.0f });
}

Game::~Game() {
    cout << "Game destroyed!\n";
}

void Game::init() {
    cout << "External init!\n";

    window->setTitle("Parcial Barra");
}

void Game::update() {
    
    rectangle->rotate({ 0.0f, 0.0f, static_cast<float>(gllib::LibTime::getDeltaTime() * 100.0f)});
    gllib::Vector3 direction = rectangle->getPosition();
    direction.x += 1.0f;
    rectangle->setPosition(direction);
    
    rectangle->setScale(rectangle->getScale() * 0.99f);
    
    gllib::Renderer::clear();

    rectangle->draw();
}


void Game::uninit() {
    cout << "External uninit!\n";
    delete rectangle;
}

int main() {
    Game game;
    game.start();
}