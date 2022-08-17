#version 430
in vec4 fragment_color;
out vec4 output_color;

uniform int window_width;
uniform int window_height;
uniform float current_time;

#define PI 3.1415926
#define INVERT true


// from https://www.shadertoy.com/view/4djSRW
float hash(vec2 p)
{
	return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

vec2 getRandomVector(float seed) {
	vec2 outVec;
	if (seed < .25) {
		outVec = vec2(1., 1.);
	} else if (seed < .5) {
		outVec = vec2(-1., 1.);
	} else if (seed < .75) {
		outVec = vec2(1., -1.);
	} else {
		outVec = vec2(-1., -1.);
	}

	return outVec;
}

float biLerp(float f0, float f1, float f2, float f3, float lerpX, float lerpY) {
	float upper = mix(f1, f2, lerpX);
	float lower = mix(f0, f3, lerpX);

	return mix(lower, upper, lerpY);
}

mat2x2 createRotationMatrix(float rotation) {
	return mat2x2(
	cos(rotation), -sin(rotation),
	sin(rotation), cos(rotation)
	);
}

float getModifiedDot(vec2 uv, vec2 p, float gridDimension, float pHash) {
	float rotation = sin(current_time * .15 + pHash) * 2. * PI;
	if (pHash < .5) {
		rotation *= -1.;
	}
	mat2x2 rotationMatrix = createRotationMatrix(rotation);

	return dot((uv - p) / gridDimension, getRandomVector(pHash) * rotationMatrix);
}

float getPerlinValue(vec2 uv, float gridDimension, bool hideLines) {
	float xCoord = floor(uv.x / gridDimension) * gridDimension;
	float yCoord = floor(uv.y / gridDimension) * gridDimension;

	float xIndex = floor(uv.x / gridDimension);
	float yIndex = floor(uv.y / gridDimension);

	float p0Hash = hash(vec2(xIndex, yIndex));
	float p1Hash = hash(vec2(xIndex, yIndex + 1.));
	float p2Hash = hash(vec2(xIndex + 1., yIndex + 1.));
	float p3Hash = hash(vec2(xIndex + 1., yIndex));

	vec2 p0 = vec2(xCoord, yCoord);
	vec2 p1 = vec2(xCoord, yCoord + gridDimension);
	vec2 p2 = vec2(xCoord + gridDimension, yCoord + gridDimension);
	vec2 p3 = vec2(xCoord + gridDimension, yCoord);

	float rotation = sin(current_time * .15) * 2. * PI;
	mat2x2 rotationMatrix = createRotationMatrix(rotation);

	float dot0 = getModifiedDot(uv, p0, gridDimension, p0Hash);
	float dot1 = getModifiedDot(uv, p1, gridDimension, p1Hash);
	float dot2 = getModifiedDot(uv, p2, gridDimension, p2Hash);
	float dot3 = getModifiedDot(uv, p3, gridDimension, p3Hash);

	float xInterp = smoothstep(p0.x, p2.x, uv.x);
	float yInterp = smoothstep(p0.y, p2.y, uv.y);

	float val = biLerp(dot0, dot1, dot2, dot3, xInterp, yInterp);

	float xLerp = mod(uv.x / 2., gridDimension);
	float revealMargin = gridDimension * .95;

	if (hideLines || xLerp < revealMargin) {
		return abs(val);
	} else {
		float marginLerp = (xLerp - revealMargin) / (1. - revealMargin);
		float distFromCenterMargin = abs(.5 - marginLerp);
		float marginSmooth = smoothstep(0.499, .5, distFromCenterMargin);
		return mix(clamp(val, 0., 1.), abs(val), marginSmooth);
	}
}


void main()
{
	vec2 uv = vec2(gl_FragCoord.x / float(window_width), gl_FragCoord.y / float(window_height));

	float aspectRatio = float(window_width) / float(window_height);
	uv.x *= aspectRatio;

	float timeScale = .08;

	uv += vec2(sin(current_time * timeScale), current_time * timeScale);

	vec3 background = vec3(0.2, 0., 0.);
	vec3 color1 = vec3(1., 0.75, .25);
	vec3 color2 = vec3(1.);
	vec3 color3 = vec3(2.5);

	float gridSize = .15;

	float perlinVal1 = pow(getPerlinValue(uv, gridSize, false), .15);
	float perlinVal2 = log(getPerlinValue(uv, gridSize, false)) / 4.;
	float perlinVal3 = pow(getPerlinValue(uv, gridSize * 5., true), 2.);

	vec3 color = mix(mix(mix(background, color1, perlinVal1), color2, perlinVal2), color3, perlinVal3 * .7);

	vec4 perlinColor;
	if (INVERT) {
		perlinColor = vec4(1. - clamp(color.r, 0., 1.), 1. - clamp(color.g, 0., 1.), 1. - clamp(color.b, 0., 1.), 1.);
	} else {
		perlinColor = vec4(vec3(color), 1.);
	}

	output_color = fragment_color;
	output_color.r = perlinColor.r;

//	float timeVal = float(current_frame) / 1000.;
//	output_color = vec4(timeVal, timeVal, timeVal, 1.);
}