#include "Laborator4.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"

using namespace std;

Laborator4::Laborator4()
{
}

Laborator4::~Laborator4()
{
}

void Laborator4::Init()
{
	polygonMode = GL_FILL;

	Mesh* mesh = new Mesh("box");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh->GetMeshID()] = mesh;
	okTranslate = true, 
	okTranslateBonus = true;
	// initialize tx, ty and tz (the translation steps)
	translateX = 0;
	translateY = 0;
	translateZ = 0;

	translateXBonus = 0;
	translateYBonus = 0;
	translateZBonus = 0;

	// initialize sx, sy and sz (the scale factors)
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;
	
	// initialize angularSteps
	angularStepOX = 0;
	angularStepOY = 0;
	angularStepOZ = 0;

	angularStepOYBonus = 0;
	angularStepOZBonus = 0;
}

void Laborator4::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator4::Update(float deltaTimeSeconds)
{
	
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	//translatie cub 1
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-2.5f, 0.5f, -1.5f);

	if (okTranslate) {
		translateX += 1.f * deltaTimeSeconds;
		modelMatrix *= Transform3D::Translate(translateX, translateY, translateZ);
		if (translateX > 1) {
			okTranslate = false;
		}

	} 
	else {
		translateX -= 1.f * deltaTimeSeconds;
		modelMatrix *= Transform3D::Translate(translateX, translateY, translateZ);
		if (translateX <-2) {
			okTranslate = true;
		}
	}
	
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	//scalare cub 2
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, 0.5f, -1.5f);
	scaleX += 1 * deltaTimeSeconds;
	scaleY += 1 * deltaTimeSeconds;
	scaleZ += 1 * deltaTimeSeconds;
	modelMatrix *= Transform3D::Scale(cos(scaleX), cos(scaleY), cos(scaleZ));
	RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

	//rotatie OY cub 3
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(2.5f, 0.5f, -1.5f);
	angularStepOY += 2.0f * deltaTimeSeconds;
	modelMatrix *= Transform3D::RotateOY(angularStepOY);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

// incercare de bonus
	modelMatrixBonus = glm::mat4(1);
	modelMatrixBonus2 = glm::mat4(1);

	modelMatrixBonus *= Transform3D::Translate(0.0f, 3.0f, 0);
	modelMatrixBonus2 *= Transform3D::Translate(0, 3.0f, -2.5f);
	angularStepOYBonus += 1.0f * deltaTimeSeconds;
	modelMatrixBonus2 *= Transform3D::RotateOY(angularStepOYBonus);
	


	modelMatrixBonus *= Transform3D::Translate(0.0f, 3.0f, -2.5f);

	modelMatrixBonus2 *= Transform3D::Translate(0, 3.0f, -1);

	if (okTranslateBonus) {
		translateXBonus += 1.f * deltaTimeSeconds;
		modelMatrixBonus *= Transform3D::Translate(translateXBonus, 0, 0);
		modelMatrixBonus2 *= Transform3D::Translate(translateXBonus,0, translateXBonus);
		if (translateXBonus > 1) {
			okTranslateBonus = false;
		}
		
	}
	else {
		translateXBonus -= 1.f * deltaTimeSeconds;
		modelMatrixBonus *= Transform3D::Translate(translateXBonus, 0, 0);
		modelMatrixBonus2 *= Transform3D::Translate(-translateXBonus, 0, -translateXBonus);
		if (translateXBonus < -2) {
			okTranslateBonus = true;
		}
	}

	

	


	modelMatrixBonus2 *= Transform3D::Scale(0.2f, 0.2f, 0.2f);

	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrixBonus);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrixBonus2);
}

void Laborator4::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator4::OnInputUpdate(float deltaTime, int mods)
{
	
	
}

void Laborator4::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_SPACE)
	{
		switch (polygonMode)
		{
		case GL_POINT:
			polygonMode = GL_FILL;
			break;
		case GL_LINE:
			polygonMode = GL_POINT;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}
}

void Laborator4::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator4::OnWindowResize(int width, int height)
{
}
