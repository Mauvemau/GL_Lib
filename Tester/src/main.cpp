#include "base_game.h"

#include <iostream>

using namespace std;

class Game : public gllib::BaseGame {
private:

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
}

Game::~Game() {
    cout << "Game destroyed!\n";
}

void Game::init() {
    cout << "External init!!!!\n";

    srand(time(nullptr));
    window->setTitle("Salazar Primer Parcial");
}

void Game::update() {

}


void Game::uninit() {
    cout << "External uninit!!!\n";
}

int main() {
    Game game;
    game.start();
}