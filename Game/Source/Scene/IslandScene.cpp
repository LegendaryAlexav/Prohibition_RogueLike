#include "IslandScene.h"

IslandScene::IslandScene(fw::GameCore* pGameCore)
	: SceneCore(pGameCore)
{

}

IslandScene::~IslandScene()
{
}

void IslandScene::loadScene(std::vector<fw::GameObject*>* pObjects)
{
	m_pObjects = pObjects;
}

void IslandScene::deleteScene()
{
	
}
