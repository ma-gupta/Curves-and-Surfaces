/*
 *Assignment 3: Bezier, B-Spline Curves and Surfaces
 *@author Manisha Gupta
 *@version 1.0
 *
 *Course: CPSC 453 L01 F2020
 *Instructor: Faramarz Famil Samavati
 *
 * Utilized skeleton/boilerplate code provided on D2L for CPSC 453
 */
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <limits>
#include <functional>

#include "Geometry.h"
#include "GLDebug.h"
#include "Log.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>


// We gave this code in one of the tutorials, so leaving it here too
void updateGPUGeometry(GPU_Geometry &gpuGeom, CPU_Geometry const &cpuGeom) {
	gpuGeom.bind();
	gpuGeom.setVerts(cpuGeom.verts);
	gpuGeom.setCols(cpuGeom.cols);
}

// EXAMPLE CALLBACKS
class Assignment3 : public CallbackInterface {

public:
	Assignment3(ShaderProgram& shader, int screenWidth, int screenHeight) :
		shader(shader),
		screenDim(screenWidth, screenHeight),
		leftMousePressed(false),
		rightMousePressed(false),
		leftMouseReleased(false),
		refresh(false),
		curveBez(false),
		curveBS(false),
		scene1(false),
		scene2(false),
		scene3(false),
		scene4(false),
		scene5(false),
		moveForward(false),
		moveBack(false),
		moveLeft(false),
		moveRight(false)
	{}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_R && action == GLFW_PRESS) {
			shader.recompile();
		}
		if (key == GLFW_KEY_F5 && action == GLFW_PRESS) {
			refresh = true;
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
			curveBez = true;
		}
		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
			curveBS = true;
		}
		if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
			scene1 = true;
		}
		if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
			scene2 = true;
		}
		if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
			scene3 = true;
		}
		if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
			scene4 = true;
		}
		if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
			scene5 = true;
		}
		if (key == GLFW_KEY_W && action == GLFW_PRESS) {
			moveForward = true;
		}
		if (key == GLFW_KEY_A && action == GLFW_PRESS) {
			moveLeft = true;
		}
		if (key == GLFW_KEY_S && action == GLFW_PRESS) {
			moveBack = true;
		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS) {
			moveRight = true;
		}
	}
	virtual void mouseButtonCallback(int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			leftMousePressed = true;
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			leftMouseReleased = true;
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			rightMousePressed = true;
		}
	}
	
	virtual void windowSizeCallback(int width, int height) {
		// The CallbackInterface::windowSizeCallback will call glViewport for us
		CallbackInterface::windowSizeCallback(width,  height);
	}

	void refreshStatus() {
		leftMousePressed = false;
		rightMousePressed = false;
		leftMouseReleased = false;
		refresh = false;
		curveBez = false;
		curveBS = false;
		scene1 = false;
		scene2 = false;
		scene3 = false;
		scene4 = false;
		scene5 = false;
		moveForward = false;
		moveBack = false;
		moveLeft = false;
		moveRight = false;
	}

	bool buttonDown(int button) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) return leftMousePressed;
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) return rightMousePressed;
		else if (button == GLFW_KEY_F5) return refresh;
		else if (button == GLFW_KEY_LEFT) return curveBez;
		else if (button == GLFW_KEY_RIGHT) return curveBS;
		else if (button == GLFW_KEY_UP) return curveBez;
		else if (button == GLFW_KEY_DOWN) return curveBS;
		else if (button == GLFW_KEY_1) return scene1;
		else if (button == GLFW_KEY_2) return scene2;
		else if (button == GLFW_KEY_3) return scene3;
		else if (button == GLFW_KEY_4) return scene4;
		else if (button == GLFW_KEY_5) return scene5;
		else if (button == GLFW_KEY_W) return moveForward;
		else if (button == GLFW_KEY_A) return moveLeft;
		else if (button == GLFW_KEY_S) return moveBack;
		else if (button == GLFW_KEY_D) return moveRight;

		return false;
	}

	bool buttonUp(int button) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) return leftMouseReleased;

		return false;
	}

	glm::vec2 mouseGL() {
		glm::vec2 startingVec(xMousePos, yMousePos);
		glm::vec2 shiftedVec = startingVec + glm::vec2(0.5f, 0.5f);
		glm::vec2 scaledVec = shiftedVec / glm::vec2(screenDim);

		glm::vec2 flippedY = glm::vec2(scaledVec.x, 1.0f - scaledVec.y);

		glm::vec2 finalVec = flippedY * 2.0f - glm::vec2(1.0f, 1.0f);

		return finalVec;
	}

	virtual void cursorPosCallback(double xpos, double ypos) {
		xMousePos = xpos;
		yMousePos = ypos;
	}

