// This example is heavily based on the tutorial at https://open.gl

// OpenGL Helpers to reduce the clutter
#include "Helpers.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include<cmath>
#include <map>
#include <vector>
#include <algorithm>
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
#include <glm/gtc/type_ptr.hpp>
// Timer
#include <chrono>

using namespace std;


VertexBufferObject VBO_cube;
VertexBufferObject VBO_cube_color;
VertexBufferObject VBO_cube_normal;
std::vector<glm::vec3> V_cube(360);
std::vector<glm::vec3> C_cube(360);
std::vector<glm::vec3> N_cube(360);
std::vector<glm::vec3> N_cube_new(360);

VertexBufferObject VBO_bumpyCube;
VertexBufferObject VBO_bumpyCube_color;
VertexBufferObject VBO_bumpyCube_normal;
std::vector<glm::vec3> V_bumpyCube(30000);
std::vector<glm::vec3> C_bumpyCube(30000);
std::vector<glm::vec3> N_bumpyCube(30000);
std::vector<glm::vec3> N_bumpyCube_new(30000);

VertexBufferObject VBO_bunny;
VertexBufferObject VBO_bunny_color;
VertexBufferObject VBO_bunny_normal;
std::vector<glm::vec3> V_bunny(30000);
std::vector<glm::vec3> C_bunny(30000);
std::vector<glm::vec3> N_bunny(30000);
std::vector<glm::vec3> N_bunny_new(30000);

int object_ID=0;
float pixelData[4];
glm::mat4x4 projection = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };
glm::mat4x4 view = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };
glm::mat4x4 model_cube = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };
glm::mat4x4 model_bunny = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };
glm::mat4x4 model_bumpyCube = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };


int totalNumCube = 0;
int totalNumBumpyCube = 0;
int totalNumBunny = 0;

vector<glm::mat4x4> cube_models;
vector<int> cube_modes;
vector<glm::mat4x4> bumpyCube_models;
vector<int> bumpyCube_modes;
vector<glm::mat4x4> bunny_models;
vector<int> bunny_modes;

int selected_spieces = 0;
int selected_spieces_ID = 0;

void calculateAverageNormal(std::vector<glm::vec3> V, std::vector<glm::vec3> N, int start, int end, std::vector<glm::vec3> &N_new);
//std::vector<glm::vec3> calculateAverageNormal1(std::vector<glm::vec3> V, std::vector<glm::vec3> N, int start, int end);

/*
const float radius = 0.1f;
float angle1 = 0;
float camX = cos(angle1) * radius;
float camY = sin(angle1) * radius;
float angle2 = atan(1) * 2;
float camZ = cos(angle2) * radius;*/
//glm::mat4 view = glm::lookAt(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
glm::vec3 camPos_per= glm::vec3(1.0f,1.0f, 3.0f);
glm::vec3 camPos_ort = glm::vec3(0.2f, 0.2f, 0.3f);

float radius = 3.0f;
float pi = atan(1) * 4;
float theta = pi/2;
float phi = pi / 2;
float camX = radius*cos(theta)*sin(phi);
float camZ = radius * sin(theta) * sin(phi);
float camY = radius * cos(phi);
glm::vec3 camPos_per_traceball = glm::vec3(camX, camZ, camY);


int camMode = 0;

