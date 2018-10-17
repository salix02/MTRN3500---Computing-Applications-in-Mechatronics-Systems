#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <map>

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/glut.h>
	#include <unistd.h>
	#include <sys/time.h>
#elif defined(WIN32)
	#include <Windows.h>2s
	#include <tchar.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
	#include <unistd.h>
	#include <sys/time.h>
#endif


#include "Camera.hpp"
#include "Ground.hpp"
#include "KeyManager.hpp"

#include "Shape.hpp"
#include "Vehicle.hpp"
#include "MyVehicle.hpp"

#include "Messages.hpp"
#include "HUD.hpp"
#include "ObstacleManager.hpp"

#include <fstream>
#include <tchar.h>
#include "SMObject.h"
#include "SMStruct.h"
#include <conio.h>

#define MAX_HB_COUNT 75

void display();
void reshape(int width, int height);
void idle();

void keydown(unsigned char key, int x, int y);
void keyup(unsigned char key, int x, int y);
void special_keydown(int keycode, int x, int y);
void special_keyup(int keycode, int x, int y);

void mouse(int button, int state, int x, int y);
void dragged(int x, int y);
void motion(int x, int y);

using namespace std;
using namespace scos;

// Used to store the previous mouse location so we
//   can calculate relative mouse movement.
int prev_mouse_x = -1;
int prev_mouse_y = -1;

int HeartbeatCounter = 0;
// vehicle control related variables
Vehicle * vehicle = NULL;
double speed = 0;
double steering = 0;

SMObject LaserObj(TEXT("LaserData"), sizeof(Laser));
SMObject PMObj(TEXT("ProcessManagement"), sizeof(ProcessManagement));
SMObject SimObj(TEXT("Simulator"), sizeof(Simulator));
SMObject RemoteObj(TEXT("RemoteControl"), sizeof(Remote));

ProcessManagement* PM; //type casting void pData to PM type
Remote* REM;
Laser* LS;

int counter = 0;

//int _tmain(int argc, _TCHAR* argv[]) {
int main(int argc, char ** argv) {

	//Request access to required mem blocks
	LaserObj.SMAccess();
	PMObj.SMAccess();
	SimObj.SMAccess();
	RemoteObj.SMAccess();

	PM = (ProcessManagement*)PMObj.pData;
	LS = (Laser*)LaserObj.pData;
	REM = (Remote*)RemoteObj.pData;

	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	glutInit(&argc, (char**)(argv));
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("MTRN3500 - GL");

	Camera::get()->setWindowDimensions(WINDOW_WIDTH, WINDOW_HEIGHT);

	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutSpecialFunc(special_keydown);
	glutSpecialUpFunc(special_keyup);

	glutMouseFunc(mouse);
	glutMotionFunc(dragged);
	glutPassiveMotionFunc(motion);

	// -------------------------------------------------------------------------
	// Please uncomment the following line of code and replace 'MyVehicle'
	//   with the name of the class you want to show as the current 
	//   custom vehicle.
	// -------------------------------------------------------------------------
	vehicle = new MyVehicle();

	glutMainLoop();
	
	if (vehicle != NULL) {
		delete vehicle;
	}

	return 0;
}


void display() {
	// -------------------------------------------------------------------------
	//  This method is the main draw routine. 
	// -------------------------------------------------------------------------

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Pursuit mode settings
	if(Camera::get()->isPursuitMode() && vehicle != NULL) {
		double x = vehicle->getX(), y = vehicle->getY(), z = vehicle->getZ();
		double dx = cos(vehicle->getRotation() * 3.141592765 / 180.0);
		double dy = sin(vehicle->getRotation() * 3.141592765 / 180.0);
		Camera::get()->setDestPos(x + (-10 * dx), y + 7, z + (-10 * dy));
		Camera::get()->setDestDir(dx, -0.25, dy);
	}
	Camera::get()->updateLocation();
	Camera::get()->setLookAt();

	Ground::draw();
	
	// draw my vehicle
	if (vehicle != NULL) {
		vehicle->draw();

	}

	// draw obstacles
	//ObstacleManager::get()->drawAll();

	//abandon obstaclemanager
	//if (counter == 20) {

	float distance = 0;

	glPushMatrix();
	glTranslated(vehicle->getX(), 0,vehicle->getZ());
	glRotatef(vehicle->getRotation(), 0, -1, 0);
		for (int i = 0; i < 181; i++)
		{
			distance = sqrt((LS->y[i] + 0.7)*(LS->y[i] + 0.7) + LS->x[i]* LS->x[i]);
			distance = distance / 5;
			if (distance > 1.0)
				distance = 1.0;

			glPushMatrix();
			glBegin(GL_LINES);
			glColor3f(1, 0, 0);
			glVertex3f(0.7, 0, 0);
			glColor3f((1.0-distance), distance, 0);
			glVertex3f(LS->y[i] + 0.7, 0, LS->x[i]);
			glEnd();
			glTranslated(LS->y[i] + 0.7, 0, LS->x[i]);
			
			//glrotatef()
			glColor3f(1.0, 0.7, 1.0);
			glLineWidth(0.1);
			glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0, 5, 0);
			glEnd();


			glPopMatrix();
			//cout << LS->x[i] << '\t' << LS->y[i] << endl;
		}

	glPopMatrix();
	//counter = 0;
	//}
	//counter++;

	// draw HUD
	HUD::Draw();

	glutSwapBuffers();
};

