#include "base_game.h"

#include <iostream>

#include "camera/first_person_camera.h"
#include "camera/third_person_camera.h"

using namespace std;

class Game : public gllib::BaseGame {
private:
    gllib::FirstPersonCamera* cameraFp;
    gllib::ThirdPersonCamera* cameraTp;

    gllib::Box* box;
    gllib::Animation* coin;
    gllib::Box* player;
    gllib::Model* playerModel;
    gllib::Box* floor;
    gllib::Box* wall;
    gllib::Box* emerald;
    gllib::Box* greenPlastic;
    gllib::Box* greenRubber;
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
    gllib::Model* nissanModel;
    gllib::Model* lemon;
    gllib::Model* lion;
    bool wireframeMode = false;

    float playerSpeed = 2.5f;

    float cameraSensitivity = .5f;
    bool thirdPerson = true;
    bool cameraLocked = true;

    bool lit = true;
    bool controllingLight = false;
    float rgbTimeAccumulator = 0.0f;

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
    cameraFp = new gllib::FirstPersonCamera(gllib::Vector3(0.0f, 0.0f, 0.0f),
                                         gllib::Vector3(0.0f, 0.0f, 1.0f), cameraSensitivity);

    cameraTp = new gllib::ThirdPersonCamera(gllib::Vector3(0.0f, 0.0f, 0.0f),
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
    trs3.rotationQuat = { 0.0f, -90.0f, 0.0f, 0.0f };
    trs3.scale = { 2.0f, 2.0f, 2.0f };

    gllib::Material playerMat = gllib::Material({0.329412f, 0.223529f, 0.027451f},
                                                {0.780392f, 0.568627f, 0.113725f},
                                                {0.992157f, 0.941176f, 0.807843f},
                                                27.8974f);
    player = new gllib::Box(trs3, { 1.0f, 1.0f, 1.0f, 1.0f }, playerMat);

    gllib::MeshGroup playerMesh = gllib::ModelImporter::loadMeshGroup("rubber_duck_toy_4k.fbx");
    gllib::MaterialGroup playerMaterial = gllib::ModelImporter::loadMaterialGroup("rubber_duck_toy_4k.fbx");
    playerModel = new gllib::Model(playerMesh, playerMaterial, trs3, {1.0f, 1.0f, 1.0f, 1.0f});

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
    trs5.position = { 0.0f, 0.0f, -3.0f };
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

#pragma region Nissan
    gllib::Material debugGreen = gllib::Material(
    {0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    1.0f);
    gllib::Material metallicRed = gllib::Material(
    {0.18f, 0.03f, 0.03f},
    {0.75f, 0.05f, 0.05f},
    {1.0f, 0.9f, 0.9f},
    128.0f );
    gllib::Material matteBlackPlastic = gllib::Material(
    {0.02f, 0.02f, 0.02f},
    {0.08f, 0.08f, 0.08f},
    {0.15f, 0.15f, 0.15f},
    12.0f);
    gllib::Material chrome = gllib::Material(
    {0.20f, 0.20f, 0.20f},
    {0.55f, 0.55f, 0.55f},
    {1.0f, 1.0f, 1.0f},
    512.0f);
    gllib::Material blackGlass = gllib::Material(
    {0.01f, 0.01f, 0.01f},
    {0.03f, 0.03f, 0.03f},
    {1.0f, 1.0f, 1.0f},
    300.0f);
    gllib::Material glossyRubber = gllib::Material(
    {0.02f, 0.02f, 0.02f},
    {0.06f, 0.06f, 0.06f},
    {0.4f, 0.4f, 0.4f},
    48.0f);
    gllib::Material imprezaGold = gllib::Material(
    {0.20f, 0.14f, 0.02f},
    {0.85f, 0.65f, 0.08f},
    {1.0f, 0.9f, 0.5f},
    96.0f);
    gllib::MeshGroup nissanMesh = gllib::ModelImporter::loadMeshGroup("NissanS30.obj");
    nissanModel = new gllib::Model(nissanMesh, trs, {1.0f, 1.0f, 1.0f, 1.0f});
    nissanModel->setMaterial(0, metallicRed); // Body
    nissanModel->setMaterial(1, chrome); // Chrome body parts
    nissanModel->setMaterial(2, matteBlackPlastic); // Back
    unsigned int lightsTexture = gllib::Loader::loadTexture("textures/lights.png", false);
    gllib::Material lightsMaterial = gllib::Material(lightsTexture);
    nissanModel->setMaterial(3, lightsTexture);
    nissanModel->setMaterial(4, matteBlackPlastic); // Inside Grill
    nissanModel->setMaterial(5, blackGlass); // Windows
    nissanModel->setMaterial(6, matteBlackPlastic); // Wing
    nissanModel->setMaterial(7, matteBlackPlastic); // Fenders
    nissanModel->setMaterial(8, lightsMaterial); // Lights
    nissanModel->setMaterial(9, chrome); // Headlight ring
    nissanModel->setMaterial(10, matteBlackPlastic); // Backlight rings
    // 11 Nothing
    nissanModel->setMaterial(12, lightsMaterial); // Headlights
    nissanModel->setMaterial(13, chrome); // Grill
    nissanModel->setMaterial(14, debugGreen); // The damn tree
    nissanModel->setMaterial(15, chrome); // Small metallic details
    unsigned int plateTexture = gllib::Loader::loadTexture("textures/plate_d-dds.png", false);
    gllib::Material plateMaterial = gllib::Material(plateTexture);
    nissanModel->setMaterial(16, plateMaterial); // Plate
    nissanModel->setMaterial(17, chrome); //pedals
    nissanModel->setMaterial(18, chrome); // Wheel
    nissanModel->setMaterial(19, metallicRed); // Cage
    nissanModel->setMaterial(20, matteBlackPlastic); // Interior
    unsigned int radioTexture = gllib::Loader::loadTexture("textures/Classic_&_Sports_Car_–_Datsun_240Z_–_09.png", false);
    gllib::Material radioMaterial = gllib::Material(radioTexture);
    nissanModel->setMaterial(21, radioMaterial);
    unsigned int radioTexture2 = gllib::Loader::loadTexture("textures/6d3540a077232b960ef9dd1991bde2c0.jpeg", false);
    gllib::Material radioMaterial2 = gllib::Material(radioTexture2);
    nissanModel->setMaterial(22, radioMaterial2);
    nissanModel->setMaterial(23, matteBlackPlastic);
    nissanModel->setMaterial(24, chrome);
    nissanModel->setMaterial(25, matteBlackPlastic);
    nissanModel->setMaterial(28, matteBlackPlastic); // Bottom
    nissanModel->setMaterial(29, matteBlackPlastic); // Carpet
    unsigned int beltTexture = gllib::Loader::loadTexture("textures/70002-H2__45943.1452186941.jpeg", false);
    gllib::Material beltMaterial = gllib::Material(beltTexture);
    nissanModel->setMaterial(30, beltMaterial);
    nissanModel->setMaterial(31, metallicRed);
    unsigned int capTexture = gllib::Loader::loadTexture("textures/large311002.jpeg", false);
    gllib::Material capMaterial = gllib::Material(capTexture);
    nissanModel->setMaterial(32, capMaterial);
    nissanModel->setMaterial(33, chrome);
    nissanModel->setMaterial(34, capMaterial);
    nissanModel->setMaterial(35, chrome);
    nissanModel->setMaterial(36, chrome);
    nissanModel->setMaterial(37, capMaterial);
    nissanModel->setMaterial(38, chrome); // Exhaust tip
    nissanModel->setMaterial(39, chrome);
    nissanModel->setMaterial(40, metallicRed);
    nissanModel->setMaterial(41, chrome);
    nissanModel->setMaterial(42, chrome);
    nissanModel->setMaterial(43, chrome); // Rear Bumper
    nissanModel->setMaterial(44, matteBlackPlastic);
    nissanModel->setMaterial(45, chrome); // Front Bumper
    nissanModel->setMaterial(46, matteBlackPlastic);
    nissanModel->setMaterial(47, matteBlackPlastic);
    nissanModel->setMaterial(48, chrome); // Mirrors
    nissanModel->setMaterial(49, chrome);
    nissanModel->setMaterial(50, blackGlass);
    nissanModel->setMaterial(52, chrome);
    nissanModel->setMaterial(53, metallicRed);
    nissanModel->setMaterial(54, chrome);
    nissanModel->setMaterial(55, chrome);
    nissanModel->setMaterial(56, metallicRed);
    nissanModel->setMaterial(57, chrome);
    nissanModel->setMaterial(58, chrome);
    nissanModel->setMaterial(59, metallicRed);
    nissanModel->setMaterial(60, chrome);
    nissanModel->setMaterial(61, chrome);
    nissanModel->setMaterial(62, chrome);
    nissanModel->setMaterial(63, metallicRed); // Lip
    nissanModel->setMaterial(64, chrome); // Bolts
    nissanModel->setMaterial(65, chrome);
    nissanModel->setMaterial(66, glossyRubber); // Tire
    nissanModel->setMaterial(67, chrome); // Rim bolts
    nissanModel->setMaterial(68, chrome);
    nissanModel->setMaterial(69, chrome); // Outer Rim
    nissanModel->setMaterial(70, imprezaGold); // Rim
    nissanModel->setMaterial(71, glossyRubber);
    nissanModel->setMaterial(72, chrome);
    nissanModel->setMaterial(73, chrome);
    nissanModel->setMaterial(74, chrome);
    nissanModel->setMaterial(75, imprezaGold);
    nissanModel->setMaterial(76, glossyRubber);
    nissanModel->setMaterial(77, chrome);
    nissanModel->setMaterial(78, chrome);
    nissanModel->setMaterial(79, chrome);
    nissanModel->setMaterial(80, imprezaGold);
    nissanModel->setMaterial(81, glossyRubber);
    nissanModel->setMaterial(82, chrome);
    nissanModel->setMaterial(83, chrome);
    nissanModel->setMaterial(84, chrome);
    nissanModel->setMaterial(85, imprezaGold);
    nissanModel->setMaterial(89, chrome);
#pragma endregion

    gllib::Transform trs14;
    trs14.position = { 5.0f, 0.0f, -1.0f };
    trs14.rotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
    trs14.scale = { 15.0f, 15.0f, 15.0f };
    gllib::MeshGroup lemonMesh = gllib::ModelImporter::loadMeshGroup("lemon_4k.fbx");
    gllib::MaterialGroup lemonMaterial = gllib::ModelImporter::loadMaterialGroup("lemon_4k.fbx");
    lemon = new gllib::Model(lemonMesh, lemonMaterial, trs14, {1.0f, 1.0f, 1.0f, 1.0f});

    gllib::Transform trs15;
    trs15.position = { -4.25f, -.35f, -1.0f };
    trs15.rotationQuat = { 0.0f, 0.0f, 90.0f, 0.0f };
    trs15.scale = { 5.0f, 5.0f, 5.0f };
    gllib::MeshGroup lionMesh = gllib::ModelImporter::loadMeshGroup("lion_head_4k.obj");
    gllib::MaterialGroup lionMaterial = gllib::ModelImporter::loadMaterialGroup("lion_head_4k.obj");
    lion = new gllib::Model(lionMesh, lionMaterial, trs15, {1.0f, 1.0f, 1.0f, 1.0f});
    cout << "Material: " << lionMaterial.getMaterials()[1].texture << "\n";
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
    lightBox3->draw();
    lightBox4->draw();
    lit ?   gllib::Shader::useShaderProgram(shaderProgramSolidColorLit) :
            gllib::Shader::useShaderProgram(shaderProgramNormals);
    gllib::Renderer::setLightingData(*lightData);

    nissanModel->draw();
    lemon->draw();
    lion->draw();
    if (thirdPerson) {
        //player->draw();
        playerModel->draw();
    }
    floor->draw();
    wall->draw();
    greenPlastic->draw();
    greenRubber->draw();
    emerald->draw();
    //nissanModel->draw();
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
    delete cameraFp;
    delete cameraTp;
    delete nissanModel;
    delete lemon;
    delete lion;
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
    if (Input::getKeyReleased(Key_L)) {
        lit = !lit;
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
                rot.z = yawDeg;

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
