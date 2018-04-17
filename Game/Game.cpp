#include "Game.h"

Game::Game() { }
Game::~Game() { }

bool Game::Init() {
	if(!glfwInit()) {
		std::cerr << "GLFW Error: Failed to initialize!" << std::endl;
		return false;
	}
	
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	m_Window = new Window;
	
	if(!m_Window->Init(1000, 1000, "OpenGL")) {
		glfwTerminate();
		return false;
	}
	
	m_Window->AddUserKeyCallback(KeyCallback, this);
	
	if(glewInit() != GLEW_OK) {
		std::cerr << "GLEW Error: Failed to initialize!" << std::endl;
		glfwTerminate();
		return false;
	}
	
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	
	m_MainGame.Init(m_Window);
	m_MainGame.SetControlState(MainGame::ControlState::KEYBOARD);
	
	return true;
}

void Game::Destroy() {
	glfwTerminate();
}

void Game::Run() {
	srand((unsigned int)time(NULL));
	
	m_CurrentFrame = glfwGetTime();
	m_LastFrame = m_CurrentFrame;
	m_LastFrameLog = m_CurrentFrame;
	
	m_MainGame.Run();
	
	while(!m_Window->IsClosed()) {
		Update();
		Render();
		m_Window->Update();
	}
	
	m_MainGame.Destroy();
}

void Game::Update() {
	m_CurrentFrame = glfwGetTime();
	m_DeltaTime = m_CurrentFrame - m_LastFrame;
	m_LastFrame = m_CurrentFrame;
	
	if(m_CurrentFrame - m_LastFrameLog > 1.0) {
		std::cout << 1.0 / m_DeltaTime << std::endl;
		m_LastFrameLog = m_CurrentFrame;
	}
	
	m_MainGame.Update(m_DeltaTime);
}

void Game::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	m_MainGame.Render();
}

void Game::KeyCallback(int _key, int _scancode, int _action, int _mods, void* _instance) {
	Game* instance = (Game*)_instance;
	
	if(_key == GLFW_KEY_SPACE && _action == GLFW_PRESS) {
		if(instance->m_MainGame.GetControlState() == MainGame::ControlState::KEYBOARD) {
			instance->m_MainGame.SetControlState(MainGame::ControlState::MOUSE);
		} else if(instance->m_MainGame.GetControlState() == MainGame::ControlState::MOUSE) {
			instance->m_MainGame.SetControlState(MainGame::ControlState::KEYBOARD);
		} else {
			instance->m_MainGame.SetControlState(MainGame::ControlState::KEYBOARD);
		}
	 }
}