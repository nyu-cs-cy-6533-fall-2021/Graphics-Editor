// This example is heavily based on the tutorial at https://open.gl

// OpenGL Helpers to reduce the clutter
#include "Helpers.h"
#include <iostream>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
// GLFW is necessary to handle the OpenGL context
#include <GLFW/glfw3.h>
#else
// GLFW is necessary to handle the OpenGL context
#include <GLFW/glfw3.h>
#endif

// OpenGL Mathematics Library
#include <glm/glm.hpp> // glm::vec3
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

// Timer
#include <chrono>

using namespace std;
// VertexBufferObject wrapper
VertexBufferObject VBO;
VertexBufferObject VBO_C;


// Contains the vertex positions
//Eigen::MatrixXf V(2,3);
std::vector<glm::vec2> V(300);
std::vector<glm::vec3> C(300);


int numberOfTimesPressed = 0;
int numOfTriangles;
int mode;
bool lbutton_down;
//bool noSelectedTriangle = true;
double diff_V0_cursor_x;
double diff_V0_cursor_y;
double diff_V1_cursor_x;
double diff_V1_cursor_y;
double diff_V2_cursor_x;
double diff_V2_cursor_y;
int selected_triangle;
int whichVertex;
glm::mat4x4 MVP = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
glm::mat4x4 inv;
glm::vec3 translate_left= glm::vec3(-0.2f,0.0f,1.0f);
glm::vec3 translate_right = glm::vec3(0.2f, 0.0f, 1.0f);
glm::vec3 translate_up = glm::vec3(0.0, 0.2f, 1.0f);
glm::vec3 translate_down = glm::vec3(0.0f, -0.2f, 1.0f);

std::chrono::steady_clock::time_point t_pressed_k;
bool k_pressed = false;
std::chrono::steady_clock::time_point t_pressed_l;
bool l_pressed = false;
glm::vec2 V0;
glm::vec2 V1;
glm::vec2 V2;
double ox, oy;
bool skip;

