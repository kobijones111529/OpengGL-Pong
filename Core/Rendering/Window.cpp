#include "Window.h"

bool Window::m_Keys[1024];
bool Window::m_MouseButtons[32];
double Window::m_CursorX;
double Window::m_CursorY;

void Window::WindowSizeCallback(GLFWwindow* _window, int _width, int _height) {
	Window* windowUserPointer = (Window*)glfwGetWindowUserPointer(_window);
	
	windowUserPointer->m_Width = _width;
	windowUserPointer->m_Height = _height;
}

void Window::FramebufferSizeCallback(GLFWwindow* _window, int _width, int _height) {
	glViewport(0, 0, _width, _height);
}

void Window::CursorPosCallback(GLFWwindow* _window, double _x, double _y) {
	m_CursorX = _x;
	m_CursorY = _y;
}

void Window::KeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods) {
	Window* windowUserPointer = (Window*)glfwGetWindowUserPointer(_window);
	
	if(_action == GLFW_PRESS) {
		if(_key == GLFW_KEY_SPACE) {
			windowUserPointer->m_ControlState = (ControlState)(((int)windowUserPointer->m_ControlState + 1) % windowUserPointer->m_ControlStates);
		}
	}
	
	if(_action == GLFW_PRESS || _action == GLFW_REPEAT) {
		if(_key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(_window, true);
			return;
		}
		
		m_Keys[_key] = true;
	} else {
		m_Keys[_key] = false;
	}
}

void Window::MouseButtonCallback(GLFWwindow *_window, int _button, int _action, int _mods) {
	if(_action == GLFW_PRESS) {
		m_MouseButtons[_button] = true;
	} else {
		m_MouseButtons[_button] = false;
	}
}

Window::Window() { }
Window::~Window() { }

bool Window::Init(int _width, int _height, const std::string &_title) {
	m_Window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);
	
	if(!m_Window) {
		std::cerr << "GLFW Error: Failed to create window!" << std::endl;
		return false;
	}
	
	m_Width = _width;
	m_Height = _height;
	m_Title = _title;
	
	glfwMakeContextCurrent(m_Window);
	
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetWindowSizeCallback(m_Window, WindowSizeCallback);
	glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);
	glfwSetCursorPosCallback(m_Window, CursorPosCallback);
	glfwSetKeyCallback(m_Window, KeyCallback);
	glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
	
	return true;
}

void Window::Update() {
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

bool Window::IsClosed() const {
	return glfwWindowShouldClose(m_Window);
}