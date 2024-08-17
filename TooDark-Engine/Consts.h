#pragma once
#include<string>
#include<cassert>
#include<stdio.h>
#include <glm.hpp>
#include <vector>
#include <unordered_map>
#include <SDL.H>
#include<array>
#include <SDL_image.h>
using v2 = glm::vec2;
using v3 = glm::vec3;
using v4 = glm::vec4;


#include <chrono>
#include <filesystem>

struct TDark_Clock
{
	std::chrono::steady_clock::time_point getTimeNow()
	{
		return std::chrono::steady_clock::now();
	}
	void setStartTimePointNow()
	{
		startPoint = std::chrono::steady_clock::now();
		tick = true;
	}
	long int getTimeSinceStart()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(getTimeNow() - startPoint).count();

	}
	void reset()
	{
		std::chrono::steady_clock::time_point s;
		startPoint = s;
	}
	bool tick = false;
	std::chrono::steady_clock::time_point startPoint;

};