private:
	ShaderProgram& shader;
	glm::ivec2 screenDim;

	double xMousePos;
	double yMousePos;

	bool leftMousePressed;
	bool rightMousePressed;
	bool leftMouseReleased;
	bool refresh;
	bool curveBez;
	bool curveBS;
	bool scene1;
	bool scene2;
	bool scene3;
	bool scene4;
	bool scene5;
	bool moveForward;
	bool moveBack;
	bool moveLeft;
	bool moveRight;
};

void bezier(CPU_Geometry& square, CPU_Geometry& drawCurve, CPU_Geometry& drawSurface) {
	drawSurface.verts.clear();
	drawCurve.verts.clear();
	int d = GLsizei(square.verts.size()) - 1;
	CPU_Geometry tempSquare = square;

	for (float u = 0.0f; u <= 1.0f; u += 0.001f) {
		for (int i = 1; i <= d; i++) {
			for (int j = 0; j <= (d - i); j++) {
				float x = (1.0f - u) * tempSquare.verts[j].x + u * tempSquare.verts[j + 1].x;
				float y = (1.0f - u) * tempSquare.verts[j].y + u * tempSquare.verts[j + 1].y;
				tempSquare.verts[j] = glm::vec3(x, y, 0.0f);
			}
		}
		drawCurve.verts.push_back(tempSquare.verts[0]);
	}
}

void bSpline(CPU_Geometry& square, CPU_Geometry& drawCurve, CPU_Geometry& drawSurface) {
	drawSurface.verts.clear();
	drawCurve.verts.clear();
	int d = GLsizei(square.verts.size()) - 1;
	CPU_Geometry tempSquare = square;

	for (int m = 0; m <= 9; m++) {
		CPU_Geometry temp;
		temp.verts.push_back(tempSquare.verts[0]);
		temp.verts.push_back(0.5f * tempSquare.verts[0] + 0.5f * tempSquare.verts[1]);
		for (int i = 1; i < d - 1; i++) {
			temp.verts.push_back(0.75f * tempSquare.verts[i] + 0.25f * tempSquare.verts[i + 1]);
			temp.verts.push_back(0.25f * tempSquare.verts[i] + 0.75f * tempSquare.verts[i + 1]);
		}
		temp.verts.push_back(0.5f * tempSquare.verts[d - 1] + 0.5f * tempSquare.verts[d]);
		temp.verts.push_back(tempSquare.verts[d]);

		tempSquare = temp;
		d = GLsizei(tempSquare.verts.size()) - 1;
	}

	for (auto i = tempSquare.verts.begin(); i < tempSquare.verts.end(); ++i) {
		drawCurve.verts.push_back(*i);
	}
}

void surfaceOfRev(CPU_Geometry& square, CPU_Geometry& drawCurve, CPU_Geometry& drawSurface) {
	drawSurface.verts.clear();
	bSpline(square, drawCurve, drawSurface);
	CPU_Geometry tempCurve = drawCurve;

	int d = GLsizei(tempCurve.verts.size()) - 1;
	int inc = 100;
	
	for (int i = 0; i < d / inc; i++) {  // goes through the points
		glm::vec3 curr = tempCurve.verts[i * inc];
		glm::vec3 currAbove = tempCurve.verts[(i + 1)* inc];
		for (float u = 0.0f; u <= 1.0f; u += 0.02f) { // goes through the num of steps
			float v = (2 * M_PI) * u;

			glm::vec3 temp = glm::vec3(tempCurve.verts[i * inc].x * cos(v), tempCurve.verts[i * inc].y, tempCurve.verts[i * inc].x * sin(v));
			drawSurface.verts.push_back(currAbove);
			drawSurface.verts.push_back(curr);
			drawSurface.verts.push_back(temp);

			glm::vec3 temp2 = glm::vec3(tempCurve.verts[(i + 1) * inc].x * cos(v), tempCurve.verts[(i + 1) * inc].y, tempCurve.verts[(i + 1) * inc].x * sin(v));

			drawSurface.verts.push_back(currAbove);
			drawSurface.verts.push_back(temp);
			drawSurface.verts.push_back(temp2);

			curr = temp;
			currAbove = temp2;
		}
	}
}

CPU_Geometry deCasteljauTensor(CPU_Geometry& surface){
	int d = GLsizei(surface.verts.size()) - 1;
	CPU_Geometry tempSurf = surface;
	CPU_Geometry row;

	for (float u = 0.0f; u <= 1.0f; u += 0.05f) {
		for (int i = 1; i <= d; i++) {
			for (int j = 0; j <= (d - i); j++) {
				float x = (1.0f - u) * tempSurf.verts[j].x + u * tempSurf.verts[j + 1].x;
				float y = (1.0f - u) * tempSurf.verts[j].y + u * tempSurf.verts[j + 1].y;
				float z = (1.0f - u) * tempSurf.verts[j].z + u * tempSurf.verts[j + 1].z;
				tempSurf.verts[j] = glm::vec3(x, y, z);
			}
		}
		row.verts.push_back(tempSurf.verts[0]);
	}

	return row;
}