bool intpoint_inside_trigon(double xworld, double yworld, glm::vec2 a, glm::vec2 b, glm::vec2 c)
{
	double as_x = xworld - a.x;
	double as_y = yworld - a.y;

	bool s_ab = (b.x - a.x) * as_y - (b.y - a.y) * as_x > 0;

	if ((c.x - a.x) * as_y - (c.y - a.y) * as_x > 0 == s_ab) return false;

	if ((c.x - b.x) * (yworld - b.y) - (c.y - b.y) * (xworld - b.x) > 0 != s_ab) return false;

	return true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (mode == 0) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		
		// Get the size of the window
		int width, height;
		glfwGetWindowSize(window, &width, &height);

		// Convert screen position to world coordinates
		double xworld = ((xpos / double(width)) * 2) - 1;
		double yworld = (((height - 1 - ypos) / double(height)) * 2) - 1;
		inv = glm::inverse(MVP);
		glm::vec4 new_cord= inv * glm::vec4(xworld, yworld, 0.0, 1.0);
		xworld = new_cord.x;
		yworld = new_cord.y;
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			V[0] = glm::vec2(xworld, yworld);
		VBO.update(V);
	}
	else if (mode == 1) {
		if ((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_PRESS)) {
			numberOfTimesPressed += 1;

			// Update the position of the first vertex if the left button is pressed
			if ((numberOfTimesPressed % 3 == 1)) {
				// Get the position of the mouse in the window
				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);

				// Get the size of the window
				int width, height;
				glfwGetWindowSize(window, &width, &height);

				// Convert screen position to world coordinates
				double xworld = ((xpos / double(width)) * 2) - 1;
				double yworld = (((height - 1 - ypos) / double(height)) * 2) - 1; // NOTE: y axis is flipped in glfw
				inv = glm::inverse(MVP);
				glm::vec4 new_cord = inv * glm::vec4(xworld, yworld, 0.0, 1.0);
				xworld = new_cord.x;
				yworld = new_cord.y;
				V[3 * numOfTriangles] = glm::vec2(xworld, yworld);
				V[3 * numOfTriangles + 1] = V[3 * numOfTriangles];
				V[3 * numOfTriangles + 2] = V[3 * numOfTriangles];
				VBO.update(V);
			}
			else if ((numberOfTimesPressed % 3 == 2)) {
				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);
				int width, height;
				glfwGetWindowSize(window, &width, &height);
				double xworld = ((xpos / double(width)) * 2) - 1;
				double yworld = (((height - 1 - ypos) / double(height)) * 2) - 1; // NOTE: y axis is flipped in glfw
				inv = glm::inverse(MVP);
				glm::vec4 new_cord = inv * glm::vec4(xworld, yworld, 0.0, 1.0);
				xworld = new_cord.x;
				yworld = new_cord.y;
				V[3 * numOfTriangles + 1] = glm::vec2(xworld, yworld);
				VBO.update(V);
			}
			else if ((numberOfTimesPressed % 3 == 0)) {
				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);
				int width, height;
				glfwGetWindowSize(window, &width, &height);
				double xworld = ((xpos / double(width)) * 2) - 1;
				double yworld = (((height - 1 - ypos) / double(height)) * 2) - 1;
				inv = glm::inverse(MVP);
				glm::vec4 new_cord = inv * glm::vec4(xworld, yworld, 0.0, 1.0);
				xworld = new_cord.x;
				yworld = new_cord.y;
				V[3 * numOfTriangles + 2] = glm::vec2(xworld, yworld);
				numOfTriangles += 1;
				VBO.update(V);
			}
		}
	}
	else if (mode == 2) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		double xworld = ((xpos / double(width)) * 2) - 1;
		double yworld = (((height - 1 - ypos) / double(height)) * 2) - 1;
		inv = glm::inverse(MVP);
		glm::vec4 new_cord = inv * glm::vec4(xworld, yworld, 0.0, 1.0);
		xworld = new_cord.x;
		yworld = new_cord.y;

		if ((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_PRESS)) {
			lbutton_down = true;
			for (int i = numOfTriangles - 1; i >= 0; i--) {
				if (intpoint_inside_trigon(xworld, yworld, V[3 * i], V[3 * i + 1], V[3 * i + 2])) {//Cursor in the triangle
					selected_triangle = i;
					cout << selected_triangle << endl;
					diff_V0_cursor_x = V[3 * i].x - xworld;
					diff_V0_cursor_y = V[3 * i].y - yworld;
					diff_V1_cursor_x = V[3 * i + 1].x - xworld;
					diff_V1_cursor_y = V[3 * i + 1].y - yworld;
					diff_V2_cursor_x = V[3 * i + 2].x - xworld;
					diff_V2_cursor_y = V[3 * i + 2].y - yworld;
					for (int j = 0; j < 300; j++) {
						C[j] = glm::vec3(1, 0, 0);
					}
					C[3 * i] = glm::vec3(0, 0, 1);
					C[3 * i + 1] = glm::vec3(0, 0, 1);
					C[3 * i + 2] = glm::vec3(0, 0, 1);
					//noSelectedTriangle = false;
					VBO_C.update(C);
					break;
				}
			}
		}
		else if ((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_RELEASE)) {
			lbutton_down = false;
			//noSelectedTriangle = true;
		}

	}
	else if (mode == 3) {
		//cout << "I am in mode 3";
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		double xworld = ((xpos / double(width)) * 2) - 1;
		double yworld = (((height - 1 - ypos) / double(height)) * 2) - 1;
		inv = glm::inverse(MVP);
		glm::vec4 new_cord = inv * glm::vec4(xworld, yworld, 0.0, 1.0);
		xworld = new_cord.x;
		yworld = new_cord.y;

		if ((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_PRESS)) {

			for (int i = numOfTriangles - 1; i >= 0; i--) {
				if (intpoint_inside_trigon(xworld, yworld, V[3 * i], V[3 * i + 1], V[3 * i + 2])) {//Cursor in the triangle
					V[3 * i] = glm::vec2(0, 0);
					V[3 * i + 1] = glm::vec2(0, 0);
					V[3 * i + 2] = glm::vec2(0, 0);
					VBO.update(V);
					break;
				}
			}
		}
	}
	else if (mode == 4) {
		if ((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_PRESS)) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			double xworld = ((xpos / double(width)) * 2) - 1;
			double yworld = (((height - 1 - ypos) / double(height)) * 2) - 1;
			inv = glm::inverse(MVP);
			glm::vec4 new_cord = inv * glm::vec4(xworld, yworld, 0.0, 1.0);
			xworld = new_cord.x;
			yworld = new_cord.y;

			double min=10;//window range from -1 to 1
			for (int i = 0; i < 3 * numOfTriangles; i++) {
				double euclideanDistance = std::powf(V[i].x-xworld,2)+ std::powf(V[i].y - yworld, 2);
				if (euclideanDistance < min) {
					min = euclideanDistance;
					whichVertex = i;
				}
			}
			//cout << "Vertex: " << whichVertex << endl;

		}
	}


}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	double x_pos, y_pos;
	glfwGetCursorPos(window, &x_pos, &y_pos);

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	// Convert screen position to world coordinates
	double xworld = ((x_pos / double(width)) * 2) - 1;
	double yworld = (((height - 1 - y_pos) / double(height)) * 2) - 1;
	inv = glm::inverse(MVP);
	glm::vec4 new_cord = inv * glm::vec4(xworld, yworld, 0.0, 1.0);
	xworld = new_cord.x;
	yworld = new_cord.y;
	if (mode == 1) {
		if ((numberOfTimesPressed % 3 == 1)) {
			V[3 * (numOfTriangles)+1] = glm::vec2(xworld, yworld);
			VBO.update(V);
		}
		else if ((numberOfTimesPressed % 3 == 2)) {
			V[3 * (numOfTriangles)+2] = glm::vec2(xworld, yworld);
			VBO.update(V);
		}
		else if ((numberOfTimesPressed % 3 == 0)) {
			VBO.update(V);
		}
	}
	else if (mode == 2 && lbutton_down == TRUE) {
		//cout << "I am dragging" << endl;

		for (int i = numOfTriangles - 1; i >= 0; i--) {
			if (i == selected_triangle) {//Cursor in the triangle

				V[3 * i] = glm::vec2(xworld + diff_V0_cursor_x, yworld + diff_V0_cursor_y);
				V[3 * i + 1] = glm::vec2(xworld + diff_V1_cursor_x, yworld + diff_V1_cursor_y);
				V[3 * i + 2] = glm::vec2(xworld + diff_V2_cursor_x, yworld + diff_V2_cursor_y);
				break;
			}
		}
		VBO.update(V);
	}

}

