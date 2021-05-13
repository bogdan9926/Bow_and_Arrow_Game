#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include <ctime>
#include <time.h>    

class Tema1 : public SimpleScene
{
	struct balon {
		bool isAdded;
		float translateX;
		float translateY;
		bool destroyed;
		int colour;
		int fallDirection;
		int riseDirection;
	};

	struct shuriken {
		bool isAdded;
		float translateX;
		float translateY;
	};
	public:
		Tema1();
		~Tema1();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		//void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		//void OnKeyPress(int key, int mods) override;
		//void OnKeyRelease(int key, int mods) override;
		//void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		//void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		//void OnWindowResize(int width, int height) override;
		void PowerBar();
		void BowShurikenCollision(int i, float deltaTimeSeconds);
		void ArrowBalloonCollision(int i, int x, int y);
		void ArrowShurikenCollision(int i, int x, int y);
		void AddShuriken(int i);
		void DeleteShuriken(int i);
		void AddBalloon(int i);
		void ChangeDirection();
		void DeleteBalloon(int i);
		void CheckOutOfBoundsBalloon(int i);
		void CheckDestroyBalloon(int i, float deltaTimeSeconds);
		void PrintBalloon(int i);
		void PrintShuriken(int i, float deltaTimeSeconds);
		void DeleteArrow();
		void CheckCollisions(float degree);
		void CalculateGravity();
		void ChangeSpeed();
	protected:
		glm::mat3 modelMatrix, modelMatrixArrow;
		float translateX, translateY, initial_translateX, initial_translateY;
		bool okTranslate,okScale, okChange;
		float scaleX, scaleY;
		float angularStep;

		float translateYBow;
		float screenLimitsBow;
		POINT mouseCoordinates;
		float co, ca, ip;
		bool startArrow, arrowExists,startTimer;
		int aux_co, aux_ca, aux_ip, aux_bow, aux_translateYBow;
		clock_t clock, respawn;
		float duration, aux_duration;
		const float PI = 3.1415926f;
		balon balloonVector[10];
		shuriken shurikenVector[10];
		int score;
		int lives_left;
		int arrowSpeed, shurikenSpeed, balloonUpSpeed,balloonDownSpeed;
		int gravity;
		float aux;
		int shurikenNumber, balloonNumber;
		float degree;
};
