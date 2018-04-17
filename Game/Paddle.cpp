#include "Paddle.h"

Paddle::Paddle() { }
Paddle::Paddle(float _width, float _height, glm::vec4 _color) : Quad(_width, _height, _color) { }
Paddle::~Paddle() { }