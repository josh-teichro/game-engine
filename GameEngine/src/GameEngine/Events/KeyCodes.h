#pragma once

namespace GameEngine {
	/**
	* Keycodes.
	*
	* Codes are the same as GLFW codes.
	*/
	enum class KeyCode {
		Unknown = -1,

		// Printable keys
		Space = 32,
		Apostrophe = 39,
		Comma = 44,
		Minus,
		Period,
		Slash,
		_0,
		_1,
		_2,
		_3,
		_4,
		_5,
		_6,
		_7,
		_8,
		_9,
		Semicolon = 59,
		Equal = 61,
		A = 65,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		LeftBracket,
		RightBracket,
		Backslash,
		GraveAccent = 96,
		World1 = 161,
		World2,

		// Function keys
		Escape = 256,
		Enter,
		Tab,
		Backspace,
		Insert,
		Delete,
		RightArrow,
		LeftArrow,
		DownArrow,
		UpArrow,
		PageUp,
		PageDown,
		Home,
		End,
		CapsLock = 280,
		ScollLock,
		NumLock,
		PrintScreen,
		Pause,
		F1 = 290,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		F16,
		F17,
		F18,
		F19,
		F20,
		F21,
		F22,
		F23,
		F24,
		F25,
		KeyPad0 = 320,
		KeyPad1,
		KeyPad2,
		KeyPad3,
		KeyPad4,
		KeyPad5,
		KeyPad6,
		KeyPad7,
		KeyPad8,
		KeyPad9,
		KeyPadDecimal,
		KeyPadDivide,
		KeyPadMultiply,
		KeyPadSubtract,
		KeyPadAdd,
		KeyPadEnter,
		KeyPadEqual,
		LeftShift = 340,
		LeftCtrl,
		LeftAlt,
		LeftSuper,
		RightShift,
		RightCtrl,
		RightAlt,
		RightSuper,
		Menu,

		MaxKeyCodes = 512
	};

}
 