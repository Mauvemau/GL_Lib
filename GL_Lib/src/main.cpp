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
    gllib::Model* scene;
    gllib::ModelNode* pawn;
    gllib::ModelNode* wallBSP;
    gllib::DirectionalLight* dirLight;
    gllib::Box* lightBox;
    gllib::PointLight* light;
    gllib::SpotLight* spotLight;
    gllib::LightingData* lightData;
    bool wireframeMode = false;

    float playerSpeed = 6.5f;

    float cameraSensitivity = .5f;
    bool thirdPerson = false;
    bool cameraLocked = true;

    bool lit = true;
    bool controllingPawn = false;
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

    gllib::Transform trs1;
    trs1.position = { 0.0f, -2.0f, 0.0f };
    trs1.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs1.scale = { 0.4f, 0.4f, 0.4f };
    gllib::ModelData sceneData = gllib::ModelData("Walls.fbx");
    scene = new gllib::Model(sceneData, trs1, {1.0f, 1.0f, 1.0f, 1.0f});
    pawn = scene->findNode(3);
    wallBSP = scene->findNode(27);

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

    gllib::Vector3 dirLightDirection = gllib::Vector3(-0.2f, -1.0f, -0.3f);
    dirLight = new gllib::DirectionalLight(dirLightDirection, {0.80f, 0.80f, 0.85f, 1.0f});
    gllib::Transform trs5;
    trs5.position = { -1.5f, 0.5f, 0.0f };
    trs5.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs5.scale = { 0.25f, 0.25f, 0.25f };
    lightBox = new gllib::Box(trs5, { 1.0f, 1.0f, 1.0f, 1.0f });
    light = new gllib::PointLight(trs5.position);

    spotLight = new gllib::SpotLight(player->getPosition(), player->forward(),{1.0f, 1.0f, 1.0f, 1.0f});
    lightData = new gllib::LightingData();
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
    lightData->AddSpotLight(*spotLight);

    wallBSP->getIsBSPPlane() ? cout << "bsp is TRUE\n" : cout << "bsp is FALSE\n";
}

void Game::update() {
    // Update
    handlePlayerInput();

    // Draw
    gllib::Renderer::clear();

    gllib::Shader::useShaderProgram(shaderProgramSolidColor);
    lightBox->draw();
    lit ? gllib::Shader::useShaderProgram(shaderProgramSolidColorLit) :
          gllib::Shader::useShaderProgram(shaderProgramNormals);
    gllib::Renderer::setLightingData(*lightData);

    scene->draw();
    if (thirdPerson) {
        playerModel->draw();
    }
    gllib::Shader::useShaderProgram(shaderProgramTexture);
}

void Game::uninit() {
    cout << "External uninit!!!\n";
    delete lightData;
    delete lightBox;
    delete dirLight;
    delete light;
    delete spotLight;
    delete scene;
    delete player;
    delete playerModel;
    delete cameraFp;
    delete cameraTp;
}

void Game::handlePlayerInput() {
    if (Input::getKeyReleased(Key_T)) {
        scene->printBSPPlanes();
    }
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
    if (Input::getKeyReleased(Key_B)) {
        gllib::Renderer::setDebug(!gllib::Renderer::isDebug());
    }
    if (Input::getKeyReleased(Key_K)) {
        controllingPawn = !controllingPawn;
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

    const float dt = static_cast<float>(gllib::LibTime::getDeltaTime());
    const float moveStep = playerSpeed * dt;

    if (Input::getKeyPressed(Key_W)) {
        if (controllingPawn) {
            gllib::Vector3 currentPos = pawn->getLocalPosition();
            pawn->setLocalPosition(currentPos - gllib::ThirdPersonCamera::forwardWorld() * moveStep);
        }
        else {
            gllib::Vector3 forwardDir = thirdPerson ? cameraTp->forward() : cameraFp->forward();
            forwardDir.y = .0f;
            forwardDir = forwardDir.normalized();
            playerModel->move(forwardDir * moveStep);
        }
    }

    if (Input::getKeyPressed(Key_A)) {
        if (controllingPawn) {
            gllib::Vector3 currentPos = pawn->getLocalPosition();
            pawn->setLocalPosition(currentPos + gllib::ThirdPersonCamera::rightWorld() * moveStep);
        }
        else {
            gllib::Vector3 rightDir = thirdPerson ? cameraTp->right() : cameraFp->right();
            playerModel->move(-rightDir * moveStep);
        }
    }

    if (Input::getKeyPressed(Key_S)) {
        if (controllingPawn) {
            gllib::Vector3 currentPos = pawn->getLocalPosition();
            pawn->setLocalPosition(currentPos + gllib::ThirdPersonCamera::forwardWorld() * moveStep);
        }
        else {
            gllib::Vector3 forwardDir = thirdPerson ? cameraTp->forward() : cameraFp->forward();
            forwardDir.y = .0f;
            forwardDir = forwardDir.normalized();
            playerModel->move(-forwardDir * moveStep);
        }
    }

    if (Input::getKeyPressed(Key_D)) {
        if (controllingPawn) {
            gllib::Vector3 currentPos = pawn->getLocalPosition();
            pawn->setLocalPosition(currentPos - gllib::ThirdPersonCamera::rightWorld() * moveStep);
        }
        else {
            gllib::Vector3 rightDir = thirdPerson ? cameraTp->right() : cameraFp->right();
            playerModel->move(rightDir * moveStep);
        }
    }

    if (Input::getKeyPressed(Key_Space)) {
        if (controllingPawn) {
            gllib::Vector3 currentPos = pawn->getLocalPosition();
            pawn->setLocalPosition(currentPos + gllib::ThirdPersonCamera::upWorld() * moveStep);
        }
        else {
            playerModel->move(gllib::Camera::upWorld() * moveStep);
        }
    }

    if (Input::getKeyPressed(Key_LeftCtrl)) {
        if (controllingPawn) {
            gllib::Vector3 currentPos = pawn->getLocalPosition();
            pawn->setLocalPosition(currentPos - gllib::ThirdPersonCamera::upWorld() * moveStep);
        }
        else {
            playerModel->move(-gllib::Camera::upWorld() * moveStep);
        }
    }

    if (thirdPerson) {
        cameraTp->updateCamera(playerModel->getPosition());
    }else {
        cameraFp->updateCamera(playerModel->getPosition());
    }

    spotLight->setPosition(playerModel->getPosition());
    if (!controllingPawn) {
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
