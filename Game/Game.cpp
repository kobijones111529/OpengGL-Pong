#include "Game.h"

Game::Game() { }
Game::~Game() { }

float Game::LeftPaddleVerticalOverlap() {
	return (leftPaddle->transform->translation.x + leftPaddle->width) - (ball->transform->translation.x);
}

float Game::RightPaddleVerticalOverlap() {
	return (ball->transform->translation.x + ball->width) - (rightPaddle->transform->translation.x);
}

float Game::LeftPaddleHorizontalOverlap() {
	return (leftPaddle->transform->translation.y) - (ball->transform->translation.y - ball->height);
}

float Game::RightPaddleHorizontalOverlap() {
	return (rightPaddle->transform->translation.y) - (ball->transform->translation.y - ball->height);
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
	
	if(!m_Window.Init(1000, 1000, "OpenGL")) {
		glfwTerminate();
		return false;
	}
	
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
	leftPaddle->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	leftPaddle->transform = new Transform;
	leftPaddle->transform->translation = glm::vec3(-1.0f, 0.0f, 0.0f);
	leftPaddle->meshRenderer = new MeshRenderer;
	leftPaddle->meshRenderer->shader = shader;
	leftPaddle->meshRenderer->mesh = new Mesh;
	leftPaddle->UpdateMesh();
	
	rightPaddle = new Paddle;
	rightPaddle->speed = 2.0f;
	rightPaddle->width = 0.05f;
	rightPaddle->height = 0.5f;
	rightPaddle->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	rightPaddle->transform = new Transform;
	rightPaddle->transform->translation = glm::vec3(0.95f, 0.0f, 0.0f);
	rightPaddle->meshRenderer = new MeshRenderer;
	rightPaddle->meshRenderer->shader = shader;
	rightPaddle->meshRenderer->mesh = new Mesh;
	rightPaddle->UpdateMesh();
	
	ball = new Ball;
	ball->velocity = glm::vec3(-0.5f, 0.0f, 0.0f);
	ball->width = 0.05f;
	ball->height = 0.05f;
	ball->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	ball->transform = new Transform;
	ball->transform->translation = glm::vec3(0.0f, 0.0f, 0.0f);
	ball->meshRenderer = new MeshRenderer;
	ball->meshRenderer->shader = shader;
	ball->meshRenderer->mesh = new Mesh;
	ball->UpdateMesh();
	
	m_CurrentFrame = glfwGetTime();
	m_LastFrame = m_CurrentFrame;
	m_LastFrameLog = m_CurrentFrame;
	
	while(!m_Window.IsClosed()) {
		Update();
		Render();
		m_Window.Update();
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
	
	if(!ballMissedLeft && !ballMissedRight) {
		if(m_Window.GetControlState() == Window::ControlState::KEYBOARD) {
			if(Window::GetKey(GLFW_KEY_W) || Window::GetKey(GLFW_KEY_UP)) {
				leftPaddle->transform->Translate(glm::vec3(0.0f, leftPaddle->speed * m_DeltaTime, 0.0f));
			}
			if(Window::GetKey(GLFW_KEY_S) || Window::GetKey(GLFW_KEY_DOWN)) {
				leftPaddle->transform->Translate(glm::vec3(0.0f, -leftPaddle->speed * m_DeltaTime, 0.0f));
			}
		} else if(m_Window.GetControlState() == Window::ControlState::MOUSE) {
			double mouseYScreen = 1.0 - Window::GetCursorY() / m_Window.GetHeight() * 2.0;
			if(leftPaddle->transform->translation.y < mouseYScreen + leftPaddle->height / 2.0f) {
				if(leftPaddle->transform->translation.y + leftPaddle->speed * m_DeltaTime > mouseYScreen + leftPaddle->height / 2.0f) {
					leftPaddle->transform->translation.y = mouseYScreen + leftPaddle->height / 2.0f;
				} else {
					leftPaddle->transform->translation.y += leftPaddle->speed * m_DeltaTime;
				}
			} else if(leftPaddle->transform->translation.y > mouseYScreen + leftPaddle->height / 2.0f) {
				if(leftPaddle->transform->translation.y - leftPaddle->speed * m_DeltaTime < mouseYScreen + leftPaddle->height / 2.0f) {
					leftPaddle->transform->translation.y = mouseYScreen + leftPaddle->height / 2.0f;
				} else {
					leftPaddle->transform->translation.y -= leftPaddle->speed * m_DeltaTime;
				}
			}
		}
	}
	
	if(leftPaddle->transform->translation.y > 1.0f) {
		leftPaddle->transform->translation.y = 1.0f;
	} else if(leftPaddle->transform->translation.y - leftPaddle->height < -1.0f) {
		leftPaddle->transform->translation.y = -1.0f + leftPaddle->height;
	}
	
	rightPaddle->transform->translation.y = leftPaddle->transform->translation.y;
	
	ball->transform->translation += ball->velocity * (float)m_DeltaTime;
	
	float leftPaddleVerticalOverlap = LeftPaddleVerticalOverlap();
	float rightPaddleVerticalOverlap = RightPaddleVerticalOverlap();
	float leftPaddleHorizontalOverlap = LeftPaddleHorizontalOverlap();
	float rightPaddleHorizontalOverlap = RightPaddleHorizontalOverlap();
	
	if(leftPaddleVerticalOverlap > 0 && ball->lastHorizontalCollision != Ball::HorizontalCollision::LEFT) {
		if(leftPaddleHorizontalOverlap > 0 && leftPaddleHorizontalOverlap < leftPaddle->height + ball->height) {
			ball->transform->translation.x = leftPaddle->transform->translation.x + leftPaddle->width + leftPaddleVerticalOverlap;
			ball->lastHorizontalCollision = Ball::HorizontalCollision::LEFT;
			ball->velocity.x *= -1.0f;
		} else {
			ballMissedLeft = true;
			leftPaddle->color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}
	
	if(rightPaddleVerticalOverlap > 0 && ball->lastHorizontalCollision != Ball::HorizontalCollision::RIGHT) {
		ball->transform->translation.x = rightPaddle->transform->translation.x - rightPaddleVerticalOverlap - ball->width;
		ball->lastHorizontalCollision = Ball::HorizontalCollision::RIGHT;
		ball->velocity.x *= -1.0f;
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