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
    gllib::Box* wall;
    gllib::Box* emerald;
    gllib::Box* greenPlastic;
    gllib::Box* greenRubber;
    gllib::DirectionalLight* dirLight;
    gllib::Box* lightBox;
    gllib::Box* lightBox2;
    gllib::PointLight* light;
    gllib::PointLight* light2;
    gllib::SpotLight* spotLight;
    gllib::LightingData* lightData;
    gllib::Model* nissanModel;
    gllib::Model* swordModel;
    bool wireframeMode = false;

    float playerSpeed = 2.5f;

    float cameraSensitivity = .5f;
    bool cameraLocked = true;

    bool lit = true;
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
    trs.position = { 0.0f, -1.05f, 0.0f };
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

    gllib::Transform trs7;
    trs7.position = { -4.0f, -1.1f, -4.0f };
    trs7.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs7.scale = { 0.75f, 0.75f, 0.75f };
    gllib::Material emeraldMat = gllib::Material({0.0215f, 0.1745f, 0.0215 },
                                                {0.07568f, 0.61424f, 0.07568f },
                                                {0.633f, 0.727811f, 0.633f },
                                                76.8f);
    emerald = new gllib::Box(trs7, { 1.0f, 1.0f, 1.0f, 0.55f }, emeraldMat);

    gllib::Transform trs8;
    trs8.position = { -3.0f, -1.1f, -4.0f };
    trs8.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs8.scale = { 0.75f, 0.75f, 0.75f };
    gllib::Material greenPlasticMat = gllib::Material({0.0f, 0.0f, 0.0f },
                                                {0.1f, 0.35f, 0.1f },
                                                {0.45f, 0.55f, 0.45f },
                                                32.0f);
    greenPlastic = new gllib::Box(trs8, { 1.0f, 1.0f, 1.0f, 1.0f }, greenPlasticMat);

    gllib::Transform trs9;
    trs9.position = { -2.0f, -1.1f, -4.0f };
    trs9.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs9.scale = { 0.75f, 0.75f, 0.75f };
    gllib::Material greenRubberMat = gllib::Material({0.0f, 0.05f, 0.0f },
                                                {0.4f, 0.5f, 0.4f },
                                                {0.04f, 0.7f, 0.04f},
                                                10.0f);
    greenRubber = new gllib::Box(trs9, { 1.0f, 1.0f, 1.0f, 1.0f }, greenRubberMat);


    gllib::Vector3 dirLightDirection = gllib::Vector3(-0.2f, -1.0f, -0.3f);
    dirLight = new gllib::DirectionalLight(dirLightDirection, {0.30f, 0.30f, 0.35f, 1.0f});
    gllib::Transform trs5;
    trs5.position = { 4.0f, 0.0f, 3.0f };
    trs5.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs5.scale = { 0.25f, 0.25f, 0.25f };
    lightBox = new gllib::Box(trs5, { 1.0f, 1.0f, 1.0f, 1.0f });
    light = new gllib::PointLight(trs5.position);

    gllib::Transform trs10;
    trs10.position = { -4.0f, 0.0f, 3.0f };
    trs10.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs10.scale = { 0.25f, 0.25f, 0.25f };
    lightBox2 = new gllib::Box(trs10, { 0.2f, 0.4f, 1.0f, 1.0f });
    light2 = new gllib::PointLight(trs10.position, 1.0f, 0.14f, 0.07f, {0.2f, 0.4f, 1.0f, 1.0f});

    spotLight = new gllib::SpotLight(player->getPosition(), player->forward(),{1.0f, 1.0f, 1.0f, 1.0f});
    lightData = new gllib::LightingData();

    gllib::Material whitePaintMaterial = gllib::Material({0.3f, 0.3f, 0.3f },
                                                {0.4f, 0.4f, 0.4f },
                                                {0.9f, 0.9f, 0.9f},
                                                50.0f);
    gllib::MeshGroup nissanMesh = gllib::ModelImporter::loadMeshGroup("NissanS30.obj");
    gllib::MeshGroup swordMesh = gllib::ModelImporter::loadMeshGroup("starpiercer.fbx");
    nissanModel = new gllib::Model(nissanMesh, trs, {1.0f, 1.0f, 1.0f, 1.0f}, whitePaintMaterial);

    gllib::Transform trsSword;
    trsSword.position = { 0.0f, 1.05f, 3.0f };
    trsSword.rotationQuat = { 0.0f, 90.0f, 0.0f, 0.0f };
    trsSword.scale = { 1.0f, 1.0f, 1.0f };
    swordModel = new gllib::Model(swordMesh, trsSword, {1.0f, 1.0f, 1.0f, 1.0f});
    unsigned int swordTexture = gllib::Loader::loadTexture("textures/GALAXY_low_DefaultMaterial_BaseColor.png", false);
    unsigned int particleTexture = gllib::Loader::loadTexture("textures/beo.png", true);
    swordModel->setTexture(0, swordTexture);
    swordModel->setTexture(1, particleTexture);
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
    lightData->AddSpotLight(*spotLight);
}

void Game::update() {
    // Update
    handlePlayerInput();

    gllib::Quaternion rot = nissanModel->getRotationQuat();
    //rot.x += 30.0f * gllib::LibTime::getDeltaTime();
    rot.y += 20.0f * gllib::LibTime::getDeltaTime();
    nissanModel->setRotationQuat(rot);

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
    lightBox2->draw();
    lit ?   gllib::Shader::useShaderProgram(shaderProgramSolidColorLit) :
            gllib::Shader::useShaderProgram(shaderProgramNormals);
    gllib::Renderer::setLightingData(*lightData);

    //box->draw();
    //nissanModel->draw();
    swordModel->draw();
    player->draw();
    floor->draw();
    wall->draw();
    greenPlastic->draw();
    greenRubber->draw();
    emerald->draw();
    gllib::Shader::useShaderProgram(shaderProgramTexture);
    //coin->draw();
}

void Game::uninit() {
    cout << "External uninit!!!\n";
    delete box;
    delete emerald;
    delete greenPlastic;
    delete greenRubber;
    delete coin;
    delete lightData;
    delete lightBox;
    delete lightBox2;
    delete dirLight;
    delete light;
    delete light2;
    delete spotLight;
    delete floor;
    delete wall;
    delete player;
    delete camera;
    delete nissanModel;
    delete swordModel;
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
    spotLight->setPosition(player->getPosition());
    if (!controllingLight) {
        gllib::Vector3 camForward = camera->forward();
        camForward.y = 0.0f;

        if (camForward.length() > 0.0001f) {
            camForward = camForward.normalized();
            spotLight->setDirection(camForward);

            float yaw = atan2(camForward.x, camForward.z);
            float yawDeg = yaw * 180.0f / M_PI;

            gllib::Quaternion rot = player->getRotationQuat();
            rot.y = yawDeg;

            player->setRotationQuat(rot);
        }
    }

    light->setPosition(lightBox->getPosition());
    if (cameraLocked) return;
    camera->updateMouseInput();
}

int main() {
    Game game;
    game.start();
}
