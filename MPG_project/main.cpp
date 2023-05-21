#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include <vector>
#include <tuple>

#include <algorithm>
#include <random>
#include <chrono>

#include <map>
#include <Windows.h>



#define M_PI 3.141592654

#define Num_RAYS 800
#define Deg_OffSet 0.075 

const int mapWidth = 24;
const int mapHeight = 24;
const int tileSize = 64;

int screenWidth = 800;
int screenHeight = 600;

const int monitorHeight = GetSystemMetrics(SM_CYSCREEN);
const int monitorWidth = GetSystemMetrics(SM_CXSCREEN);

float lineWidth = ceil((float)monitorWidth / Num_RAYS);


float nearPlane = 0.1;
float farPlane = 150.0;

const float fov = 60.0f;
const float rayStep = 0.1f;
const float maxRayDistance = 50.0f;


float playerX = 300;
float playerY = 300;

float playerAngle = 0.0f;
float playerSpeed = 40.0f;
float rotationSpeed = 1.5f;

float rayHigh = 0.0f;

bool mapView = false;

float currentTime = 0.0f;
float previousTime = 0.0f;
float deltaTime = 0.0;

float deceleration = 0.9f;
int lastKey = 0;

std::vector<int> playerStarting = { 0, 0 };

bool keyStates[4] = { false, false, false, false }; // Up, Down, Left, Right

bool gameExist = false;

enum MenuItem {
	START_GAME,
	OPTIONS,
	QUIT
};

MenuItem selectedMenu = START_GAME;

enum OptionItem {
	GRAPHICS,
	FULLSCREEN,
	ROTATION_SPEED,
	MOVEMENT_SPEED,
	DEFAULT,
	BACK
};

OptionItem selectedOption = GRAPHICS;

enum OnOffItem {
	ON,
	OFF
};
OnOffItem graphicOption = ON;
OnOffItem fullscreenOption = OFF;

enum SpeedItem {
	SLOW,
	MEDIUM,
	FAST
};
SpeedItem rotationSpeedOption = MEDIUM;
SpeedItem movementSpeedOption = MEDIUM;

std::map<std::string, std::vector<std::string>> option = {
	{"GRAPHICS", {"HIGH", "LOW"}},
	{"FULLSCREEN", {"ON", "OFF"}},
	{"ROTATION_SPEED", {"SLOW", "MEDIUM", "FAST"}},
	{"MOVEMENT_SPEED", {"SLOW", "MEDIUM", "FAST"}}

};

bool menuActive = true;
bool optionActive = false;
bool loadingActive = false;
bool gameActive = false;
/*
float playerX = 2.0f;
float playerY = 2.0f;
float playerAngle = 0.0f;*/


std::vector<std::vector<int>> map = 
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

float currentCameraX = 4.0f;
float currentCameraY = 22.0f;
float currentCameraZ = 48.0f;
float targetCameraX = currentCameraX;
float targetCameraY = currentCameraY;
float targetCameraZ = currentCameraZ;
float transitionSpeed = 0.05f;

float angleRotation = 0.0f;

//std::vector<std::vector<int>> map(mapHeight, std::vector<int>(mapWidth, 1));

/*
std::vector<std::vector<int>> map = {
	{1, 1, 1, 1, 1},
	{1, 0, 0, 0, 1},
	{1, 0, 0, 0, 1},
	{1, 0, 0, 0, 1},
	{1, 1, 1, 1, 1}
};
*/

