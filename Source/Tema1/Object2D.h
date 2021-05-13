#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

	Mesh* CreateArrow(std::string name, glm::vec3 leftBottomCorner, glm::vec3 triangleCorner,float width, glm::vec3 color);

	Mesh* CreateBow(std::string name, glm::vec3 top, float radius, glm::vec3 color);

	Mesh* CreateBalloon(std::string name, glm::vec3 top, float radiusX, float radiusY, glm::vec3 color);
	Mesh* CreateBalloonString(std::string name, glm::vec3 top, float length, glm::vec3 color);
	Mesh* createShuriken(std::string name, glm::vec3 center, float length,float width, glm::vec3 color);

}

