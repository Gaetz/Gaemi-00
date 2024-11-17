//
// Created by gaetz on 05/11/2022.
//

#ifndef MESH01_GAME_HPP
#define MESH01_GAME_HPP

#include "IGame.hpp"
#include "IScene.hpp"

/**
 * @brief Game class implementation
 * @details This class is the main class of the game. It is responsible for loading, updating, drawing and unloading
 * the game, and manage scenes.
 */
class Game : public IGame {
public:
    Game() = default;
    ~Game() override = default;
    Game(Game&) = delete;
    Game& operator=(Game&) = delete;

    void Load() override;
    void Update(f32 dt) override;
    void Draw() override;
    void Unload() override;

    void AddScene(uptr<IScene> newScene);
    void RemoveCurrentScene();
    void SwitchScene(uptr<IScene> newScene);

private:
    vector<uptr<IScene>> sceneStack {};
};


#endif //MESH01_GAME_HPP
