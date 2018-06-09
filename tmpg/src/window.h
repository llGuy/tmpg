#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace tmpg {

	extern void GLFWInit(void);
	extern void GLFWTerminate(void);
	extern void GLEWInit(void);

	using MBCallbackFunc = GLFWmousebuttonfun;

	class Window
	{
	public:
		Window(void) = default;
		Window(const char*, int32_t, int32_t);
		~Window(void);

		void Init(void);
		void Refresh(void);
		bool Open(void) const;
		bool ReceivedKey(int32_t key) const;
	public:
		int32_t Width(void) const;
		int32_t Height(void) const;
	public:
		// configuration functions
		void WindowHint(int32_t, int32_t);
		void UserPointer(void);
		void InputMode(int32_t mode, int32_t val);
		void MouseCallback(MBCallbackFunc cbfun);
	private:
		int32_t m_width;
		int32_t m_height;

		const char* m_title;
		GLFWwindow* m_glfwWindow;
	};

}

#endif	