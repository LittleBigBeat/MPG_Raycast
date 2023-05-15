#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include <vector>
#include <tuple>

#define M_PI 3.141592654
#define Num_RAYS 800
#define Deg_OffSet 0.075 

const int mapWidth = 24;
const int mapHeight = 24;
const int tileSize = 64;

const int screenWidth = 800;
const int screenHeight = 600;
const float fieldOfView = 60.0f * (M_PI / 180.0f);

float nearPlane = 0.1;
float farPlane = 150.0;

const float fov = 60.0f;
const float rayStep = 0.1f;
const float maxRayDistance = 50.0f;


float playerX = 300;
float playerY = 300;

float playerAngle = 0.0f;
float playerSpeed = 3.3f;
float rotationSpeed = 0.1f;

float rayHigh = 0.0f;

bool mapView = false;
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
	double rayAngles[Num_RAYS];
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
					std::tuple <float, float> raysCoordinates;
					raysCoordinates = correction(rayX[i], rayY[i], deltaX[i], deltaY[i]);
					distance[i] = sqrt(pow(std::get<0>(raysCoordinates)- playerX, 2) + pow(std::get<1>(raysCoordinates) - playerY, 2));

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
	for (int i = 0; i < screenWidth; i++) {
		float ca = playerAngle - rayAngles[i];
		distance[i] = distance[i] * cos(ca);
		float wallHeight = screenHeight / distance[i];
		//if (wallHeight > 400) wallHeight = 400;
		float wallTop = (screenHeight - wallHeight) / 2;
		float wallBottom = wallTop + wallHeight;
		

		
		
		float colorInterpolation = 1.0f - (distance[i] / screenWidth);

		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_LINES);

		glVertex2f(i, 0);
		glVertex2f(i, wallTop);
		glEnd();

		glColor3f(0.5f, 0.5f, 0.5f);
		glBegin(GL_LINES);

		glVertex2f(i, wallTop);
		glVertex2f(i, wallBottom);
		glEnd();

		glColor3f(0.8f, 0.8f, 0.8f);
		glBegin(GL_LINES);

		glVertex2f(i, wallBottom);
		glVertex2f(i, screenHeight);
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
void drawVerticalLine(int x, int lineHeight, int color) {
	// Set the color
	glColor3ub(color, color, color);

	// Calculate the top and bottom of the line
	int yTop = screenHeight / 2 - lineHeight / 2;
	int yBottom = screenHeight / 2 + lineHeight / 2;

	// Draw the line
	glBegin(GL_LINES);
	glVertex2i(x, yTop);
	glVertex2i(x, yBottom);
	glEnd();
}

void raycast(float playerX, float playerY, float playerAngle) {
	for (int x = 0; x < screenWidth; x++) {
		float rayAngle = (playerAngle - fieldOfView / 2) + (x / (float)screenWidth) * fieldOfView;

		// Calculate ray direction
		float rayDirX = cos(rayAngle);
		float rayDirY = sin(rayAngle);

		// Check for intersections with walls
		float distanceToWall = 0.0f;
		bool hitWall = false;
		int mapX, mapY;

		while (!hitWall && distanceToWall < 20.0f * tileSize) {
			distanceToWall += 0.1f;

			mapX = (int)(playerX + rayDirX * distanceToWall) / tileSize;
			mapY = (int)(playerY + rayDirY * distanceToWall) / tileSize;

			if (mapX >= 0 && mapX < mapWidth && mapY >= 0 && mapY < mapHeight && map[mapY][mapX] == 1) {
				hitWall = true;
			}
		}

		// Draw the wall slice
		int lineHeight = std::max(1, (int)(screenHeight / distanceToWall));
		int color = 255 - std::min(255, (int)(distanceToWall * 255 / (20.0f * tileSize)));
		drawVerticalLine(x, lineHeight, color);
	}
}

void OnSpecial(int key, int mx, int my) {

	switch (key)
	{
	case GLUT_KEY_UP:
	{
		playerX += cos(playerAngle) * playerSpeed;
		playerY += sin(playerAngle) * playerSpeed;
		
		if (map[static_cast<int>(playerY) >> 6 ][static_cast<int>(playerX) >> 6])
		{
			playerX -= cos(playerAngle) * playerSpeed;
			playerY -= sin(playerAngle) * playerSpeed;
		}
		
	}
	break;
	case GLUT_KEY_DOWN:
	{

		playerX -= cos(playerAngle) * playerSpeed;
		playerY -= sin(playerAngle) * playerSpeed;
		
		if (map[static_cast<int>(playerY) >> 6 ][static_cast<int>(playerX) >> 6])
		{
			playerX += cos(playerAngle) * playerSpeed;
			playerY += sin(playerAngle) * playerSpeed;
		}
	}
	break;
	case GLUT_KEY_LEFT:
	{
		playerAngle -= rotationSpeed;
		playerAngle = fmod(playerAngle, 2 * M_PI);
	}
	break;
	case GLUT_KEY_RIGHT:
	{

		playerAngle += rotationSpeed;
		playerAngle = fmod(playerAngle, 2 * M_PI);
	}
	break;
	case GLUT_KEY_F1:
	{
		mapView = !mapView;
	}
	break;
	default:
		break;
	}
	glutPostRedisplay();
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
	glViewport(0, 0, w, h);             // OpenGL: nastaveni rozmenu viewportu
	glMatrixMode(GL_PROJECTION);        // OpenGL: matice bude typu projekce
	glLoadIdentity();                   // OpenGL: matice bude identicka (jen jednicky v hlavni diagonale)
	gluPerspective(fieldOfView, (double)w / h, nearPlane, farPlane); // perspektivni projekce
}

void OnDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//drawMap();
	drawPlayer();
	//raycast(playerX, playerY, playerAngle);
	castRay(playerX, playerY, playerAngle);
	glutSwapBuffers();
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
	glutSpecialFunc(OnSpecial);
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
	OnInit();
	glutMainLoop();

	return 0;
}