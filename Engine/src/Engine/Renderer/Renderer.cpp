#include "engine_pch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include <Engine/Renderer/Shader.h>
#include <Platform/OpenGL/OpenGLShader.h>

namespace Engine
{
    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

    void Renderer::Init()
    {
        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(const OrthographicCamera& camera)
    {
        m_SceneData->ViewProjectionMat = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform)
    {
        std::dynamic_pointer_cast<OpenGLShader> (shader)->use();
        std::dynamic_pointer_cast<OpenGLShader> (shader)->setMat4fv("modelMat", transform);
        std::dynamic_pointer_cast<OpenGLShader> (shader)->setMat4fv("viewProjMat", m_SceneData->ViewProjectionMat);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}