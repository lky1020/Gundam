#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#include <string>
using namespace std;

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "GP Assignment - Gundam"

//Speed
float speed = 1.0f;
float fingerSpeed = 1.0f;
float thumbSpeed = 0.02f;

//Rotation whole body
float initialBodyRotate = 0.0f;
float bodyRotate = 0.0f;
float bodyRotateSpeed = 1.5f;

//Texture
BITMAP BMP;				//bitmap structure
HBITMAP hBMP = NULL;	//bitmap handle
GLuint textures;

//Texture (BMP)
string strLightBlueColor = "Blue_Dirty_Color.bmp";
string strRedDirtyColor = "Red_Dirty_Color.bmp";
string strGreyDirtyColor = "Grey_Dirty_Color.bmp";
string strDarkGreyDirtyColor = "Dark_Grey_Dirty_Color.bmp";
string strKneeTexture = "Knee_Texture.bmp";
string strBlacKColor = "black.bmp";
string strRobotArmJoint = "Robot_Arm_Joint.bmp";
string strHead_1 = "head_1.bmp";
string strHead_2 = "head_2.bmp";
string strHead_3 = "head_3.bmp";
string strHead_4 = "head_4.bmp";
string strHead_5 = "head_5.bmp";
string strHead_6 = "head_6.bmp";
string strHead_7 = "head_7.bmp";
string strHead_8 = "head_8.bmp";
string strHead_9 = "head_9.bmp";
string strShield_1 = "Shield_1.bmp";
string strShield_2 = "Shield_2.bmp";
string strShield_3 = "Shield_3.bmp";

//projection
float tz = 1.5f, tSpeed = 0.5f;
bool isOrtho = true;
float Ry = 0.0, rSpeed = 1.5;
float Tx = 0.0, TxSpeed = 0.01;
float Ty = 0.0, TySpeed = 0.01;
int x = 0.0, y = 0.0, z = 0.0;