void rotate_c(int triangle_index) {
	double ox = (V[3 * triangle_index].x + V[3 * triangle_index + 1].x + V[3 * triangle_index + 2].x) / 3.0;
	double oy = (V[3 * triangle_index].y + V[3 * triangle_index + 1].y + V[3 * triangle_index + 2].y) / 3.0;
	//cout << ox<<" " << oy<<endl;
	glm::mat3x3 trans1 = { {1,0,0} ,{0,1,0},{-ox,-oy,1} };
	glm::mat3x3 rot = { {glm::cos(10.0f / 180.0f * glm::pi<float>()),glm::sin(-10.0f / 180.0f * glm::pi<float>()),0} ,{glm::sin(10.0f / 180.0f * glm::pi<float>()),glm::cos(10.0f / 180.0f * glm::pi<float>()),0},{0,0,1} };
	glm::mat3x3 trans2 = { {1,0,0} ,{0,1,0},{ox,oy,1} };

	glm::vec3 temp1 = trans2 * rot * trans1 * glm::vec3(V[3 * triangle_index], 1.0f);
	V[3 * triangle_index].x = temp1.x;
	V[3 * triangle_index].y = temp1.y;



	glm::vec3 temp2 = trans2 * rot * trans1 * glm::vec3(V[3 * triangle_index + 1], 1.0f);
	V[3 * triangle_index + 1].x = temp2.x;
	V[3 * triangle_index + 1].y = temp2.y;



	glm::vec3 temp3 = trans2 * rot * trans1 * glm::vec3(V[3 * triangle_index + 2], 1.0f);
	V[3 * triangle_index + 2].x = temp3.x;
	V[3 * triangle_index + 2].y = temp3.y;

}

