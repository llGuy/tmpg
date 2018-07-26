#ifndef _INPUT_HANDLER_H_
#define _INPUT_HANDLER_H_

#include <array>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#define MAX_KEYS 350
#define MAX_MOUSE_BUTTONS 5

namespace tmpg {

	// input type for sequential input container
	struct Input
	{
		uint16_t flags;
		float time;
		glm::vec2 mouseMagnitude;
	};

	// handles key input AND mouse input
	class InputHandler
	{
	public:
		InputHandler(void);

		bool& Key(int32_t key);
		bool& Resized(void);
		bool& CursorMoved(void);
		bool& MouseButton(int32_t mouseButton);
		const glm::vec2& CursorPosition(void);
		const glm::vec2& CursorDifference(void);

		void PushInput(float elapsed);
		void ResetInputSequence(void);
		uint16_t SequenceSize(void);
		Input& operator[](uint32_t i);

		const glm::vec2 WindowSize(void);
	public:
		static void HandleWindowResize(GLFWwindow* win, int32_t width, int32_t height);
		static void HandleKeyInput(GLFWwindow* win, int key, int scancode, int action, int mods);
		static void HandleMouseMovement(GLFWwindow* win, double x, double y);
		static void HandleMouseInput(GLFWwindow* win, int button, int action, int mods);
	private:
		std::array<bool, MAX_KEYS> m_keyMap;
		std::array<bool, MAX_MOUSE_BUTTONS> m_mouseButtonMap;

		glm::vec2 m_currentCursorPosition;
		glm::vec2 m_cursorDifference;
		// if cursor did not move, do not calculate entity new direction
		bool m_cursorMoved;
		bool m_windowResized;

		int32_t m_width;
		int32_t m_height;

		std::vector<Input> m_inputSequence;
	};

}

#endif
