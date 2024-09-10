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
    trf.position = {10, 10, 10};
    trf.rotation = { 0, 0, 0 };
    trf.scale = { 1, 1, 1 };
    triangle = new gllib::Triangle(trf);
}

Game::~Game() {
    cout << "Game destroyed!\n";
}

void Game::init() {
    cout << "External init!!!!\n";
    window->setTitle("Example game lmao");
}

void Game::update() {
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