void addCube() {

	glm::mat4x4 id = glm::mat4(1.f);

	V_cube[0 + 36 * totalNumCube] = glm::vec3(-0.5f, -0.5f, -0.5f);
	V_cube[1 + 36 * totalNumCube] = glm::vec3(0.5f, -0.5f, -0.5f);
	V_cube[2 + 36 * totalNumCube] = glm::vec3(0.5f, 0.5f, -0.5f);
	V_cube[3 + 36 * totalNumCube] = glm::vec3(0.5f, 0.5f, -0.5f);
	V_cube[4 + 36 * totalNumCube] = glm::vec3(-0.5f, 0.5f, -0.5f);
	V_cube[5 + 36 * totalNumCube] = glm::vec3(-0.5f, -0.5f, -0.5f);
	V_cube[6 + 36 * totalNumCube] = glm::vec3(-0.5f, -0.5f, 0.5f);
	V_cube[7 + 36 * totalNumCube] = glm::vec3(0.5f, -0.5f, 0.5f);
	V_cube[8 + 36 * totalNumCube] = glm::vec3(0.5f, 0.5f, 0.5f);
	V_cube[9 + 36 * totalNumCube] = glm::vec3(0.5f, 0.5f, 0.5f);
	V_cube[10 + 36 * totalNumCube] = glm::vec3(-0.5f, 0.5f, 0.5f);
	V_cube[11 + 36 * totalNumCube] = glm::vec3(-0.5f, -0.5f, 0.5f);
	V_cube[12 + 36 * totalNumCube] = glm::vec3(-0.5f, 0.5f, 0.5f);
	V_cube[13 + 36 * totalNumCube] = glm::vec3(-0.5f, 0.5f, -0.5f);
	V_cube[14 + 36 * totalNumCube] = glm::vec3(-0.5f, -0.5f, -0.5f);
	V_cube[15 + 36 * totalNumCube] = glm::vec3(-0.5f, -0.5f, -0.5f);
	V_cube[16 + 36 * totalNumCube] = glm::vec3(-0.5f, -0.5f, 0.5f);
	V_cube[17 + 36 * totalNumCube] = glm::vec3(-0.5f, 0.5f, 0.5f);
	V_cube[18 + 36 * totalNumCube] = glm::vec3(0.5f, 0.5f, 0.5f);
	V_cube[19 + 36 * totalNumCube] = glm::vec3(0.5f, 0.5f, -0.5f);
	V_cube[20 + 36 * totalNumCube] = glm::vec3(0.5f, -0.5f, -0.5f);
	V_cube[21 + 36 * totalNumCube] = glm::vec3(0.5f, -0.5f, -0.5f);
	V_cube[22 + 36 * totalNumCube] = glm::vec3(0.5f, -0.5f, 0.5f);
	V_cube[23 + 36 * totalNumCube] = glm::vec3(0.5f, 0.5f, 0.5f);
	V_cube[24 + 36 * totalNumCube] = glm::vec3(-0.5f, -0.5f, -0.5f);
	V_cube[25 + 36 * totalNumCube] = glm::vec3(0.5f, -0.5f, -0.5f);
	V_cube[26 + 36 * totalNumCube] = glm::vec3(0.5f, -0.5f, 0.5f);
	V_cube[27 + 36 * totalNumCube] = glm::vec3(0.5f, -0.5f, 0.5f);
	V_cube[28 + 36 * totalNumCube] = glm::vec3(-0.5f, -0.5f, 0.5f);
	V_cube[29 + 36 * totalNumCube] = glm::vec3(-0.5f, -0.5f, -0.5f);
	V_cube[30 + 36 * totalNumCube] = glm::vec3(-0.5f, 0.5f, -0.5f);
	V_cube[31 + 36 * totalNumCube] = glm::vec3(0.5f, 0.5f, -0.5f);
	V_cube[32 + 36 * totalNumCube] = glm::vec3(0.5f, 0.5f, 0.5f);
	V_cube[33 + 36 * totalNumCube] = glm::vec3(0.5f, 0.5f, 0.5f);
	V_cube[34 + 36 * totalNumCube] = glm::vec3(-0.5f, 0.5f, 0.5f);
	V_cube[35 + 36 * totalNumCube] = glm::vec3(-0.5f, 0.5f, -0.5f);
	//cout << 35 + 36 * totalNumCube << endl;
	
	N_cube[0 + 36 * totalNumCube] = glm::vec3(0.0f, 0.0f, -1.0f);
	N_cube[1 + 36 * totalNumCube] = glm::vec3(0.0f, 0.0f, -1.0f);
	N_cube[2 + 36 * totalNumCube] = glm::vec3(0.0f, 0.0f, -1.0f);
	N_cube[3 + 36 * totalNumCube] = glm::vec3(0.0f, 0.0f, -1.0f);
	N_cube[4 + 36 * totalNumCube] = glm::vec3(0.0f, 0.0f, -1.0f);
	N_cube[5 + 36 * totalNumCube] = glm::vec3(0.0f, 0.0f, -1.0f);
	N_cube[6 + 36 * totalNumCube] = glm::vec3(0.0f, 0.0f, 1.0f);
	N_cube[7 + 36 * totalNumCube] = glm::vec3(0.0f, 0.0f, 1.0f);
	N_cube[8 + 36 * totalNumCube] = glm::vec3(0.0f, 0.0f, 1.0f);
	N_cube[9 + 36 * totalNumCube] = glm::vec3(0.0f, 0.0f, 1.0f);
	N_cube[10 + 36 * totalNumCube] = glm::vec3(0.0f, 0.0f, 1.0f);
	N_cube[11 + 36 * totalNumCube] = glm::vec3(0.0f, 0.0f, 1.0f);
	N_cube[12 + 36 * totalNumCube] = glm::vec3(-1.0f, 0.0f, 0.0f);
	N_cube[13 + 36 * totalNumCube] = glm::vec3(-1.0f, 0.0f, 0.0f);
	N_cube[14 + 36 * totalNumCube] = glm::vec3(-1.0f, 0.0f, 0.0f);
	N_cube[15 + 36 * totalNumCube] = glm::vec3(-1.0f, 0.0f, 0.0f);
	N_cube[16 + 36 * totalNumCube] = glm::vec3(-1.0f, 0.0f, 0.0f);
	N_cube[17 + 36 * totalNumCube] = glm::vec3(-1.0f, 0.0f, 0.0f);
	N_cube[18 + 36 * totalNumCube] = glm::vec3(1.0, 0.0f, 0.0f);
	N_cube[19 + 36 * totalNumCube] = glm::vec3(1.0, 0.0f, 0.0f);
	N_cube[20 + 36 * totalNumCube] = glm::vec3(1.0, 0.0f, 0.0f);
	N_cube[21 + 36 * totalNumCube] = glm::vec3(1.0, 0.0f, 0.0f);
	N_cube[22 + 36 * totalNumCube] = glm::vec3(1.0, 0.0f, 0.0f);
	N_cube[23 + 36 * totalNumCube] = glm::vec3(1.0, 0.0f, 0.0f);
	N_cube[24 + 36 * totalNumCube] = glm::vec3(0.0f, -1.0f, 0.0f);
	N_cube[25 + 36 * totalNumCube] = glm::vec3(0.0f, -1.0f, 0.0f);
	N_cube[26 + 36 * totalNumCube] = glm::vec3(0.0f, -1.0f, 0.0f);
	N_cube[27 + 36 * totalNumCube] = glm::vec3(0.0f, -1.0f, 0.0f);
	N_cube[28 + 36 * totalNumCube] = glm::vec3(0.0f, -1.0f, 0.0f);
	N_cube[29 + 36 * totalNumCube] = glm::vec3(0.0f, -1.0f, 0.0f);
	N_cube[30 + 36 * totalNumCube] = glm::vec3(0.0f, 1.0f, 0.0f);
	N_cube[31 + 36 * totalNumCube] = glm::vec3(0.0f, 1.0f, 0.0f);
	N_cube[32 + 36 * totalNumCube] = glm::vec3(0.0f, 1.0f, 0.0f);
	N_cube[33 + 36 * totalNumCube] = glm::vec3(0.0f, 1.0f, 0.0f);
	N_cube[34 + 36 * totalNumCube] = glm::vec3(0.0f, 1.0f, 0.0f);
	N_cube[35 + 36 * totalNumCube] = glm::vec3(0.0f, 1.0f, 0.0f);

	for (int i = 36 * totalNumCube; i < 36*(totalNumCube+1); i++) {
		C_cube[i] = glm::vec3(1.f, 0.5f, 0.31f);
	}

	
	//calculateAverageNormal(V_cube, N_cube, 36 * totalNumCube, 36 * (totalNumCube + 1) - 1, N_cube_new);
	totalNumCube++;
	VBO_cube.update(V_cube);
	VBO_cube_color.update(C_cube);
	VBO_cube_normal.update(N_cube);

	cube_models.push_back(id);
	cube_modes.push_back(0);
	
	//N_cube = calculateAverageNormal1(V_cube, N_cube, 0, 35);
	//VBO_cube_normal.update(N_cube);
}