//Draw Shape
void drawRectangle(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
void drawTrapeziumTexture(float minXBottom, float maxXBottom, float minXTop, float maxXTop, float minYBottom, float maxYBottom, float minYTop, float maxYTop, float minZBottom, float maxZBottom, float minZTop, float maxZTop);
void drawTrapeziumTexture_3Var(float minXBottom, float maxXBottom, float minXTop, float maxXTop, float minYBottom, float maxYBottom, float minYTop, float maxYTop, float minZBottom, float maxZBottom, float minZTop, float maxZTop, string frontBack, string topBottom, string leftRight);
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

//Texture
GLuint loadTexture(LPCSTR fileName);

//projection
void projection();

//leg
void constructleg();
void drawPelvicGirdle();
void drawLeg(float* initialUpperSpeed, float* upperSpeed, float* initialLowerSpeed, float* lowerSpeed, float* upperMinAngle, float* upperMaxAngle, float* lowerMinAngle, float* lowerMaxAngle);
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

//textures
GLuint loadTexture(LPCSTR filename);
bool isTextureChange = false;

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
		else if (wParam == 0x57) {
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
			bodyRotate = bodyRotateSpeed;
		}
		//'Rightwards' - rotate body right
		else if (wParam == VK_RIGHT) {
			bodyRotate = -bodyRotateSpeed;
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
		else if (wParam == '2' || wParam == VK_NUMPAD2 && !isTextureChange) {

			if (!isOrtho) {
				if (tz < 3.0) {
					tz += tSpeed;
				}
			}

		}
		//zoom in (robot)
		else if (wParam == '8' || wParam == VK_NUMPAD8 && !isTextureChange) {
			if (!isOrtho) {
				if (tz > 0.0) {
					tz -= tSpeed;
				}

			}
		}
		//move view left
		else if (wParam == '4' || wParam == VK_NUMPAD4 && !isTextureChange) {
			if (Tx > -1.0) {
				Tx -= TxSpeed;
			}

		}
		//move view right
		else if (wParam == '6' || wParam == VK_NUMPAD6 && !isTextureChange) {
			if (Tx < 1.0) {
				Tx += TxSpeed;
			}
		}
		//move view up
		else if (wParam == '5' || wParam == VK_NUMPAD5 && !isTextureChange) {
			if (Ty > -1.0) {
				Ty -= TySpeed;
			}
		}
		//move view down
		else if (wParam == '0' || wParam == VK_NUMPAD0 && !isTextureChange) {
			if (Ty < 1.0) {
				Ty += TySpeed;
			}
		}
		//rotate view left
		else if (wParam == '7' || wParam == VK_NUMPAD7 && !isTextureChange) {
			Ry += rSpeed;
		}
		//rotate view right
		else if (wParam == '9' || wParam == VK_NUMPAD9 && !isTextureChange) {
			Ry -= rSpeed;
		}
		// to activate shield and Rifle
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
		else if (wParam == VK_F5) {
			isTextureChange = !isTextureChange;
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

		if (isTextureChange) {
			if (wParam == '1' || wParam == VK_NUMPAD1) {
				//Texture (BMP)
				strLightBlueColor = "Red_Dirty_Color.bmp";
				strRedDirtyColor = "Blue_Dirty_Color.bmp";
				strGreyDirtyColor = "black.bmp";
				strDarkGreyDirtyColor = "head_2.bmp";
				strKneeTexture = "joint_white.bmp";
				strBlacKColor = "Shield_2.bmp";
				strRobotArmJoint = "joint_white.bmp";
				strHead_1 = "head_3.bmp";
				strHead_2 = "Red_Dirty_Color.bmp";
				strHead_3 = "Blue_Dirty_Color.bmp";
				strHead_4 = "Red_Dirty_Color.bmp";
				strHead_5 = "head_5.bmp";
				strHead_6 = "head_6.bmp";
				strHead_7 = "head_7.bmp";
				strHead_8 = "head_8.bmp";
				strHead_9 = "head_9.bmp";
				strShield_1 = "Shield_1.bmp";
				strShield_2 = "Shield_2.bmp";
				strShield_3 = "Shield_3.bmp";
			}
			else if (wParam == '2' || wParam == VK_NUMPAD2) {
				//Texture (BMP)
				strLightBlueColor = "green.bmp";
				strRedDirtyColor = "head_1.bmp";
				strGreyDirtyColor = "head_5.bmp";
				strDarkGreyDirtyColor = "orange.bmp";
				strKneeTexture = "joint_black.bmp";
				strBlacKColor = "indigo.bmp";
				strRobotArmJoint = "joint_black.bmp";
				strHead_1 = "black.bmp";
				strHead_2 = "green.bmp";
				strHead_3 = "head_1.bmp";
				strHead_4 = "green.bmp";
				strHead_5 = "head_5.bmp";
				strHead_6 = "head_6.bmp";
				strHead_7 = "head_7.bmp";
				strHead_8 = "head_8.bmp";
				strHead_9 = "head_9.bmp";
				strShield_1 = "Shield_1.bmp";
				strShield_2 = "Shield_2.bmp";
				strShield_3 = "Shield_3.bmp";
			}
			else if (wParam == '3' || wParam == VK_NUMPAD3) {
				//Texture (BMP)
				strLightBlueColor = "Shield_1.bmp";
				strRedDirtyColor = "Grey_Dirty_Color.bmp";
				strGreyDirtyColor = "orange.bmp";
				strDarkGreyDirtyColor = "orange.bmp";
				strKneeTexture = "joint_blue.bmp";
				strBlacKColor = "head_4.bmp";
				strRobotArmJoint = "joint_blue.bmp";
				strHead_1 = "Blue_Dirty_Color.bmp";
				strHead_2 = "Shield_1.bmp";
				strHead_3 = "Grey_Dirty_Color.bmp";
				strHead_4 = "Shield_1.bmp";
				strHead_5 = "head_5.bmp";
				strHead_6 = "head_6.bmp";
				strHead_7 = "head_7.bmp";
				strHead_8 = "head_8.bmp";
				strHead_9 = "head_9.bmp";
				strShield_1 = "Shield_1.bmp";
				strShield_2 = "Shield_2.bmp";
				strShield_3 = "Shield_3.bmp";
			}
			else if (wParam == '4' || wParam == VK_NUMPAD4) {
				//Texture (BMP)
				strLightBlueColor = "head_1.bmp";
				strRedDirtyColor = "orange.bmp";
				strGreyDirtyColor = "head_9.bmp";
				strDarkGreyDirtyColor = "Red_Dirty_Color.bmp";
				strKneeTexture = "joint_indigo.bmp";
				strBlacKColor = "black.bmp";
				strRobotArmJoint = "joint_indigo.bmp";
				strHead_1 = "indigo.bmp";
				strHead_2 = "head_1.bmp";
				strHead_3 = "orange.bmp";
				strHead_4 = "head_1.bmp";
				strHead_5 = "head_5.bmp";
				strHead_6 = "head_6.bmp";
				strHead_7 = "head_7.bmp";
				strHead_8 = "head_8.bmp";
				strHead_9 = "head_9.bmp";
				strShield_1 = "Shield_1.bmp";
				strShield_2 = "Shield_2.bmp";
				strShield_3 = "Shield_3.bmp";
			}
			else if (wParam == '5' || wParam == VK_NUMPAD5) {
				//Texture (BMP)
				strLightBlueColor = "head_3.bmp";
				strRedDirtyColor = "black.bmp";
				strGreyDirtyColor = "head_2.bmp";
				strDarkGreyDirtyColor = "Shield_1.bmp";
				strKneeTexture = "joint_purple.bmp";
				strBlacKColor = "Blue_Dirty_Color.bmp";
				strRobotArmJoint = "joint_purple.bmp";
				strHead_1 = "purple.bmp";
				strHead_2 = "head_3.bmp";
				strHead_3 = "black.bmp";
				strHead_4 = "head_3.bmp";
				strHead_5 = "head_5.bmp";
				strHead_6 = "head_6.bmp";
				strHead_7 = "head_7.bmp";
				strHead_8 = "head_8.bmp";
				strHead_9 = "head_9.bmp";
				strShield_1 = "Shield_1.bmp";
				strShield_2 = "Shield_2.bmp";
				strShield_3 = "Shield_3.bmp";
			}
			else if (wParam == '6' || wParam == VK_NUMPAD6) {
				//Texture (BMP)
				strLightBlueColor = "Shield_3.bmp";
				strRedDirtyColor = "green.bmp";
				strGreyDirtyColor = "Shield_3.bmp";
				strDarkGreyDirtyColor = "Shield_2.bmp";
				strKneeTexture = "joint_orange.bmp";
				strBlacKColor = "Dark_Grey_Dirty_Color.bmp";
				strRobotArmJoint = "joint_orange.bmp";
				strHead_1 = "orange.bmp";
				strHead_2 = "Shield_3.bmp";
				strHead_3 = "Shield_3.bmp";
				strHead_4 = "Shield_3.bmp";
				strHead_5 = "head_5.bmp";
				strHead_6 = "head_6.bmp";
				strHead_7 = "head_7.bmp";
				strHead_8 = "head_8.bmp";
				strHead_9 = "head_9.bmp";
				strShield_1 = "Shield_1.bmp";
				strShield_2 = "Shield_2.bmp";
				strShield_3 = "Shield_3.bmp";
			}
			else if (wParam == '7' || wParam == VK_NUMPAD7) {
				//Texture (BMP)
				strLightBlueColor = "purple.bmp";
				strRedDirtyColor = "Shield_2.bmp";
				strGreyDirtyColor = "head_4.bmp";
				strDarkGreyDirtyColor = "black.bmp";
				strKneeTexture = "joint_grey.bmp";
				strBlacKColor = "Shield_3.bmp";
				strRobotArmJoint = "joint_grey.bmp";
				strHead_1 = "Dark_Grey_Dirty_Color.bmp";
				strHead_2 = "purple.bmp";
				strHead_3 = "Shield_2.bmp";
				strHead_4 = "purple.bmp";
				strHead_5 = "head_5.bmp";
				strHead_6 = "head_6.bmp";
				strHead_7 = "head_7.bmp";
				strHead_8 = "head_8.bmp";
				strHead_9 = "head_9.bmp";
				strShield_1 = "Shield_1.bmp";
				strShield_2 = "Shield_2.bmp";
				strShield_3 = "Shield_3.bmp";
			}
			else if (wParam == '8' || wParam == VK_NUMPAD8) {
				//Texture (BMP)
				strLightBlueColor = "black.bmp";
				strRedDirtyColor = "indigo.bmp";
				strGreyDirtyColor = "Blue_Dirty_Color.bmp";
				strDarkGreyDirtyColor = "Shield_1.bmp";
				strKneeTexture = "joint_red.bmp";
				strBlacKColor = "Red_Dirty_Color.bmp";
				strRobotArmJoint = "joint_red.bmp";
				strHead_1 = "head_5.bmp";
				strHead_2 = "black.bmp";
				strHead_3 = "indigo.bmp";
				strHead_4 = "black.bmp";
				strHead_5 = "head_5.bmp";
				strHead_6 = "head_6.bmp";
				strHead_7 = "head_7.bmp";
				strHead_8 = "head_8.bmp";
				strHead_9 = "head_9.bmp";
				strShield_1 = "Shield_1.bmp";
				strShield_2 = "Shield_2.bmp";
				strShield_3 = "Shield_3.bmp";
			}
			else if (wParam == '9' || wParam == VK_NUMPAD9) {
				//Texture (BMP)
				strLightBlueColor = "Red_Dirty_Color.bmp";
				strRedDirtyColor = "Shield_1.bmp";
				strGreyDirtyColor = "orange.bmp";
				strDarkGreyDirtyColor = "black.bmp";
				strKneeTexture = "joint_yellow.bmp";
				strBlacKColor = "head_7.bmp";
				strRobotArmJoint = "joint_yellow.bmp";
				strHead_1 = "head_1.bmp";
				strHead_2 = "Red_Dirty_Color.bmp";
				strHead_3 = "Shield_1.bmp";
				strHead_4 = "Red_Dirty_Color.bmp";
				strHead_5 = "head_5.bmp";
				strHead_6 = "head_6.bmp";
				strHead_7 = "head_7.bmp";
				strHead_8 = "head_8.bmp";
				strHead_9 = "head_9.bmp";
				strShield_1 = "Shield_1.bmp";
				strShield_2 = "Shield_2.bmp";
				strShield_3 = "Shield_3.bmp";
			}
		
		}
		else {
			//Texture (BMP)
			strLightBlueColor = "Blue_Dirty_Color.bmp";
			strRedDirtyColor = "Red_Dirty_Color.bmp";
			strGreyDirtyColor = "Grey_Dirty_Color.bmp";
			strDarkGreyDirtyColor = "Dark_Grey_Dirty_Color.bmp";
			strKneeTexture = "Knee_Texture.bmp";
			strBlacKColor = "black.bmp";
			strRobotArmJoint = "Robot_Arm_Joint.bmp";
			strHead_1 = "head_1.bmp";
			strHead_2 = "head_2.bmp";
			strHead_3 = "head_3.bmp";
			strHead_4 = "head_4.bmp";
			strHead_5 = "head_5.bmp";
			strHead_6 = "head_6.bmp";
			strHead_7 = "head_7.bmp";
			strHead_8 = "head_8.bmp";
			strHead_9 = "head_9.bmp";
			strShield_1 = "Shield_1.bmp";
			strShield_2 = "Shield_2.bmp";
			strShield_3 = "Shield_3.bmp";
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
	glClearColor(0.6f, 0.857f, 0.918f, 0.0f);
	glPushMatrix();
		projection();
	
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	
		if (!isOrtho) {
			glTranslatef(0.0, 0.0, tz);
		}
		
		glPushMatrix();
			//glScalef(20.0,20.0,20.0);

			glPushMatrix();
				//just for rotation checking purpose (need delete afterwards)
				glRotatef(initialBodyRotate, 0.0f, 0.5f, 0.0f);
				initialBodyRotate += bodyRotate;

				glTranslatef(0.0f, -0.1f, 0.0f);

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

				//head
				glPushMatrix();
					glTranslatef(-0.08,0.7,0.0);
					drawRobotHead(&rotateH, &rotateHX, &rotateHY, &rotateHZ, &rotateHSpeed, &rotateHMaxAngle, &rotateHMinAngle);
				glPopMatrix();

				glPushMatrix();
					//translate whole body (combine with head)
					glTranslatef(0.0, -0.3, 0.0);
	
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

					//Body
					glPushMatrix();
						glScalef(0.5f, 0.5f, 0.5f);
						drawBody();
					glPopMatrix();
				glPopMatrix();

			glPopMatrix();

		glPopMatrix();
	glPopMatrix();
	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
}
//--------------------------------------------------------------------

//Texture
GLuint loadTexture(LPCSTR fileName) {

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL),
		fileName, IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	GetObject(hBMP, sizeof(BMP), &BMP);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &textures);
	glBindTexture(GL_TEXTURE_2D, textures);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth, BMP.bmHeight, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	DeleteObject(hBMP);
	return textures;
}

//projection
void projection() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Translate Viewport (left, right, up, down)
	glTranslatef(Tx, Ty, 0.0f);

	if (isOrtho) {
		//rotate Viewport
		glRotatef(Ry, 0.0, 1.0, 0.0);
		glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
		//glOrtho(-20.0, 20.0, -20.0, 20.0, -20.0, 20.0);
	}
	else {
		gluPerspective(20.0, 1.0, -1.0, 1.0);
		glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 3.0);
	}

}

