#include "base_game.h"

#include <iostream>
#include <map>

#include "collisionManager.h"

using namespace std;

enum class CharacterStatus {
    idle,
    attack,
    dying
};

class Game : public gllib::BaseGame {
private:
    gllib::Animation* erikaAttack;
    gllib::Animation* erikaDeath;
    bool erikaAttacking;
    bool erikaDead;
    gllib::Animation* hectorAttack;
    gllib::Animation* hectorDeath;
    bool hectorAttacking;
    bool hectorDead;

    bool faded;
    float nextFade;
    float fadeInterval;

    void handleInput();

protected:
    void init() override;
    void update() override;
    void uninit() override;

public:
    Game();
    ~Game() override;
};

Game::Game() {
    window->setVsyncEnabled(false);
    cout << "Game created!\n";
}

Game::~Game() {
    cout << "Game destroyed!\n";
}

void Game::init() {
    srand(time(nullptr));
    window->setTitle("Parcial 2 Salazar");
    gllib::Transform trs1;
    trs1.position = { 200.0f, 200.0f, 0.0f };
    trs1.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs1.scale = { 100.0f, 100.0f, 0.0f };
    erikaAttack = new gllib::Animation(trs1, { 1.0f, 1.0f, 1.0f, 1.0f });
    unsigned int erikaTex = gllib::Loader::loadTexture("Eirika_Lord_Sword_Sprite_Sheet.gif", true);
    erikaAttack->addFrame(erikaTex, 3, 6, 36, 34);
    erikaAttack->addFrame(erikaTex, 42, 6, 37, 34);
    erikaAttack->addFrame(erikaTex, 85, 3, 29, 40);
    erikaAttack->addFrame(erikaTex, 123, 3, 28, 41);
    erikaAttack->addFrame(erikaTex, 157, 3, 28, 41);
    erikaAttack->addFrame(erikaTex, 193, 7, 28, 33);
    erikaAttack->addFrame(erikaTex, 229, 7, 28, 31);
    erikaAttack->addFrame(erikaTex, 264, 6, 29, 31);
    erikaAttack->addFrame(erikaTex, 296, 3, 62, 30);
    erikaAttack->addFrame(erikaTex, 361, 6, 54, 27);
    erikaAttack->addFrame(erikaTex, 421, 6, 54, 27);
    erikaAttack->addFrame(erikaTex, 3, 52, 53, 28);
    erikaAttack->addFrame(erikaTex, 59, 51, 52, 29);
    erikaAttack->addFrame(erikaTex, 114, 48, 42, 32);
    erikaAttack->addFrame(erikaTex, 161, 48, 33, 34);
    erikaAttack->addFrame(erikaTex, 207, 45, 20, 32);
    erikaAttack->addFrame(erikaTex, 242, 45, 20, 32);
    erikaAttack->addFrame(erikaTex, 273, 47, 36, 33);
    erikaAttack->setDurationInSecs(1.5f);
    erikaAttack->reset();
    erikaAttack->setMirroredX(true);
    erikaDeath = new gllib::Animation(trs1, { 1.0f, 1.0f, 1.0f, 1.0f });
    erikaDeath->addFrame(erikaTex, 380, 138, 36, 35);
    erikaDeath->addFrame(erikaTex, 420, 137, 28, 36);
    erikaDeath->addFrame(erikaTex, 454, 137, 28, 36);
    erikaDeath->setDurationInSecs(.2f);
    erikaDeath->reset();
    erikaDeath->setMirroredX(true);
    erikaDead = false;
    erikaAttacking = false;

    gllib::Transform trs2;
    trs2.position = { 400.0f, 200.0f, 0.0f };
    trs2.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs2.scale = { 100.0f, 100.0f, 0.0f };
    hectorAttack = new gllib::Animation(trs2, { 1.0f, 1.0f, 1.0f, 1.0f });
    unsigned int hectorTex = gllib::Loader::loadTexture("Hector_Lord_Sprite_Sheet.gif", true);
    hectorAttack->addFrame(hectorTex, 7, 19, 39, 36);
    hectorAttack->addFrame(hectorTex, 49, 19, 30, 36);
    hectorAttack->addFrame(hectorTex, 80, 20, 27, 35);
    hectorAttack->addFrame(hectorTex, 108, 20, 27, 35);
    hectorAttack->addFrame(hectorTex, 138, 20, 27, 35);
    hectorAttack->addFrame(hectorTex, 167, 0, 45, 55);
    hectorAttack->addFrame(hectorTex, 219, 0, 59, 55);
    hectorAttack->addFrame(hectorTex, 6, 90, 45, 27);
    hectorAttack->addFrame(hectorTex, 54, 91, 45, 27);
    hectorAttack->addFrame(hectorTex, 103, 91, 45, 27);
    hectorAttack->addFrame(hectorTex, 158, 76, 34, 43);
    hectorAttack->addFrame(hectorTex, 196, 61, 29, 46);
    hectorAttack->addFrame(hectorTex, 235, 61, 29, 46);
    hectorAttack->addFrame(hectorTex, 6, 126, 41, 48);
    hectorAttack->addFrame(hectorTex, 49, 127, 42, 47);
    hectorAttack->addFrame(hectorTex, 95, 134, 45, 40);
    hectorAttack->addFrame(hectorTex, 144, 143, 47, 31);
    hectorAttack->addFrame(hectorTex, 195, 143, 46, 31);
    hectorAttack->addFrame(hectorTex, 246, 143, 41, 31);
    hectorAttack->addFrame(hectorTex, 7, 190, 54, 32);
    hectorAttack->addFrame(hectorTex, 78, 192, 47, 30);
    hectorAttack->addFrame(hectorTex, 127, 190, 44, 32);
    hectorAttack->addFrame(hectorTex, 175, 190, 28, 32);
    hectorAttack->setDurationInSecs(2.0f);
    hectorAttack->reset();
    hectorDeath = new gllib::Animation(trs2, { 1.0f, 1.0f, 1.0f, 1.0f });
    hectorDeath->addFrame(hectorTex, 185, 307, 54, 40);
    hectorDeath->addFrame(hectorTex, 6, 126, 41, 48);
    hectorDeath->addFrame(hectorTex, 48, 352, 55, 36);
    hectorDeath->addFrame(hectorTex, 242, 391, 43, 36);
    hectorDeath->setDurationInSecs(.3);
    hectorDeath->reset();
    hectorDead = false;
    hectorAttacking = false;

    faded = false;
    nextFade = .0f;
    fadeInterval = .15f;
}


