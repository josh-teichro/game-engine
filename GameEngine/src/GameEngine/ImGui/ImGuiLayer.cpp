#include "gepch.h"

#include "ImGuiLayer.h"
#include "GameEngine/Application.h"
#include "GameEngine/Input.h"

#include "Platform/OpenGL/imgui_impl_opengl3.h"

// TODO: remove
#include "GLFW/glfw3.h"

namespace GameEngine {

	ImGuiLayer::ImGuiLayer() :
		Layer("ImGuiLayer"),
		m_time(0.0f)
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		Application& app = Application::Get();

		// Setup backend
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// Setup Keyboard mappings 
		io.KeyMap[ImGuiKey_Tab] = (int)KeyCode::Tab;
		io.KeyMap[ImGuiKey_LeftArrow] = (int)KeyCode::LeftArrow;
		io.KeyMap[ImGuiKey_RightArrow] = (int)KeyCode::RightArrow;
		io.KeyMap[ImGuiKey_UpArrow] = (int)KeyCode::UpArrow;
		io.KeyMap[ImGuiKey_DownArrow] = (int)KeyCode::DownArrow;
		io.KeyMap[ImGuiKey_PageUp] = (int)KeyCode::PageUp;
		io.KeyMap[ImGuiKey_PageDown] = (int)KeyCode::PageDown;
		io.KeyMap[ImGuiKey_Home] = (int)KeyCode::Home;
		io.KeyMap[ImGuiKey_End] = (int)KeyCode::End;
		io.KeyMap[ImGuiKey_Insert] = (int)KeyCode::Insert;
		io.KeyMap[ImGuiKey_Delete] = (int)KeyCode::Delete;
		io.KeyMap[ImGuiKey_Backspace] = (int)KeyCode::Backspace;
		io.KeyMap[ImGuiKey_Space] = (int)KeyCode::Space;
		io.KeyMap[ImGuiKey_Enter] = (int)KeyCode::Enter;
		io.KeyMap[ImGuiKey_Escape] = (int)KeyCode::Escape;
		io.KeyMap[ImGuiKey_KeyPadEnter] = (int)KeyCode::KeyPadEnter;
		io.KeyMap[ImGuiKey_A] = (int)KeyCode::A;
		io.KeyMap[ImGuiKey_C] = (int)KeyCode::C;
		io.KeyMap[ImGuiKey_V] = (int)KeyCode::V;
		io.KeyMap[ImGuiKey_X] = (int)KeyCode::X;
		io.KeyMap[ImGuiKey_Y] = (int)KeyCode::Y;
		io.KeyMap[ImGuiKey_Z] = (int)KeyCode::Z;

		// Set clipboard callbacks
		io.SetClipboardTextFn = [](void* userData, const char* text) {
			Input::SetClipBoardText(text);
		};
		io.GetClipboardTextFn = [](void* userData) {
			return Input::GetClipBoardText();
		};

		// Use OpenGL
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();

		// Set window size
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		// Setup time step
		double current_time = glfwGetTime();
		io.DeltaTime = m_time > 0.0 ? (float)(current_time - m_time) : (float)(1.0f / 60.0f);
		m_time = current_time;

		// Handle events
		UpdateMousePosAndButtons();
		UpdateMouseCursor();

		// Begin frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show_demo_window = true;

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// Draw ImGui stuff here

		// End frame
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	bool ImGuiLayer::OnMouseDown(const MouseDownEvent& e)
	{
		m_mouseJustPressed[e.button] = true;
		return false;
	}

	bool ImGuiLayer::OnKeyDown(const KeyDownEvent& e)
	{
		bool handled = false;
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		if ((int)e.keyCode >= 0 && (int)e.keyCode < IM_ARRAYSIZE(io.KeysDown)) {
			io.KeysDown[(int)e.keyCode] = true;
			handled = true;
		}

		io.KeyCtrl = io.KeysDown[(int)KeyCode::LeftCtrl] || io.KeysDown[(int)KeyCode::RightCtrl];
		io.KeyShift = io.KeysDown[(int)KeyCode::LeftShift] || io.KeysDown[(int)KeyCode::RightShift];
		io.KeyAlt = io.KeysDown[(int)KeyCode::LeftAlt] || io.KeysDown[(int)KeyCode::RightAlt];
		io.KeySuper = io.KeysDown[(int)KeyCode::LeftSuper] || io.KeysDown[(int)KeyCode::RightSuper];

		return handled;
	}

