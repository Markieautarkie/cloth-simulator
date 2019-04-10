#include "precomp.h" // only include this header in source files

// helper for OpenGL
OpenGLHelper OGLHelper;

// camera movement variables
float rotation = 25.f;
float distance = -12.f;

// key logic
int oldState_1, oldState_2, oldState_3, oldState_4;
bool update = true;
bool spacePressed;

// initialize cloth object
Cloth cloth(Vec3(0.0f, 0.0f, 0.0f), 14, 10, 55, 45, Pattern::Horizontal, Vec3(0.0f, 0.8f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));

// initialize sphere object
float ballT = 0;
float ballRadius = 2.0f;
Sphere sphere(Vec3(7, 0, -5), ballRadius - 0.1f, 36, 18);

// draws the current frame to the application window
void Draw(void)
{
    if (update)
    {
        // calculate ball positions
        ballT++;
        float y = cos(ballT / 50.0) * 7;
        sphere.UpdatePosition(y);

        // add forces to the cloth and update the particle positions
        cloth.AddForce(Vec3(0, -0.2, 0) * TIMESTEP2);
        cloth.AddWindForce(Vec3(0.5, 0, 0.2) * TIMESTEP2);
        cloth.Update();

        // resolve collision with the ball
        cloth.SphereCollision(Vec3(sphere.position.f[0], sphere.position.f[2], -sphere.position.f[1]), ballRadius);
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
    glRotatef(rotation, 0, 1, 0);
    // then translate to the center of the cloth
    glTranslatef(-7, 5, 0);
	 
	// draw sphere
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	sphere.Draw();
	glPopMatrix();

	// draw the cloth
	cloth.DrawShaded();
}

// handles the user input
void HandleInput(GLFWwindow* window)
{
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

    int state_q = glfwGetKey(window, GLFW_KEY_Q);
    if (state_q == GLFW_PRESS)
        rotation+=2;

    int state_e = glfwGetKey(window, GLFW_KEY_E);
    if (state_e == GLFW_PRESS)
        rotation-=2;

    int state_w = glfwGetKey(window, GLFW_KEY_W);
    if (state_w == GLFW_PRESS)
        distance+=0.25f;

    int state_s = glfwGetKey(window, GLFW_KEY_S);
    if (state_s == GLFW_PRESS)
        distance-=0.25f;

    bool state_space = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    if (!spacePressed && state_space)
        update = !update;

    spacePressed = state_space;
}

int main()
{
	// GLFW initialization
	GLFWwindow* window;
	if (!glfwInit()) return -1;
	window = glfwCreateWindow(WINDOWWIDTH, WINDOWHEIGHT, "Simulator v1.0", NULL, NULL);
	if (!window) { glfwTerminate(); return -1; }
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// initialize OpenGL and reshape the window correctly
	OGLHelper.InitOpenGL();

	// seed the randomizer
	srand(time(0));

	// set the old states to release
	oldState_1 = GLFW_RELEASE, oldState_2 = GLFW_RELEASE;
	oldState_3 = GLFW_RELEASE, oldState_4 = GLFW_RELEASE;

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