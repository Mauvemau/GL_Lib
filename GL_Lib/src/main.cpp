#include "base_game.h"

#include <iostream>

using namespace std;

class Game : public gllib::BaseGame {
private:
    gllib::Triangle* triangle;
    gllib::Triangle* triangle2;
    
    bool movingRight;
    float color = 0;

    void moveTriangle2(float speed);

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
    gllib::Transform trs;
    trs.position = {100.0f, 100.0f, 0.0f};
    trs.rotationQuat = {0.0f, 0.0f, 0.0f, 0.0f};
    trs.scale = {57.74f, 50.0f, 0.0f};
    triangle = new gllib::Triangle(trs, {0.85f, 0.2f, 0.4f, 1.0f});

    triangle2 = new gllib::Triangle(trs * 2, {1.0f, 1.0f, 1.0f, 1.0f});
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
    gllib::Quaternion rot = triangle->getRotationQuat();
    rot.z += gllib::LibTime::getDeltaTime() * 30.0f;
    triangle->setRotationQuat(rot);


    gllib::Vector3 rotEuler = gllib::Vector3(0, 0, 10);
    triangle2->rotate(rotEuler);
    color += 0.5 * gllib::LibTime::getDeltaTime();
    if(color > 1.0f) color = 0.0f;
    triangle2->setColor({color, color, 1, 1.0f});
    
    moveTriangle2(50);
    
    // Draw
    gllib::Renderer::clear();

    triangle->draw();
    triangle2->draw();
}

void Game::moveTriangle2(float speed) {
    gllib::Vector3 position = triangle2->getPosition();
    if (movingRight)
    {
        position.x += gllib::LibTime::getDeltaTime() * speed; // Adjust speed as needed
        if (position.x >= 640.0f)
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
    triangle2->setPosition(position);
}


void Game::uninit() {
    cout << "External uninit!!!\n";
    delete triangle;
    delete triangle2;
}

int main() {
    Game game;
    game.start();
}