std::tuple<float, float> correction(float rayX, float rayY, float deltaX, float deltaY)
{

	
	while(true)
	{
		int mapX = static_cast<int>(rayX) >> 6;
		int mapY = static_cast<int>(rayY) >> 6;

		if (map[mapY][mapX] == 1)
		{
			rayX -= deltaX;
			rayY -= deltaY;
			deltaX = deltaX/10;
			deltaY = deltaY/10;
		}
		else {
			rayX += deltaX;
			rayY += deltaY;
		}

		if (abs(deltaX) < 0.001 && abs(deltaY) < 0.001)
		{
			break;
		}


	}
	return {rayX, rayY};
}
void castRay(float playerX, float playerY, float playerAngle) {
	
	float rayAngles[Num_RAYS];
	float deltaX[Num_RAYS];
	float deltaY[Num_RAYS];
	float rayX[Num_RAYS];
	float rayY[Num_RAYS];
	bool hitWall[Num_RAYS];
	double distance[Num_RAYS];



	float degree;
	degree = Deg_OffSet * 0.01745329;
	// Calculate ray angles and deltas
	for (int i = 0; i < Num_RAYS; i++) {
		rayAngles[i] = playerAngle - (Num_RAYS / 2) * degree + i * degree;
		deltaX[i] = cos(rayAngles[i]);
		deltaY[i] = sin(rayAngles[i]);
		rayX[i] = playerX;
		rayY[i] = playerY;
		hitWall[i] = false;
	}

	// Cast rays
	while (true) {
		bool allHitWall = true;

		for (int i = 0; i < Num_RAYS; i++) {
			if (!hitWall[i]) {
				rayX[i] += deltaX[i];
				rayY[i] += deltaY[i];

				int mapX = static_cast<int>(rayX[i]) >> 6;
				int mapY = static_cast<int>(rayY[i]) >> 6;

				if (mapY < 0 || mapY >= map.size() || mapX < 0 || mapX >= map[0].size()) {
					//distance[i] = sqrt(pow(rayX[i] - playerX, 2) + pow(rayY[i] - playerY, 2));
					distance[i] = 0;
					hitWall[i] = true;
				}
				else if (map[mapY][mapX] == 1) {
					if(graphicOption == OFF){
						distance[i] = sqrt(pow(rayX[i] - playerX, 2) + pow(rayY[i]- playerY, 2));
					}
					else {
						std::tuple <float, float> raysCoordinates;
						raysCoordinates = correction(rayX[i], rayY[i], deltaX[i], deltaY[i]);
						distance[i] = sqrt(pow(std::get<0>(raysCoordinates) - playerX, 2) + pow(std::get<1>(raysCoordinates) - playerY, 2));
					}
					hitWall[i] = true;
				}
			}

			allHitWall &= hitWall[i];
		}

		if (allHitWall) {
			break;
		}
	}

	if (mapView) {

	
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	for (int i = 0; i < Num_RAYS; i++) {
		glVertex2f(playerX, playerY);
		glVertex2f(rayX[i], rayY[i]);
	}
	glEnd();
	}

	else{

	for (int i = 0; i < Num_RAYS; i++) {
		float ca = playerAngle - rayAngles[i];
		distance[i] = 0.05*distance[i] * cos(ca);
		float wallHeight = screenHeight / distance[i];
		//if (wallHeight > 400) wallHeight = 400;
		float wallTop = (screenHeight - wallHeight) / 2;
		float wallBottom = wallTop + wallHeight;


		glLineWidth(lineWidth);

		//float colorInterpolation = 1.0f - (distance[i] / screenWidth);
		
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_LINES);
		
		glVertex2f(i + 1 , 0);
		glVertex2f(i + 1, wallTop);
		glEnd();
		
		glColor3f(0.2f, 0.2f, 0.2f);
		

		glColor3f(0.5f, 0.5f, 0.5f);
		glBegin(GL_LINES);
		glVertex2f(i + 1, wallTop);
		glVertex2f(i + 1, wallBottom);
		
		glEnd();


		glColor3f(0.8f, 0.8f, 0.8f);
		glBegin(GL_LINES);

		glVertex2f(i + 1, wallBottom);
		glVertex2f(i + 1, screenHeight);
		glEnd();
	}
	}
}



