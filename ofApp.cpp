//Gonçalo Fernandes Ferreira nº2022210563
#include "ofApp.h"
#include "cg_game_particle.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	glEnable(GL_DEPTH_TEST);

	//mostra o estado inicial das matrizes
	coutModelviewMatrix();
	coutProjectionMatrix();
	

	glPointSize(200);
	glLineWidth(3);

	lensAngle = 60;
	alpha = 10;
	beta = 1000;
	perfectDist = gh()*0.5/tan(lensAngle*0.5*PI/180.);
	cubeNumbers = 10;
	velX = 3;
	velY = 3;
	win = false;
	cubeOffsetY = 0;
	check2 = false;
	contLines = 3;
	initialLines = contLines;
	lastLineAddedTime = ofGetElapsedTimef();
	for (int i = 0; i < cubeNumbers*contLines; i++) {
		activePositions.push_back(1);
	}
	
	nightMode = false;

	gameAmbientOn = true;
	gameDiffuseOn = true;
	gameSpecularOn = true;
	gameDirectionalOn = true;
	barOn = true;
	sunOn = true;

	perspective1 = true;
	perspective2 = true;
	perspective3 = true;
	viewmode1 = false;
	check = false;
	newLine = false;

	check2 = false;

	score = 0;
	rotationCount = 0; 
	currentRotation = 0; 
	rotationIncrement = 1;

	k = 0;
	sunHeight = 200;

	floorTexture.load("wood1.png");

	setupBreakOut();
}
//--------------------------------------------------------------
void ofApp::update() {
	if (gameStarted) {
		float currentTime = ofGetElapsedTimef(); 
		float distance = (5 + 2 * (initialLines - 1)) * cubeDepth * 0.5;
		if (currentTime - lastLineAddedTime >= 20.0f && (ballPosition.y + ballRadius < distance - cubeDepth/2 || ballPosition.y - ballRadius > distance + cubeDepth / 2)) {
			addNewCubeLine();
			lastLineAddedTime = currentTime; 
		}
		ballPosition += ballVelocity;
		checkBall();
		//verifica colisão da janela na lateral da esquerda e da direita
		if (ballPosition.x - ballRadius <= -floorWidth*0.5 ) {
			ballVelocity.x *= -1;
			ballPosition.x = -floorWidth/2 + ballRadius+1;
		}
		if(ballPosition.x + ballRadius >= floorWidth * 0.5)
		{
			ballVelocity.x *= -1;
			ballPosition.x = floorWidth / 2 - ballRadius - 1;
		}
		///verifica colisão com a barra com a barra
		if (ballPosition.y - ballRadius < barPosition.y + barHeight / 2) {
			if(ballVelocity.x > 0)
			{
				if(ballPosition.x>= barPosition.x - barWidth / 2 && ballPosition.x < barPosition.x - barWidth*0.25)
				{
					ballVelocity.y *= -1;
					ballVelocity.x = -velX;
				}
				else if(ballPosition.x >= barPosition.x - barWidth *0.25 && ballPosition.x < barPosition.x )
				{
					ballVelocity.x = -velX * 0.5;
					ballVelocity.y *= -1;
				}
				else if (ballPosition.x < barPosition.x + barWidth * 0.25 && ballPosition.x > barPosition.x)
				{
					ballVelocity.x = velX * 0.5;
					ballVelocity.y *= -1;
				}
				else if (ballPosition.x <= barPosition.x + barWidth / 2 && ballPosition.x >= barPosition.x + barWidth * 0.25) {
					ballVelocity.x = velX;
					ballVelocity.y *= -1;
				}
				else if(ballPosition.x == barPosition.x)
				{
					ballVelocity.x = 0;
					ballVelocity.y *= -1;
				}
			}
			else
			{
				if (ballPosition.x >= barPosition.x - barWidth / 2 && ballPosition.x < barPosition.x - barWidth*0.25)
				{
					ballVelocity.x = -velX;
					ballVelocity.y *= -1;
				}
				else if (ballPosition.x >= barPosition.x - barWidth * 0.25 && ballPosition.x < barPosition.x) {
					ballVelocity.x = -velX * 0.5;
					ballVelocity.y *= -1;
				}
				else if (ballPosition.x > barPosition.x  && ballPosition.x < barPosition.x + barWidth*0.25) {
					ballVelocity.x = velX * 0.5;
					ballVelocity.y *= -1;
				}
				else if(ballPosition.x >= barPosition.x + barWidth * 0.25 && ballPosition.x <= barPosition.x + barWidth ){
					ballVelocity.x = velX;
					ballVelocity.y *= -1;
				}
				else if (ballPosition.x == barPosition.x)
				{
					ballVelocity.x = 0;
					ballVelocity.y *= -1;
				}
			}
		}

		//verifica colisão parte superior da janlea
		if (ballPosition.y + ballRadius >= floorHeight*0.5) {
			ballVelocity.y *= -1;
		}
		//verifica se a bola passa a parte inferior do mapa e recomeça o jogo
		if (ballPosition.y - ballRadius <= barPosition.y - barHeight/2 )
		{
			CreateExplosion(ballPosition);
			barPosition.set(floor(resX / 2), floor(resY / 2) - 6 * floorWidth / GLfloat(resX), baseDepth);
			ballPosition.set(floor(resX / 2), floor(resY / 2) - 6  * floorWidth / GLfloat(resX) + barHeight * 0.5 + ballRadius, ballRadius);
			for (int i = 0; i < activePositions.size(); i++)
			{
				activePositions[i] = 1;
			}
			cubeOffsetY = 0;
			score = 0;
			gameStarted = false;
			contLines = initialLines;


		}
		
		for (size_t i = 0; i < cubePositions.size(); i++) {
			if (activePositions[i] == 1)
			{
				ofVec3f Aux = cubePositions[i];
				if (barPosition.y + barHeight/2 >= Aux.y - cubeDepth / 2 )
				{
					check2 = true;
				}
			}
		}
		if(check2)
		{
			barPosition.set(floor(resX / 2), floor(resY / 2) - 6 * floorWidth / GLfloat(resX), baseDepth);
			ballPosition.set(floor(resX / 2), floor(resY / 2) - 6 * floorWidth / GLfloat(resX) + barHeight * 0.5 + ballRadius, ballRadius);
			for (int i = 0; i < activePositions.size(); i++)
			{
				activePositions[i] = 1;
			}
			cubeOffsetY = 0;
			gameStarted = false;
			contLines = initialLines;

		}
		check2 = false;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	//vista ortografica de topo
	if (perspective1 == true) {
		if (viewmode1 == false) {
			glViewport(0, 0.75 * gh(), gw() * 0.25, gh() * 0.25);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-gw() * 0.5, gw() * 0.5, -gh() * 0.5, gh() * 0.5, -10000, 10000);
			lookat(0, 0, 1, 0, 0, 0, 0, 1, 0);
			drawBreakOut();
		}
		if (viewmode1 == true) {
			glViewport(0,0, gw(), gh());
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-gw() * 0.5, gw() * 0.5, -gh() * 0.5, gh() * 0.5, -10000, 10000);
			lookat(0, 0, 1, 0, 0, 0, 0, 1, 0);
			drawBreakOut();
		}
	}
	//vista ortografica de frente
	if (perspective2 == true) {
		if (viewmode1 == false) {
			glViewport(gw() * 0.75, 0.75 * gh(), gw() * 0.25, gh() * 0.25);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-gw() * 0.5, gw() * 0.5, -gh() * 0.5, gh() * 0.5, -10000, 10000);
			lookat(0, -1, 0, 0, 0, 0, 0, 0, 1);
			drawBreakOut();
		}
		if (viewmode1 == true) {
			glViewport(0, 0, gw(), gh());
			perspective(lensAngle, alpha, beta);
			lookat(barPosition.x,barPosition.y - barHeight/2,barPosition.z,barPosition.x,barPosition.y + floorHeight,0,0, 0, 1);
			drawBreakOut();
		}
	}

	//viewport principal, ocupa toda a tela
	//camera perspectiva
	//define viewport
	if (perspective3 == true) {
		glViewport(0, 0, gw(), gh());
		perspective(lensAngle, alpha, beta);
		lookat(0, -gh() * 0.65, 0.5 * perfectDist, 0, 0, 0, 0, 1, 0);
		drawBreakOut();

	}

	glDisable(GL_LIGHTING);
	ofDrawBitmapString("Score: " + ofToString(score), 400, 300);
	glEnable(GL_LIGHTING);
	
}