void loadModel(string filename, vector<glm::vec3>& vertex, vector<glm::vec4>& face) {
	ifstream inFile;
	inFile.open(filename);
	if (!inFile) {
		cout << "File not open!" << endl;
	}
	string line;
	int vertex_count = 0, face_count = 0, edge_count = 0;
	getline(inFile, line);
	if (line == "OFF") {
		getline(inFile, line);
		istringstream iss(line);
		iss >> vertex_count >> face_count >> edge_count;
		//cout << vertex_count << face_count << edge_count << endl;
	}
	else {
		cout << "I dont expect that" << endl;
		cout << line;
	}

	for (int i = 0; i < vertex_count; i++) {
		getline(inFile, line);
		istringstream iss(line);
		glm::vec3 vx_i;
		iss >> vx_i.x >> vx_i.y >> vx_i.z;
		//cout<< vx_i.x << vx_i.y << vx_i.z<<endl;
		vertex.push_back(vx_i);
	}
	for (int j = 0; j < face_count; j++) {
		getline(inFile, line);
		istringstream iss(line);
		glm::vec4 face_i;
		iss >> face_i[0] >> face_i[1] >> face_i[2] >> face_i[3];
		//cout << face_i.x << face_i.y << face_i.z << face_i[3] << endl;
		face.push_back(face_i);
	}


}

glm::mat4x4 normalizeModel(vector<glm::vec3> vertex) {
	glm::mat4x4 id = glm::mat4(1.f);
	double x_min = vertex[0].x, y_min = vertex[0].y, z_min = vertex[0].z, x_max = vertex[0].x, y_max = vertex[0].y, z_max = vertex[0].z;
	for (int i = 0; i < vertex.size(); i++) {
		if (vertex[i].x < x_min) {
			x_min = vertex[i].x;
		}
		if (vertex[i].x > x_max) {
			x_max = vertex[i].x;
		}
		if (vertex[i].y < y_min) {
			y_min = vertex[i].y;
		}
		if (vertex[i].y > y_max) {
			y_max = vertex[i].y;
		}
		if (vertex[i].z < z_min) {
			z_min = vertex[i].z;
		}
		if (vertex[i].z > z_max) {
			z_max = vertex[i].z;
		}
	}

	glm::mat4x4 scale = glm::scale(id, glm::vec3(1 / (x_max - x_min), 1 / (y_max - y_min), 1 / (z_max - z_min)));
	glm::mat4x4 trans = glm::translate(id, glm::vec3(-(x_min + x_max) / (2 * (x_max - x_min)), -(y_min + y_max) / (2 * (y_max - y_min)), -(z_min + z_max) / (2 * (z_max - z_min))));

	return trans * scale;
}

void addBumpyCube() {
	vector<glm::vec3> vertex;
	vector<glm::vec4> face;
	loadModel("C:\\Users\\roder\\Desktop\\Computer graphics\\base\\Assignment_3\\data\\bumpy_cube.off", vertex, face);


	for (int i = 0; i < face.size(); i++) {
		glm::vec4 vec = face[i];
		V_bumpyCube[(3 * i) + 3 * 1000 * (totalNumBumpyCube)] = vertex[vec[1]];
		V_bumpyCube[(3 * i + 1) + 3 * 1000 * (totalNumBumpyCube)] = vertex[vec[2]];
		V_bumpyCube[(3 * i + 2)  + 3 * 1000 * (totalNumBumpyCube)] = vertex[vec[3]];
		//cout << "Current vertex index used up to " << (3 * i + 2) + 36 * totalNumCube + 3 * 1000 * (totalNumBumpyCube + totalNumBunny) << endl;
	}
	

	
	for (int i = 3000 * totalNumBumpyCube; i < 3000*(totalNumBumpyCube+1); i++) {
		C_bumpyCube[i] = glm::vec3(1.f, 0.5f, 0.31f);
	}

	for (int i = 3000 * totalNumBumpyCube; i < 3000 * (totalNumBumpyCube + 1); i++) {
		if (i % 3 == 0) {
			glm::vec3 vec1 = V_bumpyCube[i + 1] - V_bumpyCube[i];
			glm::vec3 vec2 = V_bumpyCube[i + 2] - V_bumpyCube[i];
			N_bumpyCube[i] = glm::normalize(glm::cross(vec1, vec2));
			i++;
		}
		if (i % 3 == 1) {
			glm::vec3 vec1 = V_bumpyCube[i + 1] - V_bumpyCube[i];
			glm::vec3 vec2 = V_bumpyCube[i - 1] - V_bumpyCube[i];
			N_bumpyCube[i] = glm::normalize(glm::cross(vec1, vec2));
			i++;
		}
		if (i % 3 == 2) {
			glm::vec3 vec1 = V_bumpyCube[i - 2] - V_bumpyCube[i];
			glm::vec3 vec2 = V_bumpyCube[i - 1] - V_bumpyCube[i];
			N_bumpyCube[i] = glm::normalize(glm::cross(vec1, vec2));
			i++;
		}
	}

	//calculateAverageNormal(V_bumpyCube, N_bumpyCube, 3000 * totalNumBumpyCube, 3000 * (totalNumBumpyCube + 1) - 1, N_bumpyCube_new);
	totalNumBumpyCube++;
	VBO_bumpyCube.update(V_bumpyCube);
	VBO_bumpyCube_color.update(C_bumpyCube);
	VBO_bumpyCube_normal.update(N_bumpyCube);
	model_bumpyCube = normalizeModel(vertex);
	bumpyCube_models.push_back(model_bumpyCube);
	bumpyCube_modes.push_back(0);
	
}

