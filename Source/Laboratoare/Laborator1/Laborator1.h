#pragma once

#include <Component/SimpleScene.h>

class Laborator1 : public SimpleScene
{

	public:
		Laborator1();
		~Laborator1();
		bool press1 = false;
		bool press2 = false;
		bool pressf = false;
		int object = 1;
		float x = -1, y = 0.5f, z = 0;
		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
};