void drawMap()
{
	int x, y;
	for (y = 0; y < mapHeight; y++)
	{
		for (x = 0; x < mapWidth; x++)
		{
			if (map[y][x] == 1)
			{
				glColor3f(1.0, 1.0, 0.0);
			}
			else {
				glColor3f(0.0, 0.0, 0.0);
			}
		
;			glBegin(GL_QUADS);
			glVertex2i(x * 64, y * 64);
			glVertex2i(x*64, y*64 + 64);
			glVertex2i(x*64 + 64, y*64 + 64);
			glVertex2i(x*64 + 64, y*64 );
			glEnd();
		}
	}
}

void keyboardCallback(unsigned char key, int x, int y) {
	if (menuActive) {
		if (optionActive) {
			switch (key) {
			case 27:
				optionActive = false;
				break;
			case 13:
				switch (selectedOption)
				{
				case GRAPHICS:
					if (graphicOption == ON) graphicOption = OFF;
					else if (graphicOption == OFF) graphicOption = ON;
					break;
				case FULLSCREEN:
					if (fullscreenOption == ON) {
						fullscreenOption = OFF;
						glutReshapeWindow(screenWidth, screenHeight);
						glutPositionWindow(200, 200);
					}
					else if (fullscreenOption == OFF) { fullscreenOption = ON; glutFullScreen(); }
					break;
				case ROTATION_SPEED:
					if (rotationSpeedOption >= FAST) rotationSpeedOption = static_cast<SpeedItem>(0);
					else {
						rotationSpeedOption = static_cast<SpeedItem>(rotationSpeedOption + 1);
					}
					break;
				case MOVEMENT_SPEED:
					if (movementSpeedOption >= FAST) {
						movementSpeedOption = static_cast<SpeedItem>(0);
						playerSpeed = 20.0f;
					}
					else {
						movementSpeedOption = static_cast<SpeedItem>(movementSpeedOption + 1);
						playerSpeed += 20.0f;
					}
					break;
				case DEFAULT:
					graphicOption = ON;
					fullscreenOption = OFF;
					rotationSpeedOption = MEDIUM;
					movementSpeedOption = MEDIUM;
					break;
				case BACK:
					optionActive = false;
					
					break;
				}
				break;
			}
		}
		else if (menuActive){
			switch (key) {
			case 27:
				if(gameExist){
				gameActive = true;
				menuActive = false;
				}
				break;
			case 13:
				switch (selectedMenu)
				{
				case START_GAME:
					loadingActive = true;
					
					
					break;
				case OPTIONS:
					optionActive = true;
					

					break;
				case QUIT:
					glutDestroyWindow(glutGetWindow());
					break;
				}
				break;
			}
		}
		
	}
	else if (gameActive) {
		switch (key) {
		case 27:
			gameActive = false;
			menuActive = true;;
			keyStates[0] = false;
			keyStates[1] = false;
			keyStates[2] = false;
			keyStates[3] = false;
			break;
		}
	}
}

void OnSpecial(int key, int mx, int my)
{
	if (menuActive)
	{
		if (optionActive) {
			switch (key)
			{

			case GLUT_KEY_UP:
				if (selectedOption > GRAPHICS) selectedOption = static_cast<OptionItem>(selectedOption - 1);
				break;
			case GLUT_KEY_DOWN:
				if (selectedOption < BACK) selectedOption = static_cast<OptionItem>(selectedOption + 1);
				break;
			}
			

		
			glutPostRedisplay();
		}
		else{
		switch (key)
		{

		case GLUT_KEY_UP:
			if (selectedMenu > START_GAME) selectedMenu = static_cast<MenuItem>(selectedMenu - 1);
			break;
		case GLUT_KEY_DOWN:
			if (selectedMenu < QUIT) selectedMenu = static_cast<MenuItem>(selectedMenu + 1);
			break;

		}
		glutPostRedisplay();
		}
	}
	else if (gameActive)
	{ 
	switch (key)
	{
	case GLUT_KEY_UP:
		keyStates[0] = true;
		break;
	case GLUT_KEY_DOWN:
		keyStates[1] = true;
		break;
	case GLUT_KEY_LEFT:
		keyStates[2] = true;
		break;
	case GLUT_KEY_RIGHT:
		keyStates[3] = true;
		break;
	case GLUT_KEY_F1:

		mapView = !mapView;
		break;

	}
	}
}

