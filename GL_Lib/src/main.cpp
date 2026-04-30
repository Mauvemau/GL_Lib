#include "base_game.h"

#include <iostream>

#include "camera/first_person_camera.h"
#include "camera/third_person_camera.h"

using namespace std;

class Game : public gllib::BaseGame {
private:
    gllib::ThirdPersonCamera* camera;

    gllib::Box* box;
    gllib::Animation* coin;
    gllib::Box* player;
    gllib::Box* floor;
    gllib::Box* lightBox;
    bool wireframeMode = false;

    float playerSpeed = 2.5f;

    float cameraSensitivity = .5f;
    bool cameraLocked = true;

    bool lit = false;
    bool controllingLight = false;

    gllib::Vector3 velocity = {1.5f, 1.25f, 0.0f};
    float myTime = 0;

protected:
    void init() override;
    void update() override;
    void uninit() override;

    void handlePlayerInput();

public:
    Game();
    ~Game() override;
};

Game::Game() {
    window->setVsyncEnabled(true);
    cout << "Game created!\n";
    gllib::Renderer::setLazyWireframeMode(wireframeMode);

    camera = new gllib::ThirdPersonCamera(gllib::Vector3(0.0f, 0.0f, 0.0f),
                                         gllib::Vector3(0.0f, 0.0f, 1.0f), cameraSensitivity, 5.0f);

    gllib::Transform trs;
    trs.position = { 0.0f, 0.0f, 0.0f };
    trs.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs.scale = { 1.0f, 1.0f, 1.0f };
    box = new gllib::Box(trs, { 0.85f, 0.2f, 0.4f, 1.0f });

    gllib::Transform trs2;
    trs2.position = { 0.0f, 0.0f, 0.0f };
    trs2.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs2.scale = { 0.5f, 0.5f, 0.0f };
    coin = new gllib::Animation(trs2, { 1.0f, 1.0f, 1.0f, 1.0f });
    int textureWidth = 16;
    unsigned int coinTex = gllib::Loader::loadTexture("coin.png", true);
    coin->addFrames(coinTex, textureWidth, 16, 8, 1);
    coin->setCurrentFrame(0);
    coin->setDurationInSecs(.6);

    gllib::Transform trs3;
    trs3.position = { 0.0f, -1.0f, -3.0f };
    trs3.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs3.scale = { 0.5f, 0.5f, 0.5f };

    gllib::Material playerMat = gllib::Material({0.329412f, 0.223529f, 0.027451f},
                                                {0.780392f, 0.568627f, 0.113725f},
                                                {0.992157f, 0.941176f, 0.807843f},
                                                27.8974f);
    player = new gllib::Box(trs3, { 1.0f, 1.0f, 1.0f, 1.0f }, playerMat);

    gllib::Transform trs4;
    trs4.position = { 0.0f, -1.5f, 0.0f };
    trs4.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs4.scale = { 6.0f, .1f, 6.0f };
    gllib::Material floorMat = gllib::Material({0.05f, 0.05f, 0.05f},
                                                {0.5f, 0.5f, 0.5f},
                                                {0.7f, 0.7f, 0.7f},
                                                10.0f);
    floor = new gllib::Box(trs4, { 1.0f, 1.0f, 1.0f, 1.0f }, floorMat);

    gllib::Transform trs5;
    trs5.position = { 3.0f, 0.0f, 3.0f };
    trs5.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs5.scale = { 0.25f, 0.25f, 0.25f };
    lightBox = new gllib::Box(trs5, { 1.0f, 1.0f, 1.0f, 1.0f });
}

Game::~Game() {
    cout << "Game destroyed!\n";
}

void Game::init() {
    cout << "External init!!!!\n";

    srand(time(nullptr));
    window->setTitle("Cool rotating box");
}



