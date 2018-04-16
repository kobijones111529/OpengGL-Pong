#include "Game.h"

Game::Game() { }
Game::~Game() { }

void Game::KeyCallback(int _key, int _scancode, int _action, int _mods, void* _instance) {
	Game* instance = (Game*)_instance;
	
	if(_key == GLFW_KEY_SPACE && _action == GLFW_PRESS) {
		instance->m_ControlState = (ControlState)(((int)instance->m_ControlState + 1) % instance->m_ControlStates);
	}
}

float Game::LeftPaddleVerticalOverlap() {
	return (leftPaddle->transform.translation.x + leftPaddle->width) - (ball->transform.translation.x);
}

float Game::RightPaddleVerticalOverlap() {
	return (ball->transform.translation.x + ball->width) - (rightPaddle->transform.translation.x);
}

float Game::LeftPaddleHorizontalOverlap() {
	return (leftPaddle->transform.translation.y) - (ball->transform.translation.y - ball->height);
}

float Game::RightPaddleHorizontalOverlap() {
	return (rightPaddle->transform.translation.y) - (ball->transform.translation.y - ball->height);
}

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
	
	return true;
}

void Game::Destroy() {
	glfwTerminate();
}

void Game::Run() {
	srand((unsigned int)time(NULL));
	
	Shader* shader = new Shader;
	shader->Init("BasicVertex.glsl", "BasicFragment.glsl");
	
	leftPaddle = new Paddle;
	leftPaddle->speed = 2.0f;
	leftPaddle->width = 0.05f;
	leftPaddle->height = 0.5f;
	leftPaddle->transform.translation.x = -1.0f;
	leftPaddle->meshRenderer = new MeshRenderer;
	leftPaddle->meshRenderer->shader = shader;
	leftPaddle->meshRenderer->mesh = new Mesh;
	leftPaddle->UpdateMesh();
	
	rightPaddle = new Paddle;
	rightPaddle->speed = 2.0f;
	rightPaddle->width = 0.05f;
	rightPaddle->height = 0.5f;
	rightPaddle->transform.translation.x = 0.95f;
	rightPaddle->meshRenderer = new MeshRenderer;
	rightPaddle->meshRenderer->shader = shader;
	rightPaddle->meshRenderer->mesh = new Mesh;
	rightPaddle->UpdateMesh();
	
	ball = new Ball;
	ball->width = 0.05f;
	ball->height = 0.05f;
	ball->meshRenderer = new MeshRenderer;
	ball->meshRenderer->shader = shader;
	ball->meshRenderer->mesh = new Mesh;
	ball->UpdateMesh();
	
	m_CurrentFrame = glfwGetTime();
	m_LastFrame = m_CurrentFrame;
	m_LastFrameLog = m_CurrentFrame;
	
	Reset();
	
	while(!m_Window->IsClosed()) {
		Update();
		Render();
		m_Window->Update();
	}
	
	delete leftPaddle->meshRenderer->mesh;
	delete leftPaddle->meshRenderer;
	delete leftPaddle;
	
	delete rightPaddle->meshRenderer->mesh;
	delete rightPaddle->meshRenderer;
	delete rightPaddle;
	
	delete ball->meshRenderer->mesh;
	delete ball->meshRenderer;
	delete ball;
	
	delete shader;
}

void Game::Update() {
	m_CurrentFrame = glfwGetTime();
	m_DeltaTime = m_CurrentFrame - m_LastFrame;
	m_LastFrame = m_CurrentFrame;
	
	if(m_CurrentFrame - m_LastFrameLog > 1.0) {
		std::cout << 1.0 / m_DeltaTime << std::endl;
		m_LastFrameLog = m_CurrentFrame;
	}
	
	if(m_LastEvent == Event::RESET) {
		if(glfwGetTime() - m_GameResetTime > 1.0) {
			Start();
		}
	}
	
	if(m_LastEvent == Event::LOST) {
		if(glfwGetTime() - m_GameLostTime > 1.0f) {
			Reset();
		} else {
			UpdateBall();
		}
	}
	
	if(m_LastEvent == Event::START) {
		UpdatePaddles();
		UpdateBall();
	}
	
	leftPaddle->UpdateMesh();
	rightPaddle->UpdateMesh();
	ball->UpdateMesh();
	
	leftPaddle->Update();
	rightPaddle->Update();
	ball->Update();
}

void Game::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	leftPaddle->Render();
	rightPaddle->Render();
	ball->Render();
}

