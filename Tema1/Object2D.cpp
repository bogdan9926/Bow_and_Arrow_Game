#include "Object2D.h"

#include <Core/Engine.h>

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}
Mesh* Object2D::CreateArrow(std::string name, glm::vec3 leftBottomCorner, glm::vec3 triangleCorner, float width, glm::vec3 color) {
	glm::vec3 corner = leftBottomCorner;
	glm::vec3 cornerTR = triangleCorner;
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),

		//VertexFormat(corner + glm::vec3(length, 0, 0), color),
		//VertexFormat(corner + glm::vec3(length, width, 0), color),

		VertexFormat(cornerTR, color),
		VertexFormat(cornerTR + glm::vec3(0, -width, 0), color),

		VertexFormat(cornerTR + glm::vec3(0, width, 0), color),
		VertexFormat(cornerTR + glm::vec3(2 * width,0, 0), color),
		VertexFormat(cornerTR + glm::vec3(0, -width, 0), color),
		VertexFormat(cornerTR, color)
	};
	Mesh* arrow = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4, 5, 6 };
	arrow->SetDrawMode(GL_LINE_STRIP);

	arrow->InitFromData(vertices, indices);
	return arrow;
}

Mesh* Object2D::CreateBow(std::string name, glm::vec3 top, float radius, glm::vec3 color) {
	const float PI = 3.1415926f;

	float increment = PI / 30;
	std::vector<VertexFormat> vertices;
	vertices.push_back(VertexFormat(top, color));
	for (float currAngle = PI * 3 / 2; currAngle <= PI * 5 / 2; currAngle += increment)
	{
		vertices.push_back(VertexFormat(glm::vec3(radius * cos(currAngle), radius * sin(currAngle), 0), color));
	}
	vertices.push_back(VertexFormat(top, color));
	Mesh* bow = new Mesh(name);
	std::vector<unsigned short> indices;
	for (int i = 0; i <= 31;i++) {
		indices.push_back(i);
	}
	bow->SetDrawMode(GL_LINE_LOOP);
	bow->InitFromData(vertices, indices);
	return bow;

}

Mesh* Object2D::CreateBalloon(std::string name, glm::vec3 top, float radiusX, float radiusY, glm::vec3 color) {

	const float PI = 3.1415926f;
	float increment = PI / 30;
	std::vector<VertexFormat> vertices;
	//vertices.push_back(VertexFormat(glm::vec3(0,radiusY,0), color));

	for (float currAngle = PI * 3 / 2; currAngle <= PI * 7 / 2; currAngle += increment)
	{
		vertices.push_back(VertexFormat(glm::vec3(radiusX * cos(currAngle), radiusY * sin(currAngle) + radiusY, 0), color));
	}
	vertices.push_back(VertexFormat(top, color));

	Mesh* balloon = new Mesh(name);
	std::vector<unsigned short> indices;
	for (int i = 0; i <= 60;i++) {
		indices.push_back(i);
	}


	vertices.push_back(VertexFormat(glm::vec3(-radiusX / 4, -radiusY / 4, 0), color));
	vertices.push_back(VertexFormat(glm::vec3(radiusX / 4, -radiusY / 4, 0), color));



	for (int i = 61; i <= 70;i++) {
		indices.push_back(i);
	}
	balloon->SetDrawMode(GL_TRIANGLE_FAN);
	balloon->InitFromData(vertices, indices);
	return balloon;

}
Mesh* Object2D::CreateBalloonString(std::string name, glm::vec3 top, float length, glm::vec3 color) {
	std::vector<VertexFormat> vertices;
	vertices.push_back(VertexFormat(top, color));
	vertices.push_back(VertexFormat(glm::vec3(-length / 3, -length / 4, 0), color));
	vertices.push_back(VertexFormat(glm::vec3(length / 3, -length / 4, 0), color));
	vertices.push_back(VertexFormat(glm::vec3(-length / 3, -length / 2, 0), color));

	vertices.push_back(VertexFormat(glm::vec3(length / 3, -length / 4, 0), color));
	vertices.push_back(VertexFormat(glm::vec3(-length / 3, -length / 4, 0), color));
	vertices.push_back(VertexFormat(top, color));

	std::vector<unsigned short> indices;
	for (int i = 0; i <= 10;i++) {
		indices.push_back(i);


	}
	Mesh* string = new Mesh(name);
	string->SetDrawMode(GL_LINE_LOOP);
	string->InitFromData(vertices, indices);
	return string;
}

Mesh* Object2D::createShuriken(std::string name, glm::vec3 center, float length, float width, glm::vec3 color) {

	std::vector<VertexFormat> vertices;
	vertices.push_back(VertexFormat(center, color));
	vertices.push_back(VertexFormat(center + glm::vec3(-length, length, 0), color));
	vertices.push_back(VertexFormat(center + glm::vec3(0, width, 0), color));

	vertices.push_back(VertexFormat(center, color));
	vertices.push_back(VertexFormat(center + glm::vec3(length, length, 0), color));
	vertices.push_back(VertexFormat(center + glm::vec3(width, 0, 0), color));

	vertices.push_back(VertexFormat(center, color));
	vertices.push_back(VertexFormat(center + glm::vec3(length, -length, 0), color));
	vertices.push_back(VertexFormat(center + glm::vec3(0, -width, 0), color));

	vertices.push_back(VertexFormat(center, color));
	vertices.push_back(VertexFormat(center + glm::vec3(-length, -length, 0), color));
	vertices.push_back(VertexFormat(center + glm::vec3(-width, 0, 0), color));
	vertices.push_back(VertexFormat(center, color));

	std::vector<unsigned short> indices;
	for (int i = 0; i <= 14;i++) {
		indices.push_back(i);

	}
	Mesh* shuriken = new Mesh(name);
	shuriken->SetDrawMode(GL_TRIANGLE_FAN);
	shuriken->InitFromData(vertices, indices);
	return shuriken;

}