void addBunny() {
	vector<glm::vec3> vertex;
	vector<glm::vec4> face;
	loadModel("C:\\Users\\roder\\Desktop\\Computer graphics\\base\\Assignment_3\\data\\bunny.off", vertex, face);
	for (int i = 0; i < face.size(); i++) {
		glm::vec4 vec = face[i];
		//cout << vec[1] << " " << vec[2] << ' ' << vec[3] << endl;
		V_bunny[(3 * i)  + 3 * 1000 * ( totalNumBunny)] = vertex[vec[1]];
		V_bunny[(3 * i + 1) + 3 * 1000 * (totalNumBunny)] = vertex[vec[2]];
		V_bunny[(3 * i + 2)  + 3 * 1000 * ( totalNumBunny)] = vertex[vec[3]];
		//cout << "Current vertex index used up to " << (3 * i + 2)  + 3 * 1000 * (totalNumBunny) << endl;
	}


	for (int i = 3000 * totalNumBunny; i < 3000 * (totalNumBunny+1); i++) {
		C_bunny[i] = glm::vec3(1.0f,0.5f, (totalNumBunny+1) / 255.0f);
	}

	for (int i = 3000 * totalNumBunny; i < 3000 * (totalNumBunny + 1); i++) {
		if (i % 3 == 0) {
			glm::vec3 vec1 = V_bunny[i + 1] - V_bunny[i];
			glm::vec3 vec2 = V_bunny[i + 2] - V_bunny[i];
			N_bunny[i] = glm::normalize(glm::cross(vec1, vec2));
			i++;
		}
		if (i % 3 == 1) {
			glm::vec3 vec1 = V_bunny[i + 1] - V_bunny[i];
			glm::vec3 vec2 = V_bunny[i - 1] - V_bunny[i];
			N_bunny[i] = glm::normalize(glm::cross(vec1, vec2));
			i++;
		}
		if (i % 3 == 2) {
			glm::vec3 vec1 = V_bunny[i - 2] - V_bunny[i];
			glm::vec3 vec2 = V_bunny[i - 1] - V_bunny[i];
			N_bunny[i] = glm::normalize(glm::cross(vec1, vec2));
			i++;
		}
	}

	//calculateAverageNormal(V_bunny, N_bunny, 3000 * totalNumBunny, 3000 * (totalNumBunny + 1) - 1, N_bunny_new);
	totalNumBunny++;
	VBO_bunny.update(V_bunny);
	VBO_bunny_color.update(C_bunny);
	VBO_bunny_normal.update(N_bunny);
	model_bunny = normalizeModel(vertex);
	bunny_models.push_back(model_bunny);
	bunny_modes.push_back(0);
	
}

void translateRight(int i, int obj_id) {
	if (i == 0) {
		cube_models[obj_id] = glm::translate(glm::mat4(1), glm::vec3(0.2f, 0.0f, 0.0f))*cube_models[obj_id];
	}
	else if (i == 1) {
		bumpyCube_models[obj_id] = glm::translate(glm::mat4(1), glm::vec3(0.2f, 0.0f, 0.0f))*bumpyCube_models[obj_id];
	}
	else if (i == 2) {
		bunny_models[obj_id] = glm::translate(glm::mat4(1), glm::vec3(0.2f, 0.0f, 0.0f))*bunny_models[obj_id];
	}
}

void translateLeft(int i, int obj_id) {
	if (i == 0) {
		cube_models[obj_id] = glm::translate(glm::mat4(1), glm::vec3(-0.2f, 0.0f, 0.0f)) * cube_models[obj_id];
	}
	else if (i == 1) {
		bumpyCube_models[obj_id] = glm::translate(glm::mat4(1), glm::vec3(-0.2f, 0.0f, 0.0f)) * bumpyCube_models[obj_id];
	}
	else if (i == 2) {
		bunny_models[obj_id] = glm::translate(glm::mat4(1), glm::vec3(-0.2f, 0.0f, 0.0f)) * bunny_models[obj_id];
	}
}

void translateUp(int i, int obj_id) {

	if (i == 0) {
		cube_models[obj_id] = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.2f, 0.0f)) * cube_models[obj_id];
	}
	else if (i == 1) {
		bumpyCube_models[obj_id] = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.2f, 0.0f)) * bumpyCube_models[obj_id];
	}
	else if (i == 2) {
		bunny_models[obj_id] = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.2f, 0.0f)) * bunny_models[obj_id];
	}
	
}

void translateDown(int i, int obj_id) {
	if (i == 0) {
		cube_models[obj_id] = glm::translate(glm::mat4(1), glm::vec3(0.0f, -0.2f, 0.0f)) * cube_models[obj_id];
	}
	else if (i == 1) {
		bumpyCube_models[obj_id] = glm::translate(glm::mat4(1), glm::vec3(0.0f, -0.2f, 0.0f)) * bumpyCube_models[obj_id];
	}
	else if (i == 2) {
		bunny_models[obj_id] = glm::translate(glm::mat4(1), glm::vec3(0.0f, -0.2f, 0.0f)) * bunny_models[obj_id];
	}
}

void translateForward(int i, int obj_id) {
	if (i == 0) {
		cube_models[obj_id] = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, 0.2f)) * cube_models[obj_id];
	}
	else if (i == 1) {
		bumpyCube_models[obj_id] = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, 0.2f)) * bumpyCube_models[obj_id];
	}
	else if (i == 2) {
		bunny_models[obj_id] = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, 0.2f)) * bunny_models[obj_id];
	}
}

void translateBack(int i, int obj_id) {
	if (i == 0) {
		cube_models[obj_id] = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, -0.2f)) * cube_models[obj_id];
	}
	else if (i == 1) {
		bumpyCube_models[obj_id] = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, -0.2f)) * bumpyCube_models[obj_id];
	}
	else if (i == 2) {
		bunny_models[obj_id] = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, -0.2f)) * bunny_models[obj_id];
	}
}

void scaleUp(int i, int obj_id) {
	if (i == 0) {
		cube_models[obj_id] = glm::scale(glm::mat4(1), glm::vec3(1.2f, 1.2f, 1.2f))*cube_models[obj_id];
	}
	else if (i == 1) {
		bumpyCube_models[obj_id] = glm::scale(glm::mat4(1), glm::vec3(1.2f, 1.2f, 1.2f))*bumpyCube_models[obj_id];
	}
	else if (i == 2) {
		bunny_models[obj_id] = glm::scale(glm::mat4(1), glm::vec3(1.2f, 1.2f, 1.2f))*bunny_models[obj_id];
	}
}

void scaleDown(int i, int obj_id) {

	if (i == 0) {
		cube_models[obj_id] = glm::scale(glm::mat4(1), glm::vec3(0.8f, 0.8f, 0.8f)) * cube_models[obj_id];
	}
	else if (i == 1) {
		bumpyCube_models[obj_id] = glm::scale(glm::mat4(1), glm::vec3(0.8f, 0.8f, 0.8f)) * bumpyCube_models[obj_id];
	}
	else if (i == 2) {
		bunny_models[obj_id] = glm::scale(glm::mat4(1), glm::vec3(0.8f, 0.8f, 0.8f)) * bunny_models[obj_id];
	}

}

