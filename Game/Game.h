#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Core/Rendering/Window.h"
#include "../Core/Rendering/GameObject.h"
#include "../Core/Rendering/Quad.h"
#include "../Game/Paddle.h"
#include "../Game/Ball.h"

class Game {
private:
	Window m_Window;
	
	double m_LastFrame;
	double m_CurrentFrame;
	double m_DeltaTime;
	double m_LastFrameLog;
	
	Paddle* leftPaddle;
	Paddle* rightPaddle;
	Ball* ball;
	
	bool ballMissedLeft = false;
	bool ballMissedRight = false;
	
	float LeftPaddleVerticalOverlap();
	float RightPaddleVerticalOverlap();
	float LeftPaddleHorizontalOverlap();
	float RightPaddleHorizontalOverlap();
	
	void Update();
	void Render();
public:
	Game();
	~Game();
	
	bool Init();
	void Destroy();
	void Run();
};