#include "EditorLayer.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace GameEngine
{

    EditorLayer::EditorLayer() :
        m_cameraController(1.0f),
        m_viewportPanelSize(0.0f, 0.0f)
    {
        FrameBufferProps props;
        props.width = 1280;
        props.height = 720;
        m_frameBuffer = FrameBuffer::Create(props);
    }

    void EditorLayer::OnAttach()
    {
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate()
    {
        GE_PROFILE_FUNCTION();

        m_cameraController.OnUpdate();

        m_frameBuffer->Bind();

        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RenderCommand::Clear();

        Renderer2D::ResetStats();
        Renderer2D::BeginScene(m_cameraController.GetCamera());

        // draw scene
        Renderer2D::RectTransform transform;
        Renderer2D::RectMaterial material;
        material.color = { 7.0f, 4.0f, 7.0f, 1.0f };
        Renderer2D::DrawRect(transform, material);

        Renderer2D::EndScene();

        m_frameBuffer->Unbind();
    }

    void EditorLayer::OnImGuiUpdate()
    {
        GE_PROFILE_FUNCTION();

        static bool open = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &open, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        else
        {
            GE_CORE_ASSERT(false, "ImGui docking is not enabled!");
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit")) GameEngine::Application::Get().Close();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // render windows here
        RenderEditorWindows();

        ImGui::End();
    }

    bool EditorLayer::OnEvent(const GameEngine::Event& e)
    {
        GE_PROFILE_FUNCTION();

        bool handled = false;
        handled = handled ||  HandlesEvents::OnEvent(e);

        if (m_isViewportFocused && m_isViewportHovered) 
        {
            handled = handled || m_cameraController.OnEvent(e);
        }
        else {
            handled = true;
        }

        return handled;
    }

    bool EditorLayer::OnKeyUp(const GameEngine::KeyUpEvent& e)
    {
        return false;
    }

    bool EditorLayer::OnWindowResize(const WindowResizeEvent& e)
    {
        return true;
    }

    void EditorLayer::RenderEditorWindows()
    {
        ImGui::Begin("Renderer Stats");

        auto stats = Renderer2D::GetStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.vertexCount);
        ImGui::Text("Indices: %d", stats.indexCount);
        ImGui::Text("Max Texture Slots Used: %d", stats.maxTextureSlotsUsed);

        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");

        m_isViewportFocused = ImGui::IsWindowFocused();
        m_isViewportHovered = ImGui::IsWindowHovered();

        uint32_t textureID = m_frameBuffer->GetColorAttachmentRendererID();
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();

        if (viewportSize.x != m_viewportPanelSize.x || viewportSize.y != m_viewportPanelSize.y)
        {
            m_frameBuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
            m_cameraController.OnEvent(WindowResizeEvent((int)viewportSize.x, (int)viewportSize.y));
            m_viewportPanelSize = { viewportSize.x, viewportSize.y };
        }

        ImGui::Image((void*)(uint64_t)textureID, ImVec2{ m_viewportPanelSize.x, m_viewportPanelSize.y });

        ImGui::End();
        ImGui::PopStyleVar();
    }

}
