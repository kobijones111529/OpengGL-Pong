#include "Window.h"

bool Window::m_Keys[1024];
bool Window::m_MouseButtons[32];
double Window::m_CursorX;
double Window::m_CursorY;

void Window::WindowSizeCallback(GLFWwindow* _window, int _width, int _height) {
	Window* windowUserPointer = (Window*)glfwGetWindowUserPointer(_window);
	
	windowUserPointer->m_Width = _width;
	windowUserPointer->m_Height = _height;
	
	for(UserCallback<void(*)(int, int, void*)> callback : windowUserPointer->m_UserWindowSizeCallbackArray) {
		callback.callback(_width, _height, callback.instance);
	}
}

void Window::FramebufferSizeCallback(GLFWwindow* _window, int _width, int _height) {
	Window* windowUserPointer = (Window*)glfwGetWindowUserPointer(_window);
	
	glViewport(0, 0, _width, _height);
	
	for(UserCallback<void(*)(int, int, void*)> callback : windowUserPointer->m_UserFramebufferSizeCallbackArray) {
		callback.callback(_width, _height, callback.instance);
	}
}

void Window::CursorPosCallback(GLFWwindow* _window, double _x, double _y) {
	Window* windowUserPointer = (Window*)glfwGetWindowUserPointer(_window);
	
	m_CursorX = _x;
	m_CursorY = _y;
	
	for(UserCallback<void(*)(double, double, void*)> callback : windowUserPointer->m_UserCursorPosCallbackArray) {
		callback.callback(_x, _y, callback.instance);
	}
}

void Window::KeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods) {
	Window* windowUserPointer = (Window*)glfwGetWindowUserPointer(_window);
	
	if(_action == GLFW_PRESS || _action == GLFW_REPEAT) {
		if(_key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(_window, true);
			return;
		}
		
		m_Keys[_key] = true;
	} else {
		m_Keys[_key] = false;
	}
	
	for(UserCallback<void(*)(int, int, int, int, void*)> callback : windowUserPointer->m_UserKeyCallbackArray) {
		callback.callback(_key, _scancode, _action, _mods, callback.instance);
	}
}

void Window::MouseButtonCallback(GLFWwindow *_window, int _button, int _action, int _mods) {
	Window* windowUserPointer = (Window*)glfwGetWindowUserPointer(_window);
	
	if(_action == GLFW_PRESS) {
		m_MouseButtons[_button] = true;
	} else {
		m_MouseButtons[_button] = false;
	}
	
	for(UserCallback<void(*)(int, int, int, void*)> callback : windowUserPointer->m_UserMouseButtonCallbackArray) {
		callback.callback(_button, _action, _mods, callback.instance);
	}
}

void Window::AddUserWindowSizeCallback(void(*_callback)(int, int, void*), void* _instance) {
	m_UserWindowSizeCallbackArray.push_back({ _callback, _instance });
}

void Window::AddUserFramebufferSizeCallback(void(*_callback)(int, int, void*), void* _instance) {
	m_UserFramebufferSizeCallbackArray.push_back({ _callback, _instance });
}

void Window::AddUserCursorPosCallback(void(*_callback)(double, double, void*), void* _instance) {
	m_UserCursorPosCallbackArray.push_back({ _callback, _instance });
}

void Window::AddUserKeyCallback(void(*_callback)(int, int, int, int, void*), void* _instance) {
	m_UserKeyCallbackArray.push_back({ _callback, _instance });
}

void Window::AddUserMouseButtonCallback(void(*_callback)(int, int, int, void*), void* _instance) {
	m_UserMouseButtonCallbackArray.push_back({ _callback, _instance });
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