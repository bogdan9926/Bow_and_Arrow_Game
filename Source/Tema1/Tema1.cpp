#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	glm::vec3 cornerTR = glm::vec3(50, 0, 0);

	//auxiliare triunghi arc-oy-mouse
	aux_co = 0;
	aux_ca = 0;
	aux_ip = 0;

	//durata power
	duration = 0;
	aux_duration = 0;
	//scor
	score = 0;
	//nr de vieti
	lives_left = 3;

	//daca exista deja o sageata in scena
	arrowExists = false;

	//odata ce incep sa tin apasat
	startTimer = false;

	//daca sageata a pornit
	startArrow = false;

	translateYBow = 200;

	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;
	initial_translateX = 0;
	initial_translateY = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;

	// initialize angularStep
	angularStep = 0;

	okChange = true;
	float squareSide = 100;
	gravity = 5;
	arrowSpeed = 100;
	shurikenSpeed = 300;
	balloonUpSpeed = 100;
	balloonDownSpeed = 500;
	shurikenNumber = 4;
	balloonNumber = 9;
	//initializare vector de baloane
	for (int i = 1; i <= 9;i++) {
		balloonVector[i].isAdded = false;
		balloonVector[i].destroyed = false;
		//directia de cadere, spre stg/dr/in jos
	}
	//initializare vector de shurikenuri
	for (int i = 1; i <= 9;i++) {
		shurikenVector[i].isAdded = false;
	}

	Mesh* square1 = Object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
	AddMeshToList(square1);

	Mesh* square2 = Object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0), true);
	AddMeshToList(square2);

	Mesh* square3 = Object2D::CreateSquare("square3", corner, squareSide, glm::vec3(1, 0, 1), true);
	AddMeshToList(square3);

	Mesh* arrow = Object2D::CreateArrow("arrow", corner, cornerTR, 5, glm::vec3(1, 0, 0));
	AddMeshToList(arrow);

	Mesh* bow = Object2D::CreateBow("bow", corner, 50, glm::vec3(0.5f, 0.5f, 0.5f));
	AddMeshToList(bow);

	Mesh* balonR = Object2D::CreateBalloon("balloonR", corner, 20, 30, glm::vec3(1, 0, 0));
	AddMeshToList(balonR);

	Mesh* balonG = Object2D::CreateBalloon("balloonG", corner, 20, 30, glm::vec3(1, 1, 0));
	AddMeshToList(balonG);

	Mesh* balonV = Object2D::CreateBalloon("balloonV", corner, 20, 30, glm::vec3(0, 1, 0));
	AddMeshToList(balonV);

	Mesh* string = Object2D::CreateBalloonString("string", corner, 20, glm::vec3(1, 1, 1));
	AddMeshToList(string);

	Mesh* shuriken = Object2D::createShuriken("shuriken", corner, 20, 15, glm::vec3(0, 0, 0));
	AddMeshToList(shuriken);
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->GetResolution();

	//luam coordonatele globale ale mouse-ului
	GetCursorPos(&mouseCoordinates);

		//translatam coordonatele globale la cele locale, din fereastra
	ScreenToClient(GetActiveWindow(), &mouseCoordinates);
	
	//co = cateta opusa (OY), ca = alaturata (OX), ip = ipotenuza
	// co = rezolutie - distanta de sus pana la mouse - distanta de jos pana la arc
	co = resolution.y - mouseCoordinates.y - translateYBow;
	ca = mouseCoordinates.x - 50;
	ip = sqrt(co * co + ca * ca);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(50, translateYBow);
	//rotim arcul spre mouse
	modelMatrix *= Transform2D::Rotate(asin(co / ip));
	//afisam arcul
	RenderMesh2D(meshes["bow"], shaders["VertexColor"], modelMatrix);
	modelMatrixArrow = glm::mat3(1);

	//daca am dar drumul la clickul de mouse, calculam directia sagetii
	if (startArrow) {
		aux_co = co, aux_ca = ca, aux_ip = ip, aux_bow = translateYBow;
		aux = 0.1f;
		initial_translateX = 50;
		initial_translateY = translateYBow;
		arrowExists = true;
		startArrow = false;
	}
	//daca inca nu am tras, sageata se misca odata cu arcul
	if (!arrowExists) {
		modelMatrixArrow *= Transform2D::Translate(50, translateYBow);
		modelMatrixArrow *= Transform2D::Rotate(asin(co / ip));
	}
	//altfel, calculam traiectoria sagetii
	else {
		//aplicam gravitatie asupra sagetii
		CalculateGravity();
		modelMatrixArrow *= Transform2D::Translate(translateX, translateY);
		modelMatrixArrow *= Transform2D::Rotate(degree);
		//verificam daca sageata iese din cadru sau loveste baloane sau shurikenuri
		CheckCollisions(degree);
	}
	//afisam sageata
	RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrixArrow);

	ChangeSpeed();

	for (int i = 1; i <= balloonNumber;i++) {
		//daca un balon nu exista in scena, exista o sansa de 1 la 75
		//in fiecare cadru ca sa fie afisat
		if (!balloonVector[i].isAdded && rand() % 75 == 0) {
			AddBalloon(i);
		}
		else if (!balloonVector[i].isAdded)
			continue;
		//balonul urca pe verticala
		balloonVector[i].translateY += balloonUpSpeed * deltaTimeSeconds;

		//exista o sansa ca balonul sa isi schimbe sensul pe orizontala
		ChangeDirection();
		if (!balloonVector[i].destroyed)
			//balonul se misca pe orizontala
			balloonVector[i].translateX += balloonUpSpeed * deltaTimeSeconds * (balloonVector[i].riseDirection);

		//verificam daca balonul iese din cadru
		CheckOutOfBoundsBalloon(i);
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(balloonVector[i].translateX, balloonVector[i].translateY);

		//daca balonul a fost lovit, i se schimba sensul de cadere si proportiile
		CheckDestroyBalloon(i, deltaTimeSeconds);

		//afisam balonul
		PrintBalloon(i);
	}

	for (int i = 1; i <= shurikenNumber;i++) {
		//daca un shuriken nu exista in scena, exista o sansa de 1 la 100
		//in fiecare cadru ca sa fie afisat
		if (!shurikenVector[i].isAdded && rand() % 100 == 0) {
			AddShuriken(i);
		}
		else if (!shurikenVector[i].isAdded) {
			angularStep += 5.0f * deltaTimeSeconds;
			continue;
		}
		//miscarea pe orizontala
		shurikenVector[i].translateX -= shurikenSpeed * deltaTimeSeconds;

		//daca a iesit din cadru
		if (shurikenVector[i].translateX < -100) {
			DeleteShuriken(i);
		}

		//verificare coliziune cu arcul
		BowShurikenCollision(i, deltaTimeSeconds);
		//afisare shuriken
		PrintShuriken(i, deltaTimeSeconds);
		//power bar
		PowerBar();
	}
}
void Tema1::ChangeSpeed() {
	arrowSpeed = 100 + score;
	shurikenSpeed = 300 + 2*score;
	balloonUpSpeed = 100 + score;
	balloonDownSpeed = 500 + 5 * score;
	shurikenNumber = 4 + score / 20;
}
//formule gravitatie
void Tema1::CalculateGravity() {
	degree = (double)asin((double)aux_co / aux_ip);
	float vx = arrowSpeed * cos(degree) * duration;
	float vy = arrowSpeed * sin(degree) * duration - gravity * aux;

	translateX = initial_translateX + vx * aux;
	translateY = initial_translateY + vy * aux - 1 / 2 * gravity * aux * aux;

	aux += 0.1f;
	if (abs(vx - 0.01f) > 0.5f)
		degree = atan((vy - gravity * aux) / vx);
	else degree = atan(vy - gravity * aux);
}
//stergerea sagetii
void Tema1::DeleteArrow()
{
	translateX = 0;
	translateY = 0;
	arrowExists = false;
}
//verificare coliziuni ale sagetii
void Tema1::CheckCollisions(float degree)
{
	glm::ivec2 resolution = window->GetResolution();
	for (int i = 1; i <= balloonNumber;i++) {
		float x = translateX + cos(degree) * 60;
		float y = translateY - sin(-degree) * 60;

		ArrowBalloonCollision(i, x, y);
		ArrowShurikenCollision(i, x, y);
	}
	//iesire din cadru
	if (translateX > resolution.x || translateY > 2 * resolution.y || translateX < -resolution.x || translateY < -resolution.y) {
		DeleteArrow();
	}
}
//coliziune punct-elipsa
void Tema1::ArrowBalloonCollision(int i, int x, int y)
{
	// x si y sunt coordonatele arfului sagetii
	float h = balloonVector[i].translateX;
	float k = balloonVector[i].translateY + 30;
	float rx = 20;
	float ry = 30;

	if ((x - h) * (x - h) / (rx * rx) + (y - k) * (y - k) / (ry * ry) < 1) {
		//daca balonul nu a fost deja lovit, se incrementeaza scorul
		if (!balloonVector[i].destroyed) {
			if (balloonVector[i].colour <=30)
			{
				score++;
				printf("Red balloon hit!\nNew score=%d\n", score);
				
			}
			else if (balloonVector[i].colour <= 47) {
				score--;
				printf("Yellow balloon hit!\nNew score=%d\n", score);
			}
			else {
				glClearColor(0, 1, 0, 1);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				lives_left++;
				printf("Green baloon hit!\nNew lives left=%d\n", lives_left);
			}
			balloonVector[i].destroyed = true;
		}
	}
}
//coliziune punct-cerc
void Tema1::ArrowShurikenCollision(int i, int x, int y)
{
	// x si y sunt coordonatele arfului sagetii
	//-25 = stanga shurikenului, +25 = dreapta
	if (x >= shurikenVector[i].translateX - 25 && x <= shurikenVector[i].translateX + 25)
		//-25 = sub shuriken, +25 = deasupra
		if (y >= shurikenVector[i].translateY - 25 && y <= shurikenVector[i].translateY + 25) {
			//scorul creste
			score += 2;
			printf("Shuriken hit!\nNew score=%d\n", score);

			//dispare sageata
			DeleteArrow();
		}
}


