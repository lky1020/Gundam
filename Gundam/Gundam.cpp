#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"

//Speed
float speed = 0.025f;
float fingerSpeed = 0.005f;
float thumbSpeed = 0.02f;

//Rotation whole body
float initialBodyRotate = 0.0f;
float bodyRotate = 0.0f;

//projection
float tz = 1.5f, tSpeed = 0.5f;
bool isOrtho = true;
float Ry = 0.0, rSpeed = 0.5;
float Tx = 0.0, TxSpeed = 0.01;
float Ty = 0.0, TySpeed = 0.01;
int x = 0.0, y = 0.0, z = 0.0;

//Draw Shape
void drawRectangle(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
void drawTrapezium(float minXBottom, float maxXBottom, float minXTop, float maxXTop, float minYBottom, float maxYBottom, float minYTop, float maxYTop, float minZBottom, float maxZBottom, float minZTop, float maxZTop); 
void drawSphere(float radius);
void drawPyramid(float minX, float maxX, float minY, float maxY, float minZ, float maxZ, float divideX, float divideZ);
void triangularPrism(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
void octagonalPrism(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
void drawCylinder(float baseRadius, float topRadius, float height, int slices, int stacks);
void drawCoverCylinder(float baseRadius, float topRadius, float height, int slices, int stacks);
void drawCircle(float xPoint, float yPoint, float radius);

//leg - data type
//left leg
float upperLeftLegMinAngle = 0.0f;
float upperLeftLegMaxAngle = 0.0f;
float lowerLeftLegMinAngle = 0.0f;
float lowerLeftLegMaxAngle = 0.0f;
float initialUpperLeftLegSpeed = 0.0f;
float upperLeftLegSpeed = 0.0f;
float initialLowerLeftLegSpeed = 0.0f;
float lowerLeftLegSpeed = 0.0f;

//right leg
float upperRightLegMinAngle = 0.0f;
float upperRightLegMaxAngle = 0.0f;
float lowerRightLegMinAngle = 0.0f;
float lowerRightLegMaxAngle = 0.0f;
float initialUpperRightLegSpeed = 0.0f;
float upperRightLegSpeed = 0.0f;
float initialLowerRightLegSpeed = 0.0f;
float lowerRightLegSpeed = 0.0f;

//projection
void projection();

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
	float* lowerArmSpeed, float* lowerArmMaxAngle, float* lowerArmMinAngle, char direction);
void drawHand(char direction);
void adjustFingerMove(float translateX, float translateY, float translateZ, float rotateX, float rotateY, float rotateZ, float maxAngle, float minAngle, char direction);
void armJoint();
void drawRobotHand();

//Hand Variable
float initialFingerMove = 0.0f;
float fingerMove = 0.0f;
float initialThumbMove = 0.0f;
float thumbMove = 0.0f;
float handDirectionZ = 0.0f; // for detecting the hand direction for the finger movement
float initialArmRotate = 0.0f;
float move_inFront = 0.0f, move_inFront_lowerArm = 0.0f, move_inFront_hand = 0.0f;
float move_Right_inFront = 0.0f, move_Right_inFront_hand = 0.0f;
float move_Left_inFront = 0.0f, move_Left_inFront_hand = 0.0f;
float activate = 0.0f; //need to change
float initialLeftUpperArmSpeed = 0.0f, upperLeftArmSpeed = 0.0f, upperLeftArmMaxAngle = 0.0f, upperLeftArmMinAngle = 0.0f;
float initialLeftLowerArmSpeed = 0.0f, lowerLeftArmSpeed = 0.0f, lowerLeftArmMaxAngle = 0.0f, lowerLeftArmMinAngle = 0.0f;
float initialRightUpperArmSpeed = 0.0f, upperRightArmSpeed = 0.0f, upperRightArmMaxAngle = 0.0f, upperRightArmMinAngle = 0.0f;
float initialRightLowerArmSpeed = 0.0f, lowerRightArmSpeed = 0.0f, lowerRightArmMaxAngle = 0.0f, lowerRightArmMinAngle = 0.0f;
float rotateH = 0.0f, rotateHX = 0.0f, rotateHY = 0.0f, rotateHZ = 0.0f;
float rotateHSpeed = 0.0f, rotateHMaxAngle = 0.0f, rotateHMinAngle = 0.0f;
bool isShield = false;
bool isRifle = false;

//Body
void drawBody();
void drawOverallBody();
void drawBackPack();
void drawRocket();

//head function
void drawBackHead();
void drawRobotHead(float* rotateH, float* rotateHX, float* rotateHY, float* rotateHZ, float* rotateHSpeed, float* rotateHMaxAngle, float* rotateHMinAngle);

//shield
void shieldLogo(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
void shieldLogoHorizontal(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
void logoSideBRight(float minZ, float maxZ);
void logoSideBLeft(float minZ, float maxZ);
void logoSideRight(float minZ, float maxZ);
void logoSideLeft(float minZ, float maxZ);
void logoSideUp(float minZ, float maxZ);
void shieldPentagon(float minZ, float maxZ);
void DrawShield();
void DrawHandle();
void controlShield();

//Weapon
void drawBazooka();
void drawLightSword(char position);
void drawBeamRifle();

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		//'Shift' - to move left leg up
		else if (wParam == VK_SHIFT) {

			//initialize left leg
			upperLeftLegMaxAngle = 90.0f;
			lowerLeftLegMaxAngle = 85.0f;

			//left leg
			upperLeftLegSpeed = speed;
			lowerLeftLegSpeed = speed;

			//right leg
			upperRightLegSpeed = -speed;
			lowerRightLegSpeed = -speed;
			
		}
		//'Upwards' - to move right leg up
		else if (wParam == VK_UP) {
			if (isOrtho) {
				//To prevent leg move when finger move
				if (activate != 1.0f) {
					//initialize right leg
					upperRightLegMaxAngle = 90.0f;
					lowerRightLegMaxAngle = 85.0f;

					//left leg
					upperLeftLegSpeed = -speed;
					lowerLeftLegSpeed = -speed;

					//right leg
					upperRightLegSpeed = speed;
					lowerRightLegSpeed = speed;
				}
			}

		}
		//'ctrl' - to move left leg down
		else if (wParam == VK_CONTROL) {
			//initialize left leg
			upperLeftLegMinAngle = 0.0f;
			lowerLeftLegMinAngle = 0.0f;

			//left leg
			upperLeftLegSpeed = -speed;
			lowerLeftLegSpeed = -speed;

		}
		//'Downwards' - to move right leg down
		else if (wParam == VK_DOWN) {
			//To prevent leg move when finger move
			if (activate != 1.0f) {
				//initialize right leg
				upperRightLegMinAngle = 0.0f;
				lowerRightLegMinAngle = 0.0f;

				//right leg
				upperRightLegSpeed = -speed;
				lowerRightLegSpeed = -speed;
			}
		}
		//'W' to walk
		else if (wParam ==  0x57) {
			//initialize angle
			upperLeftLegMaxAngle = 40.0f;
			lowerLeftLegMaxAngle = 35.0f;
			upperRightLegMaxAngle = 40.0f;
			lowerRightLegMaxAngle = 35.0f;

			//walk
			if (initialUpperRightLegSpeed == 0.0f) {
				upperLeftLegSpeed = speed;
				lowerLeftLegSpeed = speed;
				upperRightLegSpeed = -speed;
			    lowerRightLegSpeed = -speed;
			}
			
			if (initialUpperLeftLegSpeed == upperLeftLegMaxAngle) {
				upperLeftLegSpeed = -speed;
				lowerLeftLegSpeed = -speed;
				upperRightLegSpeed = speed;
				lowerRightLegSpeed = speed;
			}

			if (initialUpperRightLegSpeed == upperRightLegMaxAngle) {
				upperLeftLegSpeed = speed;
				lowerLeftLegSpeed = speed;
				upperRightLegSpeed = -speed;
				lowerRightLegSpeed = -speed;
			}

			//'W' key" to let the walk - hand and leg move
			lowerLeftArmMaxAngle = 35.0f;
			lowerRightArmMaxAngle = 35.0f;
			upperLeftArmMaxAngle = 45.0f;
			upperRightArmMaxAngle = 45.0f;

			if (initialRightUpperArmSpeed == 0.0f) {
				upperLeftArmSpeed = -speed;
				upperRightArmSpeed = speed;
				lowerLeftArmSpeed = -speed;
				lowerRightArmSpeed = speed;

			}
			
			if (initialRightUpperArmSpeed == upperRightArmMaxAngle) {
				upperLeftArmSpeed = speed;
				upperRightArmSpeed = -speed;
				lowerLeftArmSpeed = speed;
				lowerRightArmSpeed = -speed;

			}
			if (initialLeftUpperArmSpeed == upperLeftArmMaxAngle) {
				upperLeftArmSpeed = -speed;
				upperRightArmSpeed = speed;
				lowerLeftArmSpeed = -speed;
				lowerRightArmSpeed = speed;

			}

		}
		//'S' to stand
		else if (wParam == 0x53) {
			//initialize leg angle
			upperLeftLegMinAngle = 0.0f;
			lowerLeftLegMinAngle = 0.0f;
			upperRightLegMinAngle = 0.0f;
			lowerRightLegMinAngle = 0.0f;

			//initialize arm angle
			lowerLeftArmMinAngle = 0.0f;
			lowerRightArmMinAngle = 0.0f;
			upperLeftArmMinAngle = 0.0f;
			upperRightArmMinAngle = 0.0f;

			//walk
			//left leg
			upperLeftLegSpeed = -speed;
			lowerLeftLegSpeed = -speed;

			//right leg
			upperRightLegSpeed = -speed;
			lowerRightLegSpeed = -speed;

			//left arm
			upperLeftArmSpeed = -speed;
			lowerLeftArmSpeed = -speed;

			//right arm
			upperRightArmSpeed = -speed;
			lowerRightArmSpeed = -speed;
			
		}
		//'Leftwards' - rotate body left
		else if (wParam == VK_LEFT) {
			bodyRotate = speed;
		}
		//'Rightwards' - rotate body right
		else if (wParam == VK_RIGHT) {
			bodyRotate = -speed;
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

			//head
			rotateHSpeed = 0.0f;
		}
		//'Space' - Reset all
		else if (wParam == VK_SPACE) {
			//Projection
			isOrtho = true;
			tz = 1.5f;
			Ry = 0.0f;
			Tx = 0.0f;
			Ty = 0.0f;

			//Body Rotate
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
			
			//head
			rotateH = 0.0f;
			rotateHSpeed = 0.0f;
		}
		//'H' - activate upper arm (left and right)
		else if (wParam == 0x48) {
			lowerLeftArmMinAngle = 0.0f;
			lowerRightArmMinAngle = 0.0f;
			lowerLeftArmMaxAngle = 0.0f;
			lowerRightArmMaxAngle = 0.0f;
			upperLeftArmMinAngle = 0.0f;
			upperRightArmMinAngle = 0.0f;
			upperLeftArmMaxAngle = 80.0f;
			upperRightArmMaxAngle = 80.0f;

			if (initialLeftUpperArmSpeed == 0.0f) {
				upperLeftArmSpeed = speed;
				upperRightArmSpeed = -speed;
			}
			if (initialRightUpperArmSpeed == upperRightArmMaxAngle) {
				upperLeftArmSpeed = speed;
				upperRightArmSpeed = -speed;
			}
			if (initialLeftUpperArmSpeed == upperLeftArmMaxAngle) {
				upperLeftArmSpeed = -speed;
				upperRightArmSpeed = speed;
			}
		}
		//'K' - to move left upper arm up and down
		else if (wParam == 0x4B) {
			upperLeftArmMaxAngle = 80.0f;

			if (initialLeftUpperArmSpeed == 0.0f) {

				upperLeftArmSpeed = speed;

			}
			if (initialLeftUpperArmSpeed == upperLeftArmMaxAngle) {

				upperLeftArmSpeed = -speed;

			}

			upperRightArmSpeed = -speed;
		}
		//'P' - to move right upper arm up and down
		else if (wParam == 0x50) {

			upperRightArmMaxAngle = 80.0f;

			if (initialRightUpperArmSpeed == 0.0f) {

				upperRightArmSpeed = speed;

			}
			if (initialRightUpperArmSpeed == upperRightArmMaxAngle) {

				upperRightArmSpeed = -speed;

			}

			upperLeftArmSpeed = -speed;
		}
		//'F' - to move both finger (activate and deactivate)
		else if (wParam == 0x46) { 
			if (activate == 0.0f) {

				activate = 1.0f;

			}
			else if (activate == 1.0f) {

				activate = 0.0f;

			}
		}
		//'V' - to move left lower arm up and down
		else if (wParam == 0x56) {
			lowerLeftArmMaxAngle = 90.0f;

			if (initialLeftLowerArmSpeed == 0.0f) {

				lowerLeftArmSpeed = speed;

			}
			if (initialLeftLowerArmSpeed == lowerLeftArmMaxAngle) {

				lowerLeftArmSpeed = -speed;

			}

			lowerRightArmSpeed = -speed;
		}
		//'Z' - to move right lower arm up and down
		else if (wParam == 0x5A) {
			lowerRightArmMaxAngle = 90.0f;

			if (initialRightLowerArmSpeed == 0.0f) {

				lowerRightArmSpeed = speed;

			}
			if (initialRightLowerArmSpeed == lowerRightArmMaxAngle) {

				lowerRightArmSpeed = -speed;

			}

			lowerLeftArmSpeed = -speed;
		}
		//'N' - to move head to left
		else if (wParam == 'N') {

			rotateHX = 0.0f;
			rotateHY = 0.5f;
			rotateHZ = 0.0f;
			rotateHMaxAngle = 45.0f;
			rotateHMinAngle = 0.0f;

			if (rotateH == 0.0f) {

				rotateHSpeed = speed;
			}
			else if (rotateH >= rotateHMaxAngle) {

				rotateHSpeed = -speed;
			}

		}
		//'M' to move head to right
		else if (wParam == 'M') {
			rotateHX = 0.0f;
			rotateHY = -0.5f;
			rotateHZ = 0.0f;
			rotateHMaxAngle = 45.0f;
			rotateHMinAngle = 0.0f;

			if (rotateH == 0.0f) {
	
				rotateHSpeed = speed;
			}
			else if (rotateH >= rotateHMaxAngle) {

				rotateHSpeed = -speed;
			}
		}
		//projection
		else if (wParam == VK_F2) {
			isOrtho = !isOrtho;
		}
		//zoom out (robot)
		else if (wParam == '2' || wParam == VK_NUMPAD2) {

			if (!isOrtho) {
				if (tz < 3.0) {
					tz += tSpeed;
				}
			}

		}
		//zoom in (robot)
		else if (wParam == '8' || wParam == VK_NUMPAD8) {
			if (!isOrtho) {
				if (tz > 0.0) {
					tz -= tSpeed;
				}

			}
		}
		//move view left
		else if (wParam == '4' || wParam == VK_NUMPAD4) {
			if (!isOrtho) {
				if (Tx > -1.0) {
					Tx -= TxSpeed;
				}

			}
		}
		//move view right
		else if (wParam == '6' || wParam == VK_NUMPAD6) {
			if (!isOrtho) {
				if (Tx < 1.0) {
					Tx += TxSpeed;
				}

			}
		}
		//move view up
		else if (wParam == '5' || wParam == VK_NUMPAD5) {
			if (!isOrtho) {
				if (Ty > -1.0) {
					Ty -= TySpeed;
				}

			}
		}
		//move view down
		else if (wParam == '0' || wParam == VK_NUMPAD0) {
			if (!isOrtho) {
				if (Ty < 1.0) {
					Ty += TySpeed;
				}

			}
		}
		//rotate view left
		else if (wParam == '7' || wParam == VK_NUMPAD7) {
			Ry += rSpeed;
		}
		//rotate view right
		else if (wParam == '9' || wParam == VK_NUMPAD9) {
			Ry -= rSpeed;
		}
		/////////////////////////////////
		else if (wParam == VK_F1) {
			isShield = !isShield;
			isRifle = !isRifle;

			if (isShield && isRifle) {
				fingerMove = fingerSpeed;
				thumbMove = thumbSpeed;
			}
			else {
				fingerMove = -fingerSpeed;
				thumbMove = -thumbSpeed;
			}
			
		}
		//for finger moving
		if (activate == 1.0f) {

			if (wParam == VK_UP) {
				fingerMove = fingerSpeed;
				thumbMove = thumbSpeed;
			}

			else if (wParam == VK_DOWN) {
				fingerMove = -fingerSpeed;
				thumbMove = -thumbSpeed;
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
		projection();
	
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	
		if (!isOrtho) {
			glTranslatef(0.0, 0.0, tz);
		}
		
		glPushMatrix();
			//just for rotation checking purpose (need delete afterwards)
			glRotatef(initialBodyRotate, 0.0f, 0.5f, 0.0f);
			initialBodyRotate += bodyRotate;
	
			glTranslatef(0.0f, -0.1f, 0.0f);
	
			//head
			glPushMatrix();
				glTranslatef(-0.08,0.7,0.0);
				drawRobotHead(&rotateH, &rotateHX, &rotateHY, &rotateHZ, &rotateHSpeed, &rotateHMaxAngle, &rotateHMinAngle);
			glPopMatrix();

			glPushMatrix();
				//translate whole body (combine with head)
				glTranslatef(0.0, -0.3, 0.0);
	
				//Body
				glPushMatrix();
					glScalef(0.5f, 0.5f, 0.5f);
					drawBody();
				glPopMatrix();
	
				//hand
				glPushMatrix();
					glScalef(0.5f, 0.5f, 0.5f);
					drawRobotHand();
				glPopMatrix();
	
				//leg
				glPushMatrix();
					glScalef(0.5f, 0.5f, 0.5f);
					constructleg();
				glPopMatrix();
			glPopMatrix();

			//Weapon - bazooka
			glPushMatrix();
				glTranslatef(0.0f, 0.15f, -0.05f);

				glPushMatrix();
					glTranslatef(0.0f, -0.1f, 0.0f);
					glRotatef(90.0f, 0.0f, 0.1f, 0.0f);
					glTranslatef(0.0f, 0.1f, 0.0f);

					drawBazooka();
				glPopMatrix();
			glPopMatrix();

			//weapon - right light sword
			glPushMatrix();
				drawLightSword('L');
			glPopMatrix();

			//weapon - left light sword
			glPushMatrix();
				drawLightSword('R');
			glPopMatrix();
	
		glPopMatrix();

	glPopMatrix();
	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
}
//--------------------------------------------------------------------

//projection
void projection() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//rotate view
	glRotatef(Ry, 0.0, 1.0, 0.0);
	
	if (isOrtho) {
		glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	}
	else {
		glTranslatef(Tx, Ty, 0.0f);
		gluPerspective(20.0, 1.0, -1.0, 1.0);
		glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 3.0);
	}

}

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
void drawTrapezium(float minXBottom, float maxXBottom, float minXTop, float maxXTop, float minYBottom, float maxYBottom, float minYTop, float maxYTop, float minZBottom, float maxZBottom, float minZTop, float maxZTop) {
	//Back
	glBegin(GL_LINE_LOOP);
		glVertex3f(minXTop, minYTop, minZTop);
		glVertex3f(minXBottom, minYBottom, minZBottom);
		glVertex3f(maxXBottom, maxYBottom, minZBottom);
		glVertex3f(maxXTop, maxYTop, minZTop);
	glEnd();

	//Bottom
	glBegin(GL_LINE_LOOP);
		glVertex3f(minXBottom, minYBottom, minZBottom);
		glVertex3f(maxXBottom, maxYBottom, minZBottom);
		glVertex3f(maxXBottom, maxYBottom, maxZBottom);
		glVertex3f(minXBottom, minYBottom, maxZBottom);
	glEnd();

	//Left
	glBegin(GL_LINE_LOOP);
		glVertex3f(maxXTop, maxYTop, minZTop);
		glVertex3f(maxXBottom, maxYBottom, minZBottom);
		glVertex3f(maxXBottom, maxYBottom, maxZBottom);
		glVertex3f(maxXTop, maxYTop, maxZTop);
	glEnd();

	//Top
	glBegin(GL_LINE_LOOP);
		glVertex3f(maxXTop, maxYTop, minZTop);
		glVertex3f(minXTop, minYTop, minZTop);
		glVertex3f(minXTop, minYTop, maxZTop);
		glVertex3f(maxXTop, maxYTop, maxZTop);
	glEnd();

	//Right
	glBegin(GL_LINE_LOOP);
		glVertex3f(minXTop, minYTop, minZTop);
		glVertex3f(minXBottom, minYBottom, minZBottom);
		glVertex3f(minXBottom, minYBottom, maxZBottom);
		glVertex3f(minXTop, minYTop, maxZTop);
	glEnd();

	//Front
	glBegin(GL_LINE_LOOP);
		glVertex3f(minXTop, minYTop, maxZTop);
		glVertex3f(minXBottom, minYBottom, maxZBottom);
		glVertex3f(maxXBottom, maxYBottom, maxZBottom);
		glVertex3f(maxXTop, maxYTop, maxZTop);
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
void triangularPrism(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {

	// back endcap
	glBegin(GL_LINE_LOOP);
		glVertex3f(maxX, minY, minZ);
		glVertex3f(minX, minY, minZ);
		glVertex3f(minX, maxY, minZ);
	glEnd();

	// front endcap
	glBegin(GL_LINE_LOOP);
		glVertex3f(maxX, minY, maxZ);
		glVertex3f(minX, minY, maxZ);
		glVertex3f(minX, maxY, maxZ);
	glEnd();

	// bottom
	glBegin(GL_LINE_LOOP);
		glVertex3f(minX, minY, minZ);
		glVertex3f(maxX, minY, minZ);
		glVertex3f(maxX, minY, maxZ);
		glVertex3f(minX, minY, maxZ);
	glEnd();

	// back
	glBegin(GL_LINE_LOOP);
		glVertex3f(minX, minY, minZ);
		glVertex3f(minX, maxY, minZ);
		glVertex3f(minX, maxY, maxZ);
		glVertex3f(minX, minY, maxZ);
	glEnd();

	// top
	glBegin(GL_LINE_LOOP);
		glVertex3f(minX, maxY, minZ);
		glVertex3f(maxX, minY, minZ);
		glVertex3f(maxX, minY, maxZ);
		glVertex3f(minX, maxY, maxZ);
	glEnd();
}
void octagonalPrism(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {

	// back endcap
	glBegin(GL_LINE_LOOP);
		glVertex3f(minX, maxY, minZ);
		glVertex3f(maxX, minY, minZ);
		glVertex3f(maxX, -minY, minZ);
		glVertex3f(minX, -maxY, minZ);
		glVertex3f(-minX, -maxY, minZ);
		glVertex3f(-maxX, -minY, minZ);
		glVertex3f(-maxX, minY, minZ);
		glVertex3f(-minX, maxY, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(minX, maxY, minZ);
		glVertex3f(minX, maxY, maxZ);
		glVertex3f(maxX, minY, maxZ);
		glVertex3f(maxX, minY, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(maxX, minY, minZ);
		glVertex3f(maxX, minY, maxZ);
		glVertex3f(maxX, -minY, maxZ);
		glVertex3f(maxX, -minY, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(maxX, -minY, minZ);
		glVertex3f(maxX, -minY, maxZ);
		glVertex3f(minX, -maxY, maxZ);
		glVertex3f(minX, -maxY, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(minX, -maxY, minZ);
		glVertex3f(minX, -maxY, maxZ);
		glVertex3f(-minX, -maxY, maxZ);
		glVertex3f(-minX, -maxY, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-minX, -maxY, minZ);
		glVertex3f(-minX, -maxY, maxZ);
		glVertex3f(-maxX, -minY, maxZ);
		glVertex3f(-maxX, -minY, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-maxX, -minY, minZ);
		glVertex3f(-maxX, -minY, maxZ);
		glVertex3f(-maxX, minY, maxZ);
		glVertex3f(-maxX, minY, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-maxX, minY, minZ);
		glVertex3f(-maxX, minY, maxZ);
		glVertex3f(-minX, maxY, maxZ);
		glVertex3f(-minX, maxY, minZ);
	glEnd();

	//front endcap
	glBegin(GL_LINE_LOOP);
		glVertex3f(minX, maxY, maxZ);
		glVertex3f(maxX, minY, maxZ);
		glVertex3f(maxX, -minY, maxZ);
		glVertex3f(minX, -maxY, maxZ);
		glVertex3f(-minX, -maxY, maxZ);
		glVertex3f(-maxX, -minY, maxZ);
		glVertex3f(-maxX, minY, maxZ);
		glVertex3f(-minX, maxY, maxZ);
	glEnd();
}
void drawCylinder(float baseRadius, float topRadius, float height, int slices, int stacks) {
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_LINE);
	gluCylinder(cylinder, baseRadius, topRadius, height,
		slices, stacks);
	gluDeleteQuadric(cylinder);

}
void drawCoverCylinder(float baseRadius, float topRadius, float height, int slices, int stacks) {

	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();

	gluQuadricDrawStyle(cylinder, GLU_LINE);
	gluCylinder(cylinder, baseRadius, topRadius, height, slices, stacks);

	gluDeleteQuadric(cylinder);

	//base cover
	drawCircle(0.0f, 0.0f, baseRadius);

	//top cover
	glTranslatef(0.0f, 0.0f, height);
	drawCircle(0.0f, 0.0f, topRadius);
}
void drawCircle(float xPoint, float yPoint, float radius) {

	glBegin(GL_TRIANGLE_FAN);
	//origin
	glVertex2f(xPoint, yPoint);

	for (float angle = 0; angle <= 360; angle += 0.1) {
		glVertex2f(xPoint + radius * cos(angle), yPoint + radius * sin(angle));
	}
	glEnd();
}
void drawSquareLineLoop(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4) {
	glBegin(GL_LINE_LOOP);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);
	glVertex3f(x3, y3, z3);
	glVertex3f(x4, y4, z4);
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
			drawLeg(&initialUpperRightLegSpeed, &upperRightLegSpeed, &initialLowerRightLegSpeed, &lowerRightLegSpeed, &upperRightLegMinAngle, &upperRightLegMaxAngle, &lowerRightLegMinAngle, &lowerRightLegMaxAngle);
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
			drawLeg(&initialUpperLeftLegSpeed, &upperLeftLegSpeed, &initialLowerLeftLegSpeed, &lowerLeftLegSpeed, &upperLeftLegMinAngle, &upperLeftLegMaxAngle, &lowerLeftLegMinAngle, &lowerLeftLegMaxAngle);
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

			drawTrapezium(-0.05f, -0.25f, -0.05f, -0.25f, 0.275f, 0.275f, 0.45f, 0.45f, -0.15f, 0.25f, -0.15f, 0.2f);
		glPopMatrix();
	
		//draw "|"
		glPushMatrix();
			//draw front
			glPushMatrix();
				//rotate vertical
				glRotatef(90.0f, 0.5f, 0.0f, 0.0f);

				//translate trapezium
				glTranslatef(0.0f, -0.075f, -0.65f);

				drawTrapezium(-0.05f, -0.25f, -0.10f, -0.2f, 0.30f, 0.30f, 0.45f, 0.45f, -0.15f, 0.35f, -0.05f, 0.225f);
			glPopMatrix();

			//draw middle
			drawRectangle(-0.25f, -0.05f, 0.30f, 0.80f, -0.025f, 0.225f);

			//draw back
			glPushMatrix();
				//rotate vertical
				glRotatef(90.0f, -0.5f, 0.0f, 0.0f);

				//translate trapezium
				glTranslatef(0.0f, -0.275f, 0.30f);

				drawTrapezium(-0.05f, -0.25f, -0.10f, -0.20f, 0.30f, 0.30f, 0.45f, 0.45f, 0.0f, 0.5f, 0.125f, 0.4f);
			glPopMatrix();
		glPopMatrix();

		//draw "\"
		glPushMatrix();
			//rotate vertical
			glRotatef(90.0f, 0.0f, 0.5f, 0.0f);

			//translate trapezium
			glTranslatef(0.05f, 0.35f, 0.1f);

			drawTrapezium(-0.05f, -0.25f, -0.05f, -0.25f, 0.275f, 0.275f, 0.45f, 0.45f, -0.15f, 0.25f, -0.15f, 0.2f);
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
		drawTrapezium(-0.05f, 0.25f, -0.025f, 0.225f, -0.875f, -0.875f, -0.75f, -0.75f, -0.1f, 0.4f, -0.05f, 0.3f);

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
	float* lowerArmSpeed, float* lowerArmMaxAngle, float* lowerArmMinAngle, char direction) {

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
						drawHand(direction);
					glPopMatrix();

				glPopMatrix();

				if (isShield && direction == 'L') {
					glPushMatrix();

						glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
						//glRotatef(180.0f, -1.0f, 0.0f, 0.0f);
						glTranslatef(0.0f, -0.525f, 0.95f);
						glScalef(2.0f, 2.0f, 2.0f);
						controlShield();
					glPopMatrix();
				}

				if (isRifle && direction == 'R') {
					//weapon - beam rifle
					glPushMatrix();
						glTranslatef(0.35f, 0.175f, 0.25f);

						glPushMatrix();
							//rotate to point to ground
							glTranslatef(0.35f, 0.175f, 0.25f);
							glRotatef(90.0f, -0.1f, 0.0f, 0.0f);
							glTranslatef(-0.35f, -0.175f, -0.25f);

							//rotate to fit the hand
							glTranslatef(0.35f, 0.175f, 0.25f);
							glRotatef(180.0f, 0.0f, 0.0f, 0.1f);
							glTranslatef(-0.35f, -0.175f, -0.25f);

							glScalef(0.8f, 0.8f, 0.8f);
							drawBeamRifle();
						glPopMatrix();
					glPopMatrix();
				}

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
			glTranslatef(0.5f, -0.1f, -1.0f);
			shoulder();

				glPushMatrix();

					glTranslatef(-0.525f, -0.1f, 0.025f);
					glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
					glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
					arm(&initialRightUpperArmSpeed, &initialRightLowerArmSpeed,
					&move_Right_inFront_hand, &upperRightArmSpeed, &upperRightArmMaxAngle,
					&upperRightArmMinAngle, &lowerRightArmSpeed, &lowerRightArmMaxAngle, &lowerRightArmMinAngle, 'R');

				glPopMatrix();
	
		glPopMatrix();

		glPushMatrix();

			glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);
			glTranslatef(-1.0f, 0.0f, -0.75f);

				glPushMatrix();

					glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
					glTranslatef(0.0f, -0.1f, -0.5f);
					glTranslatef(0.0f, 0.0f, 0.8f);
					shoulder();

					glPushMatrix();

						glTranslatef(-0.725f, -0.1f, 0.23f);
						glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
						glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
						arm(&initialLeftUpperArmSpeed, &initialLeftLowerArmSpeed,
						&move_Left_inFront_hand, &upperLeftArmSpeed, &upperLeftArmMaxAngle,
						&upperLeftArmMinAngle, &lowerLeftArmSpeed, &lowerLeftArmMaxAngle, &lowerLeftArmMinAngle, 'L');
					
					glPopMatrix();

				glPopMatrix();

		glPopMatrix();

	glPopMatrix();



}
void drawHand(char direction) {
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
				if (direction == 'L') {
					glRotatef(initialThumbMove, 0.0f, -0.5f, 0.0f);
					handDirectionZ = 0.3f;
				}
				else {
					glRotatef(-initialThumbMove, 0.0f, -0.5f, 0.0f);
					handDirectionZ = 0.2f;
				}
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
			adjustFingerMove(1.76f, 0.65f, handDirectionZ, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f, direction);
			drawRectangle(1.76f, 1.96f, 0.50f, 0.65f, 0.2f, 0.3f);
		glPopMatrix();
	glPopMatrix();

	//Index Finger
	glPushMatrix();
		//inner
		drawRectangle(2.00f, 2.10f, 0.40f, 0.50f, 0.2f, 0.3f);

		//middle
		adjustFingerMove(2.10f, 0.50f, handDirectionZ, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f, direction);
		drawRectangle(2.10f, 2.25f, 0.40f, 0.50f, 0.2f, 0.3f);

		//outer
		adjustFingerMove(2.25f, 0.50f, handDirectionZ, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f, direction);
		drawRectangle(2.25f, 2.45f, 0.40f, 0.50f, 0.2f, 0.3f);

	glPopMatrix();

	//Middle Finger
	glPushMatrix();
		//inner
		drawRectangle(2.00f, 2.20f, 0.25f, 0.35f, 0.2f, 0.3f);

		//middle
		adjustFingerMove(2.20f, 0.35f, handDirectionZ, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f, direction);
		drawRectangle(2.20f, 2.35f, 0.25f, 0.35f, 0.2f, 0.3f);

		//outer
		adjustFingerMove(2.35f, 0.35f, handDirectionZ, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f, direction);
		drawRectangle(2.35f, 2.55f, 0.25f, 0.35f, 0.2f, 0.3f);
	glPopMatrix();

	//Ring Finger
	glPushMatrix();
		//inner
		drawRectangle(2.00f, 2.10f, 0.10f, 0.20f, 0.2f, 0.3f);

		//middle
		adjustFingerMove(2.10f, 0.20f, handDirectionZ, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f, direction);
		drawRectangle(2.10f, 2.25f, 0.10f, 0.20f, 0.2f, 0.3f);

		//outer
		adjustFingerMove(2.25f, 0.20f, handDirectionZ, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f, direction);
		drawRectangle(2.25f, 2.45f, 0.10f, 0.20f, 0.2f, 0.3f);
	glPopMatrix();

	//Little Finger
	glPushMatrix();
		//inner
		drawRectangle(2.00f, 2.10f, -0.05f, 0.05f, 0.2f, 0.3f);

		//middle
		adjustFingerMove(2.10f, 0.05f, handDirectionZ, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f, direction);
		drawRectangle(2.10f, 2.15f, -0.05f, 0.05f, 0.2f, 0.3f);

		//outer
		adjustFingerMove(2.15f, 0.05f, handDirectionZ, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f, direction);
		drawRectangle(2.15f, 2.25f, -0.05f, 0.05f, 0.2f, 0.3f);
	glPopMatrix();
}
void adjustFingerMove(float translateX, float translateY, float translateZ, float rotateX, float rotateY, float rotateZ, float maxAngle, float minAngle, char direction) {
	glTranslatef(translateX, translateY, translateZ);
		if (direction == 'L') {
			glRotatef(initialFingerMove, rotateX, -rotateY, rotateZ);
		}
		else {
			glRotatef(-initialFingerMove, rotateX, -rotateY, rotateZ);
		}
		
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

//Body
void drawBody() {
	glPushMatrix();

		drawOverallBody();

		//Back Pack
		drawBackPack();

	glPopMatrix();
}
void drawOverallBody() {
	glPushMatrix();
		//base
		drawTrapezium(-0.45f, 0.15f, -0.435f, 0.135f, 0.8f, 0.8f, 1.0f, 1.0f, 0.0f, 0.2f, 0.025f, 0.175f);

		//middle
		drawTrapezium(-0.475f, 0.175f, -0.5f, 0.2f, 1.0f, 1.0f, 1.4f, 1.4f, 0.015f, 0.185f, 0.015f, 0.35f);

		//Top
		drawTrapezium(-0.5f, 0.2f, -0.475f, 0.175f, 1.4f, 1.4f, 1.75f, 1.75f, 0.015f, 0.35f, 0.015f, 0.185f);

		//connect left hand
		drawTrapezium(0.2f, 0.45f, 0.175f, 0.45f, 1.4f, 1.525f, 1.775f, 1.775f, 0.015f, 0.25f, 0.015f, 0.185f);

		//connect right hand
		drawTrapezium(-0.75f, -0.475f, -0.75f, -0.5f, 1.775f, 1.775f, 1.525f, 1.4f, 0.015f, 0.185f, 0.015f, 0.25f);
	glPopMatrix();
}
void drawBackPack() {
	glPushMatrix();
		//Backpack
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, -0.2f);

			glPushMatrix();
				glTranslatef(-0.15f, 1.4875f, -0.085f);
					glRotatef(90.0f, -0.1f, 0.0f, 0.0f);
				glTranslatef(0.15f, -1.4875f, 0.085f);

				drawTrapezium(-0.4f, 0.1f, -0.4f, 0.1f, 1.2f, 1.2f, 1.4f, 1.4f, 0.2f, -0.485f, 0.2f, -0.185f);
			glPopMatrix();
		glPopMatrix();

		//Rocket
		glPushMatrix();
			drawRocket();
		glPopMatrix();

	glPopMatrix();
}
void drawRocket() {
	glPushMatrix();

		//left up rocket
		glPushMatrix();
			glTranslatef(0.0f, 0.75f, -0.55f);

			glPushMatrix();
				glTranslatef(0.0f, 1.0f, -1.0f);
					glRotatef(-25.0f, 0.1f, 0.0f, 0.0f);
				glTranslatef(-0.0f, -1.0f, 1.0f);

				drawCylinder(0.075f, 0.0f, 0.15f, 20, 20);
			glPopMatrix();

		glPopMatrix();

		//left down rocket
		glPushMatrix();
			glTranslatef(0.0f, 0.575f, -0.4f);

			glPushMatrix();
				glTranslatef(0.0f, 0.6f, -0.5f);
				glRotatef(-45.0f, 0.1f, 0.0f, 0.0f);
				glTranslatef(-0.0f, -0.6f, 0.5f);

				drawCylinder(0.075f, 0.0f, 0.15f, 20, 20);
			glPopMatrix();
		glPopMatrix();

		//right up rocket
		glPushMatrix();
			glTranslatef(-0.3f, 0.75f, -0.55f);

			glPushMatrix();
				glTranslatef(0.0f, 1.0f, -1.0f);
				glRotatef(-25.0f, 0.1f, 0.0f, 0.0f);
				glTranslatef(-0.0f, -1.0f, 1.0f);

				drawCylinder(0.075f, 0.0f, 0.15f, 20, 20);
			glPopMatrix();
		glPopMatrix();

		//right down rocket
		glPushMatrix();
			glTranslatef(-0.3f, 0.575f, -0.4f);

			glPushMatrix();
				glTranslatef(0.0f, 0.6f, -0.5f);
				glRotatef(-45.0f, 0.1f, 0.0f, 0.0f);
				glTranslatef(-0.0f, -0.6f, 0.5f);

				drawCylinder(0.075f, 0.0f, 0.15f, 20, 20);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

//head
void drawBackHead() {
	glPushMatrix();
		glPushMatrix();
			
			glTranslatef(-0.05, -0.06, -0.05);
			glScalef(0.2, 0.2, 0.2);
			drawRectangle(0.0, 0.5, 0.0, 0.6, 0.0, 0.1);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-0.05, -0.06, -0.03);
			glRotatef(45, 0.0, 1.0, 0.0);
			glTranslatef(0.05, 0.06, 0.03);

			glTranslatef(-0.1, -0.06, -0.05);
			glScalef(0.2, 0.2, 0.2);
			drawRectangle(0.0, 0.25, 0.0, 0.6, 0.0, 0.1);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0.05, -0.06, -0.03);
			glRotatef(45, 0.0, -1.0, 0.0);
			glTranslatef(-0.05, 0.06, 0.03);

			glTranslatef(0.05, -0.06, -0.05);
			glScalef(0.2, 0.2, 0.2);
			drawRectangle(0.0, 0.25, 0.0, 0.6, 0.0, 0.1);
		glPopMatrix();

		glPushMatrix();

			glTranslatef(-0.05, 0.06, -0.03);
			glRotatef(50, 1.0, 0.0, 0.0);
			glTranslatef(0.05, -0.06, 0.03);

			glTranslatef(-0.05, 0.05, -0.05);
			glScalef(0.2, 0.2, 0.2);
			drawRectangle(0.0, 0.5, 0.0, 0.3, 0.0, 0.1);

		glPopMatrix();

		glPushMatrix();

			glTranslatef(-0.05, -0.06, -0.05);
			glRotatef(45, -1.0, 0.0, 0.0);
			glTranslatef(0.05, 0.06, 0.05);

			glTranslatef(-0.05, -0.12, -0.05);
			glScalef(0.2, 0.2, 0.2);
			drawRectangle(0.0, 0.5, 0.0, 0.3, 0.0, 0.1);

		glPopMatrix();
	glPopMatrix();
}
void drawRobotHead(float* rotateH, float* rotateHX, float* rotateHY, float* rotateHZ, float* rotateHSpeed, float* rotateHMaxAngle, float* rotateHMinAngle) {

	glPushMatrix();

	glRotatef(*rotateH, *rotateHX, *rotateHY, *rotateHZ);

	*rotateH += *rotateHSpeed;

	if (*rotateH >= *rotateHMaxAngle) {
		*rotateH = *rotateHMaxAngle;
		*rotateHSpeed = 0.0f;
	}

	if (*rotateH <= *rotateHMinAngle) {
		*rotateH = *rotateHMinAngle;
		*rotateHSpeed = 0.0f;
	}

	glPushMatrix();
		drawBackHead();
	glPopMatrix();

	glPushMatrix();
		octagonalPrism(0.05f, 0.1f, 0.05f, 0.1f, 0.0f, 0.1f);

		glPushMatrix();
			glRotatef(90.0, 0.0f, 1.0f, 0.0f);
			glTranslatef(-0.15f, -0.05f, -0.1f);
			glScalef(0.2, 0.2, 0.2);
			drawRectangle(0.2f, 0.5f, 0.2f, 0.5f, 0.0f, 1.0f);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-0.07, -0.15, 0.0);
				glRotatef(90, 1.0, 0.0, 0.0);
			glTranslatef(0.07, 0.15, 0.0);

			glTranslatef(0.05, 0.1, 0.0);
				glRotatef(225, 0.0, 0.0, 1.0);
			glTranslatef(-0.05, -0.1, 0.0);

			glTranslatef(0.12, 0.1, -0.13);
			triangularPrism(0.05f, 0.1f, 0.05f, 0.1f, 0.0f, 0.07f);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-0.12, -0.02, 0.0);
			glScalef(0.2, 0.2, 0.2);
			drawRectangle(0.1f, 0.25f, 0.05f, 0.35f, 0.0f, 0.5f);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0.05, -0.02, 0.0);
			glScalef(0.2, 0.2, 0.2);
			drawRectangle(0.1f, 0.25f, 0.05f, 0.35f, 0.0f, 0.5f);
		glPopMatrix();

		//inner left
		glPushMatrix();
			glTranslatef(0.05, 0.05, 0.0);
				glRotatef(240, 1.0, 1.0, 1.0);
			glTranslatef(-0.05, -0.05, 0.0);

			glTranslatef(0.0, -0.1, 0.05);

			glTranslatef(0.05, 0.05, 0.0);
				glRotatef(129.8, 0.0, 0.0, 1.0);
			glTranslatef(-0.05, -0.05, 0.0);

			glTranslatef(0.0, -0.15, 0.0);
			triangularPrism(0.05, 0.08, 0.05, 0.08, 0.0, 0.125);
		glPopMatrix();

		//inner right
		glPushMatrix();
			glTranslatef(0.05, 0.05, 0.0);
				glRotatef(240, 1.0, 1.0, 1.0);
			glTranslatef(-0.05, -0.05, 0.0);

			glTranslatef(0.0, -0.1, 0.05);

			glTranslatef(0.05, 0.05, 0.0);
				glRotatef(129.8, 0.0, 0.0, 1.0);
			glTranslatef(-0.05, -0.05, 0.0);

			glTranslatef(-0.06, -0.08, 0.0);
			triangularPrism(0.05, 0.08, 0.05, 0.08, 0.0, 0.125);
		glPopMatrix();

		//outer right
		glPushMatrix();
			glTranslatef(0.05, 0.05, 0.0);
				glRotatef(240, 1.0, 1.0, 1.0);
			glTranslatef(-0.05, -0.05, 0.0);

			glTranslatef(0.05, 0.1, 0.0);
				glRotatef(129.8, 0.0, 0.0, 1.0);
			glTranslatef(-0.05, -0.1, 0.0);

			glTranslatef(0.05, 0.1, 0.0);
				glRotatef(-60, 1.0, 0.0, 0.0);
			glTranslatef(-0.05, -0.1, 0.0);

			glTranslatef(-0.18, 0.01, 0.0);
			triangularPrism(0.05, 0.08, 0.05, 0.08, 0.0, 0.3);
		glPopMatrix();

		//outer left
		glPushMatrix();
			glTranslatef(0.05, 0.05, 0.0);
				glRotatef(240, 1.0, 1.0, 1.0);
			glTranslatef(-0.05, -0.05, 0.0);

			glTranslatef(0.05, 0.1, 0.0);
				glRotatef(129.8, 0.0, 0.0, 1.0);
			glTranslatef(-0.05, -0.1, 0.0);

			glTranslatef(0.05, 0.1, 0.0);
				glRotatef(60, 0.0, 1.0, 0.0);
			glTranslatef(-0.05, -0.1, 0.0);
			
			glTranslatef(-0.1, 0.0, -0.1);
			triangularPrism(0.05, 0.08, 0.05, 0.08, 0.0, 0.3);
		glPopMatrix();

		glPushMatrix();
			glRotatef(-90, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.0, 0.1);
			drawCylinder(0.005, 0.005, 0.1, 10, 10);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-0.018, 0.1, 0.0);
			glScalef(0.2, 0.2, 0.2);
			drawRectangle(0.0, 0.2, 0.0, 0.2, 0.0, 0.5);
		glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

//Shield
void shieldLogo(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {

	glBegin(GL_LINE_LOOP);
		glVertex3f(-maxX, maxY, minZ);
		glVertex3f(minX, minY, minZ);
		glVertex3f(maxX, maxY, minZ);
		glVertex3f(maxX, -maxY, minZ);
		glVertex3f(minX, -minY, minZ);
		glVertex3f(-maxX, -maxY, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-maxX, maxY, maxZ);
		glVertex3f(minX, minY, maxZ);
		glVertex3f(maxX, maxY, maxZ);
		glVertex3f(maxX, -maxY, maxZ);
		glVertex3f(minX, -minY, maxZ);
		glVertex3f(-maxX, -maxY, maxZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-maxX, maxY, minZ);
		glVertex3f(minX, minY, minZ);
		glVertex3f(minX, minY, maxZ);
		glVertex3f(-maxX, maxY, maxZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-maxX, maxY, minZ);
		glVertex3f(-maxX, -maxY, minZ);
		glVertex3f(-maxX, -maxY, maxZ);
		glVertex3f(-maxX, maxY, maxZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-maxX, -maxY, minZ);
		glVertex3f(minX, -minY, minZ);
		glVertex3f(minX, -minY, maxZ);
		glVertex3f(-maxX, -maxY, maxZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(minX, -minY, minZ);
		glVertex3f(maxX, -maxY, minZ);
		glVertex3f(maxX, -maxY, maxZ);
		glVertex3f(minX, -minY, maxZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(maxX, -maxY, minZ);
		glVertex3f(maxX, -maxY, maxZ);
		glVertex3f(maxX, maxY, maxZ);
		glVertex3f(maxX, maxY, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(maxX, maxY, minZ);
		glVertex3f(minX, minY, minZ);
		glVertex3f(minX, minY, maxZ);
		glVertex3f(maxX, maxY, maxZ);
	glEnd();

}
void shieldLogoHorizontal(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {

	glBegin(GL_LINE_LOOP);
		glVertex3f(-maxX, maxY, minZ);
		glVertex3f(maxX, maxY, minZ);
		glVertex3f(minX, minY, minZ);
		glVertex3f(maxX, -maxY, minZ);
		glVertex3f(-maxX, -maxY, minZ);
		glVertex3f(-minX, minY, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-maxX, maxY, maxZ);
		glVertex3f(maxX, maxY, maxZ);
		glVertex3f(minX, minY, maxZ);
		glVertex3f(maxX, -maxY, maxZ);
		glVertex3f(-maxX, -maxY, maxZ);
		glVertex3f(-minX, minY, maxZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-maxX, maxY, minZ);
		glVertex3f(-minX, minY, minZ);
		glVertex3f(-minX, minY, maxZ);
		glVertex3f(-maxX, maxY, maxZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-maxX, maxY, minZ);
		glVertex3f(-maxX, maxY, maxZ);
		glVertex3f(maxX, maxY, maxZ);
		glVertex3f(maxX, maxY, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(maxX, maxY, minZ);
		glVertex3f(maxX, maxY, maxZ);
		glVertex3f(minX, minY, maxZ);
		glVertex3f(minX, minY, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(minX, minY, minZ);
		glVertex3f(minX, minY, maxZ);
		glVertex3f(maxX, -maxY, maxZ);
		glVertex3f(maxX, -maxY, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(maxX, -maxY, minZ);
		glVertex3f(maxX, -maxY, maxZ);
		glVertex3f(-maxX, -maxY, maxZ);
		glVertex3f(-maxX, -maxY, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-maxX, -maxY, minZ);
		glVertex3f(-maxX, -maxY, maxZ);
		glVertex3f(-minX, minY, maxZ);
		glVertex3f(-minX, minY, minZ);
	glEnd();


}
void logoSideUp(float minZ, float maxZ) {

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.095f, 0.195f, minZ);
		glVertex3f(0.075f, 0.18f, minZ);
		glVertex3f(-0.075f, 0.18f, minZ);
		glVertex3f(-0.09f, 0.195f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.095f, 0.195f, maxZ);
		glVertex3f(0.075f, 0.18f, maxZ);
		glVertex3f(-0.075f, 0.18f, maxZ);
		glVertex3f(-0.09f, 0.195f, maxZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.095f, 0.195f, minZ);
		glVertex3f(0.095f, 0.195f, maxZ);
		glVertex3f(0.075f, 0.18f, maxZ);
		glVertex3f(0.075f, 0.18f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.075f, 0.18f, minZ);
		glVertex3f(0.075f, 0.18f, maxZ);
		glVertex3f(-0.075f, 0.18f, maxZ);
		glVertex3f(-0.075f, 0.18f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.075f, 0.18f, minZ);
		glVertex3f(-0.075f, 0.18f, maxZ);
		glVertex3f(-0.09f, 0.195f, maxZ);
		glVertex3f(-0.09f, 0.195f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.09f, 0.195f, minZ);
		glVertex3f(-0.09f, 0.195f, maxZ);
		glVertex3f(0.095f, 0.195f, maxZ);
		glVertex3f(0.095f, 0.195f, minZ);
	glEnd();

}
void logoSideLeft(float minZ, float maxZ) {

	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.075f, 0.18f, minZ);
		glVertex3f(-0.135f, -0.075f, minZ);
		glVertex3f(-0.15f, -0.09f, minZ);
		glVertex3f(-0.09f, 0.195f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.075f, 0.18f, maxZ);
		glVertex3f(-0.135f, -0.075f, maxZ);
		glVertex3f(-0.15f, -0.09f, maxZ);
		glVertex3f(-0.09f, 0.195f, maxZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.075f, 0.18f, minZ);
		glVertex3f(-0.075f, 0.18f, maxZ);
		glVertex3f(-0.135f, -0.075f, maxZ);
		glVertex3f(-0.135f, -0.075f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.135f, -0.075f, minZ);
		glVertex3f(-0.135f, -0.075f, maxZ);
		glVertex3f(-0.15f, -0.09f, maxZ);
		glVertex3f(-0.15f, -0.09f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.15f, -0.09f, minZ);
		glVertex3f(-0.15f, -0.09f, maxZ);
		glVertex3f(-0.09f, 0.195f, maxZ);
		glVertex3f(-0.09f, 0.195f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.09f, 0.195f, minZ);
		glVertex3f(-0.09f, 0.195f, maxZ);
		glVertex3f(-0.075f, 0.18f, maxZ);
		glVertex3f(-0.075f, 0.18f, minZ);
	glEnd();

}
void logoSideRight(float minZ, float maxZ) {

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.075f, 0.18f, minZ);
		glVertex3f(0.135f, -0.075f, minZ);
		glVertex3f(0.15f, -0.09f, minZ);
		glVertex3f(0.095f, 0.195f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.075f, 0.18f, maxZ);
		glVertex3f(0.135f, -0.075f, maxZ);
		glVertex3f(0.15f, -0.09f, maxZ);
		glVertex3f(0.095f, 0.195f, maxZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.075f, 0.18f, minZ);
		glVertex3f(0.075f, 0.18f, maxZ);
		glVertex3f(0.135f, -0.075f, maxZ);
		glVertex3f(0.135f, -0.075f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.135f, -0.075f, minZ);
		glVertex3f(0.135f, -0.075f, maxZ);
		glVertex3f(0.15f, -0.09f, maxZ);
		glVertex3f(0.15f, -0.09f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.15f, -0.09f, minZ);
		glVertex3f(0.15f, -0.09f, maxZ);
		glVertex3f(0.095f, 0.195f, maxZ);
		glVertex3f(0.095f, 0.195f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.095f, 0.195f, minZ);
		glVertex3f(0.095f, 0.195f, maxZ);
		glVertex3f(0.075f, 0.18f, maxZ);
		glVertex3f(0.075f, 0.18f, minZ);
	glEnd();
}
void logoSideBLeft(float minZ, float maxZ) {

	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.15f, -0.09f, minZ);
		glVertex3f(0.0f, -0.24f, minZ);
		glVertex3f(0.0f, -0.205f, minZ);
		glVertex3f(-0.135f, -0.075f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.15f, -0.09f, maxZ);
		glVertex3f(0.0f, -0.24f, maxZ);
		glVertex3f(0.0f, -0.205f, maxZ);
		glVertex3f(-0.135f, -0.075f, maxZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.15f, -0.09f, minZ);
		glVertex3f(-0.15f, -0.09f, maxZ);
		glVertex3f(-0.135f, -0.075f, maxZ);
		glVertex3f(-0.135f, -0.075f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.135f, -0.075f, minZ);
		glVertex3f(-0.135f, -0.075f, maxZ);
		glVertex3f(0.0f, -0.205f, maxZ);
		glVertex3f(0.0f, -0.205f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.0f, -0.205f, minZ);
		glVertex3f(0.0f, -0.205f, maxZ);
		glVertex3f(0.0f, -0.24f, maxZ);
		glVertex3f(0.0f, -0.24f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.0f, -0.24f, minZ);
		glVertex3f(0.0f, -0.24f, maxZ);
		glVertex3f(-0.15f, -0.09f, maxZ);
		glVertex3f(-0.15f, -0.09f, minZ);
	glEnd();
}
void logoSideBRight(float minZ, float maxZ) {

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.15f, -0.09f, minZ);
		glVertex3f(0.0f, -0.24f, minZ);
		glVertex3f(0.0f, -0.205f, minZ);
		glVertex3f(0.135f, -0.075f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.15f, -0.09f, maxZ);
		glVertex3f(0.0f, -0.24f, maxZ);
		glVertex3f(0.0f, -0.205f, maxZ);
		glVertex3f(0.135f, -0.075f, maxZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.15f, -0.09f, minZ);
		glVertex3f(0.15f, -0.09f, maxZ);
		glVertex3f(0.135f, -0.075f, maxZ);
		glVertex3f(0.135f, -0.075f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.135f, -0.075f, minZ);
		glVertex3f(0.135f, -0.075f, maxZ);
		glVertex3f(0.0f, -0.205f, maxZ);
		glVertex3f(0.0f, -0.205f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.0f, -0.205f, minZ);
		glVertex3f(0.0f, -0.205f, maxZ);
		glVertex3f(0.0f, -0.24f, maxZ);
		glVertex3f(0.0f, -0.24f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.0f, -0.24f, minZ);
		glVertex3f(0.0f, -0.24f, maxZ);
		glVertex3f(0.15f, -0.09f, maxZ);
		glVertex3f(0.15f, -0.09f, minZ);
	glEnd();
}
void DrawHandle() {

	glPushMatrix();
		drawRectangle(0.0f, 0.3f, 0.0f, 0.05f, 0.0f, 0.05f);
	glPopMatrix();

	glPushMatrix();
		glRotatef(60.0f, 0.0f, 0.0f, -1.0f);
		glTranslatef(-0.2f, 0.0f, 0.0f);
		drawRectangle(0.0f, 0.2f, 0.0f, 0.05f, 0.0f, 0.05f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.3f, 0.0f, 0.0f);
		glRotatef(60.0f, 0.0f, 0.0f, 1.0f);
		glTranslatef(-0.3f, -0.0f, -0.0f);
		glTranslatef(0.3f, 0.0f, 0.0f);
		drawRectangle(0.0f, 0.2f, 0.0f, 0.05f, 0.0f, 0.05f);
	glPopMatrix();

}
void DrawShield() {


	glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.15f, 0.0f, 0.2f);
		glScalef(2.0f, 2.0f, 2.0f);
		shieldPentagon(0.0f, 0.04f);

		glPushMatrix();
			glTranslatef(0.0f, 0.0f, 0.04f);
			shieldLogo(0.0f, 0.025f, 0.125f, 0.15f, 0.0f, 0.01f);
			shieldLogoHorizontal(0.075f, 0.1f, 0.0f, 0.025f, 0.0f, 0.01f);

			logoSideUp(0.0f, 0.01f);
			logoSideLeft(0.0f, 0.01f);
			logoSideRight(0.0f, 0.01f);
			logoSideBLeft(0.0f, 0.01f);
			logoSideBRight(0.0f, 0.01f);
		glPopMatrix();
	glPopMatrix();
}
void shieldPentagon(float minZ, float maxZ) {


	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.1f, 0.2f, minZ);
		glVertex3f(0.1f, 0.2f, minZ);
		glVertex3f(0.16f, -0.09f, minZ);
		glVertex3f(0.0f, -0.25f, minZ);
		glVertex3f(-0.16f, -0.09f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.1f, 0.2f, maxZ);
		glVertex3f(0.1f, 0.2f, maxZ);
		glVertex3f(0.16f, -0.09f, maxZ);
		glVertex3f(0.0f, -0.25f, maxZ);
		glVertex3f(-0.16f, -0.09f, maxZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.16f, -0.09f, minZ);
		glVertex3f(0.0f, -0.25f, minZ);
		glVertex3f(0.0f, -0.25f, maxZ);
		glVertex3f(-0.16f, -0.09f, maxZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.16f, -0.09f, minZ);
		glVertex3f(-0.16f, -0.09f, maxZ);
		glVertex3f(-0.1f, 0.2f, maxZ);
		glVertex3f(-0.1f, 0.2f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.1f, 0.2f, minZ);
		glVertex3f(-0.1f, 0.2f, maxZ);
		glVertex3f(0.1f, 0.2f, maxZ);
		glVertex3f(0.1f, 0.2f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.1f, 0.2f, minZ);
		glVertex3f(0.1f, 0.2f, maxZ);
		glVertex3f(0.16f, -0.09f, maxZ);
		glVertex3f(0.16f, -0.09f, minZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.16f, -0.09f, minZ);
		glVertex3f(0.16f, -0.09f, maxZ);
		glVertex3f(0.0f, -0.25f, maxZ);
		glVertex3f(0.0f, -0.25f, minZ);
	glEnd();

}
void controlShield() {

	glPushMatrix();
		glTranslatef(-0.2, 0.23f, -0.15f);
		glScalef(0.65f, 0.65f, 0.65f);
		DrawHandle();
		DrawShield();
	glPopMatrix();
	
}

//Weapon
void drawBazooka() {
	glPushMatrix();

		//Bazooka Body
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, -0.6f);
			drawCylinder(0.04f, 0.04f, 1.0f, 20, 20);
		glPopMatrix();

		//Muzzle
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, 0.4f);
			drawCylinder(0.04f, 0.05f, 0.08f, 15, 15);
		glPopMatrix();

		//Back Seat
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, -0.6f);
			drawCoverCylinder(0.05f, 0.05f, 0.1f, 10, 10);
		glPopMatrix();

		//Clip
		glPushMatrix();
			drawRectangle(-0.025f, 0.025f, 0.04f, -0.15f, -0.35f, -0.45f);

			//Design
			drawSquareLineLoop(-0.0255f, -0.06f, -0.451f, -0.0255f, -0.06f, -0.349f, 0.0255f, -0.06f, -0.349f, 0.0255f, -0.06f, -0.451f);
			drawSquareLineLoop(-0.0255f, -0.08f, -0.451f, -0.0255f, -0.08f, -0.349f, 0.0255f, -0.08f, -0.349f, 0.0255f, -0.08f, -0.451f);
			drawSquareLineLoop(-0.0255f, -0.10f, -0.451f, -0.0255f, -0.10f, -0.349f, 0.0255f, -0.10f, -0.349f, 0.0255f, -0.10f, -0.451f);
			drawSquareLineLoop(-0.0255f, -0.12f, -0.451f, -0.0255f, -0.12f, -0.349f, 0.0255f, -0.12f, -0.349f, 0.0255f, -0.12f, -0.451f);
		glPopMatrix();

		//Gusset
		glPushMatrix();

			//pivot
			glPushMatrix();
				glTranslatef(0.0f, 0.0f, 0.04f);
				drawCylinder(0.045f, 0.045f, 0.15f, 10, 10);
			glPopMatrix();

			//draw '-' up
			drawRectangle(-0.0255f, 0.0255f, -0.045f, -0.08f, 0.205f, 0.025f);

			//draw '|'
			drawRectangle(-0.0255f, 0.0255f, -0.081f, -0.2f, 0.075f, 0.035f);

			//draw '-' down
			drawTrapezium(-0.0255f, 0.0255f, -0.0255f, 0.0255f, -0.191f, -0.191f, -0.171f, -0.171f, 0.075f, 0.2f, 0.075f, 0.175f);

			//draw '/' down
			drawTrapezium(-0.0255f, 0.0255f, -0.0255f, 0.0255f, -0.21f, -0.21f, -0.171f, -0.171f, 0.225f, 0.275f, 0.175f, 0.225f);

		glPopMatrix();

	glPopMatrix();
}
void drawLightSword(char position) {
	glPushMatrix();
		if (toupper(position) == 'L') {

			glTranslatef(0.095f, -0.04f, 0.08f);

			glPushMatrix();
			glTranslatef(-0.3f, 0.5f, -0.1f);
			glRotatef(90.0f, 0.0f, 0.0f, 0.1f);
			glTranslatef(0.3f, -0.5f, 0.1f);

			glPushMatrix();
			glPushMatrix();
			glTranslatef(-0.2f, 0.5225f, -0.125f);
			glRotatef(90.0f, 0.0f, 0.1f, 0.0f);

			drawCoverCylinder(0.015f, 0.015f, 0.15f, 10, 10);
			glPopMatrix();

			drawTrapezium(-0.2f, -0.3f, -0.2f, -0.26f, 0.5f, 0.5f, 0.55f, 0.55f, -0.1f, -0.15f, -0.1f, -0.15f);
			glPopMatrix();

			glPopMatrix();
		}
		else if (toupper(position) == 'R') {

			glTranslatef(-0.0435f, -0.04f, 0.08f);

			glPushMatrix();

			glTranslatef(0.1f, 0.5f, -0.1f);
			glRotatef(-90.0f, 0.0f, 0.0f, 0.1f);
			glTranslatef(-0.1f, -0.5f, 0.1f);

			glPushMatrix();
			glPushMatrix();
			glTranslatef(-0.15f, 0.5225f, -0.125f);
			glRotatef(90.0f, 0.0f, 0.1f, 0.0f);

			drawCoverCylinder(0.015f, 0.015f, 0.15f, 10, 10);
			glPopMatrix();

			drawTrapezium(0.0f, 0.1f, 0.0f, 0.06f, 0.5f, 0.5f, 0.55f, 0.55f, -0.1f, -0.15f, -0.1f, -0.15f);
			glPopMatrix();

			glPopMatrix();
		}
	glPopMatrix();
}
void drawBeamRifle() {
	glPushMatrix();
		//front 
		glPushMatrix();
			glTranslatef(-0.8f, 0.002f, 0.025f);
			glRotatef(90.0f, 0.0f, 0.1f, 0.0f);

			drawCoverCylinder(0.02f, 0.02f, 0.1f, 10, 10);
		glPopMatrix();

		//drawRectangle(-0.5f, -0.4f, 0.0f, 0.06f, 0.0f, 0.05f);
	
		//middle
		glPushMatrix();
			glTranslatef(-0.7f, 0.002f, 0.025f);
			glRotatef(90.0f, 0.0f, 0.1f, 0.0f);

			drawCoverCylinder(0.035f, 0.035f, 0.3f, 10, 10);
		glPopMatrix();

		//middle-back
		drawRectangle(-0.395f, 0.4f, -0.06f, 0.06f, -0.02f, 0.07f);

		//back-up
		drawTrapezium(0.4f, 0.7f, 0.4f, 0.8, 0.02f, 0.02f, 0.08f, 0.08f, -0.02f, 0.07f, -0.02f, 0.07f);

		//back-bottom
		drawRectangle(0.4f, 0.7f, -0.08f, 0.02f, -0.02f, 0.07f);

		//sight
		glPushMatrix();
			glTranslatef(-0.1f, 0.102f, 0.025f);
			glRotatef(90.0f, 0.0f, 0.1f, 0.0f);

			drawCoverCylinder(0.04f, 0.04f, 0.15f, 10, 10);
		glPopMatrix();

		//Clip
		glPushMatrix();
			drawRectangle(-0.225f, -0.125f, -0.06f, -0.3f, -0.00f, 0.05f);

			//Design
			drawSquareLineLoop(-0.2245f, -0.09f, -0.00f, -0.2245f, -0.09f, 0.05f, -0.1245f, -0.09f, 0.05f, -0.1245f, -0.09f, -0.00f);
			drawSquareLineLoop(-0.2245f, -0.12f, -0.00f, -0.2245f, -0.12f, 0.05f, -0.1245f, -0.12f, 0.05f, -0.1245f, -0.12f, -0.00f);
			drawSquareLineLoop(-0.2245f, -0.15f, -0.00f, -0.2245f, -0.15f, 0.05f, -0.1245f, -0.15f, 0.05f, -0.1245f, -0.15f, -0.00f);
			drawSquareLineLoop(-0.2245f, -0.18f, -0.00f, -0.2245f, -0.18f, 0.05f, -0.1245f, -0.18f, 0.05f, -0.1245f, -0.18f, -0.00f);
			drawSquareLineLoop(-0.2245f, -0.21f, -0.00f, -0.2245f, -0.21f, 0.05f, -0.1245f, -0.21f, 0.05f, -0.1245f, -0.21f, -0.00f);
			drawSquareLineLoop(-0.2245f, -0.24f, -0.00f, -0.2245f, -0.24f, 0.05f, -0.1245f, -0.24f, 0.05f, -0.1245f, -0.24f, -0.00f);
			drawSquareLineLoop(-0.2245f, -0.27f, -0.00f, -0.2245f, -0.27f, 0.05f, -0.1245f, -0.27f, 0.05f, -0.1245f, -0.27f, -0.00f);

		glPopMatrix();

		//Grip - back
		drawRectangle(0.345f, 0.375f, -0.06f, -0.45f, -0.00f, 0.05f);

		//Grip - bottom
		glPushMatrix();
			glTranslatef(0.36f, -0.475f, 0.0f);
			drawCoverCylinder(0.02f, 0.02f, 0.05f, 5, 5);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0.34f, -0.465f, 0.05f);
				glRotatef(8.0f, 0.0f, 0.0f, -0.1f);
			glTranslatef(-0.34f, 0.465f, -0.05f);

			drawRectangle(0.205f, 0.34f, -0.465f, -0.5f, -0.0f, 0.05f);
		glPopMatrix();

		//Grip - front
		glPushMatrix();
			glTranslatef(0.18f, -0.46f, 0.0f);
			drawCoverCylinder(0.02f, 0.02f, 0.05f, 5, 5);
		glPopMatrix();

		drawRectangle(0.155f, 0.185f, -0.06f, -0.435f, -0.00f, 0.05f);

	glPopMatrix();
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
