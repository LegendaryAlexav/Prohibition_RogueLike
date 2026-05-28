#pragma once

namespace fw
{
    class GameCore;
    class GameObject;
    class Event;

    class SceneCore
    {
    public:
        SceneCore(GameCore* pGameCore) : m_pGameCore(pGameCore) {}
        virtual ~SceneCore() = 0 {};

        virtual void loadScene(std::vector<fw::GameObject*>* pObjects);
        virtual void deleteScene();


        virtual void onEvent(Event* pEvent) = 0;
        virtual void update(float deltaTime) = 0;
        virtual void draw() = 0;

        GameCore* getCameCore() { return m_pGameCore; }
        virtual GameObject* getHeighMap() { return m_pHeightMap;}

    protected:
        GameCore* m_pGameCore = nullptr;
        GameObject* m_pHeightMap = nullptr;
        std::vector<fw::GameObject*>* m_pObjects;
    };

}// namespace fw