void rotate_cc(int triangle_index) {
	double ox = (V[3 * triangle_index].x + V[3 * triangle_index + 1].x + V[3 * triangle_index + 2].x) / 3.0;
	double oy = (V[3 * triangle_index].y + V[3 * triangle_index + 1].y + V[3 * triangle_index + 2].y) / 3.0;
	//cout << ox << " " << oy << endl;
	glm::mat3x3 trans1 = { {1,0,0} ,{0,1,0},{-ox,-oy,1} };
	glm::mat3x3 rot = { {glm::cos(10.0f / 180.0f * glm::pi<float>()),glm::sin(10.0f / 180.0f * glm::pi<float>()),0} ,{glm::sin(-10.0f / 180.0f * glm::pi<float>()),glm::cos(10.0f / 180.0f * glm::pi<float>()),0},{0,0,1} };
	glm::mat3x3 trans2 = { {1,0,0} ,{0,1,0},{ox,oy,1} };

	glm::vec3 temp1 = trans2 * rot * trans1 * glm::vec3(V[3 * triangle_index], 1.0f);
	V[3 * triangle_index].x = temp1.x;
	V[3 * triangle_index].y = temp1.y;



	glm::vec3 temp2 = trans2 * rot * trans1 * glm::vec3(V[3 * triangle_index + 1], 1.0f);
	V[3 * triangle_index + 1].x = temp2.x;
	V[3 * triangle_index + 1].y = temp2.y;



	glm::vec3 temp3 = trans2 * rot * trans1 * glm::vec3(V[3 * triangle_index + 2], 1.0f);
	V[3 * triangle_index + 2].x = temp3.x;
	V[3 * triangle_index + 2].y = temp3.y;

}