CPU_Geometry chaikinTensor(CPU_Geometry& surface) {
	int d = GLsizei(surface.verts.size()) - 1;
	CPU_Geometry tempSurf = surface;
	
	for (int m = 0; m <= 3; m++) {
		CPU_Geometry temp;
		temp.verts.push_back(tempSurf.verts[0]);
		temp.verts.push_back(0.5f * tempSurf.verts[0] + 0.5f * tempSurf.verts[1]);
		for (int i = 1; i < d - 1; i++) {
			temp.verts.push_back(0.75f * tempSurf.verts[i] + 0.25f * tempSurf.verts[i + 1]);
			temp.verts.push_back(0.25f * tempSurf.verts[i] + 0.75f * tempSurf.verts[i + 1]);
		}
		temp.verts.push_back(0.5f * tempSurf.verts[d - 1] + 0.5f * tempSurf.verts[d]);
		temp.verts.push_back(tempSurf.verts[d]);

		tempSurf = temp;
		d = GLsizei(tempSurf.verts.size()) - 1;
	}

	return tempSurf;
}

void tensorBezier(CPU_Geometry& tensorPts, CPU_Geometry& drawTensor) {
	drawTensor.verts.clear();
	tensorPts.verts.clear();
	CPU_Geometry hor[4];
	CPU_Geometry ver[4];

	hor[0].verts.push_back(glm::vec3(-0.7f, 0.75f, 0.90f)); //1st row
	hor[0].verts.push_back(glm::vec3(-0.2f, 0.70f, 0.30f));
	hor[0].verts.push_back(glm::vec3(0.30f, 0.75f, 0.90f));
	hor[0].verts.push_back(glm::vec3(0.80f, 0.70f, 0.30f));

	hor[1].verts.push_back(glm::vec3(-0.75f, 0.25f, 0.0f)); //2nd row
	hor[1].verts.push_back(glm::vec3(-0.25f, 0.20f, 0.0f));
	hor[1].verts.push_back(glm::vec3(0.25f, 0.25f, 0.0f));
	hor[1].verts.push_back(glm::vec3(0.75f, 0.20f, 0.0f));

	hor[2].verts.push_back(glm::vec3(-0.7f, -0.25f, 0.0f)); //3rd row
	hor[2].verts.push_back(glm::vec3(-0.2f, -0.30f, 0.0f));
	hor[2].verts.push_back(glm::vec3(0.30f, -0.25f, 0.0f));
	hor[2].verts.push_back(glm::vec3(0.80f, -0.30f, 0.0f));

	hor[3].verts.push_back(glm::vec3(-0.75f, -0.75f, 0.60f)); //4th row
	hor[3].verts.push_back(glm::vec3(-0.25f, -0.80f, 0.20f));
	hor[3].verts.push_back(glm::vec3(0.25f, -0.75f, 0.60f));
	hor[3].verts.push_back(glm::vec3(0.75f, -0.80f, 0.20f));

	ver[0].verts.push_back(glm::vec3(-0.7f, 0.75f, 0.90f)); //1st column
	ver[0].verts.push_back(glm::vec3(-0.75f, 0.25f, 0.0f));
	ver[0].verts.push_back(glm::vec3(-0.7f, -0.25f, 0.0f));
	ver[0].verts.push_back(glm::vec3(-0.75f, -0.75f, 0.60f));

	ver[1].verts.push_back(glm::vec3(-0.2f, 0.70f, 0.30f)); //2nd column
	ver[1].verts.push_back(glm::vec3(-0.25f, 0.20f, 0.0f));
	ver[1].verts.push_back(glm::vec3(-0.2f, -0.30f, 0.0f));
	ver[1].verts.push_back(glm::vec3(-0.25f, -0.80f, 0.20f));

	ver[2].verts.push_back(glm::vec3(0.30f, 0.75f, 0.90f)); //3rd column
	ver[2].verts.push_back(glm::vec3(0.25f, 0.25f, 0.0f));
	ver[2].verts.push_back(glm::vec3(0.30f, -0.25f, 0.0f));
	ver[2].verts.push_back(glm::vec3(0.25f, -0.75f, 0.60f));

	ver[3].verts.push_back(glm::vec3(0.80f, 0.70f, 0.30f)); //4th column
	ver[3].verts.push_back(glm::vec3(0.75f, 0.20f, 0.0f));
	ver[3].verts.push_back(glm::vec3(0.80f, -0.30f, 0.0f));
	ver[3].verts.push_back(glm::vec3(0.75f, -0.80f, 0.20f));

	CPU_Geometry rows[4];
	CPU_Geometry columns[4];

	for (int i = 0; i <= 3; i++) {
		rows[i] = deCasteljauTensor(hor[i]);
	}

	for (int i = 0; i <= 3; i++) {
		columns[i] = deCasteljauTensor(ver[i]);
	}

	CPU_Geometry bezrow[20];
	CPU_Geometry bezcol[20];

	int d = GLsizei(rows[0].verts.size()) - 1;
	for (int i = 0; i <= d; i++) {
		CPU_Geometry temp;
		temp.verts.push_back(rows[0].verts[i]);
		temp.verts.push_back(rows[1].verts[i]);
		temp.verts.push_back(rows[2].verts[i]);
		temp.verts.push_back(rows[3].verts[i]);

		bezcol[i] = deCasteljauTensor(temp);
	}

	d = GLsizei(columns[0].verts.size()) - 1;
	for (int i = 0; i <= d; i++) {
		CPU_Geometry temp;
		temp.verts.push_back(columns[0].verts[i]);
		temp.verts.push_back(columns[1].verts[i]);
		temp.verts.push_back(columns[2].verts[i]);
		temp.verts.push_back(columns[3].verts[i]);

		bezrow[i] = deCasteljauTensor(temp);
	}

	d = GLsizei(bezrow[0].verts.size()) - 2;
	for (int rownum = 0; rownum <= d; rownum++) {
		for (int rowpt = 0; rowpt <= d; rowpt++) {
			drawTensor.verts.push_back(bezrow[rownum].verts[rowpt + 1]);
			drawTensor.verts.push_back(bezrow[rownum + 1].verts[rowpt + 1]);
			drawTensor.verts.push_back(bezrow[rownum + 1].verts[rowpt]);

			drawTensor.verts.push_back(bezrow[rownum].verts[rowpt + 1]);
			drawTensor.verts.push_back(bezrow[rownum + 1].verts[rowpt]);
			drawTensor.verts.push_back(bezrow[rownum].verts[rowpt]);
		}
	}

	for (int i = 0; i <= 3; i++) {
		for (auto j = hor[i].verts.begin(); j < hor[i].verts.end(); ++j) {
			tensorPts.verts.push_back(*j);
		}
	}
}

