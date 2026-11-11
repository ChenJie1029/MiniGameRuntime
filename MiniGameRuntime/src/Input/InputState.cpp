#include <Input/InputState.h>

#include <cmath>
#include <SDL.h>

InputState ReadInputState() {
	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
	Vec2 playerDirection{};
	Vec2 cameraDirection{};

	if (keyboardState[SDL_SCANCODE_W]) {
		playerDirection.y += 1.0f;
	}
	if (keyboardState[SDL_SCANCODE_S]) {
		playerDirection.y -= 1.0f;
	}
	if (keyboardState[SDL_SCANCODE_A]) {
		playerDirection.x -= 1.0f;
	}
	if (keyboardState[SDL_SCANCODE_D]) {
		playerDirection.x += 1.0f;
	}

	const float length = std::sqrt(playerDirection.x * playerDirection.x + playerDirection.y * playerDirection.y);

	if (length > 0.0f) {
		playerDirection.x /= length;
		playerDirection.y /= length;
	}

	if (keyboardState[SDL_SCANCODE_UP]) {
		cameraDirection.y += 1.0f;
	}
	if (keyboardState[SDL_SCANCODE_DOWN]) {
		cameraDirection.y -= 1.0f;
	}
	if (keyboardState[SDL_SCANCODE_LEFT]) {
		cameraDirection.x -= 1.0f;
	}
	if (keyboardState[SDL_SCANCODE_RIGHT]) {
		cameraDirection.x += 1.0f;
	}

	return InputState{ playerDirection, cameraDirection };
}

InputActions ReadInputActions() {
	InputActions actions{};
	SDL_Event event{};

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			actions.quitRequested = true;
		}
		else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				actions.quitRequested = true;
			}
			else if (event.key.keysym.sym == SDLK_r) {
				actions.resetRequested = true;
			}
			else if (event.key.keysym.sym == SDLK_DELETE) {
				actions.deletePlayerRequested = true;
			}
			else if (event.key.keysym.sym == SDLK_p) {
				actions.pauseRequested = true;
			}
		}
	}

	return actions;
}