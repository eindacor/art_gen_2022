#pragma once

#ifndef FRACTAL_GENERATOR_H
#define FRACTAL_GENERATOR_H

#include "header.h"
#include "random_generator.h"
#include "color_manager.h"
#include "settings_manager.h"
#include "geometry_generator.h"

typedef std::pair<GLenum, attribute_index_method> render_style;

#define SEGMENTED_LINES render_style(GL_LINES, POINTS)
#define CONTINUOUS_LINES render_style(GL_LINE_STRIP, POINTS)
#define WIREFRAME_SOLIDS render_style(GL_LINES, LINES)
#define SEGMENTED_SOLIDS render_style(GL_LINES, TRIANGLES)
#define WIREFRAME_CONNECTED render_style(GL_LINE_STRIP, TRIANGLES)

class fractal_generator
{
public:
	fractal_generator(
		const string &randomization_seed,
		const shared_ptr<jep::ogl_context> &con,
		int num_points);

	~fractal_generator() { 
		glDeleteVertexArrays(1, &VAO); 
		glDeleteBuffers(1, &vertices_vbo); 
		glDeleteBuffers(1, &line_indices); 
		glDeleteBuffers(1, &triangle_indices); 
		glDeleteVertexArrays(1, &palette_vao);
		glDeleteBuffers(1, &palette_vbo);
	}

	string getSeed() const { return base_seed; }

	void setMatrices();

	void generateFractal();
	void generateFractalFromPointSequence();
	void generateFractalWithRefresh();
	void generateFractalFromPointSequenceWithRefresh();

	void renderFractal(const int &image_width, const int &image_height, const int &matrix_sequence_count);

	//vector<mat4> generateMatrixSequence(const vector<int> &matrix_indices) const;
	std::vector<mat4> generateMatrixSequence(const int &sequence_size) const;

	void updateLightColorOverride();
	void updateLineColorOverride();
	void updateTriangleColorOverride();
	void updatePointColorOverride();
	void applyBackground(const int &num_samples);
	void checkKeys(const shared_ptr<key_handler> &keys);
	void drawFractal(std::weak_ptr<ogl_camera_flying> camera) const;
	
	// keeps track of how many indices are called by draw command, set by geometry index pattern generated in geometry_generator.cpp
	int point_index_count;
	int line_index_count;
	int triangle_index_count;

	void invertColors();
	void newColors();
	void updateBackground();
	void regenerateFractal();

	vec4 getSampleColor(const int &samples, const std::vector<vec4> &color_pool) const;
	float getLineWidth() const { return sm.line_width; }

	void printMatrices() const;

	vec4 getBackgroundColor() const { return background_color; }
	void adjustBackgroundBrightness(float adjustment);

	void tickAnimation();
	void swapMatrices();
	void cycleColorPalette();
	void loadPointSequence(string name, const std::vector<vec4> &sequence);
	void printContext();
	void cycleGeometryType();
	void cycleBackgroundColorIndex();
	void cycleLightColorOverride();
	void cycleLineColorOverride();
	void cycleTriangleColorOverride();
	void cyclePointColorOverride();
	void setBackgroundColorIndex(int index);
	int getBackgroundColorIndex() const { return sm.background_front_index; }

	vec3 getFocalPoint() const { return focal_point; }
	float getAverageDelta() const { return average_delta; }

	signed int getGeneration() const { return sm.generation; }

	std::vector<vec4> getColorsFront() const { return colors_front; }
	std::vector<vec4> getColorsBack() const { return colors_back; }
	float getInterpolationState() const { return sm.interpolation_state; }

	settings_manager getSettings() const { return sm; }
	void setTwoDimensional(bool b) { sm.two_dimensional = b; }
	void setCurrentCustomSequence(int sequence_index) { current_sequence = sequence_index; }
	string getStringFromGeometryType(geometry_type gt) const;
	int getMaxPointSize() const { return max_point_size; }