void tensorBSpline(CPU_Geometry& tensorPts, CPU_Geometry& drawTensor) {
	drawTensor.verts.clear();
	tensorPts.verts.clear();
	CPU_Geometry hor[4];
	CPU_Geometry ver[4];

	hor[0].verts.push_back(glm::vec3(-0.90f, 0.50f, 0.75f)); //1st row
	hor[0].verts.push_back(glm::vec3(-0.2f, 0.70f, 0.50f));
	hor[0].verts.push_back(glm::vec3(0.30f, 0.75f, 0.75f));
	hor[0].verts.push_back(glm::vec3(0.90f, 0.50f, 0.50f));

	hor[1].verts.push_back(glm::vec3(-0.75f, 0.25f, -0.90f)); //2nd row
	hor[1].verts.push_back(glm::vec3(-0.25f, 0.20f, 0.0f));
	hor[1].verts.push_back(glm::vec3(0.25f, 0.25f, 0.0f));
	hor[1].verts.push_back(glm::vec3(0.75f, 0.20f, 0.0f));

	hor[2].verts.push_back(glm::vec3(-0.7f, -0.25f, 0.0f)); //3rd row
	hor[2].verts.push_back(glm::vec3(-0.2f, -0.30f, 0.0f));
	hor[2].verts.push_back(glm::vec3(0.30f, -0.25f, 0.0f));
	hor[2].verts.push_back(glm::vec3(0.80f, -0.30f, 0.0f));

	hor[3].verts.push_back(glm::vec3(-0.90f, -0.50f, 0.75f)); //4th row
	hor[3].verts.push_back(glm::vec3(-0.25f, -0.80f, 0.50f));
	hor[3].verts.push_back(glm::vec3(0.25f, -0.75f, 0.75f));
	hor[3].verts.push_back(glm::vec3(0.90f, -0.50f, 0.50f));

	ver[0].verts.push_back(glm::vec3(-0.90f, 0.50f, 0.75f)); //1st column
	ver[0].verts.push_back(glm::vec3(-0.75f, 0.25f, -0.90f));
	ver[0].verts.push_back(glm::vec3(-0.7f, -0.25f, 0.0f));
	ver[0].verts.push_back(glm::vec3(-0.90f, -0.50f, 0.75f));

	ver[1].verts.push_back(glm::vec3(-0.2f, 0.70f, 0.50f)); //2nd column
	ver[1].verts.push_back(glm::vec3(-0.25f, 0.20f, 0.0f));
	ver[1].verts.push_back(glm::vec3(-0.2f, -0.30f, 0.0f));
	ver[1].verts.push_back(glm::vec3(-0.25f, -0.80f, 0.50f));

	ver[2].verts.push_back(glm::vec3(0.30f, 0.75f, 0.75f)); //3rd column
	ver[2].verts.push_back(glm::vec3(0.25f, 0.25f, 0.0f));
	ver[2].verts.push_back(glm::vec3(0.30f, -0.25f, 0.0f));
	ver[2].verts.push_back(glm::vec3(0.25f, -0.75f, 0.75f));

	ver[3].verts.push_back(glm::vec3(0.90f, 0.50f, 0.50f)); //4th column
	ver[3].verts.push_back(glm::vec3(0.75f, 0.20f, 0.0f));
	ver[3].verts.push_back(glm::vec3(0.80f, -0.30f, 0.0f));
	ver[3].verts.push_back(glm::vec3(0.90f, -0.50f, 0.50f));

	CPU_Geometry rows[4];
	CPU_Geometry columns[4];

	for (int i = 0; i <= 3; i++) {
		rows[i] = chaikinTensor(hor[i]);
	}

	for (int i = 0; i <= 3; i++) {
		columns[i] = chaikinTensor(ver[i]);
	}

	CPU_Geometry bezrow[34];
	CPU_Geometry bezcol[34];

	int d = GLsizei(rows[0].verts.size()) - 1;
	
	for (int i = 0; i <= d; i++) {
		CPU_Geometry temp;
		temp.verts.push_back(rows[0].verts[i]);
		temp.verts.push_back(rows[1].verts[i]);
		temp.verts.push_back(rows[2].verts[i]);
		temp.verts.push_back(rows[3].verts[i]);

		bezcol[i] = chaikinTensor(temp);
	}
	
	d = GLsizei(columns[0].verts.size()) - 1;
	for (int i = 0; i <= d; i++) {
		CPU_Geometry temp;
		temp.verts.push_back(columns[0].verts[i]);
		temp.verts.push_back(columns[1].verts[i]);
		temp.verts.push_back(columns[2].verts[i]);
		temp.verts.push_back(columns[3].verts[i]);

		bezrow[i] = chaikinTensor(temp);
	}

	d = GLsizei(bezrow[0].verts.size()) - 2;
	for (int rownum = 0; rownum <= d; rownum++) {
		for (int rowpt = 0; rowpt <= d; rowpt++) {
			drawTensor.verts.push_back(bezrow[rownum].verts[rowpt + 1]);
			drawTensor.verts.push_back(bezrow[rownum + 1].verts[rowpt + 1]);
			drawTensor.verts.push_back(bezrow[rownum + 1].verts[rowpt]);

			drawTensor.verts.push_back(bezrow[rownum].verts[rowpt + 1]);
			drawTensor.verts.push_back(bezrow[rownum + 1].verts[rowpt]);
			drawTensor.verts.push_back(bezrow[rownum].verts[rowpt]);
		}
	}
	for (int i = 0; i <= 3; i++) {
		for (auto j = hor[i].verts.begin(); j < hor[i].verts.end(); ++j) {
			tensorPts.verts.push_back(*j);
		}
	}
}