	bool ImGuiLayer::OnKeyUp(const KeyUpEvent& e)
	{
		bool handled = false;
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		if ((int)e.keyCode >= 0 && (int)e.keyCode < IM_ARRAYSIZE(io.KeysDown)) {
			io.KeysDown[(int)e.keyCode] = false;
			handled = true;
		}

		// Modifiers are not reliable across systems
		io.KeyCtrl = io.KeysDown[(int)KeyCode::LeftCtrl] || io.KeysDown[(int)KeyCode::RightCtrl];
		io.KeyShift = io.KeysDown[(int)KeyCode::LeftShift] || io.KeysDown[(int)KeyCode::RightShift];
		io.KeyAlt = io.KeysDown[(int)KeyCode::LeftAlt] || io.KeysDown[(int)KeyCode::RightAlt];
		io.KeySuper = io.KeysDown[(int)KeyCode::LeftSuper] || io.KeysDown[(int)KeyCode::RightSuper];

		return handled;
	}

	bool ImGuiLayer::OnMouseScroll(const MouseScrollEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.offsetX;
		io.MouseWheel += e.offsetY;
		return false;
	}

	bool ImGuiLayer::OnCharTyped(const CharTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter(e.character);
		return false;
	}

	void ImGuiLayer::UpdateMousePosAndButtons()
	{
		// Update buttons
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
		{
			io.MouseDown[i] = m_mouseJustPressed[MapMouseButton(i)] || Input::GetMouseDown(MapMouseButton(i));
			m_mouseJustPressed[MapMouseButton(i)] = false;
		}

		// Update mouse position
		// TODO: add check for if window is focused once that functionality is available
		// NOTE: didn't add functionality to set cursor position from ImGui (apparently rarely used)
		Input::MousePosition pos = Input::GetMousePosition();
		io.MousePos = ImVec2(pos.x, pos.y);
	}

	void ImGuiLayer::UpdateMouseCursor()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		Input::MouseCursor cursor = Input::MouseCursor_Arrow;

		if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange))
			return;

		switch (ImGui::GetMouseCursor())
		{
			case ImGuiMouseCursor_None: cursor = Input::MouseCursor_None; break;
			case ImGuiMouseCursor_Arrow: cursor = Input::MouseCursor_Arrow; break;
			case ImGuiMouseCursor_TextInput: cursor = Input::MouseCursor_TextInput; break;
			case ImGuiMouseCursor_ResizeAll: cursor = Input::MouseCursor_ResizeAll; break;
			case ImGuiMouseCursor_ResizeNS: cursor = Input::MouseCursor_ResizeNS; break;
			case ImGuiMouseCursor_ResizeEW: cursor = Input::MouseCursor_ResizeEW; break;
			case ImGuiMouseCursor_ResizeNESW: cursor = Input::MouseCursor_ResizeNESW; break;
			case ImGuiMouseCursor_ResizeNWSE: cursor = Input::MouseCursor_ResizeNWSE; break;
			case ImGuiMouseCursor_Hand: cursor = Input::MouseCursor_Hand; break;
			case ImGuiMouseCursor_NotAllowed: cursor = Input::MouseCursor_NotAllowed; break;
		}

		if (io.MouseDrawCursor) {
			cursor = Input::MouseCursor_None;
		}

		Input::SetMouseCursor(cursor);
	}

	inline MouseButton ImGuiLayer::MapMouseButton(int button)
	{
		switch (button)
		{
			case ImGuiMouseButton_Left: return MouseButton_Left;
			case ImGuiMouseButton_Right: return MouseButton_Right;
			case ImGuiMouseButton_Middle: return MouseButton_Middle;
			default: return MouseButton_Unknown;
		}
	}

}
