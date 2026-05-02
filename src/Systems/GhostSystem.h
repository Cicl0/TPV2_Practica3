#pragma once

#include "../ecs/System.h"
#include "../ecs/EntityManager.h"
#include "../components/Transform.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"

class GhostSystem : public ecs::System {
public:
	GhostSystem();
	virtual ~GhostSystem();

	void initSystem() override;
	void update() override;
	void recieve(const Message &msg) override;

	void onNewGame();

	void onNewRound();

private:
	void trySpawnGhost();
	void spawnGhost();
	void updateGhosts(float dt);
	void handleWallBounce(ecs::entity_t ghost);

	Vector2D randomCornerPos() const;

	int countAliveGhosts() const;

	void clearAllGhosts();
	void removeGhost(ecs::entity_t ghost);

	float _lastUpdateTime;
	float _lastSpawnTime;

	bool _roundActive;
	bool _immunityActive;

	static constexpr int MAX_GHOSTS = 10;
	static constexpr float SPAWN_COOLDOWN = 5.0f;
	static constexpr float RETARGET_PROB = 0.005f;
	static constexpr float GHOST_SPEED = 1.1f;
};