void OnSpecialUp(int key, int mx, int my)
{
	if(gameActive)
	{
	switch (key)
	{
		
	case GLUT_KEY_UP:
		deceleration = 0.9;
		keyStates[0] = false;
		lastKey = 1;
		break;
	case GLUT_KEY_DOWN:
		deceleration = 0.9;
		keyStates[1] = false;
		lastKey = 2;
		break;
	case GLUT_KEY_LEFT:
		deceleration = 0.9;
		keyStates[2] = false;
		lastKey = 3;
		break;
	case GLUT_KEY_RIGHT:
		deceleration = 0.9;
		keyStates[3] = false;
		lastKey = 4;
		break;

	}
	}
}



void Movement() 
{

	if(keyStates[0])
	{
		playerX += cos(playerAngle) * playerSpeed* deltaTime;
		playerY += sin(playerAngle) * playerSpeed * deltaTime;
		
		if (map[static_cast<int>(playerY) >> 6 ][static_cast<int>(playerX) >> 6])
		{
			playerX -= cos(playerAngle) * playerSpeed * deltaTime;
			playerY -= sin(playerAngle) * playerSpeed * deltaTime;
		}
		
	}
	
	if (keyStates[1])
	{

		playerX -= cos(playerAngle) * playerSpeed * deltaTime;
		playerY -= sin(playerAngle) * playerSpeed * deltaTime;
		
		if (map[static_cast<int>(playerY) >> 6 ][static_cast<int>(playerX) >> 6])
		{
			playerX += cos(playerAngle) * playerSpeed * deltaTime;
			playerY += sin(playerAngle) * playerSpeed * deltaTime;
		}
	}
	if (keyStates[2])
	{
		playerAngle -= rotationSpeed * deltaTime;
		playerAngle = fmod(playerAngle, 2 * M_PI);
	}
	if (keyStates[3])
	{

		playerAngle += rotationSpeed * deltaTime;
		playerAngle = fmod(playerAngle, 2 * M_PI);
	}

	
}

void drawPlayer()
{
	
	glColor3f(1.0, 1.0, 1.0);
	glPointSize(8);
	glBegin(GL_POINTS);
	
	glVertex2i(playerX, playerY);
	glEnd();
	/*
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2i(playerX, playerY);
	glVertex2i(playerX + cos(playerAngle) *20, playerY + sin(playerAngle) * 20);
	
	glEnd();*/
	
}
void OnReshape(int w, int h)            // event handler pro zmenu velikosti okna
{
	if (fullscreenOption == ON) {
		glViewport(0, 0, monitorWidth, monitorHeight);  // Set the viewport to the new dimensions

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, screenWidth, screenHeight, 0, -1, 1);  // Adjust the projection matrix
	}
	else {
		glViewport(0, 0, screenWidth, screenHeight);  // Set the viewport to the new dimensions

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, screenWidth, screenHeight, 0, -1, 1);  // Adjust the projection matrix

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}	
}

void Deceleration()
{
	
		
		if (lastKey == 1)
		{
			playerX += cos(playerAngle) * playerSpeed * deltaTime * deceleration;
			playerY += sin(playerAngle) * playerSpeed * deltaTime * deceleration;

			if (map[static_cast<int>(playerY) >> 6][static_cast<int>(playerX) >> 6])
			{
				playerX -= cos(playerAngle) * playerSpeed * deltaTime;
				playerY -= sin(playerAngle) * playerSpeed * deltaTime;
			}

		}

		if (lastKey == 2)
		{

			playerX -= cos(playerAngle) * playerSpeed * deltaTime * deceleration;
			playerY -= sin(playerAngle) * playerSpeed * deltaTime * deceleration;

			if (map[static_cast<int>(playerY) >> 6][static_cast<int>(playerX) >> 6])
			{
				playerX += cos(playerAngle) * playerSpeed * deltaTime;
				playerY += sin(playerAngle) * playerSpeed * deltaTime;
			}
		}
		if (lastKey == 3)
		{
			playerAngle -= rotationSpeed * deltaTime * deceleration;
			playerAngle = fmod(playerAngle, 2 * M_PI);
		}
		if (lastKey == 4)
		{

			playerAngle += rotationSpeed * deltaTime * deceleration;
			playerAngle = fmod(playerAngle, 2 * M_PI);
		}
		
	
	if (deceleration > 0) deceleration -= 0.1; 
	if (deceleration <= 0) lastKey = 0;
}

