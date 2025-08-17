#include "PlayerModel.h"

namespace MyCraft {
    PlayerModel::PlayerModel(): GLTFModel("assets/models/Player/Steve.gltf") {
        __skin.load("assets/models/Player/Steve.png", false);
    }
    PlayerModel::~PlayerModel() {}

    void PlayerModel::draw() const {
        __skin.Bind();
        Draw(glm::mat4(0));
    }
}