void ofApp::setupBreakOut() {
	//floor parameters
	resX = 15;
	resY = 15;
	floorWidth = gw() * 0.5;
	floorHeight = gw() * 0.5;
	floorHeightPos = 0.;//gh() * 0.75;

	//cubos 
	cubeDepth = floorHeight / GLfloat(resY);
	//bar
	barWidth = 3 * floorWidth / GLfloat(resX);
	barHeight = floorHeight / GLfloat(resY);
	baseDepth = barWidth * 0.2;

	barPosition.x = floor(resX / 2);
	barPosition.y = floor(resY / 2) - 6 * floorWidth / GLfloat(resX);
	barPosition.z = baseDepth;

	//ball
	ballRadius = 10;
	ballPosition.x = floor(resX / 2);
	ballPosition.y = floor(resY / 2) - 6 * floorWidth / GLfloat(resX) + barHeight*0.5 + ballRadius;
	ballPosition.z = ballRadius*2;

}

void ofApp::drawBreakOut() {
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	//glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);

	GLfloat lightPos[] = { 0.0, -1.0, 2.0, 0.0};
	GLfloat lightAmbient[] = { 0.2, 0.2, 0.2, 1.0 };
	if (!gameAmbientOn) {
		lightAmbient[0] = 0.0;
		lightAmbient[1] = 0.0;
		lightAmbient[2] = 0.0;
	}
	GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	if (!gameDiffuseOn) {
		lightDiffuse[0] = 0.0;
		lightDiffuse[1] = 0.0;
		lightDiffuse[2] = 0.0;
	}
	GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
	if (!gameSpecularOn) {
		lightSpecular[0] = 0.0;
		lightSpecular[1] = 0.0;
		lightSpecular[2] = 0.0;
	}
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glEnable(GL_LIGHT0);

	if (!gameDirectionalOn) {
		glDisable(GL_LIGHT0);
	}

	if (win) {
		glDisable(GL_LIGHTING);
		ofBackground(0, 0, 0);
		ofSetColor(255, 255, 255); // Define a cor do texto (branco)
		ofDrawBitmapString("You Won!", 0, 600);
		glEnable(GL_LIGHTING);
		loadMaterial(11);
		glPushMatrix();
		glTranslatef(0, 0, barPosition.z);
		glScalef(3 * barWidth, 3 * barHeight, 3 * barHeight);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (rotationCount < 1000) {
			currentRotation += rotationIncrement;
			rotationCount++;
		}

		// Aplica a rotação acumulada
		glRotatef(currentRotation, 0, 1, 0);

		cube_unit();
		glPopMatrix();
		return;
	}


	glPushMatrix();//master push
	//glScalef(1, -1, 1);

	//floor
	loadMaterial(3);
	
	glEnable(GL_TEXTURE);
	floorTexture.bind();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glPushMatrix();//floor push
	glScalef(floorWidth, floorHeight, 1.);
	malha_unit(resX, resY);
	glPopMatrix();//floor pop
	
	floorTexture.unbind();
	glDisable(GL_TEXTURE);

	//bar
	if (perspective2 == true)
	{
		if (viewmode1 == true) {
			loadMaterial(11);
			glPushMatrix();
			glTranslatef(barPosition.x, barPosition.y, barPosition.z);
			glScalef(barWidth, barHeight, barHeight);
			cube_unit();
			glPopMatrix();
		}
	}
	loadMaterial(11);
	glPushMatrix();
	glTranslatef(barPosition.x, barPosition.y, barPosition.z);
	glScalef(barWidth, barHeight, barHeight);
	cube_unit();
	glPopMatrix();

	glPopMatrix();//master pop

	if (!barOn) {
		float x = barPosition.x;
		float y = barPosition.y;
		float z = barPosition.z;
		GLfloat spotPos[] = { x, y, z, 1.0f };;
		GLfloat spotDir[] = { 0.0f,1.0f,0.0f };


		GLfloat spotAmbient[] = { 0.2, 0.2, 0.2, 1.0 };
		if (!gameAmbientOn) {
			spotAmbient[0] = 0.5;
			spotAmbient[1] = 0.5;
			spotAmbient[2] = 0.5;
		}
		GLfloat spotDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
		if (!gameDiffuseOn) {
			spotDiffuse[0] = 0.5;
			spotDiffuse[1] = 0.5;
			spotDiffuse[2] = 0.5;
		}
		GLfloat spotSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
		if (!gameSpecularOn) {
			spotSpecular[0] = 0.5;
			spotSpecular[1] = 0.5;
			spotSpecular[2] = 0.5;
		}
		glLightfv(GL_LIGHT1, GL_POSITION, spotPos);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir);
		glLightfv(GL_LIGHT1, GL_AMBIENT, spotAmbient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, spotDiffuse);
		glLightfv(GL_LIGHT1, GL_SPECULAR, spotSpecular);

		GLfloat constantAttenuation = 0.75;
		GLfloat spotCutoff = 40;
		GLfloat spotExponent = 2.0;

		glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, constantAttenuation);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spotCutoff);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, spotExponent);

		glEnable(GL_LIGHT1);
	}
	if (barOn) {
		glDisable(GL_LIGHT1);
	}



	cubePositions.clear();

	float n;
	for (int i = 0; i < contLines; i++) {
		if (i % 3 == 0) {
			n = 18;
		}
		else if(i % 3 == 1) {
			n = 19;
		}
		else
		{
			n = 20;
		}
		cube_lines_draw(n,(5 + 2 *i) * cubeDepth * 0.5 + cubeOffsetY, cubePositions, activePositions, i * cubeNumbers, cubeDepth, floorWidth, cubeNumbers);
	}
	draw_ball(ballPosition,4);
	if(!sunOn)
	{
		GLfloat x = 0.0, y = gh() / 2 + gh() / 4, z = sin(k*PI/360) * sunHeight, sphereScale = 5;
		GLfloat sunPos[4] = { x,y,z,1.0 };
		ofVec3f sunLightPos = {x,y,z};
		glPushMatrix();
		glScalef(sphereScale,sphereScale,sphereScale);
		draw_ball(sunLightPos, 10);
		glPopMatrix();
		k++;
		GLfloat lightAmbient2[] = { 0.2, 0.2, 0.2, 1.0 };
		if (!gameAmbientOn) {
			lightAmbient2[0] = 0.5;
			lightAmbient2[1] = 0.5;
			lightAmbient2[2] = 0.5;
		}
		GLfloat lightDiffuse2[] = { 1.0, 1.0, 1.0, 1.0 };
		if (!gameDiffuseOn) {
			lightDiffuse2[0] = 0.5;
			lightDiffuse2[1] = 0.5;
			lightDiffuse2[2] = 0.5;
		}
		GLfloat lightSpecular2[] = { 1.0, 1.0, 1.0, 1.0 };
		if (!gameSpecularOn) {
			lightSpecular2[0] = 0.5;
			lightSpecular2[1] = 0.5;
			lightSpecular2[2] = 0.5;
		}
		glLightfv(GL_LIGHT2, GL_POSITION, sunPos);
		glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbient);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse);
		glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpecular);
		glEnable(GL_LIGHT2);

	}
	if (sunOn) {
		glDisable(GL_LIGHT2);
	}
	
	if(nightMode)
	{
		pointPos[0] = ballPosition.x/2;
		pointPos[1] = ballPosition.y/2;
		pointPos[2] = ballPosition.z+200;

		pointAmb[0] = 0.5;
		pointAmb[1] = 0.5;
		pointAmb[2] = 0.5;
		pointAmb[3] = 0.5;

		if (!gameAmbientOn) {
			pointAmb[0] = 0;
			pointAmb[1] = 0;
			pointAmb[2] = 0;
		}

		pointDif[0] = 0.5;
		pointDif[1] = 0.5;
		pointDif[2] = 0.5;
		pointDif[3] = 0.5;

		if (!gameDiffuseOn) {
			pointDif[0] = 0;
			pointDif[1] = 0;
			pointDif[2] = 0;
		}

		pointSpec[0] = 0.5;
		pointSpec[1] = 0.5;
		pointSpec[2] = 0.5;
		pointSpec[3] = 0.5;

		if(!gameSpecularOn)
		{
			pointSpec[0] = 0;
			pointSpec[1] = 0;
			pointSpec[2] = 0;
		}
		glLightfv(GL_LIGHT3, GL_POSITION, pointPos);
		glLightfv(GL_LIGHT3, GL_AMBIENT, pointAmb);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, pointDif);
		glLightfv(GL_LIGHT3, GL_SPECULAR, pointSpec);

		GLfloat constantAttenuation = 1.5;

		glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, constantAttenuation);

		glEnable(GL_LIGHT3);
	}
	if (!nightMode) {
		glDisable(GL_LIGHT3);
	}
	for (int i = 0; i < explosion.size(); i++) {
		explosion[i].update();
		explosion[i].draw();
	}
	victory_check();	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	float moveSpeed = 20;
	switch (key) {
	case '1':
		glDisable(GL_CULL_FACE);
		break;
	case '2':
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		break;
	case '3':
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		break;
	case '4':
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
		break;
	case 'g':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'f':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case OF_KEY_LEFT:
		barPosition.x -= moveSpeed;
		if (barPosition.x - (barWidth * 0.5) < -floorWidth*0.5) {
			barPosition.x = -floorWidth *  0.5 + (barWidth * 0.5);
		}
		break;
	case OF_KEY_RIGHT:
		barPosition.x += moveSpeed;
		if (barPosition.x > floorWidth*0.5 - barWidth * 0.5) {
			barPosition.x = floorWidth*0.5 - barWidth * 0.5;
		}
		break;
	case 'r':
		if (!gameStarted) {
			gameStarted = true;
			ballVelocity.set(3, 3, 0);
			win = false;
		}
		break;
	case 'y':
		perspective1 = true;
		perspective2 = false;
		perspective3 = false;
		viewmode1 = true;

		break;
	case 'u':
		perspective1 = true;
		perspective2 = true;
		perspective3 = true;
		viewmode1 = false;
		break;
	case 't':
		perspective1 = false;
		perspective2 = true;
		perspective3 = false;
		viewmode1 = true;
		break;
	case 'n':
		nightMode = !nightMode;
		break;
	case 'b':
		barOn = !barOn;
		break;
	case 'm':
		sunOn = !sunOn;
		break;
	case 'a':
		gameAmbientOn = !gameAmbientOn;
		break;
	case 'd':
		gameDiffuseOn = !gameDiffuseOn;
		break;
	case 's':
		gameSpecularOn = !gameSpecularOn;
		break;
	case 'x':
		gameDirectionalOn = !gameDirectionalOn;
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	cout << endl << x << " " << y;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	setup();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
void ofApp::checkBall() {
	int spacing = 5;
	int cubeWidth = (floorWidth - (cubeNumbers - 1) * spacing) / cubeNumbers;
	float cubeHeight = cubeDepth;
	for (size_t i = 0; i < cubePositions.size(); i++) {
		if (activePositions[i] == 1)
		{
			ofVec3f Aux = cubePositions[i];
			if (ballPosition.x >= Aux.x - cubeWidth / 2 && ballPosition.x <= Aux.x + cubeWidth / 2)
			{
				if(ballPosition.y<Aux.y-cubeHeight/2 && ballPosition.y + ballRadius >= Aux.y - cubeHeight / 2 
					|| ballPosition.y >= Aux.y + cubeHeight / 2 && ballPosition.y - ballRadius <= Aux.y + cubeHeight / 2)
				{
					check = true;
					CreateExplosion(ballPosition);
					ballVelocity.y *= -1;
					activePositions[i] = 0;
					score += 50;
					break;
				}
			}
			if(ballPosition.y >= Aux.y - cubeHeight/2 && ballPosition.y <= Aux.y + cubeHeight / 2)
			{
				if (ballPosition.x<Aux.x - cubeWidth / 2 && ballPosition.x + ballRadius >= Aux.x - cubeWidth / 2
					|| ballPosition.x >= Aux.x + cubeWidth / 2 && ballPosition.x - ballRadius <= Aux.x + cubeWidth / 2)
				{
					check = true;
					CreateExplosion(ballPosition);
					ballVelocity.x *= -1;
					activePositions[i] = 0;
					score += 50;
					break;
				}

			}
		}
	}
}

void ofApp::victory_check() {
	bool flag=true;
	for (size_t i = 0; i < activePositions.size(); i++) {
		if (activePositions[i] == 1)
		{
			flag=false;
		}
	}
	if (flag) {
		win = true;
	}
}

void ofApp::CreateExplosion(ofVec3f ballPosition) {
	int j = 10000;
	if (check)
	{
		j = 5;
	}
	for (int i = 0; i < j; i++) {
		Particle aux;
		aux.setup(ballPosition.x - ballRadius, ballPosition.x + ballRadius, ballPosition.y - ballRadius, ballPosition.y + ballRadius);
		explosion.push_back(aux);
	}
	for (int i = 0; i < explosion.size(); i++)
	{
		explosion[i].update();
		if (explosion[i].dead) {
			explosion.erase(explosion.begin() + i);
		}
	}
	check = false;
}

void ofApp::addNewCubeLine() {
	int spacing = 5;
	int cubeWidth = (floorWidth - (cubeNumbers - 1) * spacing) / cubeNumbers;
	float cubeHeight = cubeDepth;

	contLines++;
	for (int i = 0; i < cubeNumbers; i++) {
		activePositions.push_back(1);
	}
	cubeOffsetY -= cubeHeight;
}