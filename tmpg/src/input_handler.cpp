#define DEBUG 0

#include "input_handler.h"

namespace tmpg {

	bool& InputHandler::Key(int32_t key)
	{
		return m_keyMap[key];
	}

	bool& InputHandler::MouseButton(int32_t mouseButton)
	{
		return m_mouseButtonMap[mouseButton];
	}

	void InputHandler::HandleKeyInput(GLFWwindow* win, int key, int scancode, int action, int mods)
	{
		if (key < 100)
		{
			InputHandler* inputHandler = (InputHandler*)glfwGetWindowUserPointer(win);
			if (action == GLFW_PRESS) inputHandler->m_keyMap[key] = true;
			else if (action == GLFW_RELEASE) inputHandler->m_keyMap[key] = false;
		}

#if DEBUG
		std::cout << "INPUT_HANDLER.CPP : " << ((action == GLFW_RELEASE) ? "release" : "press") << std::endl;
#endif
	}

	void InputHandler::HandleMouseMovement(GLFWwindow* win, double x, double y)
	{
		InputHandler* inputHandler = (InputHandler*)glfwGetWindowUserPointer(win);
		inputHandler->m_currentCursorPosition = glm::vec2(static_cast<float>(x), static_cast<float>(y));
		inputHandler->m_cursorMoved = true;
	}

	void InputHandler::HandleMouseInput(GLFWwindow* win, int button, int action, int mods)
	{
		InputHandler* inputHandler = (InputHandler*)glfwGetWindowUserPointer(win);
		if (action == GLFW_PRESS) inputHandler->m_mouseButtonMap[button] = true;
		else if (action == GLFW_RELEASE) inputHandler->m_mouseButtonMap[button] = false;
	}

	const glm::vec2& InputHandler::CursorPosition(void)
	{
		m_cursorMoved = false;
		return m_currentCursorPosition;
	}

	bool InputHandler::CursorMoved(void)
	{
		return m_cursorMoved;
	}

}