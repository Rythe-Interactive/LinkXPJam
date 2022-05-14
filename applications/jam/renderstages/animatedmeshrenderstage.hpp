#pragma once
#include "../engine_include.hpp"

class AnimatedMeshRenderStage : public lgn::gfx::RenderStage<AnimatedMeshRenderStage>
{
public:
    virtual void setup(lgn::app::window& context) override;
    virtual void render(lgn::app::window& context, lgn::gfx::camera& cam, const lgn::gfx::camera::camera_input& camInput, lgn::time::span deltaTime) override;
    virtual lgn::priority_type priority() override;
};
