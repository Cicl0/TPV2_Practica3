#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../components/Transform.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/SDLUtils.h"
#include <cassert>

class Texture;

namespace ecs {

struct ImageWithFrames : public Component {
	ImageWithFrames() :
			_tr(nullptr),
			_tex(nullptr),
			_frame(0),
			_cols(6),
			_rows(5),
			_numFrames(30),
			_nextFrameTime(.0f),
			_frameCooldown(50.0f) {
	}

	virtual ~ImageWithFrames() {
	}

	void initComponent() override {
		_tex = &sdlutils().images().at("asteroid");
		assert(_tex != nullptr);
	}

	void setTransform(Transform *tr) {
		_tr = tr;
	}

	void update() {
		if (sdlutils().virtualTimer().currRealTime() < _nextFrameTime)
			return;

		_nextFrameTime = _frameCooldown + sdlutils().virtualTimer().currRealTime();
		_frame = (_frame + 1) % _numFrames;
	}

	void render() {
		if (_tr == nullptr || _tex == nullptr)
			return;

		int col = _frame % _cols;
		int row = _frame / _cols;

		int frameWidth = _tex->width() / _cols;
		int frameHeight = _tex->height() / _rows;

		SDL_FRect src{
			col * frameWidth,
			row * frameHeight,
			frameWidth,
			frameHeight
		};

		SDL_FRect dest{
			_tr->getPos().getX(),
			_tr->getPos().getY(),
			frameWidth / 3,
			frameHeight / 3
		};

		_tex->render(src, dest);
	}

	private:
	Transform* _tr;
	const Texture* _tex;
	int _frame;
	int _cols;
	int _rows;
	int _numFrames;
	float _nextFrameTime;
	float _frameCooldown;
};

} // namespace ecs

