#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"

//Rotation whole body
float initialBodyRotate = 0.0f;
float bodyRotate = 0.0f;

//Draw Shape
void drawRectangle(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
void drawTrapezium(float minXBottom, float maxXBottom, float minXTop, float maxXTop, float yBottom, float yTop, float minZBottom, float maxZBottom, float minZTop, float maxZTop);
void drawSphere(float radius);
void drawPyramid(float minX, float maxX, float minY, float maxY, float minZ, float maxZ, float divideX, float divideZ);

//leg - data type
//left leg
float upperLeftMinAngle = 0.0f;
float upperLeftMaxAngle = 0.0f;
float lowerLeftMinAngle = 0.0f;
float lowerLeftMaxAngle = 0.0f;
float initialUpperLeftLegSpeed = 0.0f;
float upperLeftLegSpeed = 0.0f;
float initialLowerLeftLegSpeed = 0.0f;
float lowerLeftLegSpeed = 0.0f;

//right leg
float upperRightMinAngle = 0.0f;
float upperRightMaxAngle = 0.0f;
float lowerRightMinAngle = 0.0f;
float lowerRightMaxAngle = 0.0f;
float initialUpperRightLegSpeed = 0.0f;
float upperRightLegSpeed = 0.0f;
float initialLowerRightLegSpeed = 0.0f;
float lowerRightLegSpeed = 0.0f;

//leg
void constructleg();
void drawPelvicGirdle();
void drawLeg(float *initialUpperSpeed, float *upperSpeed, float *initialLowerSpeed, float *lowerSpeed, float *upperMinAngle, float *upperMaxAngle, float *lowerMinAngle, float *lowerMaxAngle);
void drawLegKnee(float radius, int slices, int stacks);
void drawLegPivot();

//hand function
void shoulder();
void arm(float* initialUpperArmSpeed, float* initialLowerArmSpeed, float* move_inFront_hand,
	float* upperArmSpeed, float* upperArmMaxAngle, float* upperArmMinAngle,
	float* lowerArmSpeed, float* lowerArmMaxAngle, float* lowerArmMinAngle);
void drawHand();
void adjustFingerMove(float translateX, float translateY, float translateZ, float rotateX, float rotateY, float rotateZ, float maxAngle, float minAngle);
void armJoint();
void drawRobotHand();

//Hand Variable
float initialFingerMove = 0.0f;
float fingerMove = 0.0f;
float initialThumbMove = 0.0f;
float thumbMove = 0.0f;
float initialArmRotate = 0.0f;
float move_inFront = 0.0f, move_inFront_lowerArm = 0.0f, move_inFront_hand = 0.0f;
float move_Left_inFront = 0.0f, move_Left_inFront_hand = 0.0f;
float move_Right_inFront = 0.0f, move_Right_inFront_hand = 0.0f;
float activate = 0.0f; //need to change
float initialRightUpperArmSpeed = 0.0f, upperRightArmSpeed = 0.0f, upperRightArmMaxAngle = 0.0f, upperRightArmMinAngle = 0.0f;
float initialRightLowerArmSpeed = 0.0f, lowerRightArmSpeed = 0.0f, lowerRightArmMaxAngle = 0.0f, lowerRightArmMinAngle = 0.0f;
float initialLeftUpperArmSpeed = 0.0f, upperLeftArmSpeed = 0.0f, upperLeftArmMaxAngle = 0.0f, upperLeftArmMinAngle = 0.0f;
float initialLeftLowerArmSpeed = 0.0f, lowerLeftArmSpeed = 0.0f, lowerLeftArmMaxAngle = 0.0f, lowerLeftArmMinAngle = 0.0f;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		else if (wParam == VK_SHIFT) {

			//initialize left leg
			upperLeftMaxAngle = 90.0f;
			lowerLeftMaxAngle = 85.0f;

			//left leg
			upperLeftLegSpeed = 0.01f;
			lowerLeftLegSpeed = 0.01f;

			//right leg
			upperRightLegSpeed = -0.01f;
			lowerRightLegSpeed = -0.01f;


			
		}
		else if (wParam == VK_UP) {
			//initialize right leg
			upperRightMaxAngle = 90.0f;
			lowerRightMaxAngle = 85.0f;

			//left leg
			upperLeftLegSpeed = -0.01f;
			lowerLeftLegSpeed = -0.01f;

			//right leg
			upperRightLegSpeed = 0.01f;
			lowerRightLegSpeed = 0.01f;

		}
		else if (wParam == VK_CONTROL) {
			//initialize left leg
			upperLeftMinAngle = 0.0f;
			lowerLeftMinAngle = 0.0f;

			//left leg
			upperLeftLegSpeed = -0.01f;
			lowerLeftLegSpeed = -0.01f;

		}
		else if (wParam == VK_DOWN) {
			//initialize right leg
			upperRightMinAngle = 0.0f;
			lowerRightMinAngle = 0.0f;

			//right leg
			upperRightLegSpeed = -0.01f;
			lowerRightLegSpeed = -0.01f;
		}
		//'W' to walk
		else if (wParam ==  0x57) {
			//initialize angle
			upperLeftMaxAngle = 40.0f;
			lowerLeftMaxAngle = 35.0f;
			upperRightMaxAngle = 40.0f;
			lowerRightMaxAngle = 35.0f;

			//walk
			if (initialUpperRightLegSpeed == 0.0f) {
				upperLeftLegSpeed = 0.01f;
				lowerLeftLegSpeed = 0.01f;
				upperRightLegSpeed = -0.01f;
			    lowerRightLegSpeed = -0.01f;
			}
			
			if (initialUpperLeftLegSpeed == upperLeftMaxAngle) {
				upperLeftLegSpeed = -0.01f;
				lowerLeftLegSpeed = -0.01f;
				upperRightLegSpeed = 0.01f;
				lowerRightLegSpeed = 0.01f;
			}

			if (initialUpperRightLegSpeed == upperRightMaxAngle) {
				upperLeftLegSpeed = 0.01f;
				lowerLeftLegSpeed = 0.01f;
				upperRightLegSpeed = -0.01f;
				lowerRightLegSpeed = -0.01f;
			}

			//'W' key" to let the walk - hand and leg move
			lowerRightArmMaxAngle = 35.0f;
			lowerLeftArmMaxAngle = 35.0f;
			upperRightArmMaxAngle = 45.0f;
			upperLeftArmMaxAngle = 45.0f;

			if (initialLeftUpperArmSpeed == 0.0f || (initialLeftUpperArmSpeed != 0.0f && initialLeftUpperArmSpeed < upperLeftArmMaxAngle)) {
				upperRightArmSpeed = -0.01f;
				upperLeftArmSpeed = 0.01f;
				lowerRightArmSpeed = -0.01f;
				lowerLeftArmSpeed = 0.01f;

			}
			
			if (initialLeftUpperArmSpeed == upperLeftArmMaxAngle) {
				upperRightArmSpeed = 0.01f;
				upperLeftArmSpeed = -0.01f;
				lowerRightArmSpeed = 0.01f;
				lowerLeftArmSpeed = -0.01f;

			}
			if (initialRightUpperArmSpeed == upperRightArmMaxAngle) {
				upperRightArmSpeed = -0.01f;
				upperLeftArmSpeed = 0.01f;
				lowerRightArmSpeed = -0.01f;
				lowerLeftArmSpeed = 0.01f;

			}

		}
		//'S' to stand
		else if (wParam == 0x53) {
			//initialize leg angle
			upperLeftMinAngle = 0.0f;
			lowerLeftMinAngle = 0.0f;
			upperRightMinAngle = 0.0f;
			lowerRightMinAngle = 0.0f;

			//initialize arm angle
			lowerRightArmMinAngle = 0.0f;
			lowerLeftArmMinAngle = 0.0f;
			lowerRightArmMaxAngle = 0.0f;
			lowerLeftArmMaxAngle = 0.0f;
			upperRightArmMinAngle = 0.0f;
			upperLeftArmMinAngle = 0.0f;
			upperRightArmMaxAngle = 0.0f;
			upperLeftArmMaxAngle = 0.0f;

			//walk
			//left leg
			upperLeftLegSpeed = -0.01f;
			lowerLeftLegSpeed = -0.01f;

			//right leg
			upperRightLegSpeed = -0.01f;
			lowerRightLegSpeed = -0.01f;

			//left arm
			upperLeftArmSpeed = -0.01f;
			lowerLeftArmSpeed = -0.01f;
			//right arm
			upperRightArmSpeed = -0.01f;
			lowerRightArmSpeed = -0.01f;
		}
		else if (wParam == VK_LEFT) {
			bodyRotate = 0.01f;
		}
		else if (wParam == VK_RIGHT) {
			bodyRotate = -0.01f;
		}
		//'Tab' - stop
		else if (wParam == VK_TAB) {
			bodyRotate = 0.0f;

			//left leg
			upperLeftLegSpeed = 0.0f;
			lowerLeftLegSpeed = 0.0f;

			//right leg
			upperRightLegSpeed = 0.0f;
			lowerRightLegSpeed = 0.0f;

			//left hand
			upperLeftArmSpeed = 0.0f;
			lowerLeftArmSpeed = 0.0f;

			//right hand
			upperRightArmSpeed = 0.0f;
			lowerRightArmSpeed = 0.0f;
		}
		//'Space' - Reset all
		else if (wParam == VK_SPACE) {
			initialBodyRotate = 0.0f;
			bodyRotate = 0.0f;

			//left leg
			initialUpperLeftLegSpeed = 0.0f;
			upperLeftLegSpeed = 0.0f;
			initialLowerLeftLegSpeed = 0.0f;
			lowerLeftLegSpeed = 0.0f;

			//right leg
			initialUpperRightLegSpeed = 0.0f;
			upperRightLegSpeed = 0.0f;
			initialLowerRightLegSpeed = 0.0f;
			lowerRightLegSpeed = 0.0f;

			initialArmRotate = 0.0f; // need to change

			//Left Hand
			upperLeftArmSpeed = 0.0f;
			initialLeftUpperArmSpeed = 0.0f;
			lowerLeftArmSpeed = 0.0f;
			initialLeftLowerArmSpeed = 0.0f;
			move_Left_inFront = 0.0f;
			move_Left_inFront_hand = 0.0f;

			//Right Hand
			upperRightArmSpeed = 0.0f;
			initialRightUpperArmSpeed = 0.0f;
			lowerRightArmSpeed = 0.0f;
			initialRightLowerArmSpeed = 0.0f;
			move_Right_inFront = 0.0f;
			move_Right_inFront_hand = 0.0f;
		}
		//'H' - activate upper arm
		else if (wParam == 0x48) {
			lowerRightArmMinAngle = 0.0f;
			lowerLeftArmMinAngle = 0.0f;
			lowerRightArmMaxAngle = 0.0f;
			lowerLeftArmMaxAngle = 0.0f;
			upperRightArmMinAngle = 0.0f;
			upperLeftArmMinAngle = 0.0f;
			upperRightArmMaxAngle = 80.0f;
			upperLeftArmMaxAngle = 80.0f;

			if (initialRightUpperArmSpeed == 0.0f || (initialRightUpperArmSpeed != 0 && initialRightUpperArmSpeed < upperRightArmMaxAngle)) {
				upperRightArmSpeed = 0.01f;
				upperLeftArmSpeed = -0.01f;
			}
			if (initialLeftUpperArmSpeed == upperLeftArmMaxAngle) {
				upperRightArmSpeed = 0.01f;
				upperLeftArmSpeed = -0.01f;
			}
			if (initialRightUpperArmSpeed == upperRightArmMaxAngle) {
				upperRightArmSpeed = -0.01f;
				upperLeftArmSpeed = 0.01f;
			}
		}
		// 'K' - to move left upper arm up and down
		else if (wParam == 0x4B) {
			lowerRightArmMinAngle = 0.0f;
			lowerLeftArmMinAngle = 0.0f;
			lowerRightArmMaxAngle = 0.0f;
			lowerLeftArmMaxAngle = 0.0f;
			upperRightArmMinAngle = 0.0f;
			upperLeftArmMinAngle = 0.0f;
			upperRightArmMaxAngle = 0.0f;
			upperLeftArmMaxAngle = 80.0f;

			if (initialLeftUpperArmSpeed == 0.0f || (initialLeftUpperArmSpeed != 0.0f && initialLeftUpperArmSpeed < upperLeftArmMaxAngle)) {

				upperLeftArmSpeed = 0.01f;

			}
			if (initialLeftUpperArmSpeed == upperLeftArmMaxAngle) {

				upperLeftArmSpeed = -0.01f;

			}
		}
		// 'P' - to move right upper arm up and down
		else if (wParam == 0x50) {
			lowerRightArmMinAngle = 0.0f;
			lowerLeftArmMinAngle = 0.0f;
			lowerRightArmMaxAngle = 0.0f;
			lowerLeftArmMaxAngle = 0.0f;
			upperRightArmMinAngle = 0.0f;
			upperLeftArmMinAngle = 0.0f;
			upperRightArmMaxAngle = 80.0f;
			upperLeftArmMaxAngle = 0.0f;

		if (initialRightUpperArmSpeed == 0.0f || (initialRightUpperArmSpeed != 0.0 && initialRightUpperArmSpeed < upperRightArmMaxAngle)) {

			upperRightArmSpeed = 0.01f;

		}
		if (initialRightUpperArmSpeed == upperRightArmMaxAngle) {

			upperRightArmSpeed = -0.01f;

		}
		}
		//'F' - move both finger (activate and deactivate)
		else if (wParam == 0x46) { 
			if (activate == 0.0f) {

				activate = 1.0f;

			}
			else if (activate == 1.0f) {

				activate = 0.0f;

			}
		}
		else if (wParam == 0x56) {
			//'V' to activate left lower arm
			// press 'V' to up and down
			lowerRightArmMinAngle = 0.0f;
			lowerLeftArmMinAngle = 0.0f;
			lowerRightArmMaxAngle = 90.0f;
			lowerLeftArmMaxAngle = 90.0f;
			upperRightArmMinAngle = 0.0f;
			upperLeftArmMinAngle = 0.0f;
			upperRightArmMaxAngle = 0.0f;
			upperLeftArmMaxAngle = 0.0f;

			if (initialLeftLowerArmSpeed == 0.0f || (initialLeftLowerArmSpeed != 0.0f && initialLeftLowerArmSpeed < lowerLeftArmMaxAngle)) {

				lowerLeftArmSpeed = 0.01f;

			}
			if (initialLeftLowerArmSpeed == lowerLeftArmMaxAngle) {

				lowerLeftArmSpeed = -0.01f;

			}

		}
		else if (wParam == 0x5A) {
			//'Z' to activate right lower arm
			// 'Z' to up and down
			lowerRightArmMinAngle = 0.0f;
			lowerLeftArmMinAngle = 0.0f;
			lowerRightArmMaxAngle = 90.0f;
			lowerLeftArmMaxAngle = 0.0f;
			upperRightArmMinAngle = 0.0f;
			upperLeftArmMinAngle = 0.0f;
			upperRightArmMaxAngle = 0.0f;
			upperLeftArmMaxAngle = 0.0f;

			if (initialRightLowerArmSpeed == 0.0f || (initialRightLowerArmSpeed != 0.0 && initialRightLowerArmSpeed < lowerRightArmMaxAngle)) {

				lowerRightArmSpeed = 0.01f;

			}
			if (initialRightLowerArmSpeed == lowerRightArmMaxAngle) {

				lowerRightArmSpeed = -0.01f;

			}
		}
		//for finger moving
		if (activate == 1.0f) {

			if (wParam == VK_UP) {
				fingerMove = 0.005f;
				thumbMove = 0.02f;
			}

			else if (wParam == VK_DOWN) {
				fingerMove = -0.005f;
				thumbMove = -0.02f;
			}
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

void display()
{
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------

	glEnable(GL_DEPTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
		//just for rotation checkign purpose (need delete afterwards)
		glRotatef(initialBodyRotate, 0.0f, 0.5f, 0.0f);
		initialBodyRotate += bodyRotate;

		glPushMatrix();
			glScalef(0.5f, 0.5f, 0.5f);
			drawRobotHand();
		glPopMatrix();

		glPushMatrix();
			glScalef(0.5f, 0.5f, 0.5f);
			constructleg();
		glPopMatrix();
	glPopMatrix();
	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
}
//--------------------------------------------------------------------

//Draw Shape
void drawRectangle(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
	//Back
	glBegin(GL_LINE_LOOP);
		glVertex3f(minX, maxY, minZ);
		glVertex3f(minX, minY, minZ);
		glVertex3f(maxX, minY, minZ);
		glVertex3f(maxX, maxY, minZ);
	glEnd();

	//Bottom
	glBegin(GL_LINE_LOOP);
		glVertex3f(minX, minY, maxZ);
		glVertex3f(minX, minY, minZ);
		glVertex3f(maxX, minY, minZ);
		glVertex3f(maxX, minY, maxZ);
	glEnd();

	//Left
	glBegin(GL_LINE_LOOP);
		glVertex3f(minX, maxY, maxZ);
		glVertex3f(minX, maxY, minZ);
		glVertex3f(minX, minY, minZ);
		glVertex3f(minX, minY, maxZ);
	glEnd();

	//Top
	glBegin(GL_LINE_LOOP);
		glVertex3f(minX, maxY, maxZ);
		glVertex3f(minX, maxY, minZ);
		glVertex3f(maxX, maxY, minZ);
		glVertex3f(maxX, maxY, maxZ);
	glEnd();

	//Right
	glBegin(GL_LINE_LOOP);
		glVertex3f(maxX, maxY, maxZ);
		glVertex3f(maxX, maxY, minZ);
		glVertex3f(maxX, minY, minZ);
		glVertex3f(maxX, minY, maxZ);
	glEnd();

	//Front
	glBegin(GL_LINE_LOOP);
		glVertex3f(minX, maxY, maxZ);
		glVertex3f(minX, minY, maxZ);
		glVertex3f(maxX, minY, maxZ);
		glVertex3f(maxX, maxY, maxZ);
	glEnd();
}
void drawTrapezium(float minXBottom, float maxXBottom, float minXTop, float maxXTop, float yBottom, float yTop, float minZBottom, float maxZBottom, float minZTop, float maxZTop) {
	//Back
	glBegin(GL_LINE_LOOP);
		glVertex3f(minXTop, yTop, minZTop);
		glVertex3f(minXBottom, yBottom, minZBottom);
		glVertex3f(maxXBottom, yBottom, minZBottom);
		glVertex3f(maxXTop, yTop, minZTop);
	glEnd();

	//Bottom
	glBegin(GL_LINE_LOOP);
		glVertex3f(minXBottom, yBottom, minZBottom);
		glVertex3f(maxXBottom, yBottom, minZBottom);
		glVertex3f(maxXBottom, yBottom, maxZBottom);
		glVertex3f(minXBottom, yBottom, maxZBottom);
	glEnd();

	//Left
	glBegin(GL_LINE_LOOP);
		glVertex3f(maxXTop, yTop, minZTop);
		glVertex3f(maxXBottom, yBottom, minZBottom);
		glVertex3f(maxXBottom, yBottom, maxZBottom);
		glVertex3f(maxXTop, yTop, maxZTop);
	glEnd();

	//Top
	glBegin(GL_LINE_LOOP);
		glVertex3f(maxXTop, yTop, minZTop);
		glVertex3f(minXTop, yTop, minZTop);
		glVertex3f(minXTop, yTop, maxZTop);
		glVertex3f(maxXTop, yTop, maxZTop);
	glEnd();

	//Right
	glBegin(GL_LINE_LOOP);
		glVertex3f(minXTop, yTop, minZTop);
		glVertex3f(minXBottom, yBottom, minZBottom);
		glVertex3f(minXBottom, yBottom, maxZBottom);
		glVertex3f(minXTop, yTop, maxZTop);
	glEnd();

	//Front
	glBegin(GL_LINE_LOOP);
		glVertex3f(minXTop, yTop, maxZTop);
		glVertex3f(minXBottom, yBottom, maxZBottom);
		glVertex3f(maxXBottom, yBottom, maxZBottom);
		glVertex3f(maxXTop, yTop, maxZTop);
	glEnd();
}
void drawSphere(float radius) {
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();

	gluQuadricDrawStyle(sphere, GLU_POINT);
	gluSphere(sphere, radius, 30, 30);

	gluDeleteQuadric(sphere); //delete the pointer inside the memory to avoid memory full
}
void drawPyramid(float minX, float maxX, float minY, float maxY, float minZ, float maxZ, float divideX, float divideZ) {

	//face - back
	glBegin(GL_LINE_LOOP);
	glVertex3f(minX, minY, minZ);
	glVertex3f(maxX, minY, minZ);
	glVertex3f(maxX / divideX, maxY, maxZ / divideZ);
	glEnd();

	//face - top
	glBegin(GL_LINE_LOOP);
	glVertex3f(minX, minY, maxZ);
	glVertex3f(minX, minY, minZ);
	glVertex3f(maxX / divideX, maxY, maxZ / divideZ);
	glEnd();

	//face - bottom
	glBegin(GL_LINE_LOOP);
	glVertex3f(maxX, minY, minZ);
	glVertex3f(maxX, minY, maxZ);
	glVertex3f(maxX / divideX, maxY, maxZ / divideZ);
	glEnd();

	//face - front
	glBegin(GL_LINE_LOOP);
	glVertex3f(minX, minY, maxZ);
	glVertex3f(maxX, minY, maxZ);
	glVertex3f(maxX / divideX, maxY, maxZ / divideZ);
	glEnd();

	//face - base
	glBegin(GL_LINE_LOOP);
	glVertex3f(minX, minY, maxZ);
	glVertex3f(maxX, minY, maxZ);
	glVertex3f(maxX, minY, minZ);
	glVertex3f(minX, minY, minZ);
	glEnd();

}

//leg
void constructleg() {
	glPushMatrix();
	
		//draw Pelvic Girdle
		drawPelvicGirdle();

		//draw right leg pivot
		glPushMatrix();
			drawLegPivot();
		glPopMatrix();

		//draw right leg
		glPushMatrix();
			glTranslatef(-0.5f, 0.0f, 0.0f);
			drawLeg(&initialUpperRightLegSpeed, &upperRightLegSpeed, &initialLowerRightLegSpeed, &lowerRightLegSpeed, &upperRightMinAngle, &upperRightMaxAngle, &lowerRightMinAngle, &lowerRightMaxAngle);
		glPopMatrix();

		//draw right leg pivot
		glPushMatrix();
			glTranslatef(-0.3f, 0.0f, 0.2f);
			glRotatef(180.0f, 0.0f, 0.5f, 0.0f);
			drawLegPivot();
		glPopMatrix();

		//draw left leg
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, 0.0f);
			drawLeg(&initialUpperLeftLegSpeed, &upperLeftLegSpeed, &initialLowerLeftLegSpeed, &lowerLeftLegSpeed, &upperLeftMinAngle, &upperLeftMaxAngle, &lowerLeftMinAngle, &lowerLeftMaxAngle);
		glPopMatrix();

	glPopMatrix();
}
void drawPelvicGirdle() {
	glPushMatrix();
		//draw "/"
		glPushMatrix();
			//rotate vertical
			glRotatef(-90.0f, 0.0f, 0.5f, 0.0f);

			//translate trapezium
			glTranslatef(0.25f, 0.35f, 0.4f);

			drawTrapezium(-0.05f, -0.25f, -0.05f, -0.25f, 0.275f, 0.45f, -0.15f, 0.25f, -0.15f, 0.2f);
		glPopMatrix();
	
		//draw "|"
		glPushMatrix();
			//draw front
			glPushMatrix();
				//rotate vertical
				glRotatef(90.0f, 0.5f, 0.0f, 0.0f);

				//translate trapezium
				glTranslatef(0.0f, -0.075f, -0.65f);

				drawTrapezium(-0.05f, -0.25f, -0.10f, -0.2f, 0.30f, 0.45f, -0.15f, 0.35f, -0.05f, 0.225f);
			glPopMatrix();

			//draw middle
			drawRectangle(-0.25f, -0.05f, 0.30f, 0.80f, -0.025f, 0.225f);

			//draw back
			glPushMatrix();
				//rotate vertical
				glRotatef(90.0f, -0.5f, 0.0f, 0.0f);

				//translate trapezium
				glTranslatef(0.0f, -0.275f, 0.30f);

				drawTrapezium(-0.05f, -0.25f, -0.10f, -0.20f, 0.30f, 0.45f, 0.0f, 0.5f, 0.125f, 0.4f);
			glPopMatrix();
		glPopMatrix();

		//draw "\"
		glPushMatrix();
			//rotate vertical
			glRotatef(90.0f, 0.0f, 0.5f, 0.0f);

			//translate trapezium
			glTranslatef(0.05f, 0.35f, 0.1f);

			drawTrapezium(-0.05f, -0.25f, -0.05f, -0.25f, 0.275f, 0.45f, -0.15f, 0.25f, -0.15f, 0.2f);
		glPopMatrix();

	glPopMatrix();
}
void drawLeg(float *initialUpperSpeed, float *upperSpeed, float *initialLowerSpeed, float *lowerSpeed, float *upperMinAngle, float *upperMaxAngle, float *lowerMinAngle, float *lowerMaxAngle) {
	glPushMatrix();
		//draw Upper leg
		glTranslatef(-0.5f, 0.45f, 0.1f);
			glRotatef(*initialUpperSpeed, -0.5f, 0.0f, 0.0f);
		glTranslatef(0.5f, -0.45f, -0.1f);

		*initialUpperSpeed += *upperSpeed;

		if (*initialUpperSpeed >= *upperMaxAngle) {
			*initialUpperSpeed = *upperMaxAngle;
			*upperSpeed = 0.0f;
		}

		if (*initialUpperSpeed <= *upperMinAngle) {
			*initialUpperSpeed = *upperMinAngle;
			*upperSpeed = 0.0f;
		}

		drawRectangle(0.0f, 0.2f, 0.55f, 0.0f, 0.0f, 0.2f);

		glPushMatrix();
		//rotate knee to lower leg
		glTranslatef(0.155f, -0.10f, 0.1f);
			glRotatef(-*initialLowerSpeed, -0.5f, 0.0f, 0.0f);
		glTranslatef(-0.155f, 0.10f, -0.1f);

		*initialLowerSpeed += *lowerSpeed;

		if (*initialLowerSpeed >= *lowerMaxAngle) {
			*initialLowerSpeed = *lowerMaxAngle;
			*lowerSpeed = 0.0f;
		}

		if (*initialLowerSpeed <= *lowerMinAngle) {
			*initialLowerSpeed = *lowerMinAngle;
			*lowerSpeed = 0.0f;
		}

		glPushMatrix();
			glTranslatef(0.1f, -0.10f, 0.1f);
			glScalef(0.65f, 0.65f, 0.65f);
			drawLegKnee(0.15f, 20, 10);
		glPopMatrix();

		//draw lower leg (will rotate with knee)
		drawRectangle(0.0f, 0.2f, -0.2f, -0.75f, 0.0f, 0.2f);

		//draw leg sole
		drawTrapezium(-0.05f, 0.25f, -0.025f, 0.225f, -0.875f, -0.75f, -0.1f, 0.4f, -0.05f, 0.3f);

		glPopMatrix();
	glPopMatrix();
}
void drawLegKnee(float radius, int slices, int stacks) {
	GLUquadricObj *knee = NULL;
	knee = gluNewQuadric();
	gluQuadricDrawStyle(knee, GLU_LINE);
	gluSphere(knee, radius, slices, stacks);
	gluDeleteQuadric(knee);
}
void drawLegPivot() {
	glPushMatrix();
		glLineWidth(5);
		glBegin(GL_LINES);
			glVertex3f(-0.05f, 0.45f, 0.1f);
			glVertex3f(0.05f, 0.45f, 0.1f);
		glEnd();
		glLineWidth(1);

		glPushMatrix();
			glTranslatef(0.10f, 0.45f, 0.1f);
			drawLegKnee(0.05f, 10, 5);
		glPopMatrix();
	glPopMatrix();
}

//Hand
void arm(float* initialUpperArmSpeed, float* initialLowerArmSpeed, float* move_inFront_hand,
	float* upperArmSpeed, float* upperArmMaxAngle, float* upperArmMinAngle,
	float* lowerArmSpeed, float* lowerArmMaxAngle, float* lowerArmMinAngle) {

	glPushMatrix();

		glRotatef(*initialUpperArmSpeed, 0.0f, -0.1f, 0.0f); //control upper arm

		*initialUpperArmSpeed += *upperArmSpeed;

		if (*initialUpperArmSpeed >= *upperArmMaxAngle) {
			*initialUpperArmSpeed = *upperArmMaxAngle;
			*upperArmSpeed = 0.0f;
		}

		if (*initialUpperArmSpeed <= *upperArmMinAngle) {
			*initialUpperArmSpeed = *upperArmMinAngle;
			*upperArmSpeed = 0.0f;
		}

		drawRectangle(0.0f, 0.5f, 0.0f, 0.2f, 0.0f, 0.2f);
		armJoint();

			glPushMatrix();

				glTranslatef(0.52f, 0.0f, 0.0f);
				glRotatef(*initialLowerArmSpeed, 0.0f, -0.1f, 0.0f); //control the lower arm

				*initialLowerArmSpeed += *lowerArmSpeed;

				if (*initialLowerArmSpeed >= *lowerArmMaxAngle) {
					*initialLowerArmSpeed = *lowerArmMaxAngle;
					*lowerArmSpeed = 0.0f;
				}

				if (*initialLowerArmSpeed <= *lowerArmMinAngle) {
					*initialLowerArmSpeed = *lowerArmMinAngle;
					*lowerArmSpeed = 0.0f;
				}

				drawRectangle(0.0f, 0.5f, 0.0f, 0.2f, 0.0f, 0.2f);
				armJoint();

					glPushMatrix();
						glTranslatef(0.53f, 0.0f, 0.0f);
						glRotatef(*move_inFront_hand, 0.0f, 0.0f, 0.1f); //control palm
						drawRectangle(0.0f, 0.05f, 0.0f, 0.25f, 0.0f, 0.25f);
						
						glPushMatrix();
							glRotatef(100.0f, 1.0f, 0.0f, 0.0f);
							glTranslatef(-0.4f, 0.0f, -0.18f);
							glScalef(0.3f, 0.3f, 0.3f);
							drawHand();
						glPopMatrix();

					glPopMatrix();

			glPopMatrix();

	glPopMatrix();

}
void armJoint() {
	//upper arm joint
	glPushMatrix();
		glTranslatef(0.5f, 0.0f, 0.0f);
		drawSphere(0.05f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.5f, 0.2f, 0.0f);
		drawSphere(0.05f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.5f, 0.0f, 0.2f);
		drawSphere(0.05f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.5f, 0.2f, 0.2f);
		drawSphere(0.05f);
	glPopMatrix();
}
void shoulder() {

	//shoulder
	glPushMatrix();
		glTranslatef(-0.5f, -0.05f, 0.0f);
		drawRectangle(0.0f, 0.05f, 0.0f, 0.25f, 0.0f, 0.25f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.5f, 0.15f, 0.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		drawRectangle(0.0f, 0.05f, 0.0f, 0.25f, 0.0f, 0.25f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.8f, 0.2f, 0.0f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		drawRectangle(0.0f, 0.05f, 0.0f, 0.25f, 0.0f, 0.25f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.7f, 0.15f, 0.0f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		drawRectangle(0.0f, 0.05f, 0.0f, 0.25f, 0.0f, 0.25f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.6f, 0.15f, 0.0f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		drawRectangle(0.0f, 0.05f, 0.0f, 0.25f, 0.0f, 0.25f);
	glPopMatrix();
}
void drawRobotHand() {

	glPushMatrix();

		glTranslatef(0.0f, 1.7f, 0.0f);


		glPushMatrix();

			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			glTranslatef(0.0f, -0.1f, -0.8f);
			shoulder();

				glPushMatrix();

					glTranslatef(-0.725f, -0.1f, 0.23f);
					glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
					glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
					arm(&initialLeftUpperArmSpeed, &initialLeftLowerArmSpeed,
					&move_Left_inFront_hand, &upperLeftArmSpeed, &upperLeftArmMaxAngle,
					&upperLeftArmMinAngle, &lowerLeftArmSpeed, &lowerLeftArmMaxAngle, &lowerLeftArmMinAngle);
				
				glPopMatrix();
	
		glPopMatrix();

		glPushMatrix();

			glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);
			glTranslatef(-0.8f, 0.0f, 0.0f);

				glPushMatrix();

					glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
					glTranslatef(0.0f, -0.1f, -0.5f);
					glTranslatef(0.0f, 0.0f, 0.8f);
					shoulder();

					glPushMatrix();

						glTranslatef(-0.725f, -0.1f, 0.23f);
						glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
						glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
						arm(&initialRightUpperArmSpeed, &initialRightLowerArmSpeed,
						&move_Right_inFront_hand, &upperRightArmSpeed, &upperRightArmMaxAngle,
						&upperRightArmMinAngle, &lowerRightArmSpeed, &lowerRightArmMaxAngle, &lowerRightArmMinAngle);
					
					glPopMatrix();

				glPopMatrix();

		glPopMatrix();

	glPopMatrix();



}
void drawHand() {
	//Palm
	drawRectangle(1.5f, 2.0f, -0.05f, 0.55f, 0.15f, 0.35f);

	glPushMatrix();
	//Thumb
		drawPyramid(1.5f, 1.7f, 0.55f, 0.65f, 0.2f, 0.3f, 1.08f, 2.0f);

		glPushMatrix();
			//initial position of the thumb finger(inner, outer) - no animation
			glTranslatef(1.56f, 0.70f, 0.3f);
			glRotatef(42.0f, 0.0f, 0.0f, 0.5f);
			glTranslatef(-1.56f, -0.70f, -0.3f);

			//inner
			glTranslatef(1.56f, 0.65f, 0.3f);
			glRotatef(initialThumbMove, 0.0f, -0.5f, 0.0f);
			initialThumbMove += thumbMove;
			glTranslatef(-1.56f, -0.65f, -0.3f);

			if (initialThumbMove >= 45.0f) {

				initialThumbMove = 45.0f;
				thumbMove = 0.0f;

			}
			else if (initialThumbMove <= 0.0f) {

				initialThumbMove = 0.0f;
				thumbMove = 0.0f;

			}
			drawRectangle(1.56f, 1.76f, 0.50f, 0.65f, 0.2f, 0.3f);

			//outer
			adjustFingerMove(1.76f, 0.65f, 0.3f, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f);
			drawRectangle(1.76f, 1.96, 0.50f, 0.65f, 0.2f, 0.3f);
		glPopMatrix();
	glPopMatrix();

	//Index Finger
	glPushMatrix();
		//inner
		drawRectangle(2.00f, 2.10f, 0.40f, 0.50f, 0.2f, 0.3f);

		//middle
		adjustFingerMove(2.10f, 0.50f, 0.3f, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f);
		drawRectangle(2.10f, 2.25f, 0.40f, 0.50f, 0.2f, 0.3f);

		//outer
		adjustFingerMove(2.25f, 0.50f, 0.3f, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f);
		drawRectangle(2.25f, 2.45f, 0.40f, 0.50f, 0.2f, 0.3f);

	glPopMatrix();

	//Middle Finger
	glPushMatrix();
		//inner
		drawRectangle(2.00f, 2.20f, 0.25f, 0.35f, 0.2f, 0.3f);

		//middle
		adjustFingerMove(2.20f, 0.35f, 0.3f, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f);
		drawRectangle(2.20f, 2.35f, 0.25f, 0.35f, 0.2f, 0.3f);

		//outer
		adjustFingerMove(2.35f, 0.35f, 0.3f, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f);
		drawRectangle(2.35f, 2.55f, 0.25f, 0.35f, 0.2f, 0.3f);
	glPopMatrix();

	//Ring Finger
	glPushMatrix();
		//inner
		drawRectangle(2.00f, 2.10f, 0.10f, 0.20f, 0.2f, 0.3f);

		//middle
		adjustFingerMove(2.10f, 0.20f, 0.3f, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f);
		drawRectangle(2.10f, 2.25f, 0.10f, 0.20f, 0.2f, 0.3f);

		//outer
		adjustFingerMove(2.25f, 0.20f, 0.3f, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f);
		drawRectangle(2.25f, 2.45f, 0.10f, 0.20f, 0.2f, 0.3f);
	glPopMatrix();

	//Little Finger
	glPushMatrix();
		//inner
		drawRectangle(2.00f, 2.10f, -0.05f, 0.05f, 0.2f, 0.3f);

		//middle
		adjustFingerMove(2.10f, 0.05f, 0.3f, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f);
		drawRectangle(2.10f, 2.15f, -0.05f, 0.05f, 0.2f, 0.3f);

		//outer
		adjustFingerMove(2.15f, 0.05f, 0.3f, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f);
		drawRectangle(2.15f, 2.25f, -0.05f, 0.05f, 0.2f, 0.3f);
	glPopMatrix();
}
void adjustFingerMove(float translateX, float translateY, float translateZ, float rotateX, float rotateY, float rotateZ, float maxAngle, float minAngle) {
	glTranslatef(translateX, translateY, translateZ);
	glRotatef(initialFingerMove, rotateX, -rotateY, rotateZ);
	initialFingerMove += fingerMove;
	glTranslatef(-translateX, -translateY, -translateZ);

	if (initialFingerMove >= maxAngle) {

		initialFingerMove = maxAngle;
		fingerMove = 0.0f;

	}
	else if (initialFingerMove <= minAngle) {

		initialFingerMove = minAngle;
		fingerMove = minAngle;

	}
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 800,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------
