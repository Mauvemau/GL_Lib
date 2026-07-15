/*
AABB:

Transformacion nodos recursiva:

- Calculo Transform
> Llamar a calcular nodos hijo
- Calcular bounding box

Dibujar las bounding boxes
Imprimir la cantidad de nodos que se estan mandando a dibujar vs la cantidad de nodos en cpu
*/

#include "base_game.h"

#include <iostream>

using namespace std;

class Game : public gllib::BaseGame {
private:
    gllib::FirstPersonCamera* cameraFp;
    gllib::ThirdPersonCamera* cameraTp;

    gllib::Box* player;
    gllib::Model* playerModel;
    gllib::Box* floor;
    gllib::Box* wall;
    gllib::DirectionalLight* dirLight;
    gllib::Box* lightBox;
    gllib::Box* lightBox2;
    gllib::Box* lightBox3;
    gllib::Box* lightBox4;
    gllib::PointLight* light;
    gllib::PointLight* light2;
    gllib::PointLight* light3;
    gllib::PointLight* light4;
    gllib::SpotLight* spotLight;
    gllib::LightingData* lightData;
    gllib::Model* cannon;
    gllib::ModelNode* cannonFrontWheels;
    gllib::ModelNode* cannonBarrel;
    bool wireframeMode = false;

    float playerSpeed = 2.5f;

    float cameraSensitivity = .5f;
    bool thirdPerson = false;
    bool cameraLocked = true;

    bool lit = true;
    bool controllingLight = false;
    float rgbTimeAccumulator = 0.0f;

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
    cameraFp = new gllib::FirstPersonCamera(gllib::Vector3(0.0f, 0.0f, 0.0f),
                                         gllib::Vector3(0.0f, 0.0f, 1.0f), cameraSensitivity);
    cameraTp = new gllib::ThirdPersonCamera(gllib::Vector3(0.0f, 0.0f, 0.0f),
                                         gllib::Vector3(0.0f, 0.0f, 1.0f), cameraSensitivity, 5.0f);

    gllib::Transform trs3;
    trs3.position = { 0.0f, -1.0f, -3.0f };
    trs3.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs3.scale = { 2.0f, 2.0f, 2.0f };
    gllib::Material playerMat = gllib::Material({0.329412f, 0.223529f, 0.027451f},
                                                {0.780392f, 0.568627f, 0.113725f},
                                                {0.992157f, 0.941176f, 0.807843f},
                                                27.8974f);
    player = new gllib::Box(trs3, { 1.0f, 1.0f, 1.0f, 1.0f }, playerMat);

    gllib::ModelData playerModelData = gllib::ModelData("rubber_duck_toy_4k.fbx");
    playerModel = new gllib::Model(playerModelData, trs3, {1.0f, 1.0f, 1.0f, 1.0f});

    gllib::Transform trs4;
    trs4.position = { 0.0f, -1.5f, 0.0f };
    trs4.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs4.scale = { 10.0f, .1f, 10.0f };
    gllib::Material floorMat = gllib::Material({0.05f, 0.05f, 0.05f},
                                                {0.5f, 0.5f, 0.5f},
                                                {0.7f, 0.7f, 0.7f},
                                                10.0f);
    floor = new gllib::Box(trs4, { 1.0f, 1.0f, 1.0f, 1.0f }, floorMat);
    gllib::Transform trs6;
    trs6.position = { 0.0f, 1.0f, 5.0f };
    trs6.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs6.scale = { 10.0f, 5.0f, 0.1f };
    wall = new gllib::Box(trs6, { 1.0f, 1.0f, 1.0f, 1.0f }, floorMat);

    gllib::Vector3 dirLightDirection = gllib::Vector3(-0.2f, -1.0f, -0.3f);
    dirLight = new gllib::DirectionalLight(dirLightDirection, {0.30f, 0.30f, 0.35f, 1.0f});
    gllib::Transform trs5;
    trs5.position = { -1.5f, 0.5f, 0.0f };
    trs5.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs5.scale = { 0.25f, 0.25f, 0.25f };
    lightBox = new gllib::Box(trs5, { 1.0f, 1.0f, 1.0f, 1.0f });
    light = new gllib::PointLight(trs5.position);

    gllib::Transform trs10;
    trs10.position = { -2.0f, 0.0f, 3.0f };
    trs10.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs10.scale = { 0.25f, 0.25f, 0.25f };
    lightBox2 = new gllib::Box(trs10, { 0.2f, 0.4f, 1.0f, 1.0f });
    light2 = new gllib::PointLight(trs10.position, 1.0f, 0.11f, 0.05f, {0.2f, 0.4f, 1.0f, 1.0f});

