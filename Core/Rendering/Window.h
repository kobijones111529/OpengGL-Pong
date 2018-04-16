#pragma once

#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <vector>

template<class T>
struct UserCallback {
	T callback;
	void* instance;
};

class Window {
private:
	GLFWwindow* m_Window;
	int m_Width;
	int m_Height;
	std::string m_Title;
	
	static double m_CursorX;
	static double m_CursorY;
	static bool m_Keys[1024];
	static bool m_MouseButtons[32];
	
	std::vector<UserCallback<void(*)(int, int, void*)>> m_UserWindowSizeCallbackArray;
	std::vector<UserCallback<void(*)(int, int, void*)>> m_UserFramebufferSizeCallbackArray;
	std::vector<UserCallback<void(*)(double, double, void*)>> m_UserCursorPosCallbackArray;
	std::vector<UserCallback<void(*)(int, int, int, int, void*)>> m_UserKeyCallbackArray;
	std::vector<UserCallback<void(*)(int, int, int, void*)>> m_UserMouseButtonCallbackArray;
	
	static void WindowSizeCallback(GLFWwindow* _window, int _width, int _height);
	static void FramebufferSizeCallback(GLFWwindow* _window, int _width, int _height);
	static void CursorPosCallback(GLFWwindow* _window, double _x, double _y);
	static void KeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
	static void MouseButtonCallback(GLFWwindow* _window, int _button, int _action, int _mods);
public:
	Window();
	~Window();
	
	void AddUserWindowSizeCallback(void(*_callback)(int, int, void*), void* _instance);
	void AddUserFramebufferSizeCallback(void(*_callback)(int, int, void*), void* _instance);
	void AddUserCursorPosCallback(void(*_callback)(double, double, void*), void* _instance);
	void AddUserKeyCallback(void(*_callback)(int, int, int, int, void*), void* _instance);
	void AddUserMouseButtonCallback(void(*_callback)(int, int, int, void*), void* _instance);
	
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline const std::string& GetTitle() const { return m_Title; }
	static inline double GetCursorX() { return m_CursorX; }
	static inline double GetCursorY() { return m_CursorY; }
	static inline bool GetKey(int _key) { return m_Keys[_key]; }
	static inline bool GetMouseButton(int _button) { return m_MouseButtons[_button]; }
	
	bool Init(int _width, int _height, const std::string& _title);
	void Update();
	bool IsClosed() const;
};