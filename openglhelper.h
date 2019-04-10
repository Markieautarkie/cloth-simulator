/* helper class for OpenGL */
class OpenGLHelper
{
private:
	// reshapes the window to a certain width and height
	void ReshapeWindow(int w, int h)
	{
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (h == 0)
			SetOpenGLPerspective(80, (GLdouble)w, 1.0, 5000.0);
		else
			SetOpenGLPerspective(80, (GLdouble)w / (GLdouble)h, 1.0, 5000.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	// sets the window perspective for OpenGL
	void SetOpenGLPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
	{
		const GLdouble pi = 3.1415926535897932384626433832795;
		GLdouble fW, fH;

		fH = tan(fovY / 360 * pi) * zNear;
		fW = fH * aspect;

		glFrustum(-fW, fW, -fH, fH, zNear, zFar);
	}

public:
	// constructor
	OpenGLHelper() {}

	// sets up OpenGL
	void InitOpenGL(GLvoid)
	{
		// main setup
		glShadeModel(GL_SMOOTH);
		glClearColor(0.2f, 0.2f, 0.4f, 0.5f);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_COLOR_MATERIAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		// setup light 0
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		GLfloat lightPos[4] = { -1.0,1.0,0.5,0.0 };

		glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat *)&lightPos);

		// setup light 1
		glEnable(GL_LIGHT1);

		GLfloat lightAmbient1[4] = { 0.0,0.0,0.0,0.0 };
		GLfloat lightPos1[4] = { 1.0,0.0,-0.2,0.0 };
		GLfloat lightDiffuse1[4] = { 0.5,0.5,0.3,0.0 };

		glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat *)&lightPos1);
		glLightfv(GL_LIGHT1, GL_AMBIENT, (GLfloat *)&lightAmbient1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat *)&lightDiffuse1);

		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

		// reshapes the window
		ReshapeWindow(WINDOWWIDTH, WINDOWHEIGHT);
	}
};