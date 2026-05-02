#include "FoodSystem.h"

#include <algorithm>

#include "../components/FruitComponent.h"
#include "../ecs/EntityManager.h"

namespace {
        // Grid más abierto: menos filas/columnas para separar más las frutas.
        constexpr int FOOD_GRID_COLS = 8;
        constexpr int FOOD_GRID_ROWS = 6;

        // Menor margen para acercar el grid a los bordes.
        constexpr float FOOD_MARGIN = 10.0f;
}

FoodSystem::FoodSystem() :
                _lastUpdateTime(0.0f),
                _needsGridCreation(true),
                _roundActive(false) {
}

FoodSystem::~FoodSystem() {
}

void FoodSystem::initSystem() {
        _lastUpdateTime = static_cast<float>(sdlutils().currRealTime());
        onNewGame();
}

void FoodSystem::update() {
        if (!_roundActive)
                return;

        updateMagicFoodState();
        _lastUpdateTime = static_cast<float>(sdlutils().currRealTime());
}

void FoodSystem::recieve(const Message &msg) {
        switch (msg.id) {
        case _m_NEW_GAME:
                onNewGame();
                break;

        case _m_ROUND_START:
                onNewRound();
                break;

        case _m_ROUND_OVER:
                _roundActive = false;
                break;

        case _m_GAME_OVER:
                _roundActive = false;
                _needsGridCreation = true;
                break;

        case _m_PACMAN_FOOD_COLLISION:
            {
                auto fruit = _mngr->getComponent<FruitComponent>(msg.collision_data.other);
                if (fruit != nullptr && fruit->isMagic() && fruit->isCurrentlyMagic()) {
                        Message immunityStart;
                        immunityStart.id = _m_IMMUNITY_START;
                        immunityStart.immunity_data.duration = 5.0f;
                        _mngr->send(immunityStart, true);
                }
            }
                removeFood(msg.collision_data.other);

                if (noFoodLeft()) {
                        Message endGame;
                        endGame.id = _m_GAME_OVER;
                        _mngr->send(endGame);
                        _roundActive = false;
                }
                break;

        default:
                break;
        }
}

void FoodSystem::onNewGame() {
        
        for (auto food : _mngr->getEntities(ecs::grp::FRUITS)) {
                if (_mngr->isAlive(food))
                        _mngr->setAlive(food, false);
        }

        _needsGridCreation = true;
        createFoodGrid();
        resetFoodTimers();

        _roundActive = true;
        _lastUpdateTime = static_cast<float>(sdlutils().currRealTime());
}

void FoodSystem::onNewRound() {
        // Al empezar una nueva ronda nos quedamos con las frutas que ya habia
        resetFoodTimers();
        _roundActive = true;
        _lastUpdateTime = static_cast<float>(sdlutils().currRealTime());
}

void FoodSystem::createFoodGrid() {
        if (!_needsGridCreation)
                return;

        // Queremos frutas repartidas en un grid regular.
        // Como Transform usa posición top-left, colocamos cada fruta calculando la esquina superior izquierda.
        float screenW = static_cast<float>(sdlutils().width());
        float screenH = static_cast<float>(sdlutils().height());

        float usableW = screenW - 2.0f * FOOD_MARGIN;
        float usableH = screenH - 2.0f * FOOD_MARGIN;

        float cellW = usableW / FOOD_GRID_COLS;
        float cellH = usableH / FOOD_GRID_ROWS;

        float foodSize = std::min(cellW, cellH) * 0.25f;

        for (int row = 0; row < FOOD_GRID_ROWS; ++row) {
                for (int col = 0; col < FOOD_GRID_COLS; ++col) {
                        float centerX = FOOD_MARGIN + (col + 0.5f) * cellW;
                        float centerY = FOOD_MARGIN + (row + 0.5f) * cellH;

                        
                        Vector2D pos(centerX - foodSize / 2.0f, centerY - foodSize / 2.0f);

                        auto food = _mngr->addEntity(ecs::grp::FRUITS);
                        _mngr->addComponent<Transform>(food, pos, Vector2D(0.0f, 0.0f), foodSize, foodSize, 0.0f);
                        _mngr->addComponent<FruitComponent>(food);
                }
        }

        _needsGridCreation = false;
}

void FoodSystem::updateMagicFoodState() {
        // Recorremos todas las frutas vivas y dejamos que cada componente actualice su temporizador
        float now = static_cast<float>(sdlutils().currRealTime());
        float dt = (now - _lastUpdateTime) / 1000.0f;

        if (dt < 0.0f)
                dt = 0.0f;

        auto &foods = _mngr->getEntities(ecs::grp::FRUITS);
        for (auto food : foods) {
                if (!_mngr->isAlive(food))
                        continue;

                auto fruit = _mngr->getComponent<FruitComponent>(food);
                if (fruit != nullptr) {
                        fruit->update(dt);
                }
        }
}

void FoodSystem::removeFood(ecs::entity_t food) {
        if (food == nullptr)
                return;

        if (_mngr->isAlive(food))
                _mngr->setAlive(food, false);
}

bool FoodSystem::noFoodLeft() const {
        for (auto food : _mngr->getEntities(ecs::grp::FRUITS)) {
                if (_mngr->isAlive(food))
                        return false;
        }
        return true;
}

void FoodSystem::resetFoodTimers() {
        for (auto food : _mngr->getEntities(ecs::grp::FRUITS)) {
                if (!_mngr->isAlive(food))
                        continue;

                auto fruit = _mngr->getComponent<FruitComponent>(food);
                if (fruit != nullptr) {
                        fruit->resetForNewRound();
                }
        }
}