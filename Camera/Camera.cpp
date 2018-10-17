#define _USE_MATH_DEFINES
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <zmq.hpp>

#include <Windows.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <turbojpeg.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <math.h>

#include <fstream>
#include <tchar.h>
#include "SMObject.h"
#include "SMStruct.h"

using namespace std;

#define MAX_HB_COUNT 50

void display();
void idle();

GLuint tex;

zmq::context_t context(1);
zmq::socket_t subscriber(context, ZMQ_SUB);

SMObject PMObj(TEXT("ProcessManagement"), sizeof(ProcessManagement));

ProcessManagement* PM; //type casting void pData to PM type

bool DataRecFlag = TRUE;
int HeartbeatCounter = 0;

int main(int argc, char** argv)
{	
	//obtain access to PMObj
	SetConsoleTitle(TEXT("Camera.exe"));
	PMObj.SMAccess();

	PM = (ProcessManagement*)PMObj.pData;

	PM->ShutDown.Flags.Camera = 0;

	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	glutInit(&argc, (char**)(argv));
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("MTRN3500 - GL");

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	//generate texture object
	glGenTextures(1, &tex);

	//connect to server
	subscriber.connect("tcp://192.168.1.200:26000");

	//  Subscribe to zipcode, default is NYC, 10001
	subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);


	glutMainLoop();

	return 0;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
		//glColor3f(1, 0, 0);

		glTexCoord2f(0, 1);  glVertex2f(-1, -1);
		glTexCoord2f(1, 1); glVertex2f(1, -1);
		glTexCoord2f(1, 0); glVertex2f(1, 1);
		glTexCoord2f(0, 0); glVertex2f(-1, 1);
	glEnd();
	glutSwapBuffers();
}

void idle()
{
	//if shutdown = 0, then heartbeat process works normally, else exit module
	if (PM->ShutDown.Flags.Camera == 0)
	{
		Sleep(20);
		//update heartbeat
		//check if heartbeat 0ed, PM is alive
		if (PM->HeartBeat.Flags.Camera == 1)
		{
			if (HeartbeatCounter < MAX_HB_COUNT)
			{
				HeartbeatCounter++;
			}
			else
			{
				PM->ShutDown.Flags.Camera = 1;
			}
		}
		else
		{
			HeartbeatCounter = 0;
			//Reset GPS.Heartbeat
			PM->HeartBeat.Flags.Camera = 1;
		}
		//cout << HeartbeatCounter << endl;

	}
	else
		exit(0);
	//receive from ZMQ    
	zmq::message_t update;
	if (subscriber.recv(&update, ZMQ_NOBLOCK)) {

		DataRecFlag = TRUE;

		std::cout << "Received data is " << update.size() << std::endl;

		//do JPEG decompression
		long unsigned int _jpegSize = update.size(); //!< _jpegSize from above
		unsigned char* _compressedImage = static_cast<unsigned char*>(update.data()); //!< _compressedImage from above

		int jpegSubsamp, width, height;
		tjhandle _jpegDecompressor = tjInitDecompress();
		tjDecompressHeader2(_jpegDecompressor, _compressedImage, _jpegSize, &width, &height, &jpegSubsamp);
		unsigned char * buffer = new unsigned char[width*height*3]; //!< will contain the decompressed image

		std::cout << "Decompressed image dimensions: " << width << " " << height << std::endl;

		tjDecompress2(_jpegDecompressor, _compressedImage, _jpegSize, buffer, width, 0/*pitch*/, height, TJPF_RGB, TJFLAG_FASTDCT);
		tjDestroy(_jpegDecompressor);


		//load texture

		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char * pixels = NULL;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);
	
		delete[] buffer;
	
	}
	else {
		if (DataRecFlag == TRUE)
		{
			std::cout << "Received no data." << std::endl;
			DataRecFlag = FALSE;
		}
	}


	display();
}