void rotateClockwise(int i, int obj_id) {
	if (i == 0) {
		glm::vec4 sum = glm::vec4(0.f,0.f,0.f,0.f);
		for (int j = 36 * obj_id; j < 36 * (obj_id + 1); j++) {
			sum += cube_models[obj_id] * glm::vec4(V_cube[j], 1.0f);
		}
		glm::vec3 barycenter = glm::vec3(sum[0]/sum[3],sum[1]/sum[3],sum[2]/sum[3]);
		cube_models[obj_id] = glm::translate(glm::mat4(1), barycenter)*glm::rotate(glm::mat4(1), glm::radians(20.f),glm::vec3(1.0f, 1.0f, 0.0f))* glm::translate(glm::mat4(1), -barycenter) * cube_models[obj_id];
	}
	else if (i == 1) {
		glm::vec4 sum = glm::vec4(0.f, 0.f, 0.f, 0.f);
		for (int j = 3000 * obj_id; j < 3000 * (obj_id + 1); j++) {
			sum += bumpyCube_models[obj_id] * glm::vec4(V_bumpyCube[j], 1.0f);
		}
		glm::vec3 barycenter = glm::vec3(sum[0] / sum[3], sum[1] / sum[3], sum[2] / sum[3]);
		bumpyCube_models[obj_id] = glm::translate(glm::mat4(1), barycenter) * glm::rotate(glm::mat4(1), glm::radians(20.f), glm::vec3(1.0f, 1.0f, 0.0f)) * glm::translate(glm::mat4(1), -barycenter) *bumpyCube_models[obj_id];
	}
	else if (i == 2) {
		glm::vec4 sum = glm::vec4(0.f, 0.f, 0.f, 0.f);
		for (int j = 3000 * obj_id; j < 3000 * (obj_id + 1); j++) {
			sum += bunny_models[obj_id] * glm::vec4(V_bunny[j], 1.0f);
		}
		glm::vec3 barycenter = glm::vec3(sum[0] / sum[3], sum[1] / sum[3], sum[2] / sum[3]);
		bunny_models[obj_id] = glm::translate(glm::mat4(1), barycenter) * glm::rotate(glm::mat4(1), glm::radians(20.f), glm::vec3(1.0f, 1.0f, 0.0f)) * glm::translate(glm::mat4(1), -barycenter) *bunny_models[obj_id];
	}
}

void rotateCounterClockwise(int i, int obj_id) {
	if (i == 0) {
		glm::vec4 sum = glm::vec4(0.f, 0.f, 0.f, 0.f);
		for (int j = 36 * obj_id; j < 36 * (obj_id + 1); j++) {
			sum += cube_models[obj_id] * glm::vec4(V_cube[j], 1.0f);
		}
		glm::vec3 barycenter = glm::vec3(sum[0] / sum[3], sum[1] / sum[3], sum[2] / sum[3]);
		cube_models[obj_id] = glm::translate(glm::mat4(1), barycenter) * glm::rotate(glm::mat4(1), -glm::radians(20.f), glm::vec3(1.0f, 1.0f, 0.0f)) * glm::translate(glm::mat4(1), -barycenter) * cube_models[obj_id];
	}
	else if (i == 1) {
		glm::vec4 sum = glm::vec4(0.f, 0.f, 0.f, 0.f);
		for (int j = 3000 * obj_id; j < 3000 * (obj_id + 1); j++) {
			sum += bumpyCube_models[obj_id] * glm::vec4(V_bumpyCube[j], 1.0f);
		}
		glm::vec3 barycenter = glm::vec3(sum[0] / sum[3], sum[1] / sum[3], sum[2] / sum[3]);
		bumpyCube_models[obj_id] = glm::translate(glm::mat4(1), barycenter) * glm::rotate(glm::mat4(1), -glm::radians(20.f), glm::vec3(1.0f, 1.0f, 0.0f)) * glm::translate(glm::mat4(1), -barycenter) * bumpyCube_models[obj_id];
	}
	else if (i == 2) {
		glm::vec4 sum = glm::vec4(0.f, 0.f, 0.f, 0.f);
		for (int j = 3000 * obj_id; j < 3000 * (obj_id + 1); j++) {
			sum += bunny_models[obj_id] * glm::vec4(V_bunny[j], 1.0f);
		}
		glm::vec3 barycenter = glm::vec3(sum[0] / sum[3], sum[1] / sum[3], sum[2] / sum[3]);
		bunny_models[obj_id] = glm::translate(glm::mat4(1), barycenter) * glm::rotate(glm::mat4(1), -glm::radians(20.f), glm::vec3(1.0f, 1.0f, 0.0f)) * glm::translate(glm::mat4(1), -barycenter) * bunny_models[obj_id];
	}
}

void calculateAverageNormal(std::vector<glm::vec3> V, std::vector<glm::vec3> N, int start, int end, std::vector<glm::vec3> &N_new){
	vector<int> already_calculated(V.size(), 0);
	vector<int> count(V.size(),1);
	N_new = N;

	for (int i = start; i <= end; i++) {
		if (already_calculated[i] == 0) {
			already_calculated[i] = 1;
			for (int j = i + 1; j < V.size(); j++) {
				if ((V[i] == V[j])) {
					already_calculated[j] = 1;
					N[i] += N[j];
					count[i]++;
				}
			}
		}
		else {
			for (int j = 0; j < i; j++) {
				if (V[i] == V[j]) {
					N[i] = N[j];
					count[j] = count[i];
				}
			}
		}
	}
	//cout << "count of V[0] is " << count[0]<<endl;
	for (int i = start; i <= end; i++) {
		N_new[i].x = N[i].x / count[i];
		N_new[i].y = N[i].y / count[i];
		N_new[i].z = N[i].z / count[i];
		if (N_new[i].x != 0 || N_new[i].y != 0 || N_new[i].z != 0) {
			N_new[i] = glm::normalize(N[i]);
		}
		//cout << N_new[i].x << " " << N_new[i].y<<" " << N_new[i].z << " " << i << endl;
	}

	//return N_new;
}