void reset(CPU_Geometry& square, CPU_Geometry& drawCurve, CPU_Geometry& drawSurface) {
	drawSurface.verts.clear();
	drawCurve.verts.clear();
	square.verts.clear();
	square.verts.push_back(glm::vec3{ -0.5f, 0.5f, 0.0f });
	square.verts.push_back(glm::vec3{ -0.5f, -0.5f, 0.0f });
	square.verts.push_back(glm::vec3{ 0.5f, -0.5f, 0.0f });
	square.verts.push_back(glm::vec3{ 0.5f, 0.5f, 0.0f });

}

void removePt(CPU_Geometry& square, glm::vec2& clickPos) {
	for (auto i = square.verts.begin(); i < square.verts.end(); ++i)
		if (clickPos.x <= (*i).x + 0.05f && clickPos.x >= (*i).x - 0.05f && clickPos.y <= (*i).y + 0.05f && clickPos.y >= (*i).y - 0.05f) {
			square.verts.erase(i);
			return;
		}
}

void addPt(CPU_Geometry& square, glm::vec2& mousePos, int& move, int& index) {
	index = 0;
	for (auto i = square.verts.begin(); i < square.verts.end(); ++i) {
		if (mousePos.x <= (*i).x + 0.05f && mousePos.x >= (*i).x - 0.05f && mousePos.y <= (*i).y + 0.05f && mousePos.y >= (*i).y - 0.05f) {
			square.cols[index] = glm::vec3{ 0.0, 0.0, 1.0};
			move = 1;
			return;
		}
		index++;
	}
	square.verts.push_back(glm::vec3(mousePos, 0.0f));
	square.cols.push_back(glm::vec3(0.0f));
}

void movePt(CPU_Geometry& square, glm::vec2& clickPos, glm::vec2& releasePos, int& move, int& index) {
	square.verts[index] = glm::vec3(releasePos, 0.0f);
	move = 0;
	for (auto i = square.cols.begin(); i < square.cols.end(); ++i) {
		(*i) = glm::vec3(0.0, 0.0, 0.0);
	}
}