void OnTimer(int value) {
	glutPostRedisplay();
}



void generator(int r, int c) {

	std::vector<int> randoms = { 0, 1, 2, 3};
	std::shuffle(randoms.begin(), randoms.end(), std::random_device());
	for (int i = 0; i < randoms.size(); i++) {
		switch (randoms[i])
		{
		case 0:
			if (r - 2 <= 0) continue;
			if (map[r - 2][c] != 0) {
				map[r - 2][c] = 0;
				map[r - 1][c] = 0;
				generator(r - 2, c);
			}
			break;
		case 1:
			if (c + 2 >= mapWidth - 1) continue;
			if (map[r][c + 2] != 0) {
				map[r][c + 2] = 0;
				map[r][c + 1] = 0;
				generator(r, c + 2);
			}
			break;
		case 2:
			if (r + 2 >= mapHeight - 1) continue;
			if (map[r + 2][c] != 0) {
				map[r + 2][c] = 0;
				map[r + 1][c] = 0;
				generator(r + 2, c);
			}
			break;
		case 3:
			if (c - 2 <= 0) continue;
			if (map[r][c - 2] != 0) {
				map[r][c - 2] = 0;
				map[r][c - 1] = 0;
				generator(r, c - 2);
			}
			break;
		}
		
		

	}
}

void MapGenerator()
{

	srand(time(nullptr));
	
	int r = rand() % mapHeight + 1;
	while (r % 2 == 0) {
		r = rand() % mapHeight + 1;
	}

	int c = rand() % mapWidth + 1;
	while (c % 2 == 0) {
		c = rand() % mapWidth + 1;
	}

	map[r][c] = 0;
	playerStarting[0] = r;
	playerStarting[1] = c;
	generator(r, c);

	for (int i = 0; i < mapWidth; i++) {
		map[0][i] = 1;
		map[mapHeight - 1][i] = 1;
	}
	for (int i = 0; i < mapHeight; i++) {
		map[i][0] = 1;
		map[i][mapHeight - 1] = 1;
	}
}

void MapReset() {
	for (int i = 0; i < mapHeight;i++) {
		for (int j = 0; i < mapWidth; i++) {
			map[j][i] = 1;
		}
	}
}
void printMaze(const std::vector<std::vector<int>>& map) {
	for (const auto& row : map) {
		for (int cell : row) {
			std::cout << cell << " ";
		}
		std::cout << std::endl;
	}
}
void GameDisplay() {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	deltaTime = currentTime - previousTime;
	previousTime = currentTime;
	if(gameActive)Movement();
	if (lastKey != 0 && !menuActive) Deceleration();
	glutTimerFunc(1000 / 60, OnTimer, 0);
	
	drawMap();
	drawPlayer();
	//
	//raycast(playerX, playerY, playerAngle);
	castRay(playerX, playerY, playerAngle);

	glutSwapBuffers();
}

void output(int x, int y, char* string)
{
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}

void MainMenu() {
	if (selectedMenu == START_GAME) {
		glColor3f(1.0f, 0.0, 0.0f);
	}

	output(300.0f, 200.0f, (char*)"Start Game");
	glColor3f(1.0f, 1.0f, 1.0f);
	if (selectedMenu == OPTIONS) {
		glColor3f(1.0f, 0.0, 0.0f);
	}
	output(300.0f, 300.0f, (char*)"Options");
	glColor3f(1.0f, 1.0f, 1.0f);

	if (selectedMenu == QUIT) {
		glColor3f(1.0f, 0.0f, 0.0f);
	}
	output(300.0f, 400.0f, (char*)"QUIT");
	glColor3f(1.0f, 1.0f, 1.0f);
}


