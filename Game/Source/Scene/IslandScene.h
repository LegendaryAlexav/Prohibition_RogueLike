#pragma once

#include "Framework.h"
#include "DataTypes.h"


class IslandScene : fw::SceneCore
{

    public:
        IslandScene(fw::GameCore* pGameCore);
        virtual ~IslandScene() override;

        virtual void loadScene(std::vector<fw::GameObject*>* pObjects) override;
        virtual void deleteScene() override;
};