int main() {
	Log::debug("Starting main");

	// WINDOW
	glfwInit();
	Window window(800, 800, "CPSC 453"); // can set callbacks at construction if desired

	GLDebug::enable();

	//SHADERS
	ShaderProgram shader("shaders/test.vert", "shaders/test.frag");

	// CALLBACKS
	auto a3 = std::make_shared<Assignment3>(shader, 800, 800);
	window.setCallbacks(a3);

	// The current CPU_Geometry and GPU_Geometry classes are defined in
	// Geometry.h/Geometry.cpp They will work for this assignment, but for some of
	// the bonuses you may have to modify them.
	CPU_Geometry square;
	square.verts.push_back(glm::vec3{-0.5f, 0.5f, 0.0f});
	square.verts.push_back(glm::vec3{-0.5f, -0.5f, 0.0f});
	square.verts.push_back(glm::vec3{0.5f, -0.5f, 0.0f});
	square.verts.push_back(glm::vec3{0.5f, 0.5f, 0.0f});

	CPU_Geometry drawCurve;

	CPU_Geometry drawSurface;

	CPU_Geometry drawTensor;

	CPU_Geometry tensorPts;

	square.cols.resize(square.verts.size(), glm::vec3{0.0f, 0.0f, 0.0f});
	GPU_Geometry pointsGPUGeom;
	updateGPUGeometry(pointsGPUGeom, square);

	square.cols.clear();
	square.cols.resize(square.verts.size(), glm::vec3{0.0f, 0.0f, 0.0f});

	GPU_Geometry linesGPUGeom;
	updateGPUGeometry(linesGPUGeom, drawCurve);

	GPU_Geometry trisGPUGeom;
	updateGPUGeometry(trisGPUGeom, drawSurface);

	GPU_Geometry surGPUGeom;
	updateGPUGeometry(surGPUGeom, drawTensor);

	GPU_Geometry tpointsGPUGeom;
	updateGPUGeometry(tpointsGPUGeom, tensorPts);

	glPointSize(10.0f);

	// Note this call only work on some systems, unfortunately.
	// In order for them to work, you have to comment out line 60
	// If you're on a mac, you can't comment out line 60, so you
	// these will have no effect. :(
	// glLineWidth(5.0f);

	// 1 for pt being moved
	int move = 0;
	// 0 for bezier, 1 for b spline
	int curve = 0;
	// index of moving pt
	int index = 0;
	
	int scene = 1;

	float rotateTimerXL = 0.0f;
	float rotateTimerXR = 0.0f;
	float rotateTimerYL = 0.0f;
	float rotateTimerYR = 0.0f;

	//{scene2, scene3, scene4, scene5}
	int rotateTimerL[4] = { 0, 0, 0, 0 }; //left
	int rotateTimerR[4] = { 0, 0, 0, 0 }; //right
	int rotateTimerU[4] = { 0, 0, 0, 0 }; //up
	int rotateTimerD[4] = { 0, 0, 0, 0 }; //down

	glm::vec2 mousePos;
	glm::mat4 projection;
	glm::mat4 view;

	glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// RENDER LOOP
	while (!window.shouldClose()) {
		a3->refreshStatus();
		glfwPollEvents();

		if (a3->buttonDown(GLFW_KEY_1)) {
			scene = 1;
		}
		else if (a3->buttonDown(GLFW_KEY_2)) {
			scene = 2;
		}
		else if (a3->buttonDown(GLFW_KEY_3)) {
			scene = 3;
		}
		else if (a3->buttonDown(GLFW_KEY_4)) {
			scene = 4;
		}
		else if (a3->buttonDown(GLFW_KEY_5)) {
			scene = 5;
		}

		if (a3->buttonDown(GLFW_KEY_F5)) {
			reset(square, drawCurve, drawSurface);
		}
		else if (scene == 1) {
			projection = glm::mat4(1.0f);
			view = glm::mat4(1.0f);
			if (a3->buttonDown(GLFW_MOUSE_BUTTON_LEFT)) {
				mousePos = a3->mouseGL();
				addPt(square, mousePos, move, index);
				if (curve == 0) {
					bezier(square, drawCurve, drawSurface);
				}
				else {
					bSpline(square, drawCurve, drawSurface);
				}
			}
			else if (a3->buttonUp(GLFW_MOUSE_BUTTON_LEFT)) {
				if (move == 1) {
					glm::vec2 releasePos = a3->mouseGL();
					movePt(square, mousePos, releasePos, move, index);
				}
				if (curve == 0) {
					bezier(square, drawCurve, drawSurface);
				}
				else {
					bSpline(square, drawCurve, drawSurface);
				}
			
			}
			else if (a3->buttonDown(GLFW_MOUSE_BUTTON_RIGHT)) {
				glm::vec2 clickPos = a3->mouseGL();
				removePt(square, clickPos);
				if (curve == 0) {
					bezier(square, drawCurve, drawSurface);
				}
				else {
					bSpline(square, drawCurve, drawSurface);
				}
			}
			else if (a3->buttonDown(GLFW_KEY_LEFT)) {
				curve = 0;
				bezier(square, drawCurve, drawSurface);
			}
			else if (a3->buttonDown(GLFW_KEY_RIGHT)) {
				curve = 1;
				bSpline(square, drawCurve, drawSurface);
			}
		}
		else if (scene == 2) {
			projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);
			mousePos = a3->mouseGL();

			if (a3->buttonDown(GLFW_KEY_W)) {
				camPos += 0.05f * camFront;
			}
			else if (a3->buttonDown(GLFW_KEY_S)) {
				camPos -= 0.05f * camFront;
			}
			else if (a3->buttonDown(GLFW_KEY_A)) {
				camPos -= glm::normalize(glm::cross(camFront, camUp)) * 0.05f;
			}	
			else if (a3->buttonDown(GLFW_KEY_D)) {
				camPos += glm::normalize(glm::cross(camFront, camUp)) * 0.05f;
			}

			view = glm::lookAt(camPos, camPos + camFront, camUp);

			if (mousePos.x >= 0.85f) {
				view = glm::rotate(view, rotateTimerL[0] * glm::radians(0.01f), glm::vec3(0.0f, -1.0f, 0.0f));
				rotateTimerL[0]--;
			}
			else if (mousePos.x <= -0.85f) {
				view = glm::rotate(view, rotateTimerR[0] * glm::radians(0.01f), glm::vec3(0.0f, -1.0f, 0.0f));
				rotateTimerR[0]++;
			}
			else if (mousePos.y >= 0.85f) {
				view = glm::rotate(view, rotateTimerU[0] * glm::radians(0.01f), glm::vec3(-1.0f, 0.0f, 0.0f));
				rotateTimerU[0]--;
			}
			else if (mousePos.y <= -0.85f) {
				view = glm::rotate(view, rotateTimerD[0] * glm::radians(0.01f), glm::vec3(-1.0f, 0.0f, 0.0f));
				rotateTimerD[0]++;
			}
		}
		else if (scene == 3) {
			projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);
			mousePos = a3->mouseGL();

			surfaceOfRev(square, drawCurve, drawSurface);

			if (a3->buttonDown(GLFW_KEY_LEFT)) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else if (a3->buttonDown(GLFW_KEY_RIGHT)) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			if (a3->buttonDown(GLFW_KEY_W)) {
				camPos += 0.05f * camFront;
			}
			else if (a3->buttonDown(GLFW_KEY_S)) {
				camPos -= 0.05f * camFront;
			}
			else if (a3->buttonDown(GLFW_KEY_A)) {
				camPos -= glm::normalize(glm::cross(camFront, camUp)) * 0.05f;
			}
			else if (a3->buttonDown(GLFW_KEY_D)) {
				camPos += glm::normalize(glm::cross(camFront, camUp)) * 0.05f;
			}

			view = glm::lookAt(camPos, camPos + camFront, camUp);

			if (mousePos.x >= 0.85f) {
				view = glm::rotate(view, rotateTimerL[1] * glm::radians(0.05f), glm::vec3(0.0f, -1.0f, 0.0f));
				rotateTimerL[1]--;
			}
			else if (mousePos.x <= -0.85f) {
				view = glm::rotate(view, rotateTimerR[1] * glm::radians(0.05f), glm::vec3(0.0f, -1.0f, 0.0f));
				rotateTimerR[1]++;
			}
			else if (mousePos.y >= 0.85f) {
				view = glm::rotate(view, rotateTimerU[1] * glm::radians(0.05f), glm::vec3(-1.0f, 0.0f, 0.0f));
				rotateTimerU[1]--;
			}
			else if (mousePos.y <= -0.85f) {
				view = glm::rotate(view, rotateTimerD[1] * glm::radians(0.05f), glm::vec3(-1.0f, 0.0f, 0.0f));
				rotateTimerD[1]++;
			}
		}
		else if (scene == 4) {
			projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);
			mousePos = a3->mouseGL();

			tensorBezier(tensorPts, drawTensor);
			
			if (a3->buttonDown(GLFW_KEY_LEFT)) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else if (a3->buttonDown(GLFW_KEY_RIGHT)) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			if (a3->buttonDown(GLFW_KEY_W)) {
				camPos += 0.05f * camFront;
			}
			else if (a3->buttonDown(GLFW_KEY_S)) {
				camPos -= 0.05f * camFront;
			}
			else if (a3->buttonDown(GLFW_KEY_A)) {
				camPos -= glm::normalize(glm::cross(camFront, camUp)) * 0.05f;
			}
			else if (a3->buttonDown(GLFW_KEY_D)) {
				camPos += glm::normalize(glm::cross(camFront, camUp)) * 0.05f;
			}

			view = glm::lookAt(camPos, camPos + camFront, camUp);

			if (mousePos.x >= 0.85f) {
				view = glm::rotate(view, rotateTimerL[2] * glm::radians(0.05f), glm::vec3(0.0f, -1.0f, 0.0f));
				rotateTimerL[2]--;
			}
			else if (mousePos.x <= -0.85f) {
				view = glm::rotate(view, rotateTimerR[2] * glm::radians(0.05f), glm::vec3(0.0f, -1.0f, 0.0f));
				rotateTimerR[2]++;
			}
			else if (mousePos.y >= 0.85f) {
				view = glm::rotate(view, rotateTimerU[2] * glm::radians(0.05f), glm::vec3(-1.0f, 0.0f, 0.0f));
				rotateTimerU[2]--;
			}
			else if (mousePos.y <= -0.85f) {
				view = glm::rotate(view, rotateTimerD[2] * glm::radians(0.05f), glm::vec3(-1.0f, 0.0f, 0.0f));
				rotateTimerD[2]++;
			}
		}
		else if (scene == 5) {
			projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);
			mousePos = a3->mouseGL();

			tensorBSpline(tensorPts, drawTensor);

			if (a3->buttonDown(GLFW_KEY_LEFT)) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else if (a3->buttonDown(GLFW_KEY_RIGHT)) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			if (a3->buttonDown(GLFW_KEY_W)) {
				camPos += 0.05f * camFront;
			}
			else if (a3->buttonDown(GLFW_KEY_S)) {
				camPos -= 0.05f * camFront;
			}
			else if (a3->buttonDown(GLFW_KEY_A)) {
				camPos -= glm::normalize(glm::cross(camFront, camUp)) * 0.05f;
			}
			else if (a3->buttonDown(GLFW_KEY_D)) {
				camPos += glm::normalize(glm::cross(camFront, camUp)) * 0.05f;
			}

			view = glm::lookAt(camPos, camPos + camFront, camUp);

			if (mousePos.x >= 0.85f) {
				view = glm::rotate(view, rotateTimerL[3] * glm::radians(0.05f), glm::vec3(0.0f, -1.0f, 0.0f));
				rotateTimerL[3]--;
			}
			else if (mousePos.x <= -0.85f) {
				view = glm::rotate(view, rotateTimerR[3] * glm::radians(0.05f), glm::vec3(0.0f, -1.0f, 0.0f));
				rotateTimerR[3]++;
			}
			else if (mousePos.y >= 0.85f) {
				view = glm::rotate(view, rotateTimerU[3] * glm::radians(0.05f), glm::vec3(-1.0f, 0.0f, 0.0f));
				rotateTimerU[3]--;
			}
			else if (mousePos.y <= -0.85f) {
				view = glm::rotate(view, rotateTimerD[3] * glm::radians(0.05f), glm::vec3(-1.0f, 0.0f, 0.0f));
				rotateTimerD[3]++;
			}
		}
	
		shader.use();

		//GLint viewLoc = glGetUniformLocation(shader.getProgram(), "model");
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		GLint viewLoc = glGetUniformLocation(shader.getProgram(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		GLint projLoc = glGetUniformLocation(shader.getProgram(), "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_FRAMEBUFFER_SRGB);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		if (scene == 1 || scene == 2) {
			//curves
			updateGPUGeometry(linesGPUGeom, drawCurve);
			linesGPUGeom.bind();
			glDrawArrays(GL_LINE_STRIP, 0, GLsizei(drawCurve.verts.size()));
			//points
			updateGPUGeometry(pointsGPUGeom, square);
			pointsGPUGeom.bind();
			glDrawArrays(GL_POINTS, 0, GLsizei(square.verts.size()));
		}
		else if (scene == 3) {
			//surface of rev
			updateGPUGeometry(trisGPUGeom, drawSurface);
			trisGPUGeom.bind();
			glDrawArrays(GL_TRIANGLES, 0, GLsizei(drawSurface.verts.size()));
		}
		else if (scene == 4 || scene == 5) {
			//tensor surfaces
			updateGPUGeometry(surGPUGeom, drawTensor);
			surGPUGeom.bind();
			glDrawArrays(GL_TRIANGLES, 0, GLsizei(drawTensor.verts.size()));
			//points
			updateGPUGeometry(tpointsGPUGeom, tensorPts);
			tpointsGPUGeom.bind();
			glDrawArrays(GL_POINTS, 0, GLsizei(tensorPts.verts.size()));
		}

		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui

		window.swapBuffers();
	}

	glfwTerminate();
	return 0;
}
