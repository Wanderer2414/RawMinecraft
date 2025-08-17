#ifndef PLAYER_MODEL_H
#define PLAYER_MODEL_H
#include "GLTFModel.h"
#include "Texture.h"
namespace MyCraft {
    class PlayerModel: public GLTFModel {
    public:
        PlayerModel();
        ~PlayerModel();
        PlayerModel(const PlayerModel&) = delete;
        PlayerModel& operator=(const PlayerModel&) const = delete; 

        void draw() const;
    protected:
    private:
        MyBase::Texture __skin;
    };
}
#endif