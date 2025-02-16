//
// Created by gaetz on 16/02/2025.
//

#include "EcsManager.hpp"
#include "AssetsManager.hpp"
#include "Renderer.hpp"

using gassets::AssetsManager;
using gdraw::Renderer;

namespace gecs {

    u64 ECSManager::maxId = 0;

    void ECSManager::UpdateScene(f32 dt) {
        SystemPhysicsUpdate(dt);
    }

    void ECSManager::DrawScene() {
        gdraw::BeginDraw();
        SystemSpriteDraw();
        gdraw::EndDraw();

        CleanRemovedEntities();
    }

    u64 ECSManager::CreateEntity() {
        u64 newId = maxId++;
        entityIds.emplace_back(newId);
        entities.emplace_back(newId);
        return newId;
    }

    Transform2D& ECSManager::CreateTransform2DComponent(u64 entityId) {
        i32 newComponentId = static_cast<i32>(transforms.size());
        transforms.emplace_back(entityId);
        UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Transform2D);
        return transforms.back();
    }

    Sprite& ECSManager::CreateSpriteComponent(u64 entityId, const str& texName) {
        i32 newComponentId = static_cast<i32>(sprites.size());
        const Texture& tex = AssetsManager::GetTexture(texName);
        sprites.emplace_back(entityId, texName, static_cast<float>(tex.width), static_cast<float>(tex.height));
        UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Sprite);
        return sprites.back();
    }

    Rigidbody2D& ECSManager::CreateRigidbody2DComponent(u64 entityId, const Vector2& pos,
                                                        const Rectangle& box) {
        i32 newComponentId = static_cast<i32>(bodies.size());
        bodies.emplace_back(entityId, pos, box);
        UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Rigidbody2D);
        return bodies.back();
    }

    void ECSManager::UpdateEntityWithComponent(u64 entityId, i32 newComponentId, ComponentIndex componentIndex) {
        i32 iComponentIndex = static_cast<i32>(componentIndex);
        FindEntity(entityId).components[iComponentIndex] = newComponentId;
    }

    void ECSManager::SystemSpriteDraw() {
        for (auto& sprite : sprites) {
            Color colorAlpha { 255, 255, 255, sprite.opacity };
            gdraw::DrawSprite(sprite.tex, sprite.srcRect, sprite.dstRect, colorAlpha);
        }
#ifdef GDEBUG
        for (auto& body : bodies) {
            body.DrawDebug();
        }
#endif
    }

    void ECSManager::RemoveEntity(u64 entityId) {
        entitiesToRemove.push_back(entityId);
    }

    void ECSManager::CleanRemovedEntities() {
        for (auto entityId : entitiesToRemove) {
            // Transform
            RemoveEntityComponent<Transform2D>(entityId);
            // Sprites
            RemoveEntityComponent<Sprite>(entityId);
            // Rigidbodies
            RemoveEntityComponent<Rigidbody2D>(entityId);

            std::erase(entityIds, entityId);
            std::erase_if(entities, [=](Entity entity) {
                return entity.id == entityId;
            });
        }
        entitiesToRemove.clear();
    }

    Entity& ECSManager::FindEntity(u64 entityId) {
        auto itr = std::lower_bound(entityIds.begin(), entityIds.end(), entityId);
        return entities.at(std::distance(entityIds.begin(), itr));
    }

    i32 ECSManager::FindEntityComponent(u64 entityId, ComponentIndex componentIndex) {
        return FindEntity(entityId).components.at(static_cast<i32>(componentIndex));
    }


    void ECSManager::PrepareDraw() {
        // Update sprite positions
        for (auto& sprite : sprites) {
            const auto& transform = GetComponent<Transform2D>(sprite.entityId);
            sprite.dstRect = { transform.pos.x, transform.pos.y,
                               static_cast<float>(sprite.tex.width), static_cast<float>(sprite.tex.height) };
        }
    }

    void ECSManager::SystemPhysicsUpdate(float dt) {

    }
} // gecs