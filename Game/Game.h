#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Core/Rendering/Window.h"
#include "../Core/Rendering/GameObject.h"
#include "../Core/Rendering/Quad.h"
#include "Paddle.h"
#include "Ball.h"

class Game {
private:
	enum class ControlState : int { KEYBOARD, MOUSE };
	enum class Side { NONE, LEFT, RIGHT };
	enum class Event { NONE, RESET, START, LOST };
	
	Window* m_Window;
	
	double m_LastFrame;
	double m_CurrentFrame;
	double m_DeltaTime;
	double m_LastFrameLog;
	
	ControlState m_ControlState = ControlState::KEYBOARD;
	const int m_ControlStates = 2;
	
	Paddle* leftPaddle;
	Paddle* rightPaddle;
	Ball* ball;
	
	double m_GameResetTime;
	double m_GameStartTime;
	double m_GameLostTime;
	Event m_LastEvent = Event::NONE;
	
	static void KeyCallback(int _key, int _scancode, int _action, int _mods, void* _instance);
	
	float LeftPaddleVerticalOverlap();
	float RightPaddleVerticalOverlap();
	float LeftPaddleHorizontalOverlap();
	float RightPaddleHorizontalOverlap();
	
	void Update();
	void Render();
	
	void UpdatePaddles();
	void UpdateBall();
	
	void Reset();
	void Start();
	void Lost(Side _side);
public:
	Game();
	~Game();
	
	bool Init();
	void Destroy();
	void Run();
};