void reshape(int width, int height) {

	Camera::get()->setWindowDimensions(width, height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
};

double getTime()
{
#if defined(WIN32)
	LARGE_INTEGER freqli;
	LARGE_INTEGER li;
	if(QueryPerformanceCounter(&li) && QueryPerformanceFrequency(&freqli)) {
		return double(li.QuadPart) / double(freqli.QuadPart);
	}
	else {
		static ULONGLONG start = GetTickCount64();
		return (GetTickCount64() - start) / 1000.0;
	}
#else
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec + (t.tv_usec / 1000000.0);
#endif
}

void idle() {
	
	if(PM->ShutDown.Flags.Simulator == 0)
	{
		Sleep(20);
	
		//check if heartbeat 0ed, PM is alive
		if (PM->HeartBeat.Flags.Simulator == 1)
		{
			if (HeartbeatCounter < MAX_HB_COUNT)
			{
				HeartbeatCounter++;
			}
			else
			{
				PM->ShutDown.Flags.Simulator = 1;
			}
		}
		else
		{
			HeartbeatCounter = 0;
			//Reset GPS.Heartbeat
			PM->HeartBeat.Flags.Simulator = 1;
		}
		//cout << HeartbeatCounter << endl;
	}
	else
		exit(0);
	

	if (KeyManager::get()->isAsciiKeyPressed('a')) {
		Camera::get()->strafeLeft();
	}

	if (KeyManager::get()->isAsciiKeyPressed('c')) {
		Camera::get()->strafeDown();
	}

	if (KeyManager::get()->isAsciiKeyPressed('d')) {
		Camera::get()->strafeRight();
	}

	if (KeyManager::get()->isAsciiKeyPressed('s')) {
		Camera::get()->moveBackward();
	}

	if (KeyManager::get()->isAsciiKeyPressed('w')) {
		Camera::get()->moveForward();
	}

	if (KeyManager::get()->isAsciiKeyPressed(' ')) {
		Camera::get()->strafeUp();
	}

	speed = 0;
	steering = 0;

	if (KeyManager::get()->isSpecialKeyPressed(GLUT_KEY_LEFT)) {
		steering = Vehicle::MAX_LEFT_STEERING_DEGS * -1;   
	}

	if (KeyManager::get()->isSpecialKeyPressed(GLUT_KEY_RIGHT)) {
		steering = Vehicle::MAX_RIGHT_STEERING_DEGS * -1;
	}

	if (REM->SetSteering != 0) {
		steering = Vehicle::MAX_LEFT_STEERING_DEGS * -REM->SetSteering;
	}
	else if (!(KeyManager::get()->isSpecialKeyPressed(GLUT_KEY_DOWN) || KeyManager::get()->isSpecialKeyPressed(GLUT_KEY_UP)))
	{
		speed = 0;
	}

	if (KeyManager::get()->isSpecialKeyPressed(GLUT_KEY_UP)) {
		speed = Vehicle::MAX_FORWARD_SPEED_MPS;
	}

	if (KeyManager::get()->isSpecialKeyPressed(GLUT_KEY_DOWN)) {
		speed = Vehicle::MAX_BACKWARD_SPEED_MPS;
	}

	if (REM->SerSpeed != 0) {
		speed = (Vehicle::MAX_FORWARD_SPEED_MPS * REM->SerSpeed)*10;
	}
	else if (!(KeyManager::get()->isSpecialKeyPressed(GLUT_KEY_DOWN) || KeyManager::get()->isSpecialKeyPressed(GLUT_KEY_UP)))
	{
		speed = 0;
	}


	const float sleep_time_between_frames_in_seconds = 0.025;

	static double previousTime = getTime();
	const double currTime = getTime();
	const double elapsedTime = currTime - previousTime;
	previousTime = currTime;

	// do a simulation step
	if (vehicle != NULL) {
		vehicle->update(speed, steering, elapsedTime);
	}

	display();

#ifdef _WIN32 
	Sleep(sleep_time_between_frames_in_seconds * 1000);
#else
	usleep(sleep_time_between_frames_in_seconds * 1e6);
#endif
};

void keydown(unsigned char key, int x, int y) {

	// keys that will be held down for extended periods of time will be handled
	//   in the idle function
	KeyManager::get()->asciiKeyPressed(key);

	// keys that react ocne when pressed rather than need to be held down
	//   can be handles normally, like this...
	switch (key) {
	case 27: // ESC key
		exit(0);
		break;      
	case '0':
		Camera::get()->jumpToOrigin();
		break;
	case 'p':
		Camera::get()->togglePursuitMode();
		break;
	}

};

void keyup(unsigned char key, int x, int y) {
	KeyManager::get()->asciiKeyReleased(key);
};

void special_keydown(int keycode, int x, int y) {

	KeyManager::get()->specialKeyPressed(keycode);

};

void special_keyup(int keycode, int x, int y) {  
	KeyManager::get()->specialKeyReleased(keycode);  
};

void mouse(int button, int state, int x, int y) {

};

void dragged(int x, int y) {

	if (prev_mouse_x >= 0) {

		int dx = x - prev_mouse_x;
		int dy = y - prev_mouse_y;

		Camera::get()->mouseRotateCamera(dx, dy);
	}

	prev_mouse_x = x;
	prev_mouse_y = y;
};

void motion(int x, int y) {

	prev_mouse_x = x;
	prev_mouse_y = y;
};


