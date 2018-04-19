#include "MainGame.h"

MainGame::MainGame() { }
MainGame::~MainGame() { }

void MainGame::Init(Window* _window) {
	m_Window = _window;
	
	m_Window->AddUserKeyCallback(KeyCallback, this);
	
	Shader shader;
	shader.Init("BasicVertex.glsl", "BasicFragment.glsl");
	
	m_LeftPaddle = new Paddle(0.05f, 0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_LeftPaddle->transform.translation.x = -1.0f;
	m_LeftPaddle->transform.translation.y = m_LeftPaddle->height / 2.0f;
	m_LeftPaddle->AddComponent<MeshRenderer>();
	m_LeftPaddle->GetComponent<MeshRenderer>()->shader = shader;
	m_LeftPaddle->UpdateMesh();
	
	m_RightPaddle = new Paddle(0.05f, 0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_RightPaddle->transform.translation.x = 1.0f - m_RightPaddle->width;
	m_RightPaddle->transform.translation.y = m_RightPaddle->height / 2.0f;
	m_RightPaddle->AddComponent<MeshRenderer>();
	m_RightPaddle->GetComponent<MeshRenderer>()->shader = shader;
	m_RightPaddle->UpdateMesh();
	
	m_Ball = new Ball(0.05f, 0.05f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_Ball->transform.translation.x = -m_Ball->width / 2.0f;
	m_Ball->transform.translation.y = m_Ball->height / 2.0f;
	m_Ball->AddComponent<MeshRenderer>();
	m_Ball->GetComponent<MeshRenderer>()->shader = shader;
	m_Ball->UpdateMesh();
}

void MainGame::Run() {
	m_Running = true;
	m_RunTime = glfwGetTime();
}

void MainGame::Stop() {
	m_StopTime = glfwGetTime();
	m_Running = false;
}

void MainGame::Destroy() {
	delete m_LeftPaddle;
	delete m_RightPaddle;
	delete m_Ball;
}

void MainGame::Update(double _deltaTime) {
	if(!m_Running) return;
	
	if(m_LastEvent == Event::NONE) {
		if(glfwGetTime() - m_RunTime > 1.0) {
			Start();
		}
	} else if(m_LastEvent == Event::START) {
		
	} else if(m_LastEvent == Event::LOST) {
		if(glfwGetTime() - m_LastLostTime > 1.0) {
			Reset();
		}
	} else if(m_LastEvent == Event::RESET) {
		if(glfwGetTime() - m_LastResetTime > 1.0) {
			Start();
		}
	}
	
	UpdatePaddles(_deltaTime);
	UpdateBall(_deltaTime);
	
	m_LeftPaddle->UpdateMesh();
	m_RightPaddle->UpdateMesh();
	m_Ball->UpdateMesh();
	
	m_LeftPaddle->Update();
	m_RightPaddle->Update();
	m_Ball->Update();
}

void MainGame::Render() {
	if(!m_Running) return;
	
	m_LeftPaddle->Render();
	m_RightPaddle->Render();
	m_Ball->Render();
}

float MainGame::LeftPaddleVerticalOverlap() {
	return (m_LeftPaddle->transform.translation.x + m_LeftPaddle->width) - (m_Ball->transform.translation.x);
}
  
float MainGame::RightPaddleVerticalOverlap() {
	return (m_Ball->transform.translation.x + m_Ball->width) - (m_RightPaddle->transform.translation.x);
}
  
float MainGame::LeftPaddleHorizontalOverlap() {
	return (m_LeftPaddle->transform.translation.y) - (m_Ball->transform.translation.y - m_Ball->height);
}
  
float MainGame::RightPaddleHorizontalOverlap() {
	return (m_RightPaddle->transform.translation.y) - (m_Ball->transform.translation.y - m_Ball->height);
}

void MainGame::UpdatePaddles(double _deltaTime) {
	float ballSpeedX = std::abs(m_Ball->velocity.x);
	m_LeftPaddle->speed = (2.0f - m_LeftPaddle->height) / (2.0f - (m_LeftPaddle->width + m_RightPaddle->width + m_Ball->width)) * ballSpeedX * 2.0f;
	m_RightPaddle->speed = (2.0f - m_RightPaddle->height) / (2.0f - (m_LeftPaddle->width + m_RightPaddle->width + m_Ball->width)) * ballSpeedX * 2.0f;
	
	if(m_ControlState == ControlState::KEYBOARD) {
		if(Window::GetKey(GLFW_KEY_W) || Window::GetKey(GLFW_KEY_UP)) {
			m_LeftPaddle->transform.Translate(glm::vec3(0.0f, m_LeftPaddle->speed * _deltaTime, 0.0f));
		}
		if(Window::GetKey(GLFW_KEY_S) || Window::GetKey(GLFW_KEY_DOWN)) {
			m_LeftPaddle->transform.Translate(glm::vec3(0.0f, -m_LeftPaddle->speed * _deltaTime, 0.0f));
		}
	} else if(m_ControlState == ControlState::MOUSE) {
		double mouseYScreen = 1.0 - Window::GetCursorY() / m_Window->GetHeight() * 2.0;
		if(m_LeftPaddle->transform.translation.y < mouseYScreen + m_LeftPaddle->height / 2.0f) {
			if(m_LeftPaddle->transform.translation.y + m_LeftPaddle->speed * _deltaTime > mouseYScreen + m_LeftPaddle->height / 2.0f) {
				m_LeftPaddle->transform.translation.y = mouseYScreen + m_LeftPaddle->height / 2.0f;
			} else {
				m_LeftPaddle->transform.translation.y += m_LeftPaddle->speed * _deltaTime;
			}
		} else if(m_LeftPaddle->transform.translation.y > mouseYScreen + m_LeftPaddle->height / 2.0f) {
			if(m_LeftPaddle->transform.translation.y - m_LeftPaddle->speed * _deltaTime < mouseYScreen + m_LeftPaddle->height / 2.0f) {
				m_LeftPaddle->transform.translation.y = mouseYScreen + m_LeftPaddle->height / 2.0f;
			} else {
				m_LeftPaddle->transform.translation.y -= m_LeftPaddle->speed * _deltaTime;
			}
		}
	}
	
	if(m_LeftPaddle->transform.translation.y > 1.0f) {
		m_LeftPaddle->transform.translation.y = 1.0f;
	} else if(m_LeftPaddle->transform.translation.y - m_LeftPaddle->height < -1.0f) {
		m_LeftPaddle->transform.translation.y = -1.0f + m_LeftPaddle->height;
	}
	
	m_RightPaddle->transform.translation.y = m_LeftPaddle->transform.translation.y;
}

void MainGame::UpdateBall(double _deltaTime) {
	m_Ball->transform.translation += m_Ball->velocity * (float)_deltaTime;
	
	float leftPaddleVerticalOverlap = LeftPaddleVerticalOverlap();
	float rightPaddleVerticalOverlap = RightPaddleVerticalOverlap();
	float leftPaddleHorizontalOverlap = LeftPaddleHorizontalOverlap();
	float rightPaddleHorizontalOverlap = RightPaddleHorizontalOverlap();
	
	if(m_Ball->transform.translation.y > 1.0f) {
		m_Ball->transform.translation.y = 2.0 - m_Ball->transform.translation.y;
		m_Ball->velocity.y *= -1.0f;
	} else if(m_Ball->transform.translation.y - m_Ball->height < -1.0f) {
		m_Ball->transform.translation.y = -2.0f - m_Ball->transform.translation.y + 2.0f * m_Ball->height;
		m_Ball->velocity.y *= -1.0f;
	}
	
	if(m_LastEvent != Event::LOST) {
		if(leftPaddleVerticalOverlap > 0.0f && m_Ball->lastHorizontalCollision != Ball::HorizontalCollision::LEFT) {
			if(leftPaddleHorizontalOverlap > 0.0f && leftPaddleHorizontalOverlap < m_LeftPaddle->height + m_Ball->height) {
				//m_Ball->transform.translation.x = m_LeftPaddle->transform.translation.x + m_LeftPaddle->width + leftPaddleVerticalOverlap;
				m_Ball->lastHorizontalCollision = Ball::HorizontalCollision::LEFT;
				
				glm::vec3 vel = m_Ball->velocity;
				
				vel.x *= -1.0f;
				//int signX = (vel.x > 0.0f) - (vel.x < 0.0f);
				vel.x *= 1.005f;
				
				vel.y = ((m_LeftPaddle->height + m_Ball->height) / 2.0f - leftPaddleHorizontalOverlap);
				int signY = (vel.y > 0.0f) - (vel.y < 0.0f);
				vel.y = std::abs(std::powf(std::abs(vel.y) * 2.0f, 1.5f)) * 5.0f;
				vel.y *= signY;
				
				m_Ball->velocity = vel;
			} else {
				Lost(Side::LEFT);
			}
		}else if(rightPaddleVerticalOverlap > 0.0f && m_Ball->lastHorizontalCollision != Ball::HorizontalCollision::RIGHT) {
			if(rightPaddleHorizontalOverlap > 0.0f && rightPaddleHorizontalOverlap < m_RightPaddle->height + m_Ball->height) {
				//m_Ball->transform.translation.x = m_RightPaddle->transform.translation.x - rightPaddleVerticalOverlap - m_Ball->width;
				m_Ball->lastHorizontalCollision = Ball::HorizontalCollision::RIGHT;
				
				glm::vec3 vel = m_Ball->velocity;
				
				vel.x *= -1.02f;
				vel.y = ((m_LeftPaddle->height + m_Ball->height) / 2.0f - rightPaddleHorizontalOverlap);
				int sign = (vel.y > 0.0f) - (vel.y < 0.0f);
				vel.y = std::abs(std::powf(std::abs(vel.y) * 2.0f, 1.5f)) * 5.0f;
				vel.y *= sign;
				
				m_Ball->velocity = vel;
			} else {
				Lost(Side::RIGHT);
			}
		}
	}
}

void MainGame::Start() {
	m_Ball->velocity.x = 2.0f;
	m_Ball->velocity.y = 0.0f;
	
	m_LastEvent = Event::START;
	m_LastStartTime = glfwGetTime();
}

void MainGame::Lost(Side _side) {
	if(_side == Side::LEFT) {
		m_LeftPaddle->color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	} else if(_side == Side::RIGHT) {
		m_RightPaddle->color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	
	m_LeftPaddle->speed = 0.0f;
	m_RightPaddle->speed = 0.0f;
	
	m_LastEvent = Event::LOST;
	m_LastLostTime = glfwGetTime();
}

void MainGame::Reset() {
	m_LeftPaddle->speed = 0.0f;
	m_LeftPaddle->color = glm::vec4(1.0f);
	m_LeftPaddle->transform.translation.y = m_LeftPaddle->height / 2.0f;
	m_RightPaddle->speed = 0.0f;
	m_RightPaddle->transform.translation.y = m_RightPaddle->height / 2.0f;
	m_RightPaddle->color = glm::vec4(1.0f);
	m_Ball->velocity.x = 0.0f;
	m_Ball->velocity.y = 0.0f;
	m_Ball->transform.translation.x = -m_Ball->width / 2.0f;
	m_Ball->transform.translation.y = m_Ball->height / 2.0f;
	m_Ball->lastVerticalCollision = Ball::VerticalCollision::NONE;
	m_Ball->lastHorizontalCollision = Ball::HorizontalCollision::NONE;
	
	m_LastEvent = Event::RESET;
	m_LastResetTime = glfwGetTime();
}

void MainGame::KeyCallback(int _key, int _scancode, int _action, int _mods, void* _instance) {
	//MainGame* instance = (MainGame*)_instance;
	
	/*if(_key == GLFW_KEY_SPACE && _action == GLFW_PRESS) {
		if(instance->m_ControlState == ControlState::KEYBOARD) {
			instance->m_ControlState = ControlState::MOUSE;
		} else if(instance->m_ControlState == ControlState::MOUSE) {
			instance->m_ControlState = ControlState::KEYBOARD;
		} else {
			instance->m_ControlState = ControlState::KEYBOARD;
		}
	}*/
}