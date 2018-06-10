#include <iostream>
#include "window.h"

namespace tmpg {

	void GLFWInit(void)
	{
		if (!glfwInit()) std::cerr << "ERROR: unable to initialize GLFW" << std::endl;
		else std::cout << "SUCCESS: initialized GLFW" << std::endl;
	}

	void GLFWTerminate(void)
	{
		glfwTerminate();
	}

	void GLEWInit(void)
	{
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) std::cerr << "ERROR: unable to initialize GLEW" << std::endl;
		else std::cout << "SUCCESS: initialized GLEW" << std::endl;
	}

	Window::Window(const char* title, int32_t w, int32_t h)
		: m_width(w), m_height(h), m_title(title)
	{
	}

	Window::~Window(void)
	{
		glfwDestroyWindow(m_glfwWindow);
	}

	void Window::Init(void)
	{
		if (!(m_glfwWindow = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL)))
			std::cerr << "ERROR: unable to initialize window" << std::endl;
		else std::cout << "SUCCESS: initialized window" << std::endl;
		glfwMakeContextCurrent(m_glfwWindow);
	}

	void Window::Refresh(void)
	{
		glfwSwapBuffers(m_glfwWindow);
		glfwPollEvents();
	}

	bool Window::ReceivedKey(int32_t key) const
	{
		return glfwGetKey(m_glfwWindow, key);
	}

	void Window::WindowHint(int32_t maj, int32_t min)
	{
		glfwWindowHint(maj, min);
	}

	void Window::InputMode(int32_t mode, int32_t val)
	{
		glfwSetInputMode(m_glfwWindow, mode, val);
	}

	void Window::MouseButtonCallback(MBCallbackFunc cbfun)
	{
		glfwSetMouseButtonCallback(m_glfwWindow, cbfun);
	}

	void Window::KeyCallback(KeyCallbackFunc func)
	{
		glfwSetKeyCallback(m_glfwWindow, func);
	}

	void Window::CursorMovementCallback(CursorMovementFunc func)
	{
		glfwSetCursorPosCallback(m_glfwWindow, func);
	}

	bool Window::Open(void) const
	{
		return !glfwWindowShouldClose(m_glfwWindow) && !ReceivedKey(GLFW_KEY_ESCAPE);
	}

	int32_t Window::Width(void) const
	{
		return m_width;
	}

	int32_t Window::Height(void) const
	{
		return m_height;
	}

	glm::vec2 Window::CursorPosition(void)
	{
		double x, y;
		glfwGetCursorPos(m_glfwWindow, &x, &y);
		return glm::vec2(static_cast<float>(x), static_cast<float>(y));
	}

}