//Draw Shape
void drawRectangle(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
	//Back
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(minX, maxY, minZ);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(minX, minY, minZ);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(maxX, minY, minZ);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(maxX, maxY, minZ);
	glEnd();

	//Bottom
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(minX, minY, maxZ);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(minX, minY, minZ);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(maxX, minY, minZ);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(maxX, minY, maxZ);
	glEnd();

	//Left
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(minX, maxY, maxZ);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(minX, maxY, minZ);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(minX, minY, minZ);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(minX, minY, maxZ);
	glEnd();

	//Top
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(minX, maxY, maxZ);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(minX, maxY, minZ);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(maxX, maxY, minZ);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(maxX, maxY, maxZ);
	glEnd();

	//Right
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(maxX, maxY, maxZ);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(maxX, maxY, minZ);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(maxX, minY, minZ);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(maxX, minY, maxZ);
	glEnd();

	//Front
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(minX, maxY, maxZ);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(minX, minY, maxZ);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(maxX, minY, maxZ);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(maxX, maxY, maxZ);
	glEnd();
}
void drawTrapeziumTexture(float minXBottom, float maxXBottom, float minXTop, float maxXTop, float minYBottom, float maxYBottom, float minYTop, float maxYTop, float minZBottom, float maxZBottom, float minZTop, float maxZTop) {
	//Back
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(minXTop, minYTop, minZTop);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(minXBottom, minYBottom, minZBottom);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(maxXBottom, maxYBottom, minZBottom);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(maxXTop, maxYTop, minZTop);
	glEnd();

	//Bottom
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(minXBottom, minYBottom, minZBottom);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(maxXBottom, maxYBottom, minZBottom);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(maxXBottom, maxYBottom, maxZBottom);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(minXBottom, minYBottom, maxZBottom);
	glEnd();

	//Left
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(maxXTop, maxYTop, minZTop);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(maxXBottom, maxYBottom, minZBottom);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(maxXBottom, maxYBottom, maxZBottom);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(maxXTop, maxYTop, maxZTop);
	glEnd();

	//Top
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(maxXTop, maxYTop, minZTop);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(minXTop, minYTop, minZTop);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(minXTop, minYTop, maxZTop);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(maxXTop, maxYTop, maxZTop);
	glEnd();

	//Right
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(minXTop, minYTop, minZTop);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(minXBottom, minYBottom, minZBottom);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(minXBottom, minYBottom, maxZBottom);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(minXTop, minYTop, maxZTop);
	glEnd();

	//Front
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(minXTop, minYTop, maxZTop);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(minXBottom, minYBottom, maxZBottom);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(maxXBottom, maxYBottom, maxZBottom);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(maxXTop, maxYTop, maxZTop);
	glEnd();
}
void drawTrapeziumTexture_3Var(float minXBottom, float maxXBottom, float minXTop, float maxXTop, float minYBottom, float maxYBottom, float minYTop, float maxYTop, float minZBottom, float maxZBottom, float minZTop, float maxZTop, string frontBack, string topBottom, string leftRight) {
	//Back
	textures = loadTexture(frontBack.c_str());
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(minXTop, minYTop, minZTop);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(minXBottom, minYBottom, minZBottom);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(maxXBottom, maxYBottom, minZBottom);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(maxXTop, maxYTop, minZTop);
	glEnd();
	glDeleteTextures(1, &textures);
	glDisable(GL_TEXTURE_2D);

	//Bottom
	textures = loadTexture(topBottom.c_str());
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(minXBottom, minYBottom, minZBottom);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(maxXBottom, maxYBottom, minZBottom);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(maxXBottom, maxYBottom, maxZBottom);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(minXBottom, minYBottom, maxZBottom);
	glEnd();
	glDeleteTextures(1, &textures);
	glDisable(GL_TEXTURE_2D);

	//Left
	textures = loadTexture(leftRight.c_str());
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(maxXTop, maxYTop, minZTop);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(maxXBottom, maxYBottom, minZBottom);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(maxXBottom, maxYBottom, maxZBottom);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(maxXTop, maxYTop, maxZTop);
	glEnd();
	glDeleteTextures(1, &textures);
	glDisable(GL_TEXTURE_2D);

	//Top
	textures = loadTexture(topBottom.c_str());
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(maxXTop, maxYTop, minZTop);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(minXTop, minYTop, minZTop);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(minXTop, minYTop, maxZTop);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(maxXTop, maxYTop, maxZTop);
	glEnd();
	glDeleteTextures(1, &textures);
	glDisable(GL_TEXTURE_2D);

	//Right
	textures = loadTexture(leftRight.c_str());
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(minXTop, minYTop, minZTop);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(minXBottom, minYBottom, minZBottom);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(minXBottom, minYBottom, maxZBottom);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(minXTop, minYTop, maxZTop);
	glEnd();
	glDeleteTextures(1, &textures);
	glDisable(GL_TEXTURE_2D);

	//Front
	textures = loadTexture(frontBack.c_str());
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(minXTop, minYTop, maxZTop);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(minXBottom, minYBottom, maxZBottom);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(maxXBottom, maxYBottom, maxZBottom);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(maxXTop, maxYTop, maxZTop);
	glEnd();
	glDeleteTextures(1, &textures);
	glDisable(GL_TEXTURE_2D);
}
void drawSphere(float radius) {
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();

	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, radius, 30, 30);

	gluDeleteQuadric(sphere); //delete the pointer inside the memory to avoid memory full
}
void drawPyramid(float minX, float maxX, float minY, float maxY, float minZ, float maxZ, float divideX, float divideZ) {

	//face - back
	glBegin(GL_TRIANGLES);
		glTexCoord2f(minX, minZ);
		glVertex3f(minX, minY, minZ);
		glTexCoord2f(maxX, minZ);
		glVertex3f(maxX, minY, minZ);
		glTexCoord2f(maxX / divideX, maxZ / divideZ);
		glVertex3f(maxX / divideX, maxY, maxZ / divideZ);
	glEnd();

	//face - top
	glBegin(GL_TRIANGLES);
		glTexCoord2f(minX, maxZ);
		glVertex3f(minX, minY, maxZ);
		glTexCoord2f(minX, minZ);
		glVertex3f(minX, minY, minZ);
		glTexCoord2f(maxX / divideX, maxZ / divideZ);
		glVertex3f(maxX / divideX, maxY, maxZ / divideZ);
	glEnd();

	//face - bottom
	glBegin(GL_TRIANGLES);
		glTexCoord2f(maxX, minZ);
		glVertex3f(maxX, minY, minZ);
		glTexCoord2f(maxX, maxZ);
		glVertex3f(maxX, minY, maxZ);
		glTexCoord2f(maxX / divideX, maxZ / divideZ);
		glVertex3f(maxX / divideX, maxY, maxZ / divideZ);
	glEnd();

	//face - front
	glBegin(GL_TRIANGLES);
		glTexCoord2f(minX, maxZ);
		glVertex3f(minX, minY, maxZ);
		glTexCoord2f(maxX, maxZ);
		glVertex3f(maxX, minY, maxZ);
		glTexCoord2f(maxX / divideX, maxZ / divideZ);
		glVertex3f(maxX / divideX, maxY, maxZ / divideZ);
	glEnd();

	//face - base
	glBegin(GL_QUADS);
		glTexCoord2f(minX, maxZ);
		glVertex3f(minX, minY, maxZ);
		glTexCoord2f(maxX, maxZ);
		glVertex3f(maxX, minY, maxZ);
		glTexCoord2f(maxX, minZ);
		glVertex3f(maxX, minY, minZ);
		glTexCoord2f(minX, minZ);
		glVertex3f(minX, minY, minZ);
	glEnd();

}
void triangularPrism(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {

	// back endcap
	glBegin(GL_TRIANGLES);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, minY, minZ);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, minY, minZ);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, maxY, minZ);
	glEnd();

	// front endcap
	glBegin(GL_TRIANGLES);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, minY, maxZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, minY, maxZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, maxY, maxZ);
	glEnd();

	// bottom
	glBegin(GL_QUADS);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, minY, minZ);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, minY, minZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, minY, maxZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, minY, maxZ);
	glEnd();

	// back
	glBegin(GL_QUADS);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, minY, minZ);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, maxY, minZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, maxY, maxZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, minY, maxZ);
	glEnd();

	// top
	glBegin(GL_QUADS);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, maxY, minZ);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, minY, minZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, minY, maxZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, maxY, maxZ);
	glEnd();
}
void octagonalPrism(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {

	// back endcap
	glBegin(GL_POLYGON);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, maxY, minZ);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, minY, minZ);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, -minY, minZ);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, -maxY, minZ);
	glTexCoord2f(-minX, minZ);
	glVertex3f(-minX, -maxY, minZ);
	glTexCoord2f(-maxX, minZ);
	glVertex3f(-maxX, -minY, minZ);
	glTexCoord2f(-maxX, minZ);
	glVertex3f(-maxX, minY, minZ);
	glTexCoord2f(-minX, minZ);
	glVertex3f(-minX, maxY, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, maxY, minZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, maxY, maxZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, minY, maxZ);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, minY, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, minY, minZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, minY, maxZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, -minY, maxZ);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, -minY, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, -minY, minZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, -minY, maxZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, -maxY, maxZ);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, -maxY, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, -maxY, minZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, -maxY, maxZ);
	glTexCoord2f(-minX, maxZ);
	glVertex3f(-minX, -maxY, maxZ);
	glTexCoord2f(-minX, minZ);
	glVertex3f(-minX, -maxY, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(-minX, minZ);
	glVertex3f(-minX, -maxY, minZ);
	glTexCoord2f(-minX, maxZ);
	glVertex3f(-minX, -maxY, maxZ);
	glTexCoord2f(-maxX, maxZ);
	glVertex3f(-maxX, -minY, maxZ);
	glTexCoord2f(-maxX, minZ);
	glVertex3f(-maxX, -minY, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(-maxX, minZ);
	glVertex3f(-maxX, -minY, minZ);
	glTexCoord2f(-maxX, maxZ);
	glVertex3f(-maxX, -minY, maxZ);
	glTexCoord2f(-maxX, maxZ);
	glVertex3f(-maxX, minY, maxZ);
	glTexCoord2f(-maxX, minZ);
	glVertex3f(-maxX, minY, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(-maxX, minZ);
	glVertex3f(-maxX, minY, minZ);
	glTexCoord2f(-maxX, maxZ);
	glVertex3f(-maxX, minY, maxZ);
	glTexCoord2f(-minX, maxZ);
	glVertex3f(-minX, maxY, maxZ);
	glTexCoord2f(-minX, minZ);
	glVertex3f(-minX, maxY, minZ);
	glEnd();

	//front endcap
	glBegin(GL_POLYGON);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, maxY, maxZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, minY, maxZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, -minY, maxZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, -maxY, maxZ);
	glTexCoord2f(-minX, maxZ);
	glVertex3f(-minX, -maxY, maxZ);
	glTexCoord2f(-maxX, maxZ);
	glVertex3f(-maxX, -minY, maxZ);
	glTexCoord2f(-maxX, maxZ);
	glVertex3f(-maxX, minY, maxZ);
	glTexCoord2f(-minX, maxZ);
	glVertex3f(-minX, maxY, maxZ);
	glEnd();
}
void drawCylinder(float baseRadius, float topRadius, float height, int slices, int stacks) {
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricTexture(cylinder, TRUE);
	gluQuadricNormals(cylinder, GLU_SMOOTH);
	gluCylinder(cylinder, baseRadius, topRadius, height,
		slices, stacks);
	gluDeleteQuadric(cylinder);

}
void drawCoverCylinder(float baseRadius, float topRadius, float height, int slices, int stacks) {

	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();

	gluQuadricDrawStyle(cylinder, GLU_FILL);
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

		//draw left leg pivot
		glPushMatrix();
			drawLegPivot();
		glPopMatrix();

		//draw right leg pivot
		glPushMatrix();
			glTranslatef(-0.3f, 0.0f, 0.2f);
			glRotatef(180.0f, 0.0f, 0.5f, 0.0f);
			drawLegPivot();
		glPopMatrix();

	//draw right leg
	glPushMatrix();
	glTranslatef(-0.5f, 0.0f, 0.0f);
	drawLeg(&initialUpperRightLegSpeed, &upperRightLegSpeed, &initialLowerRightLegSpeed, &lowerRightLegSpeed, &upperRightLegMinAngle, &upperRightLegMaxAngle, &lowerRightLegMinAngle, &lowerRightLegMaxAngle);
	glPopMatrix();

		//draw Pelvic Girdle
		drawPelvicGirdle();

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

			textures = loadTexture(strLightBlueColor.c_str());
			drawTrapeziumTexture(-0.05f, -0.25f, -0.05f, -0.25f, 0.275f, 0.275f, 0.45f, 0.45f, -0.15f, 0.25f, -0.15f, 0.2f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	
		//draw "|"
		glPushMatrix();
			//draw front
			glPushMatrix();
				//rotate vertical
				glRotatef(90.0f, 0.5f, 0.0f, 0.0f);

	//translate trapezium
	glTranslatef(0.0f, -0.075f, -0.65f);

				textures = loadTexture(strRedDirtyColor.c_str());
				drawTrapeziumTexture(-0.05f, -0.25f, -0.10f, -0.2f, 0.30f, 0.30f, 0.45f, 0.45f, -0.15f, 0.35f, -0.05f, 0.225f);
				glDeleteTextures(1, &textures);
				glDisable(GL_TEXTURE_2D);

			glPopMatrix();

			//draw middle
			textures = loadTexture(strRedDirtyColor.c_str());
			drawRectangle(-0.25f, -0.05f, 0.30f, 0.80f, -0.025f, 0.225f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

	//draw back
	glPushMatrix();
	//rotate vertical
	glRotatef(90.0f, -0.5f, 0.0f, 0.0f);

	//translate trapezium
	glTranslatef(0.0f, -0.275f, 0.30f);

				textures = loadTexture(strRedDirtyColor.c_str());
				drawTrapeziumTexture(-0.05f, -0.25f, -0.10f, -0.20f, 0.30f, 0.30f, 0.45f, 0.45f, 0.0f, 0.5f, 0.125f, 0.4f);
				glDeleteTextures(1, &textures);
				glDisable(GL_TEXTURE_2D);

			glPopMatrix();
		glPopMatrix();

	//draw "\"
	glPushMatrix();
	//rotate vertical
	glRotatef(90.0f, 0.0f, 0.5f, 0.0f);

	//translate trapezium
	glTranslatef(0.05f, 0.35f, 0.1f);

			textures = loadTexture(strLightBlueColor.c_str());
			drawTrapeziumTexture(-0.05f, -0.25f, -0.05f, -0.25f, 0.275f, 0.275f, 0.45f, 0.45f, -0.15f, 0.25f, -0.15f, 0.2f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

		glPopMatrix();

	glPopMatrix();
}
void drawLeg(float* initialUpperSpeed, float* upperSpeed, float* initialLowerSpeed, float* lowerSpeed, float* upperMinAngle, float* upperMaxAngle, float* lowerMinAngle, float* lowerMaxAngle) {
	glPushMatrix();
		//draw Upper leg (leg at the bottom, to prevent duplicate with knee textures)
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

				textures = loadTexture(strKneeTexture.c_str());
				drawLegKnee(0.15f, 20, 10);
				glDeleteTextures(1, &textures);
				glDisable(GL_TEXTURE_2D);
			glPopMatrix();

			//draw lower leg (will rotate with knee)
			textures = loadTexture(strGreyDirtyColor.c_str());
			drawRectangle(0.0f, 0.2f, -0.2f, -0.75f, 0.0f, 0.2f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

			//draw leg sole
			textures = loadTexture(strRedDirtyColor.c_str());
			drawTrapeziumTexture(-0.05f, 0.25f, -0.025f, 0.225f, -0.875f, -0.875f, -0.75f, -0.75f, -0.1f, 0.4f, -0.05f, 0.3f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		//Upper Leg (prevent duplicate textures)
		textures = loadTexture(strGreyDirtyColor.c_str());
		drawRectangle(0.0f, 0.2f, 0.55f, 0.0f, 0.0f, 0.2f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
void drawLegKnee(float radius, int slices, int stacks) {
	GLUquadricObj* knee = NULL;
	knee = gluNewQuadric();
	gluQuadricDrawStyle(knee, GLU_FILL);
	gluQuadricTexture(knee, TRUE);
	gluQuadricNormals(knee, GLU_SMOOTH);
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
			drawLegKnee(0.05f, 10, 5); //draw joint
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

		textures = loadTexture(strGreyDirtyColor.c_str());
		drawRectangle(0.0f, 0.5f, 0.0f, 0.2f, 0.0f, 0.2f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

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

			textures = loadTexture(strGreyDirtyColor.c_str());
			drawRectangle(0.0f, 0.5f, 0.0f, 0.2f, 0.0f, 0.2f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
			armJoint();

	glPushMatrix();
	glTranslatef(0.53f, 0.0f, 0.0f);
	glRotatef(*move_inFront_hand, 0.0f, 0.0f, 0.1f); //control palm
	textures = loadTexture(strBlacKColor.c_str());
	drawRectangle(0.0f, 0.05f, 0.0f, 0.25f, 0.0f, 0.25f);
	glDeleteTextures(1, &textures);
	
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
		textures = loadTexture(strRobotArmJoint.c_str());
		drawSphere(0.05f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.5f, 0.2f, 0.0f);
		textures = loadTexture(strRobotArmJoint.c_str());
		drawSphere(0.05f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.5f, 0.0f, 0.2f);
		textures = loadTexture(strRobotArmJoint.c_str());
		drawSphere(0.05f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.5f, 0.2f, 0.2f);
		textures = loadTexture(strRobotArmJoint.c_str());
		drawSphere(0.05f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}
void shoulder() {

	//shoulder
	glPushMatrix();
		glTranslatef(-0.5f, -0.05f, 0.0f);
		textures = loadTexture(strRedDirtyColor.c_str());
		drawRectangle(0.0f, 0.05f, 0.0f, 0.25f, 0.0f, 0.25f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.5f, 0.15f, 0.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		textures = loadTexture(strRedDirtyColor.c_str());
		drawRectangle(0.0f, 0.05f, 0.0f, 0.25f, 0.0f, 0.25f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.8f, 0.2f, 0.0f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		textures = loadTexture(strRedDirtyColor.c_str());
		drawRectangle(0.0f, 0.05f, 0.0f, 0.25f, 0.0f, 0.25f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.7f, 0.15f, 0.0f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		textures = loadTexture(strRedDirtyColor.c_str());
		drawRectangle(0.0f, 0.05f, 0.0f, 0.25f, 0.0f, 0.25f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.6f, 0.15f, 0.0f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		textures = loadTexture(strRedDirtyColor.c_str());
		drawRectangle(0.0f, 0.05f, 0.0f, 0.25f, 0.0f, 0.25f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);
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
	textures = loadTexture(strGreyDirtyColor.c_str());
	drawRectangle(1.5f, 2.0f, -0.05f, 0.55f, 0.15f, 0.35f);
	glDeleteTextures(1, &textures);
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
		//Thumb
		textures = loadTexture(strBlacKColor.c_str());
		drawPyramid(1.5f, 1.7f, 0.55f, 0.65f, 0.2f, 0.3f, 1.08f, 2.0f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

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
			textures = loadTexture(strBlacKColor.c_str());
			drawRectangle(1.56f, 1.76f, 0.50f, 0.65f, 0.2f, 0.3f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

	//outer
	adjustFingerMove(1.76f, 0.65f, handDirectionZ, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f, direction);

			textures = loadTexture(strBlacKColor.c_str());
			drawRectangle(1.76f, 1.96f, 0.50f, 0.65f, 0.2f, 0.3f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glPopMatrix();

	//Index Finger
	glPushMatrix();
		//inner
		textures = loadTexture(strBlacKColor.c_str());
		drawRectangle(2.00f, 2.10f, 0.40f, 0.50f, 0.2f, 0.3f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

		//middle
		adjustFingerMove(2.10f, 0.50f, handDirectionZ, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f, direction);
		textures = loadTexture(strBlacKColor.c_str());
		drawRectangle(2.10f, 2.25f, 0.40f, 0.50f, 0.2f, 0.3f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

		//outer
		adjustFingerMove(2.25f, 0.50f, handDirectionZ, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f, direction);
		textures = loadTexture(strBlacKColor.c_str());
		drawRectangle(2.25f, 2.45f, 0.40f, 0.50f, 0.2f, 0.3f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	//Middle Finger
	glPushMatrix();
		//inner
		textures = loadTexture(strBlacKColor.c_str());
		drawRectangle(2.00f, 2.20f, 0.25f, 0.35f, 0.2f, 0.3f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

		//middle
		adjustFingerMove(2.20f, 0.35f, handDirectionZ, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f, direction);
		textures = loadTexture(strBlacKColor.c_str());
		drawRectangle(2.20f, 2.35f, 0.25f, 0.35f, 0.2f, 0.3f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

		//outer
		adjustFingerMove(2.35f, 0.35f, handDirectionZ, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f, direction);
		textures = loadTexture(strBlacKColor.c_str());
		drawRectangle(2.35f, 2.55f, 0.25f, 0.35f, 0.2f, 0.3f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Ring Finger
	glPushMatrix();
		//inner
		textures = loadTexture(strBlacKColor.c_str());
		drawRectangle(2.00f, 2.10f, 0.10f, 0.20f, 0.2f, 0.3f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

		//middle
		adjustFingerMove(2.10f, 0.20f, handDirectionZ, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f, direction);
		textures = loadTexture(strBlacKColor.c_str());
		drawRectangle(2.10f, 2.25f, 0.10f, 0.20f, 0.2f, 0.3f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

		//outer
		adjustFingerMove(2.25f, 0.20f, handDirectionZ, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f, direction);
		textures = loadTexture(strBlacKColor.c_str());
		drawRectangle(2.25f, 2.45f, 0.10f, 0.20f, 0.2f, 0.3f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	//Little Finger
	glPushMatrix();
		//inner
		textures = loadTexture(strBlacKColor.c_str());
		drawRectangle(2.00f, 2.10f, -0.05f, 0.05f, 0.2f, 0.3f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

		//middle
		adjustFingerMove(2.10f, 0.05f, handDirectionZ, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f, direction);
		textures = loadTexture(strBlacKColor.c_str());
		drawRectangle(2.10f, 2.15f, -0.05f, 0.05f, 0.2f, 0.3f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

		//outer
		adjustFingerMove(2.15f, 0.05f, handDirectionZ, 0.0f, 0.5f, 0.0f, 90.0f, 0.0f, direction);
		textures = loadTexture(strBlacKColor.c_str());
		drawRectangle(2.15f, 2.25f, -0.05f, 0.05f, 0.2f, 0.3f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);
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

		if (initialBodyRotate > 90.0f || initialBodyRotate < -90.0f) {
			drawOverallBody();
		
			//Back Pack
			drawBackPack();
		}
		else {
			//Back Pack
			drawBackPack();

			drawOverallBody();
		}

	glPopMatrix();
}
void drawOverallBody() {
	glPushMatrix();
		//base
		textures = loadTexture(strGreyDirtyColor.c_str());
		drawTrapeziumTexture(-0.45f, 0.15f, -0.435f, 0.135f, 0.8f, 0.8f, 1.0f, 1.0f, 0.0f, 0.2f, 0.025f, 0.175f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

		//middle
		drawTrapeziumTexture_3Var(-0.475f, 0.175f, -0.5f, 0.2f, 1.0f, 1.0f, 1.4f, 1.4f, 0.015f, 0.185f, 0.015f, 0.35f, strRedDirtyColor, strRedDirtyColor, strGreyDirtyColor);

		if (initialBodyRotate < 0.0f) {
			//connect right hand
			textures = loadTexture(strRedDirtyColor.c_str());
			drawTrapeziumTexture(-0.75f, -0.475f, -0.75f, -0.5f, 1.775f, 1.775f, 1.525f, 1.4f, 0.015f, 0.185f, 0.015f, 0.25f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

			//Top
			textures = loadTexture(strRedDirtyColor.c_str());
			drawTrapeziumTexture(-0.5f, 0.2f, -0.475f, 0.175f, 1.4f, 1.4f, 1.75f, 1.75f, 0.015f, 0.35f, 0.015f, 0.185f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

			//connect left hand
			textures = loadTexture(strRedDirtyColor.c_str());
			drawTrapeziumTexture(0.2f, 0.45f, 0.175f, 0.45f, 1.4f, 1.525f, 1.775f, 1.775f, 0.015f, 0.25f, 0.015f, 0.185f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		}
		else if (initialBodyRotate > 0.0f) {
			//connect left hand
			textures = loadTexture(strRedDirtyColor.c_str());
			drawTrapeziumTexture(0.2f, 0.45f, 0.175f, 0.45f, 1.4f, 1.525f, 1.775f, 1.775f, 0.015f, 0.25f, 0.015f, 0.185f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

			//Top
			textures = loadTexture(strRedDirtyColor.c_str());
			drawTrapeziumTexture(-0.5f, 0.2f, -0.475f, 0.175f, 1.4f, 1.4f, 1.75f, 1.75f, 0.015f, 0.35f, 0.015f, 0.185f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

			//connect right hand
			textures = loadTexture(strRedDirtyColor.c_str());
			drawTrapeziumTexture(-0.75f, -0.475f, -0.75f, -0.5f, 1.775f, 1.775f, 1.525f, 1.4f, 0.015f, 0.185f, 0.015f, 0.25f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		}
		else {
			//connect right hand
			textures = loadTexture(strRedDirtyColor.c_str());
			drawTrapeziumTexture(-0.75f, -0.475f, -0.75f, -0.5f, 1.775f, 1.775f, 1.525f, 1.4f, 0.015f, 0.185f, 0.015f, 0.25f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

			//Top
			textures = loadTexture(strRedDirtyColor.c_str());
			drawTrapeziumTexture(-0.5f, 0.2f, -0.475f, 0.175f, 1.4f, 1.4f, 1.75f, 1.75f, 0.015f, 0.35f, 0.015f, 0.185f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

			//connect left hand
			textures = loadTexture(strRedDirtyColor.c_str());
			drawTrapeziumTexture(0.2f, 0.45f, 0.175f, 0.45f, 1.4f, 1.525f, 1.775f, 1.775f, 0.015f, 0.25f, 0.015f, 0.185f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		}

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

				textures = loadTexture(strDarkGreyDirtyColor.c_str());
				drawTrapeziumTexture(-0.4f, 0.1f, -0.4f, 0.1f, 1.2f, 1.2f, 1.4f, 1.4f, 0.2f, -0.485f, 0.2f, -0.185f);
				glDeleteTextures(1, &textures);
				glDisable(GL_TEXTURE_2D);
				
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

			glTranslatef(-0.05f, -0.06f, -0.05f);
			glScalef(0.2f, 0.2f, 0.2f);
			textures = loadTexture(strHead_9.c_str());
				drawRectangle(0.0f, 0.5f, 0.0f, 0.6f, 0.0f, 0.1f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.05f, -0.06f, -0.03f);
	glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.05f, 0.06f, 0.03f);

			glTranslatef(-0.1f, -0.06f, -0.05f);
			glScalef(0.2f, 0.2f, 0.2f);
			textures = loadTexture(strHead_9.c_str());
				drawRectangle(0.0f, 0.25f, 0.0f, 0.6f, 0.0f, 0.1f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

	glPushMatrix();
	glTranslatef(0.05f, -0.06f, -0.03f);
	glRotatef(45.0f, 0.0f, -1.0f, 0.0f);
	glTranslatef(-0.05f, 0.06f, 0.03f);

			glTranslatef(0.05f, -0.06f, -0.05f);
			glScalef(0.2f, 0.2f, 0.2f);
			textures = loadTexture(strHead_9.c_str());
				drawRectangle(0.0f, 0.25f, 0.0f, 0.6f, 0.0f, 0.1f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

	glPushMatrix();

	glTranslatef(-0.05f, 0.06f, -0.03f);
	glRotatef(50.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.05f, -0.06f, 0.03f);

			glTranslatef(-0.05f, 0.05f, -0.05f);
			glScalef(0.2f, 0.2f, 0.2f);
			textures = loadTexture(strHead_9.c_str());
				drawRectangle(0.0f, 0.5f, 0.0f, 0.3f, 0.0f, 0.1f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		glPushMatrix();

			glTranslatef(-0.05f, -0.06f, -0.05f);
			glRotatef(45.0f, -1.0f, 0.0f, 0.0f);
			glTranslatef(0.05f, 0.06f, 0.05f);

			glTranslatef(-0.05f, -0.12f, -0.05f);
			textures = loadTexture(strHead_9.c_str());
			glScalef(0.2f, 0.2f, 0.2f);
			drawRectangle(0.0f, 0.5f, 0.0f, 0.3f, 0.0f, 0.1f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
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
			textures = loadTexture(strHead_3.c_str());
				octagonalPrism(0.05f, 0.1f, 0.05f, 0.1f, 0.0f, 0.1f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		glPushMatrix();
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, 0.0f, 0.1f);
			textures = loadTexture(strHead_7.c_str());
				drawCylinder(0.005f, 0.005f, 0.1f, 10.0f, 10.0f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-0.018f, 0.1f, 0.0f);
			glScalef(0.2f, 0.2f, 0.2f);
			textures = loadTexture(strHead_6.c_str());
				drawRectangle(0.0f, 0.2f, 0.0f, 0.2f, 0.0f, 0.5f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		glPushMatrix();
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			glTranslatef(-0.15f, -0.05f, -0.1f);
			glScalef(0.2f, 0.2f, 0.2f);
			textures = loadTexture(strHead_2.c_str());
				drawRectangle(0.2f, 0.5f, 0.2f, 0.5f, 0.0f, 1.0f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-0.07f, -0.15f, 0.0f);
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.07f, 0.15f, 0.0f);

			glTranslatef(0.05f, 0.1f, 0.0f);
			glRotatef(225.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(-0.05f, -0.1f, 0.0f);

			glTranslatef(0.12f, 0.1f, -0.13f);
			textures = loadTexture(strHead_6.c_str());
				triangularPrism(0.05f, 0.1f, 0.05f, 0.1f, 0.0f, 0.07f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-0.12f, -0.02f, 0.0f);
			glScalef(0.2f, 0.2f, 0.2f);
			textures = loadTexture(strHead_4.c_str());
				drawRectangle(0.1f, 0.25f, 0.05f, 0.35f, 0.0f, 0.5f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0.05f, -0.02f, 0.0f);
			glScalef(0.2f, 0.2f, 0.2f);
			textures = loadTexture(strHead_4.c_str());
				drawRectangle(0.1f, 0.25f, 0.05f, 0.35f, 0.0f, 0.5f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		//inner left
		glPushMatrix();
			glTranslatef(0.05f, 0.05f, 0.0f);
			glRotatef(240.0f, 1.0f, 1.0f, 1.0f);
			glTranslatef(-0.05f, -0.05f, 0.0f);

			glTranslatef(0.0f, -0.1f, 0.05f);

			glTranslatef(0.05f, 0.05f, 0.0f);
			glRotatef(129.8f, 0.0f, 0.0f, 1.0f);
			glTranslatef(-0.05f, -0.05f, 0.0f);

			glTranslatef(0.0f, -0.15f, 0.0f);
			textures = loadTexture(strHead_1.c_str());
				triangularPrism(0.05f, 0.08f, 0.05f, 0.08f, 0.0f, 0.125f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		//inner right
		glPushMatrix();
			glTranslatef(0.05f, 0.05f, 0.0f);
			glRotatef(240.0f, 1.0f, 1.0f, 1.0f);
			glTranslatef(-0.05f, -0.05f, 0.0f);

			glTranslatef(0.0f, -0.1f, 0.05f);

			glTranslatef(0.05f, 0.05f, 0.0f);
			glRotatef(129.8f, 0.0f, 0.0f, 1.0f);
			glTranslatef(-0.05f, -0.05f, 0.0f);

			glTranslatef(-0.06f, -0.08f, 0.0f);
			textures = loadTexture(strHead_1.c_str());
				triangularPrism(0.05f, 0.08f, 0.05f, 0.08f, 0.0f, 0.125f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		//outer right
		glPushMatrix();
			glTranslatef(0.05f, 0.05f, 0.0f);
			glRotatef(240.0f, 1.0f, 1.0f, 1.0f);
			glTranslatef(-0.05f, -0.05f, 0.0f);

			glTranslatef(0.05f, 0.1f, 0.0f);
			glRotatef(129.8f, 0.0f, 0.0f, 1.0f);
			glTranslatef(-0.05f, -0.1f, 0.0f);

			glTranslatef(0.05f, 0.1f, 0.0f);
			glRotatef(-60.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(-0.05f, -0.1f, 0.0f);

			glTranslatef(-0.18f, 0.01f, 0.0f);
			textures = loadTexture(strHead_1.c_str());
				triangularPrism(0.05f, 0.08f, 0.05f, 0.08f, 0.0f, 0.3f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		//outer left
		glPushMatrix();
			glTranslatef(0.05f, 0.05f, 0.0f);
			glRotatef(240.0f, 1.0f, 1.0f, 1.0f);
			glTranslatef(-0.05f, -0.05f, 0.0f);

			glTranslatef(0.05f, 0.1f, 0.0f);
			glRotatef(129.8f, 0.0f, 0.0f, 1.0f);
			glTranslatef(-0.05f, -0.1f, 0.0f);

			glTranslatef(0.05f, 0.1f, 0.0f);
			glRotatef(60, 0.0f, 1.0f, 0.0f);
			glTranslatef(-0.05f, -0.1f, 0.0f);

			glTranslatef(-0.1f, 0.0f, -0.1f);
			textures = loadTexture(strHead_1.c_str());
				triangularPrism(0.05f, 0.08f, 0.05f, 0.08f, 0.0f, 0.3f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

	glPopMatrix();
}

//Shield
void shieldLogo(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {

	glBegin(GL_QUADS);
	glTexCoord2f(-maxX, minZ);
	glVertex3f(-maxX, maxY, minZ);
	glTexCoord2f(-maxX, minZ);
	glVertex3f(-maxX, -maxY, minZ);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, -minY, minZ);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, minY, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, minY, minZ);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, maxY, minZ);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, -maxY, minZ);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, -minY, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(-maxX, maxZ);
	glVertex3f(-maxX, maxY, maxZ);
	glTexCoord2f(-maxX, maxZ);
	glVertex3f(-maxX, -maxY, maxZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, -minY, maxZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, minY, maxZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, minY, maxZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, maxY, maxZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, -maxY, maxZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, -minY, maxZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(-maxX, minZ);
	glVertex3f(-maxX, maxY, minZ);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, minY, minZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, minY, maxZ);
	glTexCoord2f(-maxX, maxZ);
	glVertex3f(-maxX, maxY, maxZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(-maxX, minZ);
	glVertex3f(-maxX, maxY, minZ);
	glTexCoord2f(-maxX, minZ);
	glVertex3f(-maxX, -maxY, minZ);
	glTexCoord2f(-maxX, maxZ);
	glVertex3f(-maxX, -maxY, maxZ);
	glTexCoord2f(-maxX, maxZ);
	glVertex3f(-maxX, maxY, maxZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(-maxX, minZ);
	glVertex3f(-maxX, -maxY, minZ);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, -minY, minZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, -minY, maxZ);
	glTexCoord2f(-maxX, maxZ);
	glVertex3f(-maxX, -maxY, maxZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, -minY, minZ);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, -maxY, minZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, -maxY, maxZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, -minY, maxZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, -maxY, minZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, -maxY, maxZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, maxY, maxZ);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, maxY, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, maxY, minZ);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, minY, minZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, minY, maxZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, maxY, maxZ);
	glEnd();

}
void shieldLogoHorizontal(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {

	glBegin(GL_QUADS);
	glTexCoord2f(-maxX, minZ);
	glVertex3f(-maxX, maxY, minZ);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, maxY, minZ);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, minY, minZ);
	glTexCoord2f(-minX, minZ);
	glVertex3f(-minX, minY, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, minY, minZ);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, -maxY, minZ);
	glTexCoord2f(-maxX, minZ);
	glVertex3f(-maxX, -maxY, minZ);
	glTexCoord2f(-minX, minZ);
	glVertex3f(-minX, minY, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(-maxX, maxZ);
	glVertex3f(-maxX, maxY, maxZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, maxY, maxZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, minY, maxZ);
	glTexCoord2f(-minX, maxZ);
	glVertex3f(-minX, minY, maxZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, minY, maxZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, -maxY, maxZ);
	glTexCoord2f(-maxX, maxZ);
	glVertex3f(-maxX, -maxY, maxZ);
	glTexCoord2f(-minX, maxZ);
	glVertex3f(-minX, minY, maxZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(-maxX, minZ);
	glVertex3f(-maxX, maxY, minZ);
	glTexCoord2f(-minX, minZ);
	glVertex3f(-minX, minY, minZ);
	glTexCoord2f(-minX, maxZ);
	glVertex3f(-minX, minY, maxZ);
	glTexCoord2f(-maxX, maxZ);
	glVertex3f(-maxX, maxY, maxZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(-maxX, minZ);
	glVertex3f(-maxX, maxY, minZ);
	glTexCoord2f(-maxX, maxZ);
	glVertex3f(-maxX, maxY, maxZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, maxY, maxZ);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, maxY, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, maxY, minZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, maxY, maxZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, minY, maxZ);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, minY, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(minX, minZ);
	glVertex3f(minX, minY, minZ);
	glTexCoord2f(minX, maxZ);
	glVertex3f(minX, minY, maxZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, -maxY, maxZ);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, -maxY, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(maxX, minZ);
	glVertex3f(maxX, -maxY, minZ);
	glTexCoord2f(maxX, maxZ);
	glVertex3f(maxX, -maxY, maxZ);
	glTexCoord2f(-maxX, maxZ);
	glVertex3f(-maxX, -maxY, maxZ);
	glTexCoord2f(-maxX, minZ);
	glVertex3f(-maxX, -maxY, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(-maxX, minZ);
	glVertex3f(-maxX, -maxY, minZ);
	glTexCoord2f(-maxX, maxZ);
	glVertex3f(-maxX, -maxY, maxZ);
	glTexCoord2f(-minX, maxZ);
	glVertex3f(-minX, minY, maxZ);
	glTexCoord2f(-minX, minZ);
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
		textures = loadTexture(strShield_3.c_str());
		drawRectangle(0.0f, 0.3f, 0.0f, 0.05f, 0.0f, 0.05f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
		glRotatef(60.0f, 0.0f, 0.0f, -1.0f);
		glTranslatef(-0.2f, 0.0f, 0.0f);
		textures = loadTexture(strShield_3.c_str());
		drawRectangle(0.0f, 0.2f, 0.0f, 0.05f, 0.0f, 0.05f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.3f, 0.0f, 0.0f);
		glRotatef(60.0f, 0.0f, 0.0f, 1.0f);
		glTranslatef(-0.3f, -0.0f, -0.0f);
		glTranslatef(0.3f, 0.0f, 0.0f);
		textures = loadTexture(strShield_3.c_str());
		drawRectangle(0.0f, 0.2f, 0.0f, 0.05f, 0.0f, 0.05f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}
void DrawShield() {

	glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.15f, 0.0f, 0.2f);
		glScalef(2.0f, 2.0f, 2.0f);
		textures = loadTexture(strShield_2.c_str());
		shieldPentagon(0.0f, 0.04f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

		glPushMatrix();
			glTranslatef(0.0, 0.0, 0.04);
			textures = loadTexture(strShield_1.c_str());
			shieldLogo(0.0f, 0.025f, 0.125f, 0.15f, 0.0f, 0.01f);
			shieldLogoHorizontal(0.075f, 0.1f, 0.0f, 0.025f, 0.0f, 0.01f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

			logoSideUp(0.0f, 0.01f);
			logoSideLeft(0.0f, 0.01f);
			logoSideRight(0.0f, 0.01f);
			logoSideBLeft(0.0f, 0.01f);
			logoSideBRight(0.0f, 0.01f);
		glPopMatrix();
	glPopMatrix();
}
void shieldPentagon(float minZ, float maxZ) {



	glBegin(GL_POLYGON);
	glTexCoord2f(-0.1f, minZ);
	glVertex3f(-0.1f, 0.2f, minZ);
	glTexCoord2f(0.1f, minZ);
	glVertex3f(0.1f, 0.2f, minZ);
	glTexCoord2f(0.16f, minZ);
	glVertex3f(0.16f, -0.09f, minZ);
	glTexCoord2f(0.0f, minZ);
	glVertex3f(0.0f, -0.25f, minZ);
	glTexCoord2f(-0.16f, minZ);
	glVertex3f(-0.16f, -0.09f, minZ);
	glEnd();

	glBegin(GL_POLYGON);
	glTexCoord2f(-0.1f, maxZ);
	glVertex3f(-0.1f, 0.2f, maxZ);
	glTexCoord2f(0.1f, maxZ);
	glVertex3f(0.1f, 0.2f, maxZ);
	glTexCoord2f(0.16f, maxZ);
	glVertex3f(0.16f, -0.09f, maxZ);
	glTexCoord2f(0.0f, maxZ);
	glVertex3f(0.0f, -0.25f, maxZ);
	glTexCoord2f(-0.16f, maxZ);
	glVertex3f(-0.16f, -0.09f, maxZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(-0.16f, minZ);
	glVertex3f(-0.16f, -0.09f, minZ);
	glTexCoord2f(0.0f, minZ);
	glVertex3f(0.0f, -0.25f, minZ);
	glTexCoord2f(0.0f, maxZ);
	glVertex3f(0.0f, -0.25f, maxZ);
	glTexCoord2f(-0.16f, maxZ);
	glVertex3f(-0.16f, -0.09f, maxZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(-0.16f, minZ);
	glVertex3f(-0.16f, -0.09f, minZ);
	glTexCoord2f(-0.16f, maxZ);
	glVertex3f(-0.16f, -0.09f, maxZ);
	glTexCoord2f(-0.1f, maxZ);
	glVertex3f(-0.1f, 0.2f, maxZ);
	glTexCoord2f(-0.1f, minZ);
	glVertex3f(-0.1f, 0.2f, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(-0.1f, minZ);
	glVertex3f(-0.1f, 0.2f, minZ);
	glTexCoord2f(-0.1f, maxZ);
	glVertex3f(-0.1f, 0.2f, maxZ);
	glTexCoord2f(0.1f, maxZ);
	glVertex3f(0.1f, 0.2f, maxZ);
	glTexCoord2f(0.1f, minZ);
	glVertex3f(0.1f, 0.2f, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.1f, minZ);
	glVertex3f(0.1f, 0.2f, minZ);
	glTexCoord2f(0.1f, maxZ);
	glVertex3f(0.1f, 0.2f, maxZ);
	glTexCoord2f(0.16f, maxZ);
	glVertex3f(0.16f, -0.09f, maxZ);
	glTexCoord2f(0.16f, minZ);
	glVertex3f(0.16f, -0.09f, minZ);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.16f, minZ);
	glVertex3f(0.16f, -0.09f, minZ);
	glTexCoord2f(0.16f, maxZ);
	glVertex3f(0.16f, -0.09f, maxZ);
	glTexCoord2f(0.0f, maxZ);
	glVertex3f(0.0f, -0.25f, maxZ);
	glTexCoord2f(0.0f, minZ);
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
			textures = loadTexture(strBlacKColor.c_str());
			drawCylinder(0.04f, 0.04f, 1.0f, 20, 20);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		//Muzzle
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, 0.4f);
			textures = loadTexture(strDarkGreyDirtyColor.c_str());
			drawCylinder(0.04f, 0.05f, 0.08f, 15, 15);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		//Back Seat
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, -0.6f);
			textures = loadTexture(strDarkGreyDirtyColor.c_str());
			drawCoverCylinder(0.05f, 0.05f, 0.1f, 10, 10);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		//Clip
		glPushMatrix();
			textures = loadTexture(strDarkGreyDirtyColor.c_str());
			drawRectangle(-0.025f, 0.025f, -0.04f, -0.15f, -0.35f, -0.45f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

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
				textures = loadTexture(strDarkGreyDirtyColor.c_str());
				drawCylinder(0.045f, 0.045f, 0.15f, 10, 10);
				glDeleteTextures(1, &textures);
				glDisable(GL_TEXTURE_2D);
			glPopMatrix();

			//draw '-' up
			textures = loadTexture(strGreyDirtyColor.c_str());
			drawRectangle(-0.0255f, 0.0255f, -0.045f, -0.08f, 0.205f, 0.025f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

			//draw '|'
			textures = loadTexture(strDarkGreyDirtyColor.c_str());
			drawRectangle(-0.0255f, 0.0255f, -0.081f, -0.2f, 0.075f, 0.035f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

			//draw '-' down
			textures = loadTexture(strDarkGreyDirtyColor.c_str());
			drawTrapeziumTexture(-0.0255f, 0.0255f, -0.0255f, 0.0255f, -0.191f, -0.191f, -0.171f, -0.171f, 0.075f, 0.2f, 0.075f, 0.175f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

			//draw '/' down
			textures = loadTexture(strBlacKColor.c_str());
			drawTrapeziumTexture(-0.0255f, 0.0255f, -0.0255f, 0.0255f, -0.21f, -0.21f, -0.171f, -0.171f, 0.225f, 0.275f, 0.175f, 0.225f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

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

						textures = loadTexture(strGreyDirtyColor.c_str());
						drawCoverCylinder(0.015f, 0.015f, 0.15f, 10, 10);
						glDeleteTextures(1, &textures);
						glDisable(GL_TEXTURE_2D);

					glPopMatrix();

					textures = loadTexture(strDarkGreyDirtyColor.c_str());
					drawTrapeziumTexture(-0.2f, -0.3f, -0.2f, -0.26f, 0.5f, 0.5f, 0.55f, 0.55f, -0.1f, -0.15f, -0.1f, -0.15f);
					glDeleteTextures(1, &textures);
					glDisable(GL_TEXTURE_2D);

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

						textures = loadTexture(strGreyDirtyColor.c_str());
						drawCoverCylinder(0.015f, 0.015f, 0.15f, 10, 10);
						glDeleteTextures(1, &textures);
						glDisable(GL_TEXTURE_2D);

					glPopMatrix();

					textures = loadTexture(strDarkGreyDirtyColor.c_str());
					drawTrapeziumTexture(0.0f, 0.1f, 0.0f, 0.06f, 0.5f, 0.5f, 0.55f, 0.55f, -0.1f, -0.15f, -0.1f, -0.15f);
					glDeleteTextures(1, &textures);
					glDisable(GL_TEXTURE_2D);

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

			textures = loadTexture(strBlacKColor.c_str());
			drawCoverCylinder(0.02f, 0.02f, 0.1f, 10, 10);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	
		//middle
		glPushMatrix();
			glTranslatef(-0.7f, 0.002f, 0.025f);
			glRotatef(90.0f, 0.0f, 0.1f, 0.0f);

			textures = loadTexture(strDarkGreyDirtyColor.c_str());
			drawCoverCylinder(0.035f, 0.035f, 0.3f, 10, 10);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		//middle-back
		textures = loadTexture(strGreyDirtyColor.c_str());
		drawRectangle(-0.395f, 0.4f, -0.06f, 0.06f, -0.02f, 0.07f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

		//back-up
		textures = loadTexture(strBlacKColor.c_str());
		drawTrapeziumTexture(0.4f, 0.7f, 0.4f, 0.8, 0.02f, 0.02f, 0.08f, 0.08f, -0.02f, 0.07f, -0.02f, 0.07f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

		//back-bottom
		textures = loadTexture(strBlacKColor.c_str());
		drawRectangle(0.4f, 0.7f, -0.08f, 0.02f, -0.02f, 0.07f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

	//sight
	glPushMatrix();
	glTranslatef(-0.1f, 0.102f, 0.025f);
	glRotatef(90.0f, 0.0f, 0.1f, 0.0f);

			textures = loadTexture(strDarkGreyDirtyColor.c_str());
			drawCoverCylinder(0.04f, 0.04f, 0.15f, 10, 10);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		//Clip
		glPushMatrix();
			textures = loadTexture(strBlacKColor.c_str());
			drawRectangle(-0.225f, -0.125f, -0.06f, -0.3f, -0.00f, 0.05f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

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
		textures = loadTexture(strDarkGreyDirtyColor.c_str());
		drawRectangle(0.345f, 0.375f, -0.06f, -0.45f, -0.00f, 0.05f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

		//Grip - bottom
		glPushMatrix();
			glTranslatef(0.36f, -0.475f, 0.0f);
			textures = loadTexture(strKneeTexture.c_str());
			drawCoverCylinder(0.02f, 0.02f, 0.05f, 5, 5);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

	glPushMatrix();
	glTranslatef(0.34f, -0.465f, 0.05f);
	glRotatef(8.0f, 0.0f, 0.0f, -0.1f);
	glTranslatef(-0.34f, 0.465f, -0.05f);

			textures = loadTexture(strDarkGreyDirtyColor.c_str());
			drawRectangle(0.205f, 0.34f, -0.465f, -0.5f, -0.0f, 0.05f);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);

		glPopMatrix();

		//Grip - front
		glPushMatrix();
			glTranslatef(0.18f, -0.46f, 0.0f);
			textures = loadTexture(strKneeTexture.c_str());
			drawCoverCylinder(0.02f, 0.02f, 0.05f, 5, 5);
			glDeleteTextures(1, &textures);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		textures = loadTexture(strDarkGreyDirtyColor.c_str());
		drawRectangle(0.155f, 0.185f, -0.06f, -0.435f, -0.00f, 0.05f);
		glDeleteTextures(1, &textures);
		glDisable(GL_TEXTURE_2D);

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