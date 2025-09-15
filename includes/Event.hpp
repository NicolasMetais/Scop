#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <Camera.hpp>
#include <Transform.hpp>

void event(SDL_Event& e,Transform& transform, Camera& camera, bool& run, bool& triggerTexture);