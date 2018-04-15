#pragma once

#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

class Window {
public:
	enum class ControlState : int { KEYBOARD, MOUSE };
private:
	GLFWwindow* m_Window;
	int m_Width;
	int m_Height;
	std::string m_Title;
	
	static double m_CursorX;
	static double m_CursorY;
	static bool m_Keys[1024];
	static bool m_MouseButtons[32];
	
	ControlState m_ControlState = ControlState::KEYBOARD;
	int m_ControlStates = 2;
	
	static void WindowSizeCallback(GLFWwindow* _window, int _width, int _height);
	static void FramebufferSizeCallback(GLFWwindow* _window, int _width, int _height);
	static void CursorPosCallback(GLFWwindow* _window, double _x, double _y);
	static void KeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
	static void MouseButtonCallback(GLFWwindow* _window, int _button, int _action, int _mods);
public:
	Window();
	~Window();
	
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline const std::string& GetTitle() const { return m_Title; }
	static inline double GetCursorX() { return m_CursorX; }
	static inline double GetCursorY() { return m_CursorY; }
	static inline bool GetKey(int _key) { return m_Keys[_key]; }
	static inline bool GetMouseButton(int _button) { return m_MouseButtons[_button]; }
	
	inline ControlState GetControlState() { return m_ControlState; }
	
	bool Init(int _width, int _height, const std::string& _title);
	void Update();
	bool IsClosed() const;
};