void Game::UpdatePaddles() {
	leftPaddle->speed = std::abs(ball->velocity.x) * 1.5f;
	rightPaddle->speed = std::abs(ball->velocity.x) * 1.5f;
	
	if(m_ControlState == ControlState::KEYBOARD) {
		if(Window::GetKey(GLFW_KEY_W) || Window::GetKey(GLFW_KEY_UP)) {
			leftPaddle->transform.Translate(glm::vec3(0.0f, leftPaddle->speed * m_DeltaTime, 0.0f));
		}
		if(Window::GetKey(GLFW_KEY_S) || Window::GetKey(GLFW_KEY_DOWN)) {
			leftPaddle->transform.Translate(glm::vec3(0.0f, -leftPaddle->speed * m_DeltaTime, 0.0f));
		}
	} else if(m_ControlState == ControlState::MOUSE) {
		double mouseYScreen = 1.0 - Window::GetCursorY() / m_Window->GetHeight() * 2.0;
		if(leftPaddle->transform.translation.y < mouseYScreen + leftPaddle->height / 2.0f) {
			if(leftPaddle->transform.translation.y + leftPaddle->speed * m_DeltaTime > mouseYScreen + leftPaddle->height / 2.0f) {
				leftPaddle->transform.translation.y = mouseYScreen + leftPaddle->height / 2.0f;
			} else {
				leftPaddle->transform.translation.y += leftPaddle->speed * m_DeltaTime;
			}
		} else if(leftPaddle->transform.translation.y > mouseYScreen + leftPaddle->height / 2.0f) {
			if(leftPaddle->transform.translation.y - leftPaddle->speed * m_DeltaTime < mouseYScreen + leftPaddle->height / 2.0f) {
				leftPaddle->transform.translation.y = mouseYScreen + leftPaddle->height / 2.0f;
			} else {
				leftPaddle->transform.translation.y -= leftPaddle->speed * m_DeltaTime;
			}
		}
	}
	
	if(leftPaddle->transform.translation.y > 1.0f) {
		leftPaddle->transform.translation.y = 1.0f;
	} else if(leftPaddle->transform.translation.y - leftPaddle->height < -1.0f) {
		leftPaddle->transform.translation.y = -1.0f + leftPaddle->height;
	}
	
	rightPaddle->transform.translation.y = leftPaddle->transform.translation.y;
}

void Game::UpdateBall() {
	ball->transform.translation += ball->velocity * (float)m_DeltaTime;
	
	float leftPaddleVerticalOverlap = LeftPaddleVerticalOverlap();
	float rightPaddleVerticalOverlap = RightPaddleVerticalOverlap();
	float leftPaddleHorizontalOverlap = LeftPaddleHorizontalOverlap();
	float rightPaddleHorizontalOverlap = RightPaddleHorizontalOverlap();
	
	if(ball->transform.translation.y > 1.0f) {
		ball->transform.translation.y = 2.0 - ball->transform.translation.y;
		ball->velocity.y *= -1.0f;
	} else if(ball->transform.translation.y - ball->height < -1.0f) {
		ball->transform.translation.y = -2.0f - ball->transform.translation.y + 2.0f * ball->height;
		ball->velocity.y *= -1.0f;
	}
	
	if(m_LastEvent != Event::LOST) {
		if(leftPaddleVerticalOverlap > 0.0f && ball->lastHorizontalCollision != Ball::HorizontalCollision::LEFT) {
			if(leftPaddleHorizontalOverlap > 0.0f && leftPaddleHorizontalOverlap < leftPaddle->height + ball->height) {
				ball->transform.translation.x = leftPaddle->transform.translation.x + leftPaddle->width + leftPaddleVerticalOverlap;
				ball->lastHorizontalCollision = Ball::HorizontalCollision::LEFT;
				
				glm::vec3 vel = ball->velocity;
				
				vel.x *= -1.0f;
				int signX = (vel.x > 0.0f) - (vel.x < 0.0f);
				vel.x += 0.02f * signX;
				
				vel.y = ((leftPaddle->height + ball->height) / 2.0f - leftPaddleHorizontalOverlap);
				int signY = (vel.y > 0.0f) - (vel.y < 0.0f);
				vel.y = std::abs(std::powf(std::abs(vel.y), 1.0f)) * 5.0f;
				vel.y *= signY;
				
				ball->velocity = vel;
			} else {
				Lost(Side::LEFT);
			}
		}else if(rightPaddleVerticalOverlap > 0.0f && ball->lastHorizontalCollision != Ball::HorizontalCollision::RIGHT) {
			if(rightPaddleHorizontalOverlap > 0.0f && rightPaddleHorizontalOverlap < rightPaddle->height + ball->height) {
				ball->transform.translation.x = rightPaddle->transform.translation.x - rightPaddleVerticalOverlap - ball->width;
				ball->lastHorizontalCollision = Ball::HorizontalCollision::RIGHT;
				
				glm::vec3 vel = ball->velocity;
				
				vel.x *= -1.02f;
				vel.y = ((leftPaddle->height + ball->height) / 2.0f - rightPaddleHorizontalOverlap);
				int sign = (vel.y > 0.0f) - (vel.y < 0.0f);
				vel.y = std::abs(std::powf(std::abs(vel.y), 1.0f)) * 5.0f;
				vel.y *= sign;
				
				ball->velocity = vel;
			} else {
				Lost(Side::RIGHT);
			}
		}
	}
}

void Game::Reset() {
	leftPaddle->transform.translation.y = leftPaddle->height / 2.0f;
	rightPaddle->transform.translation.y = rightPaddle->height / 2.0f;
	ball->transform.translation.x = ball->width / 2.0f;
	ball->transform.translation.y = ball->height / 2.0f;
	ball->velocity = glm::vec3(0.0f);
	ball->lastVerticalCollision = Ball::VerticalCollision::NONE;
	ball->lastHorizontalCollision = Ball::HorizontalCollision::NONE;
	
	leftPaddle->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	rightPaddle->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	ball->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	m_LastEvent = Event::RESET;
	m_GameResetTime = glfwGetTime();
}

void Game::Start() {
	ball->velocity.x = 1.0f;
	ball->velocity.y = (float)rand() / INT_MAX - 0.5f;
	
	m_LastEvent = Event::START;
	m_GameStartTime = glfwGetTime();
}

void Game::Lost(Side _side) {
	if(_side == Side::LEFT) {
		leftPaddle->color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	} else if(_side == Side::RIGHT) {
		rightPaddle->color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	
	m_LastEvent = Event::LOST;
	m_GameLostTime = glfwGetTime();
}