void Options() {

	if (selectedOption == GRAPHICS) {
		glColor3f(1.0f, 0.0, 0.0f);
	}
	output(250.0f, 200.0f, (char*)"Graphics");
	
	if (graphicOption == ON) output(500.0f, 200.0f, (char*)"High");
	else if (graphicOption == OFF) output(500.0f, 200.0f, (char*)"Low");
		

	glColor3f(1.0f, 1.0f, 1.0f);
	if (selectedOption == FULLSCREEN) {
		glColor3f(1.0f, 0.0, 0.0f);
	}
	output(250.0f, 250.0f, (char*)"Fullscreen");
	if (fullscreenOption == OFF) output(500.0f, 250.0f, (char*)"Off");
	else if (fullscreenOption == ON) output(500.0f, 250.0f, (char*)"On");
	glColor3f(1.0f, 1.0f, 1.0f);
	if (selectedOption == ROTATION_SPEED) {
		glColor3f(1.0f, 0.0, 0.0f);
	}
	output(250.0f, 300.0f, (char*)"Rotation speed");
	if (rotationSpeedOption == SLOW) output(500.0f, 300.0f, (char*)"Slow");
	if (rotationSpeedOption == MEDIUM) output(500.0f, 300.0f, (char*)"Medium");
	if (rotationSpeedOption == FAST) output(500.0f, 300.0f, (char*)"Fast");
	glColor3f(1.0f, 1.0f, 1.0f);

	if (selectedOption == MOVEMENT_SPEED) {
		glColor3f(1.0f, 0.0f, 0.0f);
	}
	output(250.0f, 350.0f, (char*)"Movement speed");
	if (movementSpeedOption == SLOW) output(500.0f, 350.0f, (char*)"Slow");
	if (movementSpeedOption == MEDIUM) output(500.0f, 350.0f, (char*)"Medium");
	if (movementSpeedOption == FAST) output(500.0f, 350.0f, (char*)"Fast");
	glColor3f(1.0f, 1.0f, 1.0f);
	if (selectedOption == DEFAULT) {
		glColor3f(1.0f, 0.0f, 0.0f);
	}
	output(250.0f, 400.0f, (char*)"Restore default options");
	glColor3f(1.0f, 1.0f, 1.0f);
	if (selectedOption == BACK) {
		glColor3f(1.0f, 0.0f, 0.0f);
	}
	output(250.0f, 450.0f, (char*)"Back");
	glColor3f(1.0f, 1.0f, 1.0f);
}




void smoothCameraTransition(float x, float y, float z) {
    targetCameraX = x;
    targetCameraY = y;
    targetCameraZ = z;
    
}