    gllib::Transform trs11;
    trs11.position = { 0.0f, 0.0f, 3.0f };
    trs11.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs11.scale = { 0.25f, 0.25f, 0.25f };
    lightBox3 = new gllib::Box(trs11, { 1.0f, 0.2f, 0.2f, 1.0f });
    light3 = new gllib::PointLight(trs11.position, 1.0f, 0.11f, 0.05f, { 1.0f, 0.2f, 0.2f, 1.0f });;;

    gllib::Transform trs12;
    trs12.position = { 2.0f, 0.0f, 3.0f };
    trs12.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs12.scale = { 0.25f, 0.25f, 0.25f };
    lightBox4 = new gllib::Box(trs12, { 0.2f, 1.0f, 0.2f, 1.0f });
    light4 = new gllib::PointLight(trs12.position, 1.0f, 0.11f, 0.05f, { 0.2f, 1.0f, 0.2f, 1.0f });

    spotLight = new gllib::SpotLight(player->getPosition(), player->forward(),{1.0f, 1.0f, 1.0f, 1.0f});
    lightData = new gllib::LightingData();

    gllib::Transform trs;
    trs.position = { 0.0f, -0.75f, 0.0f };
    trs.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs.scale = { 1.0f, 1.0f, 1.0f };
    gllib::ModelData cannonModelData = gllib::ModelData("cannon.fbx");
    cannon = new gllib::Model(cannonModelData, trs, {1.0f, 1.0f, 1.0f, 1.0f});
    cannonFrontWheels = cannon->findNode(16);
    cannonBarrel = cannon->findNode(3);
}

Game::~Game() {
    cout << "Game destroyed!\n";
}

void Game::init() {
    cout << "External init!!!!\n";

    srand(time(nullptr));
    window->setTitle("Cool rotating box");

    lightData->SetDirectionalLight(*dirLight);
    lightData->AddPointLight(*light);
    lightData->AddPointLight(*light2);
    lightData->AddPointLight(*light3);
    lightData->AddPointLight(*light4);
    lightData->AddSpotLight(*spotLight);
}

void Game::update() {
    // Update
    handlePlayerInput();

    // Draw
    gllib::Renderer::clear();

    gllib::Shader::useShaderProgram(shaderProgramSolidColor);
    lightBox->draw();
    lightBox2->draw();
    lightBox3->draw();
    lightBox4->draw();
    lit ? gllib::Shader::useShaderProgram(shaderProgramSolidColorLit) :
          gllib::Shader::useShaderProgram(shaderProgramNormals);
    gllib::Renderer::setLightingData(*lightData);

    cannon->draw();
    if (thirdPerson) {
        playerModel->draw();
    }
    floor->draw();
    wall->draw();
    gllib::Shader::useShaderProgram(shaderProgramTexture);
}

void Game::uninit() {
    cout << "External uninit!!!\n";
    delete lightData;
    delete lightBox;
    delete lightBox2;
    delete lightBox3;
    delete lightBox4;
    delete dirLight;
    delete light;
    delete light2;
    delete light3;
    delete light4;
    delete spotLight;
    delete floor;
    delete wall;
    delete player;
    delete playerModel;
    delete cannon;
    delete cameraFp;
    delete cameraTp;
}