//adaugare balon in scena
void Tema1::AddBalloon(int i)
{
	balloonVector[i].translateX = rand() % 5 * 100 + 42 * i + 200;
	balloonVector[i].translateY = -100;
	balloonVector[i].isAdded = true;
	balloonVector[i].destroyed = false;
	//culoare, sansa 30/50 sa fie rosu, 18/50 sa fie galben, 2/50 sa fie verde
	balloonVector[i].colour = rand() % 50;
	//directia caderii (stg/dr/jos)
	balloonVector[i].fallDirection = (rand() % 3 - 1);
	//directia urcarii (stg/dr/sus)
	balloonVector[i].riseDirection = (rand() % 3 - 1);
}
//sansa de schimbare a directiei de urcare
void Tema1::ChangeDirection()
{
	//1/500/cadru
	if ((rand() % 500 == 0 && okChange)) {
		okChange = false;
		for (int i = 1; i <= balloonNumber;i++) {
			//fiecare balon are sansa 1/3 sa schimbe directia
			if (rand() % 3 == 1)
				balloonVector[i].riseDirection = (rand() % 3 - 1);
		}
	}
	//cooldown schimbare de directie
	else if (rand() % 300 == 1)
		okChange = true;
}
//verificare daca balonul a fot lovit si schimbarea datelor lui
void Tema1::CheckDestroyBalloon(int i, float deltaTimeSeconds) {
	if (balloonVector[i].destroyed) {
		modelMatrix = glm::mat3(1);
		balloonVector[i].translateY -= balloonDownSpeed * deltaTimeSeconds;

		balloonVector[i].translateX += 100 * deltaTimeSeconds * balloonVector[i].fallDirection;
		modelMatrix *= Transform2D::Translate(balloonVector[i].translateX, balloonVector[i].translateY);
		modelMatrix *= Transform2D::Scale(0.5f, 1);
	}
}
//verificare daca balonul a iesit din cadru
void Tema1::CheckOutOfBoundsBalloon(int i)
{
	//daca iese din cadru in sus
	glm::ivec2 resolution = window->GetResolution();
	if (balloonVector[i].translateY - 40 > resolution.y) {
		DeleteBalloon(i);
	}
	//daca iese din cadru in jos dupa ce a fost distrus
	if (balloonVector[i].translateY + 40 < 0 && balloonVector[i].destroyed) {
		DeleteBalloon(i);
	}
	//daca iese din cadru stg/dr
	if (balloonVector[i].translateX - 40 > resolution.x) {
		DeleteBalloon(i);
	}
	if (balloonVector[i].translateX + 40 < 0) {
		DeleteBalloon(i);
	}
}
//afisare balon
void Tema1::PrintBalloon(int i)
{
	if (balloonVector[i].isAdded) {
		if(balloonVector[i].colour<=30)
			RenderMesh2D(meshes["balloonR"], shaders["VertexColor"], modelMatrix);
		else if (balloonVector[i].colour<=47)
			RenderMesh2D(meshes["balloonG"], shaders["VertexColor"], modelMatrix);
		else
			RenderMesh2D(meshes["balloonV"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(balloonVector[i].translateX, balloonVector[i].translateY - 8);
		RenderMesh2D(meshes["string"], shaders["VertexColor"], modelMatrix);
	}
}
//stergere balon
void Tema1::DeleteBalloon(int i)
{
	balloonVector[i].isAdded = false;
	balloonVector[i].translateY = -100;
	balloonVector[i].translateX = -100;
}


//adaugare shuriken in scena
void Tema1::AddShuriken(int i) {
	shurikenVector[i].translateX = 1500;
	shurikenVector[i].translateY = (rand() % 7) * 100 + 22 * i;
	shurikenVector[i].isAdded = true;
}


//verificare coliziune arc-shuriken
void Tema1::BowShurikenCollision(int i, float deltaTimeSeconds)
{
	//100 = coordonata pe ox a capatului de dr al arcului (eroare de 2)
	if (shurikenVector[i].translateX - 15 < 102 && shurikenVector[i].translateX - 15 > 95) {
		// +15 = partea de sus a shuriken, - 43 = partea de jos a arcului (cu o eroare de 7)
		// -15 = partea de jos a shuriken, + 43 = partea de sus a arcului (cu o eroare de 7)
		if (shurikenVector[i].translateY + 15 > translateYBow - 43 && shurikenVector[i].translateY - 15 < translateYBow + 43) {
			//schimbare de culoare a ecranului pt un frame pt a arata lovirea
			glClearColor(1, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			lives_left--;
			printf("Hit by a shuriken!\nLives left=%d\n", lives_left);
			if (lives_left == 0) {
				printf("\nYou died!\n\nScore=%d\n\n", score);
				//se inchide fereastra jocului
				CloseWindow(GetActiveWindow());
				exit(5);
			}
			DeleteShuriken(i);
		}
	}
}
//afisare shuriken
void Tema1::PrintShuriken(int i, float deltaTimeSeconds)
{
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(shurikenVector[i].translateX, shurikenVector[i].translateY);
	angularStep += 5.0f * deltaTimeSeconds;
	modelMatrix *= Transform2D::Rotate(angularStep);
	RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);
}
//stergere shuriken
void Tema1::DeleteShuriken(int i) {
	shurikenVector[i].isAdded = false;
	shurikenVector[i].translateX = -100;
	shurikenVector[i].translateY = -100;
}

//afisare powerbar
void Tema1::PowerBar()
{
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(0, -75);
	//crestere power bar
	if (startTimer) {
		duration = (std::clock() - clock) / (double)CLOCKS_PER_SEC;
		//pt ca bara sa nu creasca la infinit
		if (duration > 2) duration = 4;
		modelMatrix *= Transform2D::Scale(6.4f * duration, 1);
		RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
	}
}
//miscare sus jos a arcului din W si S
void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	glm::ivec2 resolution = window->GetResolution();
	if (window->KeyHold(GLFW_KEY_W) && translateYBow < resolution.y) {
		translateYBow += (200.0f + score) * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_S) && translateYBow > 0) {
		translateYBow -= (200.0f + score) * deltaTime ;
	}
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	//cand se apasa pe mouse se activeaza timer-ul pt power bar
	if (!arrowExists) {
		startTimer = true;
		clock = std::clock();
	}
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	//se activeaza sageata
	if (!arrowExists && startTimer) {
		aux_translateYBow = translateYBow;
		startArrow = true;
		startTimer = false;
		//se calculeaza viteza sagetii tinand cont de timpul apasat
		if (duration > 2) duration = 4;
		else if (duration < 1 && duration >0.1f) duration = 2;
		else if (duration < 1) duration = 1;
		else if (duration > 1 && duration < 2) duration = 3;

		aux_duration = duration;
	}
	
}