/*
void scale_up(int triangle_index, glm::mat3x3 scale) {
	double ox = (V[3 * triangle_index].x + V[3 * triangle_index + 1].x + V[3 * triangle_index + 2].x) / 3.0;
	double oy = (V[3 * triangle_index].y + V[3 * triangle_index + 1].y + V[3 * triangle_index + 2].y) / 3.0;
	

	glm::mat3x3 trans1 = { {1,0,0} ,{0,1,0},{-ox,-oy,1} };
	glm::mat3x3 trans2 = { {1,0,0} ,{0,1,0},{ox,oy,1} };

	
	glm::vec3 temp1 = trans2 * scale * trans1 * glm::vec3(V[3 * triangle_index], 1.0f);
	V[3 * triangle_index].x = temp1.x;
	V[3 * triangle_index].y = temp1.y;



	glm::vec3 temp2 = trans2 * scale * trans1 * glm::vec3(V[3 * triangle_index + 1], 1.0f);
	V[3 * triangle_index + 1].x = temp2.x;
	V[3 * triangle_index + 1].y = temp2.y;



	glm::vec3 temp3 = trans2 * scale * trans1 * glm::vec3(V[3 * triangle_index + 2], 1.0f);
	V[3 * triangle_index + 2].x = temp3.x;
	V[3 * triangle_index + 2].y = temp3.y;

	VBO.update(V);

}


void scale_down(int triangle_index) {
	double ox = (V[3 * triangle_index].x + V[3 * triangle_index + 1].x + V[3 * triangle_index + 2].x) / 3.0;
	double oy = (V[3 * triangle_index].y + V[3 * triangle_index + 1].y + V[3 * triangle_index + 2].y) / 3.0;

	glm::mat3x3 trans1 = { {1,0,0} ,{0,1,0},{-ox,-oy,1} };
	glm::mat3x3 scale = { {0.75f,0,0} ,{0,0.75f,0},{0,0,1} };
	glm::mat3x3 trans2 = { {1,0,0} ,{0,1,0},{ox,oy,1} };

	glm::vec3 temp1 = trans2 * scale * trans1 * glm::vec3(V[3 * triangle_index], 1.0f);
	V[3 * triangle_index].x = temp1.x;
	V[3 * triangle_index].y = temp1.y;



	glm::vec3 temp2 = trans2 * scale * trans1 * glm::vec3(V[3 * triangle_index + 1], 1.0f);
	V[3 * triangle_index + 1].x = temp2.x;
	V[3 * triangle_index + 1].y = temp2.y;



	glm::vec3 temp3 = trans2 * scale * trans1 * glm::vec3(V[3 * triangle_index + 2], 1.0f);
	V[3 * triangle_index + 2].x = temp3.x;
	V[3 * triangle_index + 2].y = temp3.y;

	VBO.update(V);

}
*/
void color_vertex(int vertex, int color) {
	switch(color){
	case 1:
		C[whichVertex] = glm::vec3(243.0 / 255.0, 226.0 / 255.0, 159.0 / 255.0);
		break;
	case 2:
		C[whichVertex] = glm::vec3(181.0 / 255.0, 222.0 / 255.0, 220.0 / 255.0);
		break;
	case 3:
		C[whichVertex] = glm::vec3(66.0 / 255.0, 156.0 / 255.0, 139.0 / 255.0);
		break;
	case 4:
		C[whichVertex] = glm::vec3(47.0 / 255.0, 182.0 / 255.0, 122.0 / 255.0);
		break;
	case 5:
		C[whichVertex] = glm::vec3(248.0 / 255.0, 207.0 / 255.0, 30.0 / 255.0);
		break;
	case 6:
		C[whichVertex] = glm::vec3(103.0 / 255.0, 143.0 / 255.0, 225.0 / 255.0);
		break;
	case 7:
		C[whichVertex] = glm::vec3(81.0 / 255.0, 18.0 / 255.0, 117.0 / 255.0);
		break;
	case 8:
		C[whichVertex] = glm::vec3(143.0 / 255.0, 53.0 / 255.0, 90.0 / 255.0);
		break;
	case 9:
		C[whichVertex] = glm::vec3(16.0 / 255.0, 250.0 / 255.0, 146.0 / 255.0);
		break;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case  GLFW_KEY_I://insertion
		mode = 1;
		break;
	case  GLFW_KEY_O://selection
		mode = 2;
		break;
	case  GLFW_KEY_P://deletion
		mode = 3;
		break;
	case  GLFW_KEY_C://color
		mode = 4;
		break;
	default:
		break;
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		MVP = glm::mat4({ {1,0,0,0},{0,1,0,0},{0,0,1,0},{0.2,0,0,1} }) * MVP;
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		MVP = glm::mat4({ {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,-0.2,0,1} }) * MVP;
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		MVP = glm::mat4({ {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0.2,0,1} }) * MVP;
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		MVP = glm::mat4({ {1,0,0,0},{0,1,0,0},{0,0,1,0},{-0.2,0,0,1} }) * MVP;
	}
	else if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
		MVP = glm::mat4({ {1.2,0,0,0},{0,1.2,0,0},{0,0,1.2,0},{0,0,0,1} }) * MVP;
	}
	else if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
		MVP = glm::mat4({ {0.8,0,0,0},{0,0.8,0,0},{0,0,0.8,0},{0,0,0,1} }) * MVP;
	}


	if (mode == 0) {
		switch (key) {
		case  GLFW_KEY_1:
			V[0] = glm::vec2(-0.5, 0.5);
			break;
		case GLFW_KEY_2:
			V[0] = glm::vec2(0, 0.5);
			break;
		case  GLFW_KEY_3:
			V[0] = glm::vec2(0.5, 0.5);
			break;
		}
		
	}

	if (mode == 2) {
		if (key == GLFW_KEY_H && action == GLFW_PRESS) {
			rotate_c(selected_triangle);
		}
		else if (key == GLFW_KEY_J && action == GLFW_PRESS) {
			rotate_cc(selected_triangle);
		}
		else if ((key == GLFW_KEY_K && action == GLFW_PRESS)) {
			k_pressed = true;
			t_pressed_k= std::chrono::high_resolution_clock::now();
			V0 = V[3*selected_triangle];
			V1 = V[3*selected_triangle+1];
			V2 = V[3*selected_triangle+2];
			ox = (V0.x + V1.x + V2.x) / 3.0;
			oy = (V0.y + V1.y + V2.y) / 3.0;
			
			//scale_up(selected_triangle);
		}
		else if (key == GLFW_KEY_L && action == GLFW_PRESS) {
			l_pressed = true;
			t_pressed_l = std::chrono::high_resolution_clock::now();
			V0 = V[3*selected_triangle];
			V1 = V[3*selected_triangle + 1];
			V2 = V[3*selected_triangle + 2];
			ox = (V0.x + V1.x + V2.x) / 3.0;
			oy = (V0.y + V1.y + V2.y) / 3.0;
			//scale_down(selected_triangle);
		}

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
			skip = true;
		}

	}

	if (mode == 4) {
		if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
			color_vertex(whichVertex, 1);
		}
		else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
			color_vertex(whichVertex, 2);
		}
		else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
			color_vertex(whichVertex, 3);
		}
		else if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
			color_vertex(whichVertex, 4);
		}
		else if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
			color_vertex(whichVertex, 5);
		}
		else if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
			color_vertex(whichVertex, 6);
		}
		else if (key == GLFW_KEY_7 && action == GLFW_PRESS) {
			color_vertex(whichVertex, 7);
		}
		else if (key == GLFW_KEY_8 && action == GLFW_PRESS) {
			color_vertex(whichVertex, 8);
		}
		else if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
			color_vertex(whichVertex, 9);
		}

	}

	VBO.update(V);
	VBO_C.update(C);
}