void Game::update() {
    // Update
    handleInput();
    erikaDeath->update();
    hectorDeath->update();

    if (nextFade < gllib::LibTime::getElapsedTime()) {
        nextFade = gllib::LibTime::getElapsedTime() + fadeInterval;
        faded = !faded;
    }

    // Draw
    gllib::Renderer::clear();

    gllib::Shader::useShaderProgram(shaderProgramTexture);
    // draw textures
    if (!erikaDead) {
        erikaAttack->draw();
    }
    else {
        if (faded) {
            erikaDeath->draw();
        }
    }
    if (!hectorDead) {
        hectorAttack->draw();
    }
    else {
        if (faded) {
            hectorDeath->draw();
        }
    }

    gllib::Shader::useShaderProgram(shaderProgramSolidColor);
    // draw shapes
}

void Game::uninit() {
    delete erikaAttack;
    delete erikaDeath;
    delete hectorAttack;
    delete hectorDeath;
}

void Game::handleInput() {
    if (Input::getKeyPressed(Key_Q)) {
        erikaAttack->update();
        hectorDead = true;
    }
    if (Input::getKeyReleased(Key_Q)) {
        erikaAttack->reset();
        hectorDead = false;
    }

    if (Input::getKeyPressed(Key_E)) {
        hectorAttack->update();
        erikaDead = true;
    }
    if (Input::getKeyReleased(Key_E)) {
        hectorAttack->reset();
        erikaDead = false;
    }
}

int main() {
    Game game;
    game.start();
}