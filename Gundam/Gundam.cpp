#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"

//Draw Shape
void drawRectangle(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
void drawTrapezium(float minXBottom, float maxXBottom, float minXTop, float maxXTop, float yBottom, float yTop, float minZBottom, float maxZBottom, float minZTop, float maxZTop);

//leg - data type
float initialLegRotate = 0.0f;
float legRotate = 0.0f;

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
void drawLeg(float *initialUpperSpeed, float *upperSpeed, float *initialLowerSpeed, float *lowerSpeed, float *upperMinAngle, float *upperMaxAngle, float *lowerMinAngle, float *lowerMaxAngle);
void drawLegKnee(float radius, int slices, int stacks);

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
		}
		else if (wParam == VK_UP) {
			//initialize right leg
			upperRightMaxAngle = 90.0f;
			lowerRightMaxAngle = 85.0f;

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
			upperLeftMaxAngle = 30.0f;
			lowerLeftMaxAngle = 25.0f;
			upperRightMaxAngle = 30.0f;
			lowerRightMaxAngle = 25.0f;

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
			
		}
		//'S' to stand
		else if (wParam == 0x53) {
			//initialize angle
			upperLeftMinAngle = 0.0f;
			lowerLeftMinAngle = 0.0f;
			upperRightMinAngle = 0.0f;
			lowerRightMinAngle = 0.0f;

			//walk
			//left
			upperLeftLegSpeed = -0.01f;
			lowerLeftLegSpeed = -0.01f;

			//right
			upperRightLegSpeed = -0.01f;
			lowerRightLegSpeed = -0.01f;
		}
		else if (wParam == VK_LEFT) {
			legRotate = 0.01f;
		}
		else if (wParam == VK_RIGHT) {
			legRotate = -0.01f;
		}
		//'Tab' - stop
		else if (wParam == VK_TAB) {
			legRotate = 0.0f;

			//left leg
			upperLeftLegSpeed = 0.0f;
			lowerLeftLegSpeed = 0.0f;

			//right leg
			upperRightLegSpeed = 0.0f;
			lowerRightLegSpeed = 0.0f;
		}
		//'Space' - Reset all
		else if (wParam == VK_SPACE) {
			initialLegRotate = 0.0f;
			legRotate = 0.0f;

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

		glRotatef(initialLegRotate, 0.0f, 0.5f, 0.0f);
		initialLegRotate += legRotate;

		//draw right leg
		glPushMatrix();
			glTranslatef(-0.5f, 0.0f, 0.0f);
			drawLeg(&initialUpperRightLegSpeed, &upperRightLegSpeed, &initialLowerRightLegSpeed, &lowerRightLegSpeed, &upperRightMinAngle, &upperRightMaxAngle, &lowerRightMinAngle, &lowerRightMaxAngle);
		glPopMatrix();

		//draw left leg
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, 0.0f);
			drawLeg(&initialUpperLeftLegSpeed, &upperLeftLegSpeed, &initialLowerLeftLegSpeed, &lowerLeftLegSpeed, &upperLeftMinAngle, &upperLeftMaxAngle, &lowerLeftMinAngle, &lowerLeftMaxAngle);
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

//leg
void drawLeg(float *initialUpperSpeed, float *upperSpeed, float *initialLowerSpeed, float *lowerSpeed, float *upperMinAngle, float *upperMaxAngle, float *lowerMinAngle, float *lowerMaxAngle) {
	glPushMatrix();
		//draw Upper leg
		glTranslatef(-0.5f, 0.5f, 0.0f);
			glRotatef(*initialUpperSpeed, -0.5f, 0.0f, 0.0f);
		glTranslatef(0.5f, -0.5f, 0.0f);

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
