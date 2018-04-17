#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Core/Rendering/Window.h"
#include "MainGame.h"

class Game {
private:
	Window* m_Window;
	MainGame m_MainGame;
	
	double m_LastFrame;
	double m_CurrentFrame;
	double m_DeltaTime;
	double m_LastFrameLog;
	
	void Update();
	void Render();
	
	static void KeyCallback(int _key, int _scancode, int _action, int _mods, void* _instance);
public:
	
	Game();
	~Game();
	
	bool Init();
	void Destroy();
	void Run();
};