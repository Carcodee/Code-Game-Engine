#include "Renderer.h"

Renderer::Renderer()
{

}

void Renderer::RenderElements(ShaderClass& shader, std::vector<int> indices, VAO vao)
{
	shader.use();
	vao.Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::RenderArrays(ShaderClass& shader, std::vector<float> vertices, VAO vao)
{
	shader.use();
	vao.Bind();
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void Renderer::RenderInstancedArrays(ShaderClass& shader, std::vector<float> vertices, VAO vao, unsigned int amount)
{
	shader.use();
	vao.Bind();
	glDrawArraysInstanced(GL_TRIANGLES, 0, vertices.size(), amount);
}

void Renderer::RenderModel(ShaderClass& shader, Model model)
{
	model.Draw(shader);
}

Renderer::~Renderer()
{
	delete(this);
}
