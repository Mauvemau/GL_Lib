#include "base_game.h"

#include <iostream>

using namespace std;

class Game : public gllib::BaseGame {
private:
    gllib::Triangle* triangle;
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
    trs.position = { 0.0f, 0.0f, -5.0f };
    trs.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs.scale = { .5645f, .5f, 0.0f };
    triangle = new gllib::Triangle(trs, { 0.85f, 0.2f, 0.4f, 1.0f });
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

    if (Input::getKeyPressed(Keys::Key_A))
    {
        cout << "A key is pressed!\n";
    }

    if (Input::getKeyReleased(Key_A))
    {
        cout << "A key is released!\n";

    }
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
