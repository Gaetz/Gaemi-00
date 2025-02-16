#ifndef GAMEIMPL_COMPONENTS_HPP
#define GAMEIMPL_COMPONENTS_HPP

#include <optional>
#include <utility>
#include <vector>
#include <memory>

#include "raylib.h"
#include "Defines.hpp"
#include "AssetsManager.hpp"
#include "Renderer.hpp"

namespace gecs {

    class ECSManager;

    using std::vector;

    enum class ComponentIndex {
        Transform2D = 0,
        Sprite = 1,
        Rigidbody2D = 2,
    };

    struct Transform2D {
        explicit Transform2D(u64 entityIdP) : entityId{entityIdP} {}

        u64 entityId;
        Vector2 pos{0.0f, 0.0f};
        float rotation{0.0f};
        Vector2 scale{0.0f, 0.0f};
    };

    struct Sprite {
        explicit Sprite(u64 entityIdP, const str &textNameP, float width, float height) :
                entityId{entityIdP},
                srcRect{0, 0, width, height},
                dstRect{0, 0, width, height},
                texName{textNameP},
                tex{gassets::AssetsManager::GetTexture(textNameP)} {}

        u64 entityId;
        u8 opacity{255};
        Rect srcRect{0, 0, 1, 1};
        Rect dstRect{0, 0, 1, 1};
        str texName;
        Texture tex;
    };

    struct Rigidbody2D {
        explicit Rigidbody2D(u64 entityIdP, const Vector2 &pos, const Rectangle &box) :
                entityId{entityIdP},
                pos{pos},
                boundingBox{box} {}

        u64 entityId;
        Vector2 pos{0.0f, 0.0f};
        Rectangle boundingBox{0, 0, 1, 1};
        Vector2 velocity{0.0f, 0.0f};

        [[nodiscard]] Rectangle GetPositionedRectangle() const {
            return Rectangle{pos.x + boundingBox.x, pos.y + boundingBox.y, boundingBox.width, boundingBox.height};
        }

        [[nodiscard]] f32 GetRealX() const {
            return pos.x + boundingBox.x;
        }

        [[nodiscard]] f32 GetRealY() const {
            return pos.y + boundingBox.y;
        }

#ifdef GDEBUG

        void DrawDebug() {
            const Rectangle box{pos.x + boundingBox.x,
                                pos.y + boundingBox.y,
                                boundingBox.width, boundingBox.height};
            render::DrawRectangleLines(box, BLUE);
        }

#endif
    };

// Utils
    struct Collision2D {
        Collision2D(u64 entityId, Rectangle currentBox, Vector2 velocity,
                    u64 otherId, Rectangle otherCurrentBox, Vector2 otherVelocity) :
                entityId{entityId}, currentBox{currentBox}, velocity{velocity},
                otherId{otherId}, otherCurrentBox{otherCurrentBox}, otherVelocity{otherVelocity} {}

        u64 entityId;
        Rectangle currentBox;
        Vector2 velocity;
        u64 otherId;
        Rectangle otherCurrentBox;
        Vector2 otherVelocity;
    };
}
#endif //GAMEIMPL_COMPONENTS_HPP