#include "base_game.h"

#include <iostream>

using namespace std;

class Game : public gllib::BaseGame {
private:
    gllib::Triangle* triangle;
    gllib::Rectangle* rectangle;

    bool movingRight;
    float color = 0;

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
    window->setVsyncEnabled(true);
    cout << "Game created!\n";

   
    gllib::Transform trs;
    trs.position = { 100.0f, 100.0f, 0.0f };
    trs.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs.scale = { 57.74f, 50.0f, 0.0f };
    triangle = new gllib::Triangle(trs, { 0.85f, 0.2f, 0.4f, 1.0f });

    gllib::Transform trs2;
    trs2.position = { 200.0f, 200.0f, 0.0f };
    trs2.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs2.scale = { 100.0f, 100.0f, 0.0f };
    rectangle = new gllib::Rectangle(trs2, { 1.0f, 1.0f, 1.0f, 1.0f });

    movingRight = false;
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

    if(Input::getKeyDown(Keys::Key_A))
    {
        cout << "A key is pressed!\n";
    }

    if(Input::getKeyUp(Key_A))
    {
        cout << "A key is released!\n";
        
    }
    gllib::Quaternion rot = triangle->getRotationQuat();
    rot.z += gllib::LibTime::getDeltaTime() * 30.0f;
    triangle->setRotationQuat(rot);

    rectangle->rotate({ 0.0f, 0.0f, static_cast<float>(gllib::LibTime::getDeltaTime() * 100.0f)});
    color += 0.5 * gllib::LibTime::getDeltaTime();
    if (color > 1.0f) color = 0.0f;
    rectangle->setColor({ color, color, 1, 1.0f });

    moveRectangle(50);

    // Draw
    gllib::Renderer::clear();

    triangle->draw();
    rectangle->draw();
}

void Game::moveRectangle(float speed) {
    gllib::Vector3 position = rectangle->getPosition();
    if (movingRight)
    {
        position.x += gllib::LibTime::getDeltaTime() * speed; // Adjust speed as needed
        if (position.x >= window->getWidth() - 100.0f)
        {
            movingRight = false;
        }
    }
    else
    {
        position.x -= gllib::LibTime::getDeltaTime() * speed; // Adjust speed as needed
        if (position.x <= 100.0f)
        {
            movingRight = true;
        }
    }
    rectangle->setPosition(position);
}


void Game::uninit() {
    cout << "External uninit!!!\n";
    delete triangle;
    delete rectangle;
}

int main() {
    Game game;
    game.start();
}