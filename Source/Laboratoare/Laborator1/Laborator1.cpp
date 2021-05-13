#include "Laborator1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

Laborator1::Laborator1()
{
}

Laborator1::~Laborator1()
{
}

void Laborator1::Init()
{
	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		Mesh* sphereMesh = new Mesh("sphere");
		Mesh* teaMesh = new Mesh("tea");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		sphereMesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		teaMesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
		meshes[mesh->GetMeshID()] = mesh;
		meshes[sphereMesh->GetMeshID()] = sphereMesh;
		meshes[teaMesh->GetMeshID()] = teaMesh;
	}
}

void Laborator1::FrameStart()
{

}
/* taste: 1,2 - culoare ecran; WASDQE - miscare sacadata; WASDQE+left_ctrl - miscare continua; f - schimbare obiect*/
void Laborator1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->props.resolution;

	//cand apas tasta 1 se schimba culoarea in rosu
	if (press1) {
		glClearColor(1, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	//cand apas tasta 2 se schimba inapoi culoarea in negru
	else if (press2) {
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	} 
	else {
		// sets the clear color for the color buffer
		glClearColor(0, 0, 0, 1);

		// clears the color buffer (using the previously set color) and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	// render the object
	RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

	// render the object again but with different properties
	RenderMesh(meshes["box"], glm::vec3(x, y, z));
	
	//apas pe f si se cicleaza al treilea obiect, prin 3 variante
	if (pressf) {
		if (object == 0) {
			RenderMesh(meshes["sphere"], glm::vec3(2, 0.5f, 0));
			
		}
		else if (object == 1) {
			RenderMesh(meshes["box"], glm::vec3(2, 0.5f, 0), glm::vec3(0.5f));
			
		}
		else if (object == 2) {
			RenderMesh(meshes["tea"], glm::vec3(2, 0.5f, 0));
		}
	} else RenderMesh(meshes["sphere"], glm::vec3(2, 0.5f, 0));
	

}

void Laborator1::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator1::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input

	//cand este apasat ctrl obiectul se misca cat timp apasam o tasta
		if (window->KeyHold(GLFW_KEY_LEFT_CONTROL)) {

			if (window->KeyHold(GLFW_KEY_D)) {
				x += 0.1f;
			}
			if (window->KeyHold(GLFW_KEY_A)) {
				x -= 0.1f;
			}
			if (window->KeyHold(GLFW_KEY_Q)) {
				y += 0.1f;
			}
			if (window->KeyHold(GLFW_KEY_E)) {
				y -= 0.1f;
			}
			if (window->KeyHold(GLFW_KEY_S)) {
				z += 0.1f;
			}
			if (window->KeyHold(GLFW_KEY_W)) {
				z -= 0.1f;
			}
		}
		
	
};

void Laborator1::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_1) {
		press1 = true;
		press2 = false;
	}
	else if (key == GLFW_KEY_2) {
		press1 = false;
		press2 = true;
	}
	//la fiecare apasare pe f se schimba indexul obiectului afisat
	else if (key == GLFW_KEY_F) {
		pressf = true;
		object = (object + 1) % 3;
	}
	//la apasarea unei taste obiectul se misca o unitate
	else if (key == GLFW_KEY_D) {
		x += 0.1f;
	}
	else if (key == GLFW_KEY_A) {
		x -= 0.1f;
	}
	else if (key == GLFW_KEY_Q) {
		y += 0.1f;
	}
	else if (key == GLFW_KEY_E) {
		y -= 0.1f;
	}
	else if (key == GLFW_KEY_S) {
		z += 0.1f;
	}
	else if (key == GLFW_KEY_W) {
		z -= 0.1f;
	}
	
};

void Laborator1::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Laborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Laborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Laborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator1::OnWindowResize(int width, int height)
{
	// treat window resize event
}
