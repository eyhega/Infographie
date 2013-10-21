
/*
 * Exemple pour les callback de GLUT
 */
#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>

/* 
 * Constantes pour le menu
 */
enum faces
{
	F_NONE, 
	F_FRONT,
	F_BACK
};

enum faces cullFace=GL_BACK;     /* Face a  eliminer          */

static int width, height;        /* Dimension de la fenetre   */ 
static int curx, cury;           /* Position de la souris     */

static GLfloat rotx = 0.0;       /* Rotation autour de x      */
static GLfloat roty = 0.0;       /* Rotation autour de y      */
static GLfloat posx,posy, posz;

static unsigned int delay = 100; /* milisecondes              */

static int prex = -1, prey = -1;

static GLfloat sommets[][3]={{-0.5, -0.5, 0.0}, {0.5, -0.5, 0.0}, {0.5, 0.5, 0.0}, {-0.5, 0.5, 0.0}};

/*  
 * GL_SMOOTH is actually the default shading model.  
 */
void init (void)
{
	glClearColor (0.0, 0.0, 0.0, 1.0);    /* Couleur de fond en noir        */
	glShadeModel (GL_SMOOTH);			      /* Model d'ombrage (couleur unie) */

	glPolygonMode(GL_FRONT, GL_FILL);
	
	glFrontFace (GL_CCW);				  /* Activation l'elimination de faces arrieres */
	glEnable (GL_CULL_FACE);
}

void triangle(GLfloat sommets[][3])
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.0f, 1.0f);
		glVertex3fv(sommets[1]);
		glColor3f(0.f, 1.f, 1.f);
		glVertex3fv(sommets[2]);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3fv(sommets[3]);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3fv(sommets[0]);
		glColor3f(0.5f, 0.5f, 0.f);
		glVertex3fv(sommets[1]);
		glColor3f(0.5f, 0.f, 0.9f);
		glVertex3fv(sommets[3]);
	glEnd();
	
	glColor3f(1.f, 1.f, 1.f);
}

/* 
 * Callback : display                              
 */
void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0); 
	
    glPushMatrix();
	glRotatef (rotx, 1.0, 0.0, 0.0);
    glRotatef (roty, 0.0, 1.0, 0.0);
	
	glutWireTeapot(0.5);
	
	triangle(sommets);
	
	
	
	glPopMatrix();
	
    glutSwapBuffers ();
}


/* 
 * Callback : redimensionnement de fenetre graphique
 */
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	width = w; height = h;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	
	glMatrixMode(GL_MODELVIEW);
}


typedef enum zoomDir { ZOOM_UP , ZOOM_DOWN} zoomDir_t;
void zoom(zoomDir_t dir)
{
	if(dir == ZOOM_UP)
	{
		gluLookAt(0.5,0.5,0.5,0,0,0,0,1,1);
	} 
	else
	{
		
	}
}


/* 
 * Callback : programmation des touches (normales) du clavier 
 */
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'h':
			roty-=5.0;
			if (roty < 0.0) roty += 360.0;
			break;
		case 'l':
			roty+=5.0;
			if (roty > 360.0) roty -= 360.0;
				break;
		case 'j':
			rotx-=5.0;
			if (rotx < 0.0) rotx += 360.0;
			break;
		case 'k':
			rotx+=5.0;
			if (rotx > 360.0) rotx -= 360.0;
			break;
		case '+': zoom(ZOOM_UP);
			break;
		case '-': zoom(ZOOM_DOWN);
			break;
		case 27: // ESC
			exit(0);
			break;

		default:
			break;
	}
	glutPostRedisplay ();
}

/* 
 * Callback : programmation des touches de fonction
 */
void specialkey (int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_LEFT :
			roty -= 5.0;
			if (roty < 0.0) roty += 360.0;
			break;
		case GLUT_KEY_RIGHT :
			roty += 5.0;
			if (roty > 360.0) roty -= 360.0;
			break;
		case GLUT_KEY_UP :
			rotx -= 5.0;
			if (rotx < 0.0) rotx += 360.0;
			break;
		case GLUT_KEY_DOWN :
			rotx += 5.0;
			if (rotx > 360.0) rotx -= 360.0;
			break;
		case GLUT_KEY_END :
			exit(0);
	}
    glutPostRedisplay ();
}


/* 
 * Callback : traitement des actions des boutons de la souris 
 */
void mouse (int button, int state, int x, int y)
{
	switch (button) {
		case GLUT_LEFT_BUTTON :
			if (state == GLUT_UP) {
				prex = prey = -1;
			}
			break;
			
		case GLUT_RIGHT_BUTTON :
			if (state == GLUT_UP) {
				prex = prey = -1;
			}
			break;
		default :
			break;
	}
}


/*
 *      @   --------------    @
 *      |                     |
 *      @   --------------    @                     
 */
void    drawRectangle2D(float width,
                        float height)
{
    float   sh = abs(width * 0.5f);
    float   sw = abs(height * 0.5f);
    
    glBegin(GL_POLYGON);
        glVertex3f(-sw, -sh, .0f);
        glVertex3f(+sw, -sh, .0f);
        glVertex3f(+sw, +sh, .0f);
        glVertex3f(-sw, +sh, .0f);
    glEnd();
}




/* 
 * Callback : traitement du mouvement de la souris
 */
void motion(int x, int y)
{
	
	if (prex != -1) {
		roty += x-prex;			// rotation autour de Y
	}
	
	if (prey != -1) {
		 rotx += y-prey;	// Rotation autour de X
	}
	
	glutPostRedisplay ();
	prey = y;
	prex = x;
}

/* 
 * Callback : timer
 */
void timer(int t)
{
	rotx++;
	glutPostRedisplay ();
	glutTimerFunc(delay, timer, t);
}

/* 
 * Callback : fonction idle 
 */
void idle(void)
{
	roty++;
	glutPostRedisplay ();
}

/* 
 * Traitement lié aux actions du menu
 */
void menu(int value)
{
	switch (value) 
	{
		case F_NONE:
			break;
			
		case F_FRONT:
			cullFace = GL_BACK;
			break;
		case F_BACK:
			cullFace = GL_FRONT;
			break;
	}
	
	glCullFace (cullFace);
	glutPostRedisplay();
}


/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
	glutInit ( &argc, argv );
	glutInitDisplayMode (GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	
	glutInitWindowSize ( 500, 500 );
	glutInitWindowPosition ( 100, 100 );
	glutCreateWindow (argv[0]);
	
	glutCreateMenu(menu);
	glutAddMenuEntry("Display Face :", F_NONE          );  
	glutAddMenuEntry("   Front      ", F_FRONT      );
	glutAddMenuEntry("   Back       ", F_BACK     );
	glutAddMenuEntry("              ", F_NONE          );
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	init();
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);
	
	glutSpecialFunc (specialkey);
	glutKeyboardFunc(keyboard);
	
	glutMouseFunc (mouse);
	glutMotionFunc(motion);
	
//	glutTimerFunc(delay, timer, 0);
//	glutIdleFunc(idle); /* tourne trop vite; timer est mieux */
	
	glutMainLoop();
	
	return 0;
}
