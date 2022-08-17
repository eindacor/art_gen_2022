#version 430
in vec4 fragment_color;
out vec4 output_color;

uniform int window_width;
uniform int window_height;
uniform float current_time;

void main()
{
	vec2 uv = vec2(gl_FragCoord.x / float(window_width), gl_FragCoord.y / float(window_height));

	float aspectRatio = float(window_width) / float(window_height);
	uv.x *= aspectRatio;

	output_color = fragment_color;
}