void updateCameraPosition() {
	// Calculate the distance between the current and target positions
	float dx = targetCameraX - currentCameraX;
	float dy = targetCameraY - currentCameraY;
	float dz = targetCameraZ - currentCameraZ;

	// Calculate the next position based on the transition speed
	float nextCameraX = currentCameraX + dx * transitionSpeed;
	float nextCameraY = currentCameraY + dy * transitionSpeed;
	float nextCameraZ = currentCameraZ + dz * transitionSpeed;

	// Update the current camera position
	currentCameraX = nextCameraX;
	currentCameraY = nextCameraY;
	currentCameraZ = nextCameraZ;

	// Update the camera using gluLookAt with the updated position
	gluLookAt(currentCameraX, currentCameraY, currentCameraZ, 0.0f, 10.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}



void rotateCube(int value) {
	angleRotation += 1.0f;
	if (angleRotation > 360.0f) angleRotation -= 360.0f;

	glutPostRedisplay();
	glutTimerFunc(1, rotateCube, 0);
}
void createCube(float x1, float y1, float z1, float x2, float y2, float z2) {
	float width = x2 - x1;
	float height = y2 - y1;
	float depth = z2 - z1;

	float centerX = x1 + (width / 2.0f);
	float centerY = y1 + (height / 2.0f);
	float centerZ = z1 + (depth / 2.0f);

	float vertices[][3] = {
		{ centerX + (width / 2.0f), centerY - (height / 2.0f), centerZ - (depth / 2.0f) },  // Front-bottom-left
		{ centerX - (width / 2.0f), centerY - (height / 2.0f), centerZ - (depth / 2.0f) },  // Front-bottom-right
		{ centerX - (width / 2.0f), centerY + (height / 2.0f), centerZ - (depth / 2.0f) },  // Front-top-right
		{ centerX + (width / 2.0f), centerY + (height / 2.0f), centerZ - (depth / 2.0f) },  // Front-top-left
		{ centerX + (width / 2.0f), centerY - (height / 2.0f), centerZ + (depth / 2.0f) },  // Back-bottom-left
		{ centerX - (width / 2.0f), centerY - (height / 2.0f), centerZ + (depth / 2.0f) },  // Back-bottom-right
		{ centerX - (width / 2.0f), centerY + (height / 2.0f), centerZ + (depth / 2.0f) },  // Back-top-right
		{ centerX + (width / 2.0f), centerY + (height / 2.0f), centerZ + (depth / 2.0f) }   // Back-top-left
	};

	int indices[][2] = {
		{ 0, 1 },  // Front-bottom-right to Front-bottom-left
		{ 1, 2 },  // Front-bottom-left to Front-top-left
		{ 2, 3 },  // Front-top-left to Front-top-right
		{ 3, 0 },  // Front-top-right to Front-bottom-right
		{ 4, 5 },  // Back-bottom-right to Back-bottom-left
		{ 5, 6 },  // Back-bottom-left to Back-top-left
		{ 6, 7 },  // Back-top-left to Back-top-right
		{ 7, 4 },  // Back-top-right to Back-bottom-right
		{ 0, 4 },  // Front-bottom-right to Back-bottom-right
		{ 1, 5 },  // Front-bottom-left to Back-bottom-left
		{ 2, 6 },  // Front-top-left to Back-top-left
		{ 3, 7 }   // Front-top-right to Back-top-right
	};

	// Render the cube using the provided vertices and indices
	glBegin(GL_LINES);
	for (int i = 0; i < 12; ++i) {
		glVertex3f(vertices[indices[i][0]][0], vertices[indices[i][0]][1], vertices[indices[i][0]][2]);
		glVertex3f(vertices[indices[i][1]][0], vertices[indices[i][1]][1], vertices[indices[i][1]][2]);
	}
	glEnd();
}

void fillAreaWithCubes(float x1, float y1, float z1, float x2, float y2, float z2, float sizeOfCubes) {
	// Calculate the number of cubes in each dimension
	int numCubesX = (x2 - x1) / sizeOfCubes;
	int numCubesY = (y2 - y1) / sizeOfCubes;
	int numCubesZ = (z2 - z1) / sizeOfCubes;

	// Calculate the size of each cube
	float cubeSizeX = (x2 - x1) / numCubesX;
	float cubeSizeY = (y2 - y1) / numCubesY;
	float cubeSizeZ = (z2 - z1) / numCubesZ;

	// Render the cubes
	for (int i = 0; i < numCubesX; ++i) {
		for (int j = 0; j < numCubesY; ++j) {
			for (int k = 0; k < numCubesZ; ++k) {
				float cubeX1 = x1 + i * cubeSizeX;
				float cubeY1 = y1 + j * cubeSizeY;
				float cubeZ1 = z1 + k * cubeSizeZ;
				float cubeX2 = cubeX1 + cubeSizeX;
				float cubeY2 = cubeY1 + cubeSizeY;
				float cubeZ2 = cubeZ1 + cubeSizeZ;

				createCube(cubeX1, cubeY1, cubeZ1, cubeX2, cubeY2, cubeZ2);
			}
		}
	}
}
void backgroundAnimation() {
	glFrontFace(GL_CW);					// nastaveni front == clockwise
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   // mod predni steny fill
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_LINE_SMOOTH);		//Aktivace antialiasingu - funguje pouze spole�n� se zapnut�m pr�hlednosti!!!
	glHint(GL_LINE_SMOOTH, GL_NICEST);	//Nastaveni Algoritmu
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	updateCameraPosition();
	glRotatef(angleRotation, 0.0f, 1.0f, 0.0f);
	glLineWidth(2.0);

	glColor3f(1.0, 1.0, 1.0);
	fillAreaWithCubes(-20.0f, 0.0f, -20.0f, 20.0f, 5.0f, 20.0f, 5.0);
	glColor3f(1.0, 0.0, 0.0);
	fillAreaWithCubes(-5.0f, 5.1f, -5.0f, 5.0f, 40.0f, 5.0f, 5.0);
	glFlush();
	
}

