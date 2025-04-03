//Gonçalo Fernandes Ferreira nº2022210563
#pragma once

#include "ofMain.h"
#include "cg_extras.h"
#include "cg_drawing_extras.h"
#include "cg_cam_extras.h"
#include "cg_game_particle.h"
#include "materiais.h"


class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void CreateExplosion(ofVec3f ballPosition);
	void checkBall();
	void victory_check();
	void addNewCubeLine();

	ofImage ballTexture;
	ofImage floorTexture;
	ofImage barTexture;


	//camera variables
	int view;
	GLfloat lensAngle;
	GLfloat alpha, beta;
	GLfloat perfectDist;

	ofVec3f camPos;
	ofVec3f lookatPos;
	ofVec3f camUp;
	GLfloat orbitAngle;

	//game variables
	void setupBreakOut();
	void drawBreakOut();

	//floor
	GLint resX, resY;
	GLfloat floorWidth, floorHeight, floorHeightPos;

	GLfloat baseWidth, baseDepth, baseHeight;
	GLint basePosX, basePosY;
	ofVec3f basePos;

	ofVec3f barPosition;
	GLfloat barWidth, barDepth, barHeight;

	std::vector<ofVec3f> cubePositions;
	std::vector<int> activePositions;

	//linhas de cubos
	float cubeDepth;
	ofVec3f ballPosition;
	GLfloat ballRadius,velX,velY;

	ofVec3f ballVelocity;

	int cubeNumbers;
	bool win;

	int rotationCount;
	float currentRotation, rotationIncrement;
	vector <Particle> explosion;

	int score;
	int contLines, initialLines;

	bool nightMode;
	bool gameAmbientOn, gameDiffuseOn, gameSpecularOn, gameDirectionalOn,barOn,sunOn;
	GLfloat pointPos[3];
	GLfloat pointAmb[4];
	GLfloat pointDif[4];
	GLfloat pointSpec[4];
	GLfloat pointAtC, pointAtL, pointAtQ;

	bool gameStarted;
	bool perspective1;
	bool perspective2;
	bool perspective3 ;
	bool viewmode1;
	bool check;
	bool newLine;
	float startTime;
	float lastLineAddedTime;
	float cubeOffsetY;
	bool check2;

	int k;
	int sunHeight;
};
