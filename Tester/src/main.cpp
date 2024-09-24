#include "base_game.h"

#include <iostream>

using namespace std;

class Game : public gllib::BaseGame {
private:
    gllib::Triangle* yellowTriangle;
    gllib::Triangle* pinkTraingle;

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
    pinkTraingle = new gllib::Triangle(
        { static_cast<float>(window->getWidth() - 100.0f), static_cast<float>(window->getHeight() - 100.0f), 0.0f },
        {0.0f, 0.0f, 0.0f},
        {57.74f, 50.0f, 1.0f},
        {1.0f, 0.25f, 0.6f, 1.0f}
    );
    yellowTriangle = new gllib::Triangle(
        { 100.0f, 100.0f, 0.0f },
        {0.0f, 0.0f, 0.0f},
        {57.74f, 50.0f, 1.0f},
        {1.0f, 1.0f, 0.0f, 1.0f}
    );

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
    gllib::Renderer::clear();

    gllib::Vector3 ytp = yellowTriangle->getPosition();
    ytp.y += static_cast<float>(gllib::LibTime::getDeltaTime() * 100.0f);
    yellowTriangle->setPosition(ytp);
    yellowTriangle->rotate({0.0f, 0.0f, -static_cast<float>(gllib::LibTime::getDeltaTime() * 100.0f)});

    gllib::Vector3 ptp = pinkTraingle->getPosition();
    ptp.y -= static_cast<float>(gllib::LibTime::getDeltaTime() * 100.0f);
    pinkTraingle->setPosition(ptp);
    pinkTraingle->rotate({ 0.0f, 0.0f, static_cast<float>(gllib::LibTime::getDeltaTime() * 100.0f) });

    yellowTriangle->draw();
    pinkTraingle->draw();
}


void Game::uninit() {
    cout << "External uninit!!!\n";
    delete yellowTriangle;
    delete pinkTraingle;
}

int main() {
    Game game;
    game.start();
}