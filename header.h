#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#define THETA 1.61803398875f
#define PI 3.14159f
#define LIGHT_COUNT 128
#define POINT_SCALE_MIN 0.01f
#define POINT_SCALE_MAX 0.1f

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <list>
#include <chrono>
#include <iostream>
#include <time.h>
#include "ogl_tools.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/string_cast.hpp>
#include <gtx/intersect.hpp>
#include <typeinfo>
#include <limits>

//for bitmap creation
#include <Windows.h>
#include <memory>

enum lighting_mode { UNIFORM_LIGHTING, CAMERA, ORIGIN, CENTERPOINT, DYNAMIC_LIGHTING, LIGHTING_MODE_SIZE };
enum image_extension {JPG, TIFF, PNG, BMP};

using jep::ogl_context;
using jep::ogl_camera;
using jep::key_handler;
using jep::text_handler;
using jep::texture_handler;
using jep::ogl_camera_free;
using jep::ogl_camera_flying;

using std::shared_ptr;

using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using std::string;

typedef std::pair<bool, float> random_switch;

class random_generator;
class fractal_generator;

template<class T>
void cycleEnum(T first, T last, T &current)
{
	if (current == last)
	{
		current = first;
	}

	else current = T(int(current) + 1);
}

static string getStringFromLightingMode(lighting_mode lm)
{
	switch (lm)
	{
	case UNIFORM_LIGHTING: return "uniform";
	case ORIGIN: return "origin"; 
	case CENTERPOINT: return "centerpoint";
	case CAMERA: return "camera";
	case DYNAMIC_LIGHTING: return "dynamic";
	default: return "unknown";
	}
}

template<class T>
T influenceElement(const T &target, const T &influence, float degree)
{
	return (target * (1.0f - degree)) + (influence * degree);
}