#pragma once

/**
* Mouse button codes.
*
* Codes are the same as GLFW codes.
*/
enum class MouseButton {
	Unknown = - 1,
	Button1 = 0,
	Button2,
	Button3,
	Button4,
	Button5,
	Button6,
	Button7,
	Button8,
	NumMouseButtons,
	Left = Button1,
	Right = Button2,
	Middle = Button3
};