void Game::handlePlayerInput() {
    if (Input::getKeyPressed(Key_Escape)) {
        stop();
    }
    if (Input::getKeyReleased(Key_Q)) {
        wireframeMode = !wireframeMode;
        gllib::Renderer::setLazyWireframeMode(wireframeMode);
    }
    if (Input::getKeyReleased(Key_V)) {
        thirdPerson = !thirdPerson;
    }
    if (Input::getKeyReleased(Key_K)) {
        controllingLight = !controllingLight;
    }
    if (Input::getKeyPressed(Key_C)) {
        rgbTimeAccumulator += static_cast<float>(gllib::LibTime::getDeltaTime()) * 2.0f;

        float r = sin(rgbTimeAccumulator) * 0.5f + 0.5f;
        float g = sin(rgbTimeAccumulator + 2.0f) * 0.5f + 0.5f;
        float b = sin(rgbTimeAccumulator + 4.0f) * 0.5f + 0.5f;

        light->setColor(gllib::Color({r, g, b, 1.0f}));
        lightBox->setColor({r, g, b, 1.0f});
    }
    if (Input::getKeyReleased(Key_F)) {
        spotLight->setActive(!spotLight->isActive());
    }
    if (Input::getKeyReleased(Key_P)) {
        dirLight->setActive(!dirLight->isActive());
    }
    if (Input::getKeyReleased(Key_O)) {
        light2->setActive(!light2->isActive());
    }
    if (Input::getKeyReleased(Key_I)) {
        light3->setActive(!light3->isActive());
    }
    if (Input::getKeyReleased(Key_U)) {
        light4->setActive(!light4->isActive());
    }
    if (Input::getKeyReleased(Key_X)) {
        light->setActive(!light->isActive());
    }
    if (Input::getKeyReleased(Key_Z)) {
        light->setColor(gllib::Color({1.0f, 1.0f, 1.0f, 1.0f}));
        lightBox->setColor(gllib::Color({1.0f, 1.0f, 1.0f, 1.0f}));
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
            gllib::Vector3 forwardDir = thirdPerson ? cameraTp->forward() : cameraFp->forward();
            forwardDir.y = .0f;
            forwardDir = forwardDir.normalized();
            playerModel->move(forwardDir * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
    }
    if (Input::getKeyPressed(Key_A)) {
        if (controllingLight) {
            lightBox->move(gllib::ThirdPersonCamera::rightWorld() * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
        else {
            gllib::Vector3 rightDir = thirdPerson ? cameraTp->right() : cameraFp->right();
            playerModel->move(-rightDir * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
    }
    if (Input::getKeyPressed(Key_S)) {
        if (controllingLight) {
            lightBox->move(gllib::ThirdPersonCamera::forwardWorld() * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
        else {
            gllib::Vector3 forwardDir = thirdPerson ? cameraTp->forward() : cameraFp->forward();
            forwardDir.y = .0f;
            forwardDir = forwardDir.normalized();
            playerModel->move(-forwardDir * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
    }
    if (Input::getKeyPressed(Key_D)) {
        if (controllingLight) {
            lightBox->move(-gllib::ThirdPersonCamera::rightWorld() * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
        else {
            gllib::Vector3 rightDir = thirdPerson ? cameraTp->right() : cameraFp->right();
            playerModel->move(rightDir * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
    }
    if (Input::getKeyPressed(Key_Space)) {
        if (controllingLight) {
            lightBox->move(gllib::ThirdPersonCamera::upWorld() * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
        else {
            playerModel->move(gllib::Camera::upWorld() * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
    }
    if (Input::getKeyPressed(Key_LeftCtrl)) {
        if (controllingLight) {
            lightBox->move(-gllib::ThirdPersonCamera::upWorld() * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
        else {
            playerModel->move(-gllib::Camera::upWorld() * playerSpeed * static_cast<float>(gllib::LibTime::getDeltaTime()));
        }
    }

    if (Input::getKeyPressed(Key_M)) {
        gllib::Quaternion currentWheelRot = cannonFrontWheels->getLocalRotation();
        cannonFrontWheels->setLocalRotation(
            {currentWheelRot.x, currentWheelRot.y + 75.0f * static_cast<float>(gllib::LibTime::getDeltaTime()), currentWheelRot.z});
    }
    if (Input::getKeyPressed(Key_N)) {
        gllib::Vector3 currentCannonSize = cannon->getScale();
        cannon->setScale({currentCannonSize.x, currentCannonSize.y + 1.0f * static_cast<float>(gllib::LibTime::getDeltaTime()), currentCannonSize.z});
    }
    if (Input::getKeyPressed(Key_B)) {
        gllib::Quaternion currentBarrelRot = cannonBarrel->getLocalRotation();
        cannonBarrel->setLocalRotation(
            {currentBarrelRot.x, currentBarrelRot.y, currentBarrelRot.z + 75.0f * static_cast<float>(gllib::LibTime::getDeltaTime())});
    }

    if (thirdPerson) {
        cameraTp->updateCamera(playerModel->getPosition());
    }else {
        cameraFp->updateCamera(playerModel->getPosition());
    }

    spotLight->setPosition(playerModel->getPosition());
    if (!controllingLight) {
        gllib::Vector3 camForward = thirdPerson ? cameraTp->forward() : cameraFp->forward();
        if (thirdPerson) {
            camForward.y = 0.0f;
        }

        if (camForward.length() > 0.0001f) {
            camForward = camForward.normalized();
            spotLight->setDirection(camForward);

            if (thirdPerson) {
                float yaw = atan2(camForward.x, camForward.z);
                float yawDeg = yaw * 180.0f / M_PI;

                gllib::Quaternion rot = playerModel->getRotationQuat();
                rot.y = yawDeg;

                playerModel->setRotationQuat(rot);
            }
        }
    }
    light->setPosition(lightBox->getPosition());

    if (cameraLocked) return;
    if (thirdPerson) {
        cameraTp->updateMouseInput();
    }else {
        cameraFp->updateMouseInput();
    }
}

int main() {
    Game game;
    game.start();
}
