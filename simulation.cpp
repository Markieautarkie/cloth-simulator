#include "precomp.h" // only include this header in source files

// helper for OpenGL
OpenGLHelper OGLHelper;

// camera( lookFrom, worldUp, yaw, pitch, FoV, aspectRatio )
Camera camera(Vec3(-7.0f, 5.0f, -12.0f), Vec3(0.0f, 1.0f, 0.0f),
	0.0, 120.0f, 90.0f, (float)SCRWIDTH / (float)SCRHEIGHT);
// current and previous mouse positions
double xMouse, yMouse, xPrevMouse, yPrevMouse; 
float distance = -12.f;

// key logic
int oldState_1, oldState_2, oldState_3, oldState_4;
int oldState_r, oldState_t, oldState_s, oldState_w, oldState_b, oldState_space;
bool update = false, updateWindForce = true, updateBallPos = true;

// initialize cloth object
Cloth cloth(Vec3(0.0f, 0.0f, 0.0f), 14, 10, 60, 45, Pattern::Horizontal, Vec3(0.0f, 0.8f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));

// initialize sphere object
float ballT = 0;
float ballRadius = 2.0f;
Sphere sphere(Vec3(7, cos(0 / 50.0) * 7, -5), Vec3(1.0f, 0.0f, 0.0f), ballRadius - 0.1f, 36, 18);

// floor sphere
Sphere floorSphere(Vec3(7, 0, -215), Vec3(0.486f, 0.988f, 0.0f), 200.0f);

// draws the current frame to the application window
void Draw(void)
{
    if (update)
    {
        // calculate ball positions
		if (updateBallPos)
		{
			ballT++;
			float y = cos(ballT / 50.0) * 7;
			sphere.UpdatePosition(y);
		}

        // add forces to the cloth and update the particle positions
        cloth.AddForce(Vec3(0, -0.2, 0) * TIMESTEP2);
		if (updateWindForce)
			cloth.AddWindForce(Vec3(0.5, 0, 0.2) * TIMESTEP2);
        cloth.Update();

        // resolve collision with the balls
        cloth.SphereCollision(Vec3(sphere.position.f[0], sphere.position.f[2], -sphere.position.f[1]), ballRadius);
		cloth.SphereCollision(Vec3(floorSphere.position.f[0], floorSphere.position.f[2], -floorSphere.position.f[1]), 200.1f);
    }

	// drawing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// drawing lerped background
	glDisable(GL_LIGHTING);
	glBegin(GL_POLYGON);
	glColor3f(0.8f, 0.8f, 1.0f);
	glVertex3f(-200.0f, -100.0f, -100.0f);
	glVertex3f(200.0f, -100.0f, -100.0f);
	glColor3f(0.4f, 0.4f, 0.8f);
	glVertex3f(200.0f, 100.0f, -100.0f);
	glVertex3f(-200.0f, 100.0f, -100.0f);
	glEnd();
	glEnable(GL_LIGHTING);

	// move the camera back first
    glTranslatef(0, 0, distance);
    // then rotate the camera
	glRotatef(camera.yaw, 0, 1, 0);
	//glRotatef(camera.pitch, 1, 0, 0);
    // then translate to the center of the cloth
    glTranslatef(-7, 5, 0);
	 
	// draw sphere
	glPushMatrix();
	glRotatef(-90, 1, 0, 0); // <-- THIS REALLY NEEDS TO BE CHANGED TO USE WORLD COORDS
	sphere.Draw();
	floorSphere.Draw();
	glPopMatrix();

	// draw the cloth
	cloth.DrawShaded();
}

