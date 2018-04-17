#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Paddle.h"
#include "Ball.h"

class MainGame {
public:
	enum class Event { NONE, START, LOST, RESET };
	enum class ControlState { NONE, KEYBOARD, MOUSE };
	enum class Side { NONE, LEFT, RIGHT };
private:
	Window* m_Window;
	
	Event m_LastEvent = Event::NONE;
	ControlState m_ControlState = ControlState::NONE;
	
	Paddle* m_LeftPaddle;
	Paddle* m_RightPaddle;
	Ball* m_Ball;
	
	double m_Running;
	double m_RunTime;
	double m_StopTime;
	
	double m_LastStartTime;
	double m_LastLostTime;
	double m_LastResetTime;
	
	float LeftPaddleVerticalOverlap();
	float RightPaddleVerticalOverlap();
	float LeftPaddleHorizontalOverlap();
	float RightPaddleHorizontalOverlap();
	
	void UpdatePaddles(double _deltaTime);
	void UpdateBall(double _deltaTime);
	
	void Start();
	void Lost(Side _side);
	void Reset();
	
	static void KeyCallback(int _key, int _scancode, int _action, int _mods, void* _instance);
public:
	MainGame();
	~MainGame();
	
	void Init(Window* _window);
	void Run();
	void Stop();
	void Destroy();
	
	void Update(double _deltaTime);
	void Render();
	
	inline const ControlState& GetControlState() const { return m_ControlState; }
	inline void SetControlState(ControlState _controlState) { m_ControlState = _controlState; }
};