void MenuDisplay() {
	glFlush();
	glutPostRedisplay();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//backgroundAnimation();
	
	if(!optionActive){
		MainMenu();
	}
	else {
		Options();
	}
	
	glutSwapBuffers();
}

void Loading() {

	/*
	MapGenerator();
	playerX = playerStarting[0] * 64 + 10;
	playerY = playerStarting[1] * 64 + 10;

	while (map[(int)playerY / 64][(int)playerX / 64] != 0) {
		MapReset();
		MapGenerator();
		playerX = playerStarting[0] * 64 + 10;
		playerY = playerStarting[1] * 64 + 10;
	}

	*/
	
	menuActive = false;
	loadingActive = false;
	gameActive = true;
	gameExist = true;
}


void OnDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();
	glutPostRedisplay();
	if (!menuActive) {
		GameDisplay();
	}
	else if (loadingActive) {
	 Loading();		
	
	}
	else{
		MenuDisplay();
	}
	
	
	//glutSwapBuffers();
}



void OnInit(void)
{
	// steny
			// nastaveni front == clockwise
	   // mod predni steny fill
	
	glClearColor(0.3, 0.3, 0.3, 0);
	glMatrixMode(GL_PROJECTION);        // OpenGL: matice bude typu projekce
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, screenHeight, 0);
	//gluPerspective(fov, (double)w / (double)h, nearPlane, farPlane);
	
	
	// dalsi stavy a vlastnosti (napr. nastaveni svetel, materialu,...)
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);					// inicializace knihovny GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // init double buffer

	glutInitWindowSize( screenWidth  , screenHeight );			// nastaveni pocatecni velikosti dale oteviranych oken
	glutInitWindowPosition(200, 200);		// nastaveni pocatecniho umisteni dale oteviranych oken

	glutCreateWindow("Projekt 2023 � Autor: xsible00");	// vytvoreni okna
	
	glutDisplayFunc(OnDisplay);				// registrace funkce volane pri prekreslovani aktualniho okna
	//glutReshapeFunc(OnReshape);
	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(OnSpecial);
	glutSpecialUpFunc(OnSpecialUp);
	glutTimerFunc(1, rotateCube, 0);
	//glutFullScreen();
	glutReshapeFunc(OnReshape);
	// registracni funkce
	/*
	glutDisplayFunc(OnDisplay);				// registrace funkce volane pri prekreslovani aktualniho okna
	glutReshapeFunc(OnReshape);				// registrace funkce volane pri zmene velikosti aktualniho okna
	glutMouseFunc(OnMouseButton);			// registrace funkce pro stisk tlacitka mysi
	glutMotionFunc(OnMouseMotion);			// registrace funkce pro pohyb mysi pri stisknutem tlacitku
	glutSpecialFunc(OnSpecial);			// registrace funkce pro zachytavani specialnich klaves

	// pripadne dalsi udalosti...

	// inicializace sceny
	
	*/
	selectedMenu = START_GAME;

	OnInit();
	glutMainLoop();

	return 0;
}