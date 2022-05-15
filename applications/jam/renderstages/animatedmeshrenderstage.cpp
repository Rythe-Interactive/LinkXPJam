#include "animatedmeshrenderstage.hpp"
#include "../components/animated_mesh_renderer.hpp"

using namespace lgn;
using namespace lgn::gfx;

void AnimatedMeshRenderStage::setup(app::window& context)
{

}

void AnimatedMeshRenderStage::render(app::window& context, L_MAYBEUNUSED camera& cam, const camera::camera_input& camInput, time::span deltaTime)
{
    static auto mainId = nameHash("main");
    static auto lightsId = nameHash("light buffer");
    static auto lightCountId = nameHash("light count");
    static auto matricesId = nameHash("model matrix buffer");
    static auto entityBufferId = nameHash("entity id buffer");
    static auto flipbookBufferId = nameHash("flipbook frame buffer");

    static ecs::filter<position, rotation, scale, animated_mesh_renderer> filter;

    auto* lightsBuffer = get_meta<buffer>(lightsId);
    if (!lightsBuffer)
        return;

    auto* lightCount = get_meta<size_type>(lightCountId);
    if (!lightCount)
        return;

    auto* modelMatrixBuffer = get_meta<buffer>(matricesId);
    if (!modelMatrixBuffer)
        return;

    auto* entityIdBuffer = get_meta<buffer>(entityBufferId);
    if (!entityIdBuffer)
        return;

    auto* flipbookBuffer = get_meta<buffer>(flipbookBufferId);
    if (!flipbookBuffer)
        return;

    auto* fbo = getFramebuffer(mainId);
    if (!fbo)
    {
        log::error("Main frame buffer is missing.");
        abort();
        return;
    }

    texture_handle sceneColor;
    auto colorAttachment = fbo->getAttachment(FRAGMENT_ATTACHMENT);
    if (std::holds_alternative<texture_handle>(colorAttachment))
        sceneColor = std::get<texture_handle>(colorAttachment);

    texture_handle sceneNormal;
    auto normalAttachment = fbo->getAttachment(NORMAL_ATTACHMENT);
    if (std::holds_alternative<texture_handle>(normalAttachment))
        sceneNormal = std::get<texture_handle>(normalAttachment);

    texture_handle scenePosition;
    auto positionAttachment = fbo->getAttachment(POSITION_ATTACHMENT);
    if (std::holds_alternative<texture_handle>(positionAttachment))
        scenePosition = std::get<texture_handle>(positionAttachment);

    texture_handle hdrOverdraw;
    auto overdrawAttachment = fbo->getAttachment(OVERDRAW_ATTACHMENT);
    if (std::holds_alternative<texture_handle>(overdrawAttachment))
        hdrOverdraw = std::get<texture_handle>(overdrawAttachment);

    texture_handle sceneDepth;
    auto depthAttachment = fbo->getAttachment(GL_DEPTH_ATTACHMENT);
    if (std::holds_alternative<std::monostate>(depthAttachment))
        depthAttachment = fbo->getAttachment(GL_DEPTH_STENCIL_ATTACHMENT);
    if (std::holds_alternative<texture_handle>(depthAttachment))
        sceneDepth = std::get<texture_handle>(depthAttachment);

    texture_handle skyboxTex;
    ecs::filter<skybox_renderer> skyboxFilter;
    if (!skyboxFilter.empty())
        skyboxTex = skyboxFilter[0].get_component<skybox_renderer>()->material.get_param<texture_handle>(SV_SKYBOX);

    app::context_guard guard(context);
    if (!guard.contextIsValid())
    {
        abort();
        return;
    }

    auto [valid, message] = fbo->verify();
    if (!valid)
    {
        log::error("Main frame buffer isn't complete: {}", message);
        abort();
        return;
    }

    fbo->bind();

    for (auto& ent : filter)
    {
        transform transf = ent.get_component<transform>();
        auto& renderer = *ent.get_component<animated_mesh_renderer>();

        if (renderer.keyFrames.empty())
        {
            log::warn("Empty animated mesh found on entity {}.", ecs::Registry::getEntity(ent)->name);
            continue;
        }

        auto& frameLength = renderer.keyFrames[renderer.currentFrame].frameLength;
        if (renderer.playing)
        {
            renderer.timeBuffer += deltaTime;
            if (renderer.timeBuffer >= frameLength)
            {
                renderer.timeBuffer -= frameLength;
                renderer.currentFrame++;
                if (renderer.currentFrame >= renderer.keyFrames.size())
                {
                    if (!renderer.looping)
                        renderer.playing = false;
                    else
                        renderer.currentFrame = 0ull;
                }
            }
        }

        auto& model = renderer.keyFrames[renderer.currentFrame].model;

        if (model.id == invalid_id)
        {
            log::warn("Invalid mesh found on entity {}, frame {}.", ecs::Registry::getEntity(ent)->name, renderer.currentFrame);
            continue;
        }

        auto& mesh = model.get_model();

        if (!mesh.buffered)
            model.buffer_data(*modelMatrixBuffer, *entityIdBuffer, *flipbookBuffer);

        if (mesh.submeshes.empty())
        {
            log::warn("Empty mesh found. Model name: {},  Model ID {}", ModelCache::get_model_name(model.id), model.id);
            continue;
        }

        auto& material = renderer.material;

        camInput.bind(material);
        if (material.has_param<uint>(SV_LIGHTCOUNT))
            material.set_param<uint>(SV_LIGHTCOUNT, *lightCount);

        if (sceneColor && material.has_param<texture_handle>(SV_SCENECOLOR))
            material.set_param<texture_handle>(SV_SCENECOLOR, sceneColor);

        if (sceneNormal && material.has_param<texture_handle>(SV_SCENENORMAL))
            material.set_param<texture_handle>(SV_SCENENORMAL, sceneNormal);

        if (scenePosition && material.has_param<texture_handle>(SV_SCENEPOSITION))
            material.set_param<texture_handle>(SV_SCENEPOSITION, scenePosition);

        if (hdrOverdraw && material.has_param<texture_handle>(SV_HDROVERDRAW))
            material.set_param<texture_handle>(SV_HDROVERDRAW, hdrOverdraw);

        if (sceneDepth && material.has_param<texture_handle>(SV_SCENEDEPTH))
            material.set_param<texture_handle>(SV_SCENEDEPTH, sceneDepth);

        if (skyboxTex && material.has_param<texture_handle>(SV_SKYBOX))
            material.set_param(SV_SKYBOX, skyboxTex);

        material.bind();

        entityIdBuffer->bufferData(std::vector<lgn::id_type>{1, ent->id});
        modelMatrixBuffer->bufferData(std::vector<math::mat4>{1, transf.to_world_matrix()});

        {
            mesh.vertexArray.bind();
            mesh.indexBuffer.bind();
            lightsBuffer->bind();
            for (auto submesh : mesh.submeshes)
                glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLuint>(submesh.indexCount), GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(submesh.indexOffset * sizeof(uint)), 1);

            lightsBuffer->release();
            mesh.indexBuffer.release();
            mesh.vertexArray.release();
        }

        material.release();
    }

    fbo->release();
}

priority_type AnimatedMeshRenderStage::priority()
{
    return opaque_priority;
}
