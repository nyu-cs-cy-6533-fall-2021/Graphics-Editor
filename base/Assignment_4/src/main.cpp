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
#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

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

VertexBufferObject VBO_plane;
VertexBufferObject VBO_plane_color;
VertexBufferObject VBO_plane_normal;
VertexBufferObject VBO_plane_texcord;
std::vector<glm::vec3> V_plane(100);
std::vector<glm::vec3> C_plane(100);
std::vector<glm::vec3> N_plane(100);
std::vector<glm::vec3> N_plane_new(100);
std::vector<glm::vec2> T_plane(100);

VertexBufferObject skyBoxVBO;
std::vector<glm::vec3> skyBox(100);
vector<string> skyFace(6, "");
float drawingPlane = 0.0;

vector<float> cubeMirror;
vector<float> bunnyMirror;
vector<float> bumpyMirror;



float shadowMode = 0.0;
float planeMode = 0.0;

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

glm::vec3 camPos_per= glm::vec3(1.0f,2.0f, 4.0f);
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

unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


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

	
	calculateAverageNormal(V_cube, N_cube, 36 * totalNumCube, 36 * (totalNumCube + 1) - 1, N_cube_new);
	totalNumCube++;
	VBO_cube.update(V_cube);
	VBO_cube_color.update(C_cube);
	VBO_cube_normal.update(N_cube);

	cube_models.push_back(id);
	cube_modes.push_back(0);
	cubeMirror.push_back(0.0);
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

	calculateAverageNormal(V_bumpyCube, N_bumpyCube, 3000 * totalNumBumpyCube, 3000 * (totalNumBumpyCube + 1) - 1, N_bumpyCube_new);
	totalNumBumpyCube++;
	VBO_bumpyCube.update(V_bumpyCube);
	VBO_bumpyCube_color.update(C_bumpyCube);
	VBO_bumpyCube_normal.update(N_bumpyCube);
	model_bumpyCube = normalizeModel(vertex);
	bumpyCube_models.push_back(model_bumpyCube);
	bumpyCube_modes.push_back(0);
	bumpyMirror.push_back(0.0);
	
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

	calculateAverageNormal(V_bunny, N_bunny, 3000 * totalNumBunny, 3000 * (totalNumBunny + 1) - 1, N_bunny_new);
	totalNumBunny++;
	VBO_bunny.update(V_bunny);
	VBO_bunny_color.update(C_bunny);
	VBO_bunny_normal.update(N_bunny);
	model_bunny = normalizeModel(vertex);
	bunny_models.push_back(model_bunny);
	bunny_modes.push_back(0);
	bunnyMirror.push_back(0.0);
}

