#include "MeshRenderer.h"

MeshRenderer::MeshRenderer() { }
MeshRenderer::~MeshRenderer() { }

void MeshRenderer::Update(Transform _transform) { }

void MeshRenderer::Render(Transform _transform) {
	if(shader) {
		shader->SetUniformMat4fv("u_Model", &_transform.GetTransformation()[0][0]);
		shader->Use();
	}
	
	mesh.Draw();
}