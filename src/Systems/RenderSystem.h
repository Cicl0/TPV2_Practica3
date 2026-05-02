// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"

class Texture;
struct Transform;

class RenderSystem: public ecs::System {
public:

	RenderSystem();
	virtual ~RenderSystem();
	void initSystem() override;
	void update() override;
	void recieve(const Message &msg) override;
private:
	void drawPacMan();
	void drawHealth();
	void drawFood();
	void drawGhosts();
	void draw(Transform *tr, const Texture *tex);

	bool _immunityActive;

	static constexpr float CELL_W = 128.0f;  // 1024 / 8
	static constexpr float CELL_H = 128.0f;  // 1024 / 8
};

