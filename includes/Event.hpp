#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <Camera.hpp>
#include <Renderer.hpp>
#include <Transform.hpp>

void event(SDL_Event& e,Transform& transform, Camera& camera, bool& run, bool& triggerTexture, uint8_t& moveFlags, Renderer& render);
void applyMovement(Camera& camera, uint8_t& moveFlags);