	std::vector<std::pair<string, std::vector<vec4> > > getLoadedSequences() const { return loaded_sequences; }

private:
	settings_manager sm;
	string base_seed;
	string generation_seed;
	std::vector<unsigned int> matrix_sequence_front;
	std::vector<unsigned int> matrix_sequence_back;
	std::vector<std::pair<string, mat4> > matrices_front;
	std::vector<std::pair<string, mat4> > matrices_back;
	std::vector<vec4> colors_front;
	std::vector<vec4> colors_back;
	vec4 background_color;
	vec4 seed_color_front;
	vec4 seed_color_back;
	std::vector<float> sizes_front;
	std::vector<float> sizes_back;
	geometry_type geo_type_front = GEOMETRY_TYPE_SIZE;
	geometry_type geo_type_back = GEOMETRY_TYPE_SIZE;
	random_generator rg;
	color_manager color_man;
	geometry_generator gm;
	vec3 focal_point;
	float average_delta, max_x, max_y, max_z;
	// -3 = no override, -2 = black, -1 = white, 0 - n for each interpolated matrix_color
	int light_color_override_index = -3;
	int line_color_override_index = -3;
	int triangle_color_override_index = -3;
	int point_color_override_index = -3;
	bool dof_enabled = false;
	int dof_passes = 10;
	float dof_aperture = 0.005;
	int max_point_size;
	bool matrix_geometry_uses_solid_geometry = false;
	float point_size_modifier = 1.0f;

	unsigned int current_frame = 0;
	unsigned int frame_increment = 1;
	bool reverse_growth = false;
	int vertices_to_render = 0;
	bool show_growth = false;
	
	// current gen parameters	
	vec4 origin = vec4(0.0f, 0.0f, 0.0f, 1.0f);	
	// vector instead of a map to make cycling easy
	std::vector<std::pair<std::string, std::vector<vec4> > > loaded_sequences;
	int current_sequence = 0;
	mat4 fractal_scale_matrix;

	vec4 light_positions[LIGHT_COUNT];
	vec4 light_colors[LIGHT_COUNT];
	std::vector<int> light_indices;

	bool initialized = false;

	const unsigned short vertex_size = 9;
	int vertex_count;
	int palette_vertex_count;

	GLuint vertices_vbo;
	GLuint palette_vbo;
	GLuint line_indices;
	GLuint triangle_indices;

	GLuint VAO;
	GLuint palette_vao;

	shared_ptr<ogl_context> context;

	void addNewPointAndIterate(
		vec4 &starting_point,
		vec4 &starting_color,
		float &starting_size,
		int matrix_index_front,
		int matrix_index_back,
		std::vector<float> &points);

	void addPointSequenceAndIterate(
		mat4 &origin_matrix,
		vec4 &starting_color,
		float &starting_size,
		int matrix_index_front,
		int matrix_index_back,
		std::vector<float> &points,
		std::vector<unsigned short> &line_indices,
		std::vector<unsigned short> &triangle_indices,
		int &current_sequence_index_lines,
		int &current_sequence_index_triangles);

	void addNewPoint(
		const vec4 &point,
		const vec4 &color,
		const float &size,
		std::vector<float> &points);

	void bufferData(const std::vector<float> &vertex_data, const std::vector<unsigned short> &line_indices, const std::vector<unsigned short> &triangle_indices);
	void bufferPalette(const std::vector<float> &vertex_data);
	void bufferLightData(const std::vector<float> &vertex_data);

	std::vector<std::pair<string, mat4> > generateMatrixVector(const int &count, geometry_type &geo_type);
	std::vector<vec4> generateColorVector(const vec4 &seed, color_palette palette, const int &count, color_palette &random_selection) const;
	std::vector<float> generateSizeVector(const int &count) const;
	std::vector<float> getPalettePoints();
	void addDataToPalettePoints(const vec2 &point, const vec4 &color, std::vector<float> &points) const;
	void addPalettePointsAndBufferData(const std::vector<float> &vertex_data, const std::vector<unsigned short> &line_indices, const std::vector<unsigned short> &triangle_indices);

	void drawVertices() const;
	void drawLines() const;
	void drawTriangles() const;
	void drawPalette() const;

	void generateLights();
};

#endif