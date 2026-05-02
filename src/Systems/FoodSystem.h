
#pragma once

#include "../ecs/System.h"
#include "../ecs/EntityManager.h"
#include "../components/Transform.h"
#include "../sdlutils/SDLUtils.h"

class FoodSystem : public ecs::System {
public:
	FoodSystem();
	virtual ~FoodSystem();

	void initSystem() override;
	void update() override;
	void recieve(const Message &msg) override;

	void onNewGame();

	void onNewRound();

private:
	void createFoodGrid();
	void updateMagicFoodState();
	void removeFood(ecs::entity_t food);
	bool noFoodLeft() const;

	void resetFoodTimers();

	float _lastUpdateTime;
	bool _needsGridCreation;
	bool _roundActive;
};
