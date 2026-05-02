// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"
#include "../game/Game.h"

class GameCtrlSystem: public ecs::System {
public:

	GameCtrlSystem();
	virtual ~GameCtrlSystem();

	void initSystem() override;
	void update() override;
	void recieve(const Message &m) override;

	void setGame(Game* game) {
		_game = game;
	}
private:
	Game* _game;
};