// handles the user input
void HandleInput(GLFWwindow* window)
{
	/* keys for making corners static or dynamic, clockwise from top left */
	int state_1 = glfwGetKey(window, GLFW_KEY_1);
	if (state_1 == GLFW_RELEASE && oldState_1 == GLFW_PRESS)
		cloth.SwitchCorner(1);
	oldState_1 = state_1;

	int state_2 = glfwGetKey(window, GLFW_KEY_2);
	if (state_2 == GLFW_RELEASE && oldState_2 == GLFW_PRESS)
		cloth.SwitchCorner(2);
	oldState_2 = state_2;

	int state_3 = glfwGetKey(window, GLFW_KEY_3);
	if (state_3 == GLFW_RELEASE && oldState_3 == GLFW_PRESS)
		cloth.SwitchCorner(3);
	oldState_3 = state_3;

	int state_4 = glfwGetKey(window, GLFW_KEY_4);
	if (state_4 == GLFW_RELEASE && oldState_4 == GLFW_PRESS)
		cloth.SwitchCorner(4);
	oldState_4 = state_4;

	/* translational and rotational movements */
	// get the current mouse position and calculate the delta to find mouse change
	glfwGetCursorPos(window, &xMouse, &yMouse);
	double deltaXMouse = xMouse - xPrevMouse;
	double deltaYMouse = yPrevMouse - yMouse;
	camera.yaw += deltaXMouse * MOUSESENSITIVITY;  
	camera.pitch += deltaYMouse * MOUSESENSITIVITY; 
	xPrevMouse = xMouse; yPrevMouse = yMouse;

	// constrain the yaw and pitch
	camera.yaw = fmodf(camera.yaw, 360.0f);
	camera.pitch = (camera.pitch > 89.0f) ? 89.0f : (camera.pitch < -89.0f) ? -89.0f : camera.pitch;
	camera.UpdateCamera();

    int state_equal = glfwGetKey(window, GLFW_KEY_EQUAL);
    if (state_equal == GLFW_PRESS)
        distance+=0.25f;

    int state_minus = glfwGetKey(window, GLFW_KEY_MINUS);
    if (state_minus == GLFW_PRESS)
        distance-=0.25f;

	/* application controls */
	int state_r = glfwGetKey(window, GLFW_KEY_R);
	if (state_r == GLFW_RELEASE && oldState_r == GLFW_PRESS)
		cloth.ResetCloth();
	oldState_r = state_r;

	int state_t = glfwGetKey(window, GLFW_KEY_T);
	if (state_t == GLFW_RELEASE && oldState_t == GLFW_PRESS)
		cloth.SwitchTearable();
	oldState_t = state_t;

	int state_s = glfwGetKey(window, GLFW_KEY_S);
	if (state_s == GLFW_RELEASE && oldState_s == GLFW_PRESS)
		cloth.SwitchShowTears();
	oldState_s = state_s;

	int state_w = glfwGetKey(window, GLFW_KEY_W);
	if (state_w == GLFW_RELEASE && oldState_w == GLFW_PRESS)
		updateWindForce = !updateWindForce;
	oldState_w = state_w;

	int state_b = glfwGetKey(window, GLFW_KEY_B);
	if (state_b == GLFW_RELEASE && oldState_b == GLFW_PRESS)
		updateBallPos = !updateBallPos;
	oldState_b = state_b;

	int state_space = glfwGetKey(window, GLFW_KEY_SPACE);
    if (state_space == GLFW_RELEASE && oldState_space == GLFW_PRESS)
        update = !update;
    oldState_space = state_space;

	int state_escape = glfwGetKey(window, GLFW_KEY_ESCAPE);
	if (state_escape == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	// GLFW initialization
	GLFWwindow* window;
	if (!glfwInit()) return -1;
	window = glfwCreateWindow(SCRWIDTH, SCRHEIGHT, "Simulator v1.0", NULL, NULL);
	if (!window) { glfwTerminate(); return -1; }
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// initialize OpenGL and reshape the window correctly
	OGLHelper.InitOpenGL();

	// seed the randomizer
	srand(time(0));

	// render loop
	while (!glfwWindowShouldClose(window))
	{
        // handle input
        HandleInput(window);

        // display the window
		Draw();

		// swap buffers and check/call events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// terminate and quit
	glfwTerminate();
	return 0;
}