void Game::update() {
    // Update
    handlePlayerInput();

    gllib::Quaternion rot = box->getRotationQuat();
    rot.x += 30.0f * gllib::LibTime::getDeltaTime();
    rot.y += 20.0f * gllib::LibTime::getDeltaTime();
    box->setRotationQuat(rot);

    myTime += gllib::LibTime::getDeltaTime();
    coin->update();
    gllib::Vector3 pos = coin->getPosition();
    pos.x += velocity.x * gllib::LibTime::getDeltaTime();
    pos.y += velocity.y * gllib::LibTime::getDeltaTime();

    if (pos.x < -1.0f || pos.x > 1.0f) {
        velocity.x *= -1.0f;
    }
    if (pos.y < -1.0f || pos.y > 1.0f) {
        velocity.y *= -1.0f;
    }
    float amplitude = 1.0f;
    float speed = 2.0f;

    pos.z = sin(myTime * speed) * amplitude;
    coin->setPosition(pos);

    // Draw
    gllib::Renderer::clear();

    gllib::Shader::useShaderProgram(shaderProgramSolidColor);
    lightBox->draw();
    lit ?   gllib::Shader::useShaderProgram(shaderProgramSolidColorLit) :
            gllib::Shader::useShaderProgram(shaderProgramSolidColor);
    gllib::Renderer::setUpLightingUniforms();

    box->draw();
    player->draw();
    floor->draw();
    gllib::Shader::useShaderProgram(shaderProgramTexture);
    coin->draw();
}

void Game::uninit() {
    cout << "External uninit!!!\n";
    delete box;
    delete coin;
    delete lightBox;
    delete floor;
    delete player;
    delete camera;
}

void Game::handlePlayerInput() {
    if (Input::getKeyPressed(Key_Escape)) {
        stop();
    }
    if (Input::getKeyReleased(Key_Q)) {
        wireframeMode = !wireframeMode;
        gllib::Renderer::setLazyWireframeMode(wireframeMode);
    }
    if (Input::getKeyReleased(Key_L)) {
        lit = !lit;
    }
    if (Input::getKeyReleased(Key_K)) {
        controllingLight = !controllingLight;
    }
    if (Input::getKeyReleased(Key_Tab)) {
        cameraLocked = !cameraLocked;
        Input::setCursorLocked(!cameraLocked);
    }

    if (Input::getKeyPressed(Key_W)) {
        if (controllingLight) {
            lightBox->move(-gllib::ThirdPersonCamera::forwardWorld() * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
        else {
            gllib::Vector3 forwardDir = camera->forward();
            forwardDir.y = .0f;
            forwardDir = forwardDir.normalized();
            player->move(forwardDir * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
    }
    if (Input::getKeyPressed(Key_A)) {
        if (controllingLight) {
            lightBox->move(gllib::ThirdPersonCamera::rightWorld() * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
        else {
            player->move(-camera->right() * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
    }
    if (Input::getKeyPressed(Key_S)) {
        if (controllingLight) {
            lightBox->move(gllib::ThirdPersonCamera::forwardWorld() * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
        else {
            gllib::Vector3 forwardDir = camera->forward();
            forwardDir.y = .0f;
            forwardDir = forwardDir.normalized();
            player->move(-forwardDir * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
    }
    if (Input::getKeyPressed(Key_D)) {
        if (controllingLight) {
            lightBox->move(-gllib::ThirdPersonCamera::rightWorld() * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
        else {
            player->move(camera->right() * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
    }
    if (Input::getKeyPressed(Key_Space)) {
        if (controllingLight) {
            lightBox->move(gllib::ThirdPersonCamera::upWorld() * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
        else {
            player->move(gllib::Camera::upWorld() * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
    }
    if (Input::getKeyPressed(Key_LeftCtrl)) {
        if (controllingLight) {
            lightBox->move(-gllib::ThirdPersonCamera::upWorld() * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
        else {
            player->move(-gllib::Camera::upWorld() * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
    }

    camera->updateCamera(player->getPosition());
    gllib::Renderer::setLightPos(glm::vec3(lightBox->getPosition()));
    if (cameraLocked) return;
    camera->updateMouseInput();
}

int main() {
    Game game;
    game.start();
}
