#include "base_game.h"

#include <iostream>

using namespace std;

class Game : public gllib::BaseGame {
private:
    gllib::Triangle triangle;
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

    triangle.draw();
}

void Game::uninit() {
    cout << "External uninit!!!\n";
}

int main() {
    Game game;
    game.start();
}