int returnIterator(vector<glm::vec3> Uni_V, glm::vec3 V_i) {
	for (int i = 0; i <= Uni_V.size(); i++) {
		if (Uni_V[i]==V_i) {
			return i;
		}
	}

	return -1;
}
/*
std::vector<glm::vec3> calculateAverageNormal1(std::vector<glm::vec3> V, std::vector<glm::vec3> N, int start, int end) {
	vector<glm::vec3> N_new = N;

	vector<glm::vec3> unique_V;
	vector<int> unique_V_count;
	vector<glm::vec3> unique_N;
	for (int i = start; i <= end; i++) {
		int iterator= returnIterator(unique_V, V[i]);
		if (iterator!=-1) {//already have
			unique_N[iterator] += N[i];
			unique_V_count[iterator]+=1;
		}
		else {//not have
			unique_V.push_back(V[i]);
			unique_V_count.push_back(1);
			unique_N.push_back(N[i]);
		}
	}

	for (int i = 0; i < unique_V.size(); i++) {
		unique_N[i].x = unique_N[i].x / unique_V_count[i];
		unique_N[i].y = unique_N[i].y / unique_V_count[i];
		unique_N[i].z = unique_N[i].z / unique_V_count[i];

		unique_N[i] = glm::normalize(unique_N[i]);
	}

	int current = 0;
	for (int i = start; i <= end; i++) {
		if (V[i] == unique_V[current]) {
			N[i] = unique_N[current];
			current++;
		}
	}

}
*/
void drawStuff(Program program, vector<glm::mat4x4> models, VertexBufferObject VBO, VertexBufferObject VBO_color, VertexBufferObject VBO_normal) {

	

	for (int i = 0; i < models.size(); i++) {
		glUniformMatrix4fv(program.uniform("model"), 1, GL_FALSE, &models[i][0][0]);
		program.bindVertexAttribArray("position", VBO);
		program.bindVertexAttribArray("color", VBO_color);
		program.bindVertexAttribArray("normal", VBO_normal);
		glDrawArrays(GL_TRIANGLES, 0, 30000);
	}

}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	
	if ((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_PRESS)) {

		for (int i = 36 * selected_spieces_ID; i < 36 * (selected_spieces_ID + 1); i++) {
			C_cube[i] = glm::vec3(1.0f, 0.5f, 0.31f);
		}
		VBO_cube_color.update(C_cube);
		for (int i = 3000 * selected_spieces_ID; i < 3000 * (selected_spieces_ID + 1); i++) {
			C_bumpyCube[i] = glm::vec3(1.0f, 0.5f, 0.31f);
		}
		VBO_bumpyCube_color.update(C_bumpyCube);
		for (int i = 3000 * selected_spieces_ID; i < 3000 * (selected_spieces_ID + 1); i++) {
			C_bunny[i] = glm::vec3(1.0f, 0.5f, 0.31f);
		}
		VBO_bunny_color.update(C_bunny);

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		double xworld = ((xpos / double(width)) * 2) - 1;
		double yworld = (((height - 1 - ypos) / double(height)) * 2) - 1;
		glm::vec4 new_cord = glm::inverse(view) * glm::vec4(xworld, yworld, 0.0, 1.0);
		xworld = new_cord.x;
		yworld = new_cord.y;
		glFlush();
		glFinish();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glReadPixels(xpos, height - 1 - ypos, 1, 1, GL_STENCIL_INDEX, GL_FLOAT, &pixelData);
		

		selected_spieces = ((int)pixelData[0]) / 10;
		selected_spieces_ID = ((int)pixelData[0]) % 10;
		cout << selected_spieces << " " << selected_spieces_ID << endl;
		
		if (selected_spieces == 0) {
			for (int i = 36 * selected_spieces_ID; i < 36 * (selected_spieces_ID + 1); i++) {
				C_cube[i] = glm::vec3(0.4f, 0.3f, 0.7f);
			}
			VBO_cube_color.update(C_cube);
		}
		else if (selected_spieces == 1) {
			for (int i = 3000 * selected_spieces_ID; i < 3000 * (selected_spieces_ID + 1); i++) {
				C_bumpyCube[i] = glm::vec3(0.4f, 0.3f, 0.7f);
			}
			VBO_bumpyCube_color.update(C_bumpyCube);
		}
		else if (selected_spieces == 2) {
			for (int i = 3000 * selected_spieces_ID; i < 3000 * (selected_spieces_ID + 1); i++) {
				C_bunny[i] = glm::vec3(0.4f, 0.3f, 0.7f);
			}
			VBO_bunny_color.update(C_bunny);
		}

	}
	
		
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {


}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		addCube();
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		addBumpyCube();
	}
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		addBunny();
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		translateLeft(selected_spieces, selected_spieces_ID);
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		translateRight(selected_spieces, selected_spieces_ID);
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		translateUp(selected_spieces, selected_spieces_ID);
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		translateDown(selected_spieces, selected_spieces_ID);
	}
	else if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		translateForward(selected_spieces, selected_spieces_ID);
	}
	else if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		translateBack(selected_spieces, selected_spieces_ID);
	}
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		rotateClockwise(selected_spieces, selected_spieces_ID);
	}
	else if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		rotateCounterClockwise(selected_spieces, selected_spieces_ID);
	}
	else if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
		scaleDown(selected_spieces, selected_spieces_ID);
	}
	else if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
		scaleUp(selected_spieces, selected_spieces_ID);
	}
	else if (key == GLFW_KEY_I && action == GLFW_PRESS) {
		if (selected_spieces == 0) {
			cube_modes[selected_spieces_ID] = 1;
		}
		else if (selected_spieces == 1) {
			bumpyCube_modes[selected_spieces_ID] = 1;
		}
		else if (selected_spieces == 2) {
			bunny_modes[selected_spieces_ID] = 1;
		}
	}
	else if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		if (selected_spieces == 0) {
			cube_modes[selected_spieces_ID] = 2;
		}
		else if (selected_spieces == 1) {
			bumpyCube_modes[selected_spieces_ID] = 2;
		}
		else if (selected_spieces == 2) {
			bunny_modes[selected_spieces_ID] = 2;
		}
	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		if (selected_spieces == 0) {
			cube_modes[selected_spieces_ID] = 3;
		}
		else if (selected_spieces == 1) {
			bumpyCube_modes[selected_spieces_ID] = 3;
		}
		else if (selected_spieces == 2) {
			bunny_modes[selected_spieces_ID] = 3;
		}
	}
	else if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		if (camMode == 0) {
			camPos_per.y += 0.1;
		}
		else if (camMode == 1) {
			camPos_ort.y += 0.1;
		}
		else if (camMode == 2) {
			phi -= pi / 8;
		}
		
	}
	else if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		if (camMode == 0) {
			camPos_per.x -= 0.1;
		}
		else if (camMode == 1) {
			camPos_ort.x -= 0.1;
		}
		else if (camMode == 2) {
			theta += pi / 8;
		}
	}
	else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		if (camMode == 0) {
			camPos_per.y -= 0.1;
		}
		else if (camMode == 1) {
			camPos_ort.y -= 0.1;
		}
		else if (camMode == 2) {
			phi += pi / 8;
		}
	}
	else if (key == GLFW_KEY_H && action == GLFW_PRESS) {
		if (camMode == 0) {
			camPos_per.x += 0.1;
		}
		else if (camMode == 1) {
			camPos_ort.x += 0.1;
		}
		else if (camMode == 2) {
			theta -= pi / 8;
		}
	}
	else if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		if (camMode == 0) {
			camPos_per.z += 0.1;
		}
		else if (camMode == 1) {
			camPos_ort.z += 0.1;
		}
		else if (camMode == 2) {
			radius -= 0.2;
		}
	}
	else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		if (camMode == 0) {
			camPos_per.z -= 0.1;
		}
		else if (camMode == 1) {
			camPos_ort.z -= 0.1;
		}
		else if (camMode == 2) {
			radius += 0.2;
		}
	}
	else if (key == GLFW_KEY_R && action == GLFW_PRESS) {//ortho cam
		camMode = 0;
	}
	else if (key == GLFW_KEY_Y && action == GLFW_PRESS) {//per cam
		camMode = 1;
	}
	else if (key == GLFW_KEY_U && action == GLFW_PRESS) {//per cam
		camMode = 2;
	}
	VBO_bumpyCube_normal.update(N_bumpyCube);
	VBO_bunny_normal.update(N_bunny);
	VBO_cube_normal.update(N_cube);

	VBO_bumpyCube_color.update(C_bumpyCube);
	VBO_bunny_color.update(C_bunny);
	VBO_cube_color.update(C_cube);

	VBO_bumpyCube.update(V_bumpyCube);
	VBO_bunny.update(V_bunny);
	VBO_cube.update(V_cube);
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

	VertexArrayObject VAO;
	VAO.init();
	VAO.bind();

	VBO_cube.init();
	VBO_cube_color.init();
	VBO_cube_normal.init();
	C_cube.resize(360);

	VBO_bumpyCube.init();
	VBO_bumpyCube_color.init();
	VBO_bumpyCube_normal.init();
	C_bumpyCube.resize(30000);

	VBO_bunny.init();
	VBO_bunny_color.init();
	VBO_bunny_normal.init();
	C_bunny.resize(30000);

	for (int i = 0; i < V_cube.size(); i++) {
		V_cube[i] = glm::vec3(0, 0, 0);
	}
	VBO_cube.update(V_cube);
	VBO_cube_color.update(C_cube);
	VBO_cube_normal.update(N_cube);

	for (int i = 0; i < V_cube.size(); i++) {
		V_bumpyCube[i] = glm::vec3(0, 0, 0);
	}
	VBO_bumpyCube.update(V_cube);
	VBO_bumpyCube_color.update(C_cube);

	for (int i = 0; i < V_bunny.size(); i++) {
		V_bunny[i] = glm::vec3(0, 0, 0);
	}
	VBO_bunny.update(V_bunny);
	VBO_bunny_color.update(C_bunny);

	Program program;
	//Reference from https://learnopengl.com/Lighting/Basic-Lighting
	const GLchar* vertex_shader =
		"#version 150 core\n"
		"in vec3 position;"
		"in vec3 color;"
		"in vec3 normal;"
		"uniform mat4 projection;"
		"uniform mat4 view;"
		"uniform mat4 model;"
		"out vec3 f_color;"
		"out vec3 Normal;"
		"out vec3 FragPos;"
		"void main()"
		"{"
		"    FragPos=vec3(model*vec4(position,1.0));"
		"    Normal=mat3(transpose(inverse(model)))*normal;"
		"    gl_Position = projection*view*model*vec4(position, 1.0);"
		"    f_color = color;"
		"}";
	const GLchar* fragment_shader =
		"#version 150 core\n"
		"in vec3 f_color;"
		"in vec3 FragPos;"
		"in vec3 Normal;"
		"uniform vec3 lightPos;"
		"uniform vec3 ViewPos;"
		"uniform vec3 lightColor;"//color of light
		"out vec4 outColor;"
		"void main()"
		"{"
		//ambient
		"    float ambientStrength=0.1f;"
		"    vec3 ambient=ambientStrength*lightColor;"
		//diffuse
		"    vec3 norm=normalize(Normal);"
		"    vec3 lightDirection=normalize(lightPos-FragPos);"
		"    float diff=max(dot(norm,lightDirection),0.0);"
		"    vec3 diffuse=diff*lightColor;"
		//specular
		"    float specularStrength=1.5f;"
		"    float faceLight=0.0f;"
		"    vec3 viewDirection=normalize(ViewPos-FragPos);"
		"    vec3 reflectDirection=reflect(-lightDirection,norm);"
		"    float spec=pow(max(dot(viewDirection,reflectDirection),0.0),32);"
		"    vec3 specular=specularStrength*spec*lightColor;"
		"	 if(diff>0){faceLight=1.0;};"
		"    vec3 result=(ambient+diffuse+specular*faceLight)*f_color;"
		"    outColor = vec4(result,1.0);"
		"}";


	program.init(vertex_shader, fragment_shader, "outColor");
	program.bind();
	
	

	auto t_start = std::chrono::high_resolution_clock::now();

	glfwSetKeyCallback(window, key_callback);

	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(window, cursor_position_callback);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{

		VAO.bind();

		program.bind();
		glUniform3f(program.uniform("lightColor"), 1.0f, 1.0f, 1.0f);
		glUniform3f(program.uniform("lightPos"), 1.2f, 1.0f, 3.0f);
		//place of camera
		//glUniform3f(program.uniform("ViewPos"), camPos_per.x, camPos_per.y,camPos_per.z);

		glUniformMatrix4fv(program.uniform("projection"), 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(program.uniform("view"), 1, GL_FALSE, &view[0][0]);

		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

		//projection = glm::mat4(1);
		//projection = glm::ortho(-2.5f, 2.5f, -2.5f, 2.5f, -2.5f, 2.5f);

		
		if (camMode == 0) {
			glUniform3f(program.uniform("ViewPos"), camPos_per.x, camPos_per.y, camPos_per.z);
			view = glm::lookAt(glm::vec3(camPos_per.x, camPos_per.y, camPos_per.z), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
			int height, width;
			glfwGetWindowSize(window, &width, &height);

			projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 1.5f, 100.0f);
		}
		else if (camMode == 1) {
			glUniform3f(program.uniform("ViewPos"), camPos_ort.x, camPos_ort.y, camPos_ort.z);
			view = glm::lookAt(glm::vec3(camPos_ort.x, camPos_ort.y, camPos_ort.z), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
			projection = glm::ortho(-1.5f,1.5f, -1.5f, 1.5f, -1.5f, 1.5f);
		}
		else if (camMode == 2) {
			camX = radius * cos(theta) * sin(phi);
			camZ = radius * sin(theta) * sin(phi);
			camY = radius * cos(phi);
			glUniform3f(program.uniform("ViewPos"), camPos_per.x, camPos_per.y, camPos_per.z);
			view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
			int height, width;
			glfwGetWindowSize(window, &width, &height);

			projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 1.5f, 100.0f);

			
		}
		

		
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		
		

		for (int i = 0; i < cube_models.size(); i++) {
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, i, 0xFF);
			glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
			glUniformMatrix4fv(program.uniform("model"), 1, GL_FALSE, &cube_models[i][0][0]);
			program.bindVertexAttribArray("position", VBO_cube);
			program.bindVertexAttribArray("color", VBO_cube_color);
			program.bindVertexAttribArray("normal", VBO_cube_normal);
			if (cube_modes[i]==0) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawArrays(GL_TRIANGLES, 36*i, 36);
			}
			else if(cube_modes[i] == 1){
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawArrays(GL_TRIANGLES, 36 * i, 36);
			}
			else if (cube_modes[i] == 2) {
				std::vector<glm::vec3> C_cube_temp = C_cube;
				std::vector<glm::vec3> C_cube_mode2(360,glm::vec3(0.0f,0.0f,0.0f));
				C_cube = C_cube_mode2;
				VBO_cube_color.update(C_cube);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawArrays(GL_TRIANGLES, 36 * i, 36);
				C_cube = C_cube_temp;
				VBO_cube_color.update(C_cube);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawArrays(GL_TRIANGLES, 36 * i, 36);
			}
			else if (cube_modes[i] == 3) {
				std::vector<glm::vec3> N_temp=N_cube;
				N_cube = N_cube_new;
				VBO_cube_normal.update(N_cube);
				N_cube = N_temp;
				glDrawArrays(GL_TRIANGLES, 36 * i, 36);
			}
			
		}

		//drawStuff(program, cube_models, VBO_cube, VBO_cube_color,VBO_cube_normal);



		for (int i = 0; i < bumpyCube_models.size(); i++) {
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 10+i, 0xFF);
			glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
			glUniformMatrix4fv(program.uniform("model"), 1, GL_FALSE, &bumpyCube_models[i][0][0]);
			program.bindVertexAttribArray("position", VBO_bumpyCube);
			program.bindVertexAttribArray("color", VBO_bumpyCube_color);
			program.bindVertexAttribArray("normal", VBO_bumpyCube_normal);
			if (bumpyCube_modes[i] == 0) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawArrays(GL_TRIANGLES, 3000 * i, 3000);
			}
			else if (bumpyCube_modes[i] == 1) {

				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawArrays(GL_TRIANGLES, 3000 * i, 3000);
			}
			else if (bumpyCube_modes[i] == 2) {
				std::vector<glm::vec3> C_bumpyCube_temp = C_bumpyCube;
				std::vector<glm::vec3> C_bumpyCube_mode2(30000, glm::vec3(0.0f, 0.0f, 0.0f));
				C_bumpyCube = C_bumpyCube_mode2;
				VBO_bumpyCube_color.update(C_bumpyCube);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawArrays(GL_TRIANGLES, 3000 * i, 3000);
				C_bumpyCube = C_bumpyCube_temp;
				VBO_bumpyCube_color.update(C_bumpyCube);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawArrays(GL_TRIANGLES, 3000 * i, 3000);
			}
			else if (bumpyCube_modes[i] == 3) {
				std::vector<glm::vec3> N_temp = N_bumpyCube;
				N_bumpyCube = N_bumpyCube_new;
				VBO_bumpyCube_normal.update(N_bumpyCube);
				N_bumpyCube = N_temp;
				glDrawArrays(GL_TRIANGLES, 3000 * i, 3000);
			}

		}
		//drawStuff(program, bumpyCube_models, VBO_bumpyCube, VBO_bumpyCube_color,VBO_bumpyCube_normal);

		
		for (int i = 0; i < bunny_models.size(); i++) {
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 20 + i, 0xFF);
			glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
			glUniformMatrix4fv(program.uniform("model"), 1, GL_FALSE, &bunny_models[i][0][0]);
			program.bindVertexAttribArray("position", VBO_bunny);
			program.bindVertexAttribArray("color", VBO_bunny_color);
			program.bindVertexAttribArray("normal", VBO_bunny_normal);

			if (bunny_modes[i] == 0) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawArrays(GL_TRIANGLES, 3000 * i, 3000);
			}
			else if (bunny_modes[i] == 1) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawArrays(GL_TRIANGLES, 3000 * i, 3000);
			}
			else if (bunny_modes[i] == 2) {
				std::vector<glm::vec3> C_bunny_temp = C_bunny;
				std::vector<glm::vec3> C_bunny_mode2(30000, glm::vec3(0.0f, 0.0f, 0.0f));
				C_bunny = C_bunny_mode2;
				VBO_bunny_color.update(C_bunny);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawArrays(GL_TRIANGLES, 3000 * i, 3000);
				C_bunny = C_bunny_temp;
				VBO_bunny_color.update(C_bunny);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawArrays(GL_TRIANGLES, 3000 * i, 3000);
			}
			else if (bunny_modes[i] == 3) {
				std::vector<glm::vec3> N_temp = N_bunny;
				N_bunny = N_bunny_new;
				VBO_bumpyCube_normal.update(N_bunny);
				N_bunny = N_temp;
				glDrawArrays(GL_TRIANGLES, 3000 * i, 3000);
			}
		}
		//drawStuff(program, bunny_models, VBO_bunny, VBO_bunny_color,VBO_bunny_normal);
	


		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	program.free();
	VAO.free();
	VBO_cube.free();
	VBO_cube_color.free();

	VBO_bunny.free();
	VBO_bunny_color.free();

	glfwTerminate();
	return 0;
}
