#pragma once

namespace GameEngine {

	/**
	* Mouse button codes.
	*
	* Codes are the same as GLFW codes.
	*/
	enum class MouseButton {
		Unknown = -1,
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

	/**
	* Mouse cursors.
	*
	* Codes are the same as GLFW codes.
	*/
	enum class MouseCursor {
		None = -1,
		Arrow = 0,
		Hand,
		TextInput,
		ResizeNS,		  // When hovering over a horizontal border
		ResizeEW,		  // When hovering over a vertical border or a column
		ResizeNESW,		  // When hovering over the bottom-left corner of a window
		ResizeNWSE,		  // When hovering over the bottom-right corner of a window
		ResizeAll,
		NotAllowed,
		Count,
		ResizeVertical = ResizeNS,
		ResizeHorizontal = ResizeEW
	};

}