void deleteObject(int i, int obj_id) {
	if (i == 0) {
		cube_models[obj_id] = glm::translate(glm::mat4(0), glm::vec3(0.0f, 0.0f, 0.0f)) * cube_models[obj_id];
	}
	else if (i == 1) {
		bumpyCube_models[obj_id] = glm::translate(glm::mat4(0), glm::vec3(0.0f, 0.0f, 0.0f)) * bumpyCube_models[obj_id];
	}
	else if (i == 2) {
		bunny_models[obj_id] = glm::translate(glm::mat4(0), glm::vec3(0.0f, 0.0f, 0.0f)) * bunny_models[obj_id];
	}
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
	//add models
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		addCube();
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		addBumpyCube();
	}
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		addBunny();
	}
	//transforms
	else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		deleteObject(selected_spieces, selected_spieces_ID);
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
	//camera
	else if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		if (camMode == 0) {
			camPos_per.y += 0.2;
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
			camPos_per.x -= 0.2;
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
			camPos_per.y -= 0.2;
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
			camPos_per.x += 0.2;
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
	//shadowMode control
	else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		shadowMode = 1.0;
	}
	else if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
		shadowMode = 0.0;
	}
	//mirror/glass cortrol
	else if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		//cube
		if (selected_spieces == 0) {
			cubeMirror[selected_spieces_ID] = 1.0;
		}
		//bumpy
		if (selected_spieces == 1) {
			bumpyMirror[selected_spieces_ID] = 1.0;
		}
		//bunny
		if (selected_spieces == 2) {
			bunnyMirror[selected_spieces_ID] = 1.0;
		}

	}
	else if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		//cube
		if (selected_spieces == 0) {
			cubeMirror[selected_spieces_ID] = 0.0;
		}
		//bumpy
		if (selected_spieces == 1) {
			bumpyMirror[selected_spieces_ID] = 0.0;
		}
		//bunny
		if (selected_spieces == 2) {
			bunnyMirror[selected_spieces_ID] = 0.0;
		}
	}
	else if (key == GLFW_KEY_COMMA && action == GLFW_PRESS) {
		//cube
		if (selected_spieces == 0) {
			cubeMirror[selected_spieces_ID] = 2.0;
		}
		//bumpy
		if (selected_spieces == 1) {
			bumpyMirror[selected_spieces_ID] = 2.0;
		}
		//bunny
		if (selected_spieces == 2) {
			bunnyMirror[selected_spieces_ID] = 2.0;
		}
	}
	else if (key == GLFW_KEY_8 && action == GLFW_PRESS) {
		//plane
		if (planeMode == 0.0) {
			planeMode = 1.0;
		}
		else if (planeMode == 1.0) {
			planeMode = 0.0;
		}
	}
	else if (key == GLFW_KEY_I && action == GLFW_PRESS) {//ortho cam
		camMode = 0;
	}
	/*
	else if (key == GLFW_KEY_Y && action == GLFW_PRESS) {//per cam
		camMode = 1;
	}
	*/
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

	VBO_plane.init();
	V_plane[0] = glm::vec3(4.0f, -0.5f, 4.0f);
	V_plane[1] = glm::vec3(-4.0f, -0.5f, 4.0f);
	V_plane[2] = glm::vec3(-4.0f, -0.5f, -4.0f);
	V_plane[3] = glm::vec3(4.0f, -0.5f, 4.0f);
	V_plane[4] = glm::vec3(-4.0f, -0.5f, -4.0f);
	V_plane[5] = glm::vec3(4.0f, -0.5f, -4.0f);
	VBO_plane.update(V_plane);

	VBO_plane_color.init();
	C_plane.resize(100);
	for (int i = 0; i < 6; i++) {
		C_plane[i] = glm::vec3(0.871, 0.722, 0.529);
	}
	VBO_plane_color.update(C_plane);

	VBO_plane_normal.init();
	N_plane[0] = glm::vec3(0.0f, 1.0f, 0.0f);
	N_plane[1] = glm::vec3(0.0f, 1.0f, 0.0f);
	N_plane[2] = glm::vec3(0.0f, 1.0f, 0.0f);
	N_plane[3] = glm::vec3(0.0f, 1.0f, 0.0f);
	N_plane[4] = glm::vec3(0.0f, 1.0f, 0.0f);
	N_plane[5] = glm::vec3(0.0f, 1.0f, 0.0f);
	VBO_plane_normal.update(N_plane);


	VBO_plane_texcord.init();
	T_plane[0] = glm::vec2(1.0f, 0.0f);
	T_plane[1] = glm::vec2(0.0f, 0.0f);
	T_plane[2] = glm::vec2(0.0f, 1.0f);
	T_plane[3] = glm::vec2(1.0f, 0.0f);
	T_plane[4] = glm::vec2(0.0f, 1.0f);
	T_plane[5] = glm::vec2(1.0f, 1.0f);
	VBO_plane_texcord.update(T_plane);

	skyBoxVBO.init();
	skyBox[0] = glm::vec3(-1.0f, 1.0f, -1.0f);
	skyBox[1] = glm::vec3(-1.0f, -1.0f, -1.0f);
	skyBox[2] = glm::vec3(1.0f, -1.0f, -1.0f);
	skyBox[3] = glm::vec3(1.0f, -1.0f, -1.0f);
	skyBox[4] = glm::vec3(1.0f, 1.0f, -1.0f);
	skyBox[5] = glm::vec3(-1.0f, 1.0f, -1.0f);

	skyBox[6] = glm::vec3(-1.0f, -1.0f, 1.0f);
	skyBox[7] = glm::vec3(-1.0f, -1.0f, -1.0f);
	skyBox[8] = glm::vec3(-1.0f, 1.0f, -1.0f);
	skyBox[9] = glm::vec3(-1.0f, 1.0f, -1.0f);
	skyBox[10] = glm::vec3(-1.0f, 1.0f, 1.0f);
	skyBox[11] = glm::vec3(-1.0f, -1.0f, 1.0f);

	skyBox[12] = glm::vec3(1.0f, -1.0f, -1.0f);
	skyBox[13] = glm::vec3(1.0f, -1.0f, 1.0f);
	skyBox[14] = glm::vec3(1.0f, 1.0f, 1.0f);
	skyBox[15] = glm::vec3(1.0f, 1.0f, 1.0f);
	skyBox[16] = glm::vec3(1.0f, 1.0f, -1.0f);
	skyBox[17] = glm::vec3(1.0f, -1.0f, -1.0f);

	skyBox[18] = glm::vec3(-1.0f, -1.0f, 1.0f);
	skyBox[19] = glm::vec3(-1.0f, 1.0f, 1.0f);
	skyBox[20] = glm::vec3(1.0f, 1.0f, 1.0f);
	skyBox[21] = glm::vec3(1.0f, 1.0f, 1.0f);
	skyBox[22] = glm::vec3(1.0f, -1.0f, 1.0f);
	skyBox[23] = glm::vec3(-1.0f, -1.0f, 1.0f);

	skyBox[24] = glm::vec3(-1.0f, 1.0f, -1.0f);
	skyBox[25] = glm::vec3(1.0f, 1.0f, -1.0f);
	skyBox[26] = glm::vec3(1.0f, 1.0f, 1.0f);
	skyBox[27] = glm::vec3(1.0f, 1.0f, 1.0f);
	skyBox[28] = glm::vec3(-1.0f, 1.0f, 1.0f);
	skyBox[29] = glm::vec3(-1.0f, 1.0f, -1.0f);

	skyBox[30] = glm::vec3(-1.0f, -1.0f, -1.0f);
	skyBox[31] = glm::vec3(-1.0f, -1.0f, 1.0f);
	skyBox[32] = glm::vec3(1.0f, -1.0f, -1.0f);
	skyBox[33] = glm::vec3(1.0f, -1.0f, -1.0f);
	skyBox[34] = glm::vec3(-1.0f, -1.0f, 1.0f);
	skyBox[35] = glm::vec3(1.0f, -1.0f, 1.0f);
	skyBoxVBO.update(skyBox);

	skyFace[0] = "C:\\Users\\roder\\Desktop\\Computer graphics\\base\\Assignment_4\\data\\night_posx.png";
	skyFace[1] = "C:\\Users\\roder\\Desktop\\Computer graphics\\base\\Assignment_4\\data\\night_negx.png";
	skyFace[2] = "C:\\Users\\roder\\Desktop\\Computer graphics\\base\\Assignment_4\\data\\night_posy.png";
	skyFace[3] = "C:\\Users\\roder\\Desktop\\Computer graphics\\base\\Assignment_4\\data\\night_negy.png";
	skyFace[4] = "C:\\Users\\roder\\Desktop\\Computer graphics\\base\\Assignment_4\\data\\night_posz.png";
	skyFace[5] = "C:\\Users\\roder\\Desktop\\Computer graphics\\base\\Assignment_4\\data\\night_negz.png";

	unsigned int cubemapTexture = loadCubemap(skyFace);

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

	//Reference from https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Program program;
	//Reference from https://learnopengl.com/Lighting/Basic-Lighting
	const GLchar* vertex_shader =
		"#version 150 core\n"
		"in vec3 position;"
		"in vec3 color;"
		"in vec3 normal;"
		"in vec2 texcoords;"
		"uniform mat4 projection;"
		"uniform mat4 view;"
		"uniform mat4 model;"
		"uniform mat4 lightSpaceMatrix;"
		"out vec3 f_color;"
		"out vec3 Normal;"
		"out vec3 FragPos;"
		"out vec2 TexCoords;"
		"out vec4 FragPosLightSpace;"
		"out vec3 Position;"
		"void main()"
		"{"
		"    Position=vec3(model*vec4(position,1.0));"
		"    FragPos=vec3(model*vec4(position,1.0));"
		"    Normal=mat3(transpose(inverse(model)))*normal;"
		"    gl_Position = projection*view*model*vec4(position, 1.0);"
		"    TexCoords=texcoords;"
		"    FragPosLightSpace=lightSpaceMatrix*vec4(FragPos,1.0);"
		"    f_color = color;"
		"}";
	const GLchar* fragment_shader =
		"#version 150 core\n"
		"in vec3 f_color;"
		"in vec3 FragPos;"
		"in vec3 Normal;"
		"in vec2 TexCoords;"
		"in vec4 FragPosLightSpace;"
		"in vec3 Position;"

		"uniform vec3 lightPos;"
		"uniform vec3 ViewPos;"
		"uniform vec3 lightColor;"//color of light
		"uniform sampler2D diffuseTexture;"
		"uniform sampler2D shadowMap;"
		"uniform float shadowMode;"
		"uniform samplerCube skybox;"
		"uniform float drawPlane;"
		"uniform float mirror;"
		"uniform float planeMode;"
		"out vec4 outColor;"
		//"uniform vec3 triangleColor;"
		"float ShadowCalculation(vec4 fragPosLightSpace)"
		"{"
		// perform perspective divide
		"vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;"
		// transform to [0,1] range
		"projCoords = projCoords * 0.5 + 0.5;"
		// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
		"float closestDepth = texture(shadowMap, projCoords.xy).r;"
		//"if(projCoords.z>1.0){projCoords.z=1.0;};"
		// get depth of current fragment from light's perspective
		"float currentDepth = projCoords.z;"
		// calculate bias (based on depth map resolution and slope)
		"vec3 normal = normalize(Normal);"
		"vec3 lightDir = normalize(lightPos - FragPos);"
		"float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);"
		// check whether current frag pos is in shadow
			//"float shadow = currentDepth > closestDepth ? 1.0 : 0.0;"
			//"return shadow;"
		"float shadow=0.0;"
		"vec2 texelSize=1.0/textureSize(shadowMap,0);"
		"for(int x=-1; x<=1; ++x)"
		"{"
			"for(int y = -1; y <= 1; ++y)"
			"{"
				"float pcfDepth=texture(shadowMap,projCoords.xy+vec2(x,y)*texelSize).r;"
				"shadow+=currentDepth-bias>pcfDepth ? 1.0:0.0;"
			"}"
		"}"
		"shadow/=9.0;"
		"if(projCoords.z>1.0){shadow=0.0;};"
		"return shadow;"
		"}"

		"void main()"
		"{"
		"    vec3 color = texture(diffuseTexture, TexCoords).rgb;"
		"    vec3 norm=normalize(Normal);"
		//"vec3 lightColor = vec3(0.3);"
		//ambient
		"    float ambientStrength=0.3f;"
		"    vec3 ambient=ambientStrength*lightColor;"
		//diffuse
		"    vec3 lightDirection=normalize(lightPos-FragPos);"
		"    float diff=max(dot(lightDirection,norm),0.0);"
		"    vec3 diffuse=diff*lightColor;"
		//specular
		"    vec3 viewDirection=normalize(ViewPos-FragPos);"
		"    vec3 reflectDirection=reflect(-lightDirection,norm);"
		"    float spec=0.0;"
		"    vec3 halfwayDir=normalize(lightDirection+viewDirection);"
		"    spec=pow(max(dot(norm,halfwayDir),0.0),64);"
		"    vec3 specular=spec*lightColor;"
		//"    if(diff>0){lightSide=1.0;};"
		//"    vec3 specular=specularStrength*spec*lightColor;"
		"    float shadow=ShadowCalculation(FragPosLightSpace);"
		"    vec3 result=(ambient+(1-shadow)*(diffuse+specular))*f_color;"
		//"    if(isOffScreen==0.0f){ outColor = vec4(result,1.0);};"
		"   if(drawPlane==1.0)"
			"{"
			"    if(shadowMode==0.0)"
				"{"
					"if(planeMode==0.0)"
					"{"
					"   outColor=vec4(result,1.0);"
					"}"
					"if(planeMode==1.0)"
					"{"
					"   float ratio=1.00;"
					"   vec3 I=normalize(Position-ViewPos);"
					"   vec3 R=refract(I,normalize(Normal),ratio);"
					"   outColor=vec4(texture(skybox,R).rgb,1.0);"
					"}"
				"}"
			"    if(shadowMode==1.0)"
				"{"
				"	if(shadow==1){outColor=vec4(1.0,0,0,1.0);}"
				"   if(shadow!=1){ outColor = vec4(result,1.0);};"
				"}"
			"}"

		"   if(drawPlane==0.0)"
			"{"
			"   if(mirror==1.0)"
				"{"
				"   vec3 I=normalize(Position-ViewPos);"
				"   vec3 R=reflect(I,normalize(Normal));"
				"   outColor=vec4(texture(skybox,R).rgb,1.0);"
				"}"
			"   if(mirror==2.0)"
				"{"
				"   float ratio=0.8;"
				"   vec3 I=normalize(Position-ViewPos);"
				"   vec3 R=refract(I,normalize(Normal),ratio);"
				"   outColor=vec4(texture(skybox,R).rgb,1.0);"
				"}"
			"   if(mirror==0.0)"
				"{"
					"if(shadowMode==0.0)"
					"{"
						"outColor=vec4(result,1.0);"
					"}"
					"if(shadowMode==1.0)"
					"{"
						"if(shadow==1){outColor=vec4(1.0,0,0,1.0);}"
						"if(shadow!=1){ outColor = vec4(result,1.0);};"
					"}"
				"}"
			"}"
		"}";



	program.init(vertex_shader, fragment_shader, "outColor");
	program.bind();
	glUniform1i(program.uniform("diffuseTexture"), 0);
	glUniform1i(program.uniform("shadowMap"), 1);


	Program program2;//depth mapping
	const GLchar* vertex_shader_depth =
		"#version 150 core\n"
		"in vec3 position;"

		"uniform mat4 model;"
		"uniform mat4 lightSpaceMatrix;"

		"void main()"
		"{"
		"    gl_Position = lightSpaceMatrix*model*vec4(position, 1.0);"
		"}";
	const GLchar* fragment_shader_depth =
		//code learned from openGL website
		"#version 150 core\n"
		"out vec4 outColor2;"
		"void main()"
		"{"
		"}";

	program2.init(vertex_shader_depth, fragment_shader_depth, "outColor2");

	Program program3;//drawing skybox
	const GLchar* vertex_shader_skyBox =
		"#version 150 core\n"
		"in vec3 position;"
		"out vec3 TexCoords;"
		"uniform mat4 projection;"
		"uniform mat4 view;"

		"void main()"
		"{"
		"     TexCoords = position;"
		"     vec4 pos = projection * view * vec4(position, 1.0);"
		"     gl_Position = pos.xyww;"
		"}";
	const GLchar* fragment_shader_skyBox =
		"#version 150 core\n"
		"out vec4 outColor3;"
		"in vec3 TexCoords;"
		"uniform samplerCube skybox;"
		"void main()"
		"{"
		"	outColor3 = texture(skybox,TexCoords);"
		"}";

	program3.init(vertex_shader_skyBox, fragment_shader_skyBox, "outColor3");

	glfwSetKeyCallback(window, key_callback);

	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(window, cursor_position_callback);

	glEnable(GL_DEPTH_TEST);
	

	
	while (!glfwWindowShouldClose(window))
	{
		

		glUniform1f(program.uniform("shadowMode"), shadowMode);
		glUniform1f(program.uniform("planeMode"), planeMode);

		//Configure Light
		glUniform3f(program.uniform("lightColor"), 1.0f, 1.0f, 1.0f);
		glm::vec3 lightPos = glm::vec3(3.0f*cos(glfwGetTime()), 3.5f, 3.0f*sin(glfwGetTime()));
		glUniform3f(program.uniform("lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 8.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;


		//Configure camera
		glUniformMatrix4fv(program.uniform("projection"), 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(program.uniform("view"), 1, GL_FALSE, &view[0][0]);

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
			projection = glm::ortho(-1.5f, 1.5f, -1.5f, 1.5f, -1.5f, 1.5f);
		}
		else if (camMode == 2) {
			camX = radius * cos(theta) * sin(phi);
			camZ = radius * sin(theta) * sin(phi);
			camY = radius * cos(phi);
			glUniform3f(program.uniform("ViewPos"), camPos_per.x, camPos_per.y, camPos_per.z);
			view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
			int height, width;
			glfwGetWindowSize(window, &width, &height);

			projection = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 1.5f, 100.0f);

		}
		

		VAO.bind();
		//first time Render scene
		program2.bind();
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glUniformMatrix4fv(program2.uniform("lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		for (int i = 0; i < cube_models.size(); i++) {
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, i, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glUniformMatrix4fv(program2.uniform("model"), 1, GL_FALSE, &cube_models[i][0][0]);
			program2.bindVertexAttribArray("position", VBO_cube);
			VBO_cube_normal.update(N_cube_new);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 36 * i, 36);

		}
		for (int i = 0; i < bumpyCube_models.size(); i++) {
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 10 + i, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glUniformMatrix4fv(program2.uniform("model"), 1, GL_FALSE, &bumpyCube_models[i][0][0]);
			program2.bindVertexAttribArray("position", VBO_bumpyCube);

			VBO_bumpyCube_normal.update(N_bumpyCube_new);
			glDrawArrays(GL_TRIANGLES, 3000 * i, 3000);

		}
		for (int i = 0; i < bunny_models.size(); i++) {
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 20 + i, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glUniformMatrix4fv(program2.uniform("model"), 1, GL_FALSE, &bunny_models[i][0][0]);
			program2.bindVertexAttribArray("position", VBO_bunny);

			VBO_bunny_normal.update(N_bunny_new);
			glDrawArrays(GL_TRIANGLES, 3000 * i, 3000);
		}
		drawingPlane = 1.0;//start drawing plane
		glm::mat4 floorModel = glm::mat4(1.0f);
		glUniformMatrix4fv(program2.uniform("model"), 1, GL_FALSE, &floorModel[0][0]);
		program2.bindVertexAttribArray("position", VBO_plane);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		drawingPlane = 0.0;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		

		int height = 0;
		int width = 0;
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//second time render scene
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		program.bind();
		glUniformMatrix4fv(program.uniform("lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
		glUniform1i(program.uniform("skybox"), 0);

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		
		drawingPlane = 1.0;
		glUniform1f(program.uniform("drawPlane"), drawingPlane);
		glUniformMatrix4fv(program.uniform("model"), 1, GL_FALSE, &floorModel[0][0]);
		program.bindVertexAttribArray("position", VBO_plane);
		program.bindVertexAttribArray("normal", VBO_plane_normal);
		program.bindVertexAttribArray("texcoords", VBO_plane_texcord);
		program.bindVertexAttribArray("color",VBO_plane_color);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		drawingPlane = 0.0;
		glUniform1f(program.uniform("drawPlane"), drawingPlane);

		for (int i = 0; i < cube_models.size(); i++) {
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, i, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glUniformMatrix4fv(program.uniform("model"), 1, GL_FALSE, &cube_models[i][0][0]);
			program.bindVertexAttribArray("position", VBO_cube);
			program.bindVertexAttribArray("color", VBO_cube_color);
			program.bindVertexAttribArray("normal", VBO_cube_normal);
			glUniform1f(program.uniform("mirror"), cubeMirror[i]);
			
			VBO_cube_normal.update(N_cube_new);
			glDrawArrays(GL_TRIANGLES, 36 * i, 36);
			
		}
		for (int i = 0; i < bumpyCube_models.size(); i++) {
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 10+i, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glUniformMatrix4fv(program.uniform("model"), 1, GL_FALSE, &bumpyCube_models[i][0][0]);
			program.bindVertexAttribArray("position", VBO_bumpyCube);
			program.bindVertexAttribArray("color", VBO_bumpyCube_color);
			program.bindVertexAttribArray("normal", VBO_bumpyCube_normal);
			glUniform1f(program.uniform("mirror"), bumpyMirror[i]);

			VBO_bumpyCube_normal.update(N_bumpyCube_new);
			glDrawArrays(GL_TRIANGLES, 3000 * i, 3000);

		}
		for (int i = 0; i < bunny_models.size(); i++) {
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 20 + i, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glUniformMatrix4fv(program.uniform("model"), 1, GL_FALSE, &bunny_models[i][0][0]);
			program.bindVertexAttribArray("position", VBO_bunny);
			program.bindVertexAttribArray("color", VBO_bunny_color);
			program.bindVertexAttribArray("normal", VBO_bunny_normal);
			glUniform1f(program.uniform("mirror"), bunnyMirror[i]);
			VBO_bunny_normal.update(N_bunny_new);
			glDrawArrays(GL_TRIANGLES, 3000 * i, 3000);
		}
		glDepthFunc(GL_LEQUAL);

		//draw skyBox
		program3.bind();
		glUniform1i(program3.uniform("skybox"), 0);
		glm::mat4 newView = glm::mat4(glm::mat3(view)); 
		glUniformMatrix4fv(program3.uniform("projection"), 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(program3.uniform("view"), 1, GL_FALSE, &newView[0][0]);
		program3.bindVertexAttribArray("position", skyBoxVBO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); 


		program.bind();
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
