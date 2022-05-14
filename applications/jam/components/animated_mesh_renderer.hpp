#pragma once
#include "../engine_include.hpp"

struct key_frame
{
    lgn::gfx::model_handle model;
    lgn::time::span frameLength;
};

using key_frame_list = std::vector<key_frame>;

struct animated_mesh_renderer
{
    key_frame_list keyFrames;
    lgn::time::span timeBuffer;
    lgn::index_type currentFrame;
    lgn::gfx::material_handle material;
    bool playing;
    bool looping;

    animated_mesh_renderer() noexcept : keyFrames(), timeBuffer(), currentFrame(), material(), playing(false), looping(true) {}
    animated_mesh_renderer(lgn::gfx::material_handle mat) noexcept : keyFrames(), timeBuffer(), currentFrame(), material(mat), playing(false), looping(true) {}
    animated_mesh_renderer(lgn::gfx::material_handle mat, const key_frame_list& frames) noexcept : keyFrames(frames), timeBuffer(), currentFrame(), material(mat), playing(true), looping(true) {}
    animated_mesh_renderer(lgn::gfx::material_handle mat, key_frame_list&& frames) noexcept : keyFrames(frames), timeBuffer(), currentFrame(), material(mat), playing(true), looping(true) {}

    MOVE_FUNCS(animated_mesh_renderer);
    COPY_FUNCS(animated_mesh_renderer);
};