int main(void)
{
	GLFWwindow* window;

	// Initialize the library
	if (!glfwInit())
		return -1;

	// Activate supersampling
	glfwWindowHint(GLFW_SAMPLES, 8);

	// Ensure that we get at least a 3.2 context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	// On apple we have to load a core profile with forward compatibility
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	

#ifndef __APPLE__
	glewExperimental = true;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	glGetError(); // pull and savely ignonre unhandled errors like GL_INVALID_ENUM
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

	int major, minor, rev;
	major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
	minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
	rev = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
	printf("OpenGL version recieved: %d.%d.%d\n", major, minor, rev);
	printf("Supported OpenGL is %s\n", (const char*)glGetString(GL_VERSION));
	printf("Supported GLSL is %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Initialize the VAO
	// A Vertex Array Object (or VAO) is an object that describes how the vertex
	// attributes are stored in a Vertex Buffer Object (or VBO). This means that
	// the VAO is not the actual object storing the vertex data,
	// but the descriptor of the vertex data.
	VertexArrayObject VAO;
	VAO.init();
	VAO.bind();

	// Initialize the VBO with the vertices data
	// A VBO is a data container that lives in the GPU memory
	VBO.init();

	//V.resize(3);
	V[0] = glm::vec2(0, 0.5);
	V[1] = glm::vec2(-0.5, -0.5);
	V[2] = glm::vec2(0.5, -0.5);
	VBO.update(V);

	VBO_C.init();

	C.resize(300);
	for (int i = 0; i < 300; i++) {
		C[i]= glm::vec3(1, 0, 0);
	}
	

	VBO_C.update(C);


	// Initialize the OpenGL Program
	// A program controls the OpenGL pipeline and it must contains
	// at least a vertex shader and a fragment shader to be valid
	Program program;
	const GLchar* vertex_shader =
		"#version 150 core\n"
		"in vec2 position;"
		"in vec3 color;"
		"uniform mat4 MVP;"
		"out vec3 f_color;"
		"void main()"
		"{"
		"    gl_Position = MVP*vec4(position, 0.0, 1.0);"
		"    f_color = color;"
		"}";
	const GLchar* fragment_shader =
		"#version 150 core\n"
		"in vec3 f_color;"
		"out vec4 outColor;"
		"uniform vec3 triangleColor;"
		"void main()"
		"{"
		"    outColor = vec4(f_color,1.0);"
		"}";

	// Compile the two shaders and upload the binary to the GPU
	// Note that we have to explicitly specify that the output "slot" called outColor
	// is the one that we want in the fragment buffer (and thus on screen)
	program.init(vertex_shader, fragment_shader, "outColor");
	program.bind();

	// The vertex shader wants the position of the vertices as an input.
	// The following line connects the VBO we defined above with the position "slot"
	// in the vertex shader
	program.bindVertexAttribArray("position", VBO);
	program.bindVertexAttribArray("color", VBO_C);

	// Save the current time --- it will be used to dynamically change the triangle color
	auto t_start = std::chrono::high_resolution_clock::now();

	// Register the keyboard callback
	glfwSetKeyCallback(window, key_callback);

	// Register the mouse callback
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Update viewport
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(window, cursor_position_callback);

	//glm::mat3x3 scale_matrix = { {1.25f,0,0} ,{0,1.25f,0},{0,0,1} };
	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		// Bind your VAO (not necessary if you have only one)
		VAO.bind();
		
		// Bind your program
		program.bind();

		// Set the uniform value depending on the time difference
		
		//auto t_now = std::chrono::high_resolution_clock::now();
		//float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_cur - t_pressed).count();
		//glUniform3f(program.uniform("triangleColor"), (float)(sin(time * 4.0f) + 1.0f) / 1.5f, 0.8f, 0.0f);
		
		if (k_pressed == true) {
			auto t_now = std::chrono::high_resolution_clock::now();
			float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_pressed_k).count();

			if (skip == true) {
				time = 1;
				skip = false;
			}

			glm::mat3x3 scale = { {1 + time * 0.25,0,0} ,{0,1 + time * 0.25,0},{0,0,1} };

			glm::mat3x3 trans1 = { {1,0,0} ,{0,1,0},{-ox,-oy,1} };
			glm::mat3x3 trans2 = { {1,0,0} ,{0,1,0},{ox,oy,1} };
			cout << "main o: " << ox << " " << oy << endl;

			glm::vec3 temp1 = trans2 * scale * trans1 * glm::vec3(V0, 1.0f);
			
			V[3 * selected_triangle].x = temp1.x;
			V[3 * selected_triangle].y = temp1.y;
			//cout << 3 * selected_triangle << " " << V[3 * selected_triangle].x<<" "<< V[3 * selected_triangle].y<<endl;


			glm::vec3 temp2 = trans2 * scale * trans1 * glm::vec3(V1, 1.0f);
			
			V[3 * selected_triangle + 1].x = temp2.x;
			V[3 * selected_triangle + 1].y = temp2.y;



			glm::vec3 temp3 = trans2 * scale * trans1 * glm::vec3(V2, 1.0f);
			
			V[3 * selected_triangle + 2].x = temp3.x;
			V[3 * selected_triangle + 2].y = temp3.y;

			VBO.update(V);
			if (time >= 1) {
				//cout<<"false"<<endl;
				k_pressed = false;
			}
		}

		if (l_pressed == true) {
			auto t_now = std::chrono::high_resolution_clock::now();
			float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_pressed_l).count();

			if (skip == true) {
				time = 1;
				skip = false;
			}

			glm::mat3x3 scale = { {1 - time * 0.25,0,0} ,{0,1 - time * 0.25,0},{0,0,1} };


			glm::mat3x3 trans1 = { {1,0,0} ,{0,1,0},{-ox,-oy,1} };
			glm::mat3x3 trans2 = { {1,0,0} ,{0,1,0},{ox,oy,1} };


			glm::vec3 temp1 = trans2 * scale * trans1 * glm::vec3(V0, 1.0f);
			V[3 * selected_triangle].x = temp1.x;
			V[3 * selected_triangle].y = temp1.y;



			glm::vec3 temp2 = trans2 * scale * trans1 * glm::vec3(V1, 1.0f);
			V[3 * selected_triangle + 1].x = temp2.x;
			V[3 * selected_triangle + 1].y = temp2.y;



			glm::vec3 temp3 = trans2 * scale * trans1 * glm::vec3(V2, 1.0f);
			V[3 * selected_triangle + 2].x = temp3.x;
			V[3 * selected_triangle + 2].y = temp3.y;

			VBO.update(V);
			if (time >= 1) {
				//cout<<"false"<<endl;
				l_pressed = false;
			}
		}

		glUniformMatrix4fv(program.uniform("MVP"), 1, GL_FALSE, &MVP[0][0]);
		// Clear the framebuffer
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw a triangle
		glDrawArrays(GL_LINES, 3 * numOfTriangles, 2);
		//glDrawArrays(GL_LINES, 2, 3);
		//cout << "drawing"<<endl;
		glDrawArrays(GL_TRIANGLES, 0, 300);


		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	// Deallocate opengl memory
	program.free();
	VAO.free();
	VBO.free();
	VBO_C.free();

	// Deallocate glfw internals
	glfwTerminate();
	return 0;
}
