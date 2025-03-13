/** ScarfyScene.cpp
 *
 * See header file for details.
 */

#include "ScarfyScene.h"

#include "TrackingCamera2D.h"
#include "Common.h"
#include "Collectable.h"
#include "GameOverlay.h"
#include "ResourceManager.h"

ScarfyScene::ScarfyScene() {
}
	
ScarfyScene::~ScarfyScene() {
}

std::shared_ptr<Scarfy> scarfy;

void ScarfyScene::loadResources() {
	setOverlay(std::make_shared<GameOverlay>());

	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	auto sceneSize = get2DSize();
	raylib::Vector2 cameraOffset((float)screenWidth/2, (float)screenHeight/2);
	raylib::Vector2 cameraTarget((float)screenWidth/(2 * worldScale), 
		(float)(1765 + sceneSize.y * worldScale - screenHeight/2) / worldScale); 
	float groundYPos = (cameraTarget.y + screenHeight / (4.0f * worldScale));
	
	tileMap = std::make_shared<TileMap2D>(DATADIR "Level 1.tmj", "Ground");
	b2World &physicsWorld = getPhysicsWorld();
	tileMap->generatePhysicsObjects(physicsWorld, worldScale);
	
	//std::shared_ptr<Scarfy> 
		scarfy = std::make_shared<Scarfy>();
	
	b2Vec2 initialPos((float)screenWidth / (2 * worldScale), (float)groundYPos);
	
	addActor(scarfy, initialPos);
	setPlayerAvatar(scarfy);


	for(unsigned i = 0; i < 400; ++i) {
		float xPos = (float)(((double)rand() / RAND_MAX) * sceneSize.x * worldScale);
		std::shared_ptr<Collectable> coin = std::make_shared<Collectable>("coin", DATADIR "Collectables/coin.rtpa",
			DATADIR "Fast-Collection-of-coins-www.fesliyanstudios.com.mp3");
		addActor(coin, b2Vec2(xPos, 0.0));
	}
	
	
	auto camera = std::make_shared<TrackingCamera2D>(cameraOffset, cameraTarget, 0.0f, worldScale);
	setCamera(camera);
	camera->setTarget(scarfy);
	
	musicStream = MusicManager::load(DATADIR "Scarfy-colonel-bogey.mp3");
	musicStream->SetVolume(0.5f);
	musicStream->Play();
}

void ScarfyScene::freeResources() {
	musicStream.reset();
}


void ScarfyScene::handleKeyPress(int key)
{
	//auto& layers = 
		tileMap->getLayers();

	auto tms = tileMap->getMapSize();

	// Toggle the debug flags on and off
	switch (key) {
	case '0':
		printf("0 2: \r\n");
		printf("Size: %f %f\r\n", tms.x, tms.y);
		break;
	case '7':
	{
		printf("0 2: \r\n");
		scarfy->setPosition({ 2.0f, 40.0f });
		break;
	}

	case '6':
	{
		printf("Beaming\r\n");
		scarfy->setPosition({ 42.0f, 5.0f });
		break;
	}


	case '8':
	{
		printf("0 2: \r\n");
		b2Vec2 sp = scarfy->getPosition();
		printf("Scarfy Pos: %f %f\r\n", sp.x, sp.y);
		//scarfy->setPosition({ 2.0f, 40.0f });
		break;
	}

	default:
		;
	}
	Scene::handleKeyPress(key);
}

