
/*
 * Exemple pour les callback de GLUT
 */
#ifdef _WIN32
#include <windows.h>
#endif

#ifndef GLUT_WHEEL_UP
#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

static float xLight=1., yLight=1.5, zLight = 1.;
GLfloat light_pos[4] = {1.,1.5,1.,1.};

static unsigned int delay = 100; /* milisecondes              */

static int prex = -1, prey = -1;

static GLfloat sommets[][3]={{-0.5, -0.5, 0.0}, {0.5, -0.5, 0.0}, {0.5, 0.5, 0.0}, {-0.5, 0.5, 0.0}};

GLfloat plane[4] = {0.,1.,0.,0.8}; /* equation du sol */

void    drawParaCube( float inXR,
                              float inYR,
                              float inZR);
void    drawRectangle2D(float width,
                        float height);
void rotateLight();
void initCastShadows();
void drawTable(float inXR,float inYR,float inZR,float footWidth,float footHeight);
void drawChair(float xPos,float yPos,float zPos,float inHeight,float width, float footWidth);
/*  
 * GL_SMOOTH is actually the default shading model.  
 */
void init (void)
{
	glClearColor (0.0, 0.0, 0.0, 1.0);    /* Couleur de fond en noir        */
	glShadeModel (GL_SMOOTH);			  /* Model d'ombrage (couleur unie) */

	glPolygonMode(GL_FRONT, GL_FILL);
	
	glFrontFace (GL_CCW);				  /* Activation l'elimination de faces arrieres */
	glEnable (GL_CULL_FACE);

	//Q1 TP3
	glEnable(GL_DEPTH_TEST); // do not display hidden faces
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//LIGHT DEFINITION
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	GLfloat light_ambient[] = {0.4,0.4,0.4,1.};
	GLfloat light_diffuse[] = {1.,1.,1.,1.};
	GLfloat light_specular[] = {1.,1.,1.,1.};
	
	glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
	glLightfv(GL_LIGHT0,GL_POSITION,light_pos);
	
	//FIX LIGHT SOURCE
	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);*/
	
	//LIGHT SOURCE MOVING
	glMatrixMode(GL_MODELVIEW);
	
	//Proprietes materielles
	GLfloat matZero[4] = {0.,0.,0.,1.};
	GLfloat matAmb[4] = {0.2,0.5,0.5,0.3};
	GLfloat matDif[4] = {1.,1.,1.,0.5};//{0.7,0.3,0.,0.8};
	GLfloat matShine = 20.00;
	
	glMaterialfv(GL_FRONT,GL_AMBIENT,matAmb);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,matDif);
	glMaterialfv(GL_FRONT,GL_SPECULAR,matZero);
	glMaterialfv(GL_FRONT,GL_SHININESS,&matShine);
	
	initCastShadows();
	
	glLoadIdentity();
}

void shadowMatrix(GLfloat shadowMat[4][4])
{
	GLfloat dot;
	
	dot = plane[0]*light_pos[0] + plane[1]*light_pos[1] + plane[2]*light_pos[2] + plane[3]*light_pos[3];
	
	shadowMat[0][0] = dot - light_pos[0] * plane[0];
	shadowMat[1][0] = 0.f - light_pos[0] * plane[1];
	shadowMat[2][0] = 0.f - light_pos[0] * plane[2];
	shadowMat[3][0] = 0.f - light_pos[0] * plane[3];
	
	shadowMat[0][1] = 0.f - light_pos[1] * plane[0];
	shadowMat[1][1] = dot - light_pos[1] * plane[1];
	shadowMat[2][1] = 0.f - light_pos[1] * plane[2];
	shadowMat[3][1] = 0.f - light_pos[1] * plane[3];
	
	shadowMat[0][2] = 0.f - light_pos[2] * plane[0];
	shadowMat[1][2] = 0.f - light_pos[2] * plane[1];
	shadowMat[2][2] = dot - light_pos[2] * plane[2];
	shadowMat[3][2] = 0.f - light_pos[2] * plane[3];
	
	shadowMat[0][3] = 0.f - light_pos[3] * plane[0];
	shadowMat[1][3] = 0.f - light_pos[3] * plane[1];
	shadowMat[2][3] = 0.f - light_pos[3] * plane[2];
	shadowMat[3][3] = dot - light_pos[3] * plane[3];
}

void initCastShadows()
{
	glClear(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS,1,1);
	glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
}

void rotateLight() {
	
	xLight += 0.05;
	zLight += 0.05;
	//yLight += 0.1;
	//float sinX = sinf(xLight);
	float cosX = sinf(xLight);
	float sinZ = cosf(zLight);
	//float cosY = cosf(yLight);
	light_pos[0] = cosX;
	light_pos[2] = sinZ;
	glLightfv(GL_LIGHT0,GL_POSITION,light_pos);
	
	/*
	static a = 0;
	glRotated(a,0,1,0);
	glLightfv(GL_LIGHT0,GL_POSITION,light_pos);
	glRotated(-a,0,1,0);
	a++;*/
	
	glutPostRedisplay();
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
}

void randSolidCubeOnTable() {
	
	float zPos;
	float yPos;
	float xPos;
	int i = 0;
	for(; i < 100 ; ++i) {
		zPos = (rand() % 10) / 10.;
		yPos = (rand() % 5) / 10.;
		xPos = (rand() % 15) / 10.;
		
		glPushMatrix();
			glTranslatef(xPos-(1.5/2.) + 0.05,yPos+0.075,zPos-0.5 + 0.05);
			glutSolidCube(0.1);
		glPopMatrix();
	}
}

void drawChair(float xPos,float yPos,float zPos,float inHeight,float width, float footWidth) {
	
	glColor3f(0.5,0.4,0.5);
	glPushMatrix();
		glTranslatef(xPos,yPos,zPos);
		drawParaCube(0.025,width,width);
		
		//draw feet
		glPushMatrix();
			glTranslatef(-width/2. + footWidth/2.,-(inHeight*0.66)/2.,-width/2. + footWidth/2.);
			drawParaCube(inHeight*0.66,footWidth,footWidth);
			glTranslatef(width - footWidth,0.,0.);
			drawParaCube(inHeight*0.66,footWidth,footWidth);
			glTranslatef(0.,0.,width - footWidth);
			drawParaCube(inHeight*0.66,footWidth,footWidth);
			glTranslatef(-width + footWidth,0.,0.);
			drawParaCube(inHeight*0.66,footWidth,footWidth);
		glPopMatrix();
		
		
		//draw the base by transparence
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		//draw the rest of the chair
		glPushMatrix();
			glTranslatef(0.,(inHeight*0.75)/2.,width/2. - 0.025/2.);
			drawParaCube(inHeight*0.75,width,0.025);
		glPopMatrix();
		glDisable(GL_BLEND); //disable transparence
		
	glPopMatrix();
}


void drawScene(float scale) {
	//triangle(sommets);
	
	//draw objects on the table first (to have a logic shadow)
	glPushMatrix();
		glTranslatef(0.5*scale,0.075*scale,0.);
		glutSolidCube(0.1*scale);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(0.,0.05*scale+0.05*scale,0.);
		glutWireTeapot(0.1*scale);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(-0.5*scale,0.075*scale,0.*scale);
		glutSolidCube(0.1*scale);
		glTranslatef(-0.01*scale,0.1*scale,0.*scale);
		glutSolidCube(0.1*scale);
		glTranslatef(+0.03*scale,0.1*scale,0.);
		glutSolidCube(0.1*scale);
	glPopMatrix();
	
	
	//draw some chairs
	drawChair(0.,-0.30*scale,0.75*scale,0.75*scale,0.4*scale,0.05*scale);
	glPushMatrix();
		glRotatef(180.,0.,1.,0.);
		drawChair(0.,-0.30*scale,0.75*scale,0.75*scale,0.4*scale,0.05*scale);
	glPopMatrix();
	glPushMatrix();
		glRotatef(90.,0.,1.,0.);
		drawChair(0.,-0.30*scale,0.75*scale,0.75*scale,0.4*scale,0.05*scale);
	glPopMatrix();
	glPushMatrix();
		glRotatef(-90.,0.,1.,0.);
		drawChair(0.,-0.30*scale,0.75*scale,0.75*scale,0.4*scale,0.05*scale);
	glPopMatrix();
	
	//draw the table
	drawTable(0.05*scale,1.5*scale,1.0*scale,0.1*scale,0.8*scale);
	
}

/* 
 * Callback : display                              
 */
void display(void)
{
	static float posXChair	= -2.;
	static float posYChair 	= 2.;
	static float posZChair 	= 0.;
	static int counterRotate = 0;
	static GLfloat matrix[4][4];
	
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0); 
	glPushMatrix();
	
	glRotatef (rotx, 1.0, 0.0, 0.0);
    glRotatef (roty, 0.0, 1.0, 0.0);


	glPushMatrix();
		glTranslatef(0.,-0.80,0.);
		drawParaCube(0.05,5.,5.);
	glPopMatrix();
	
	//randSolidCubeOnTable();
	shadowMatrix(matrix);
	glStencilFunc(GL_EQUAL,1,1);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glPushMatrix();
		glMultMatrixf((GLfloat*)matrix);
		drawScene(1.);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_LIGHTING);
	
	drawScene(1.);
	glPushMatrix();
		float scale = 0.3;
		for(scale ; scale > 0. ; scale-=0.1) {
			glTranslatef(0.,0.3,0.);
			drawScene(scale);
		}
    glPopMatrix();
    
    /*
    glPushMatrix();
		counterRotate++;
		switch(counterRotate%4) {
			case 0: glRotatef(90.,0.,1.,0.);
					break;
			case 1: glRotatef(180.,0.,1.,0.);
					break;
			
			case 2: glRotatef(270.,0.,1.,0.);
					break;
			
			case 3:glRotatef(360.,0.,1.,0.);
					break;
		}
		
		posYChair+=0.0005;
		posZChair+=0.0005;
		drawChair(posXChair,cos(posYChair),sin(posZChair),0.75,0.4,5);
	glPopMatrix();*/
	
    
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
	
//	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	gluPerspective(45.,
               (double) w / (double) h,
               1.,
               200.0);
	
	
	
	glMatrixMode(GL_MODELVIEW);
}


typedef enum zoomDir { ZOOM_UP , ZOOM_DOWN} zoomDir_t;
void zoom(zoomDir_t dir)
{
	static float eyeX = 0.;
	static float eyeY = 0.;
	static float eyeZ = 2.;
	static float centerX = 0.;
	static float centerY = 0.;
	static float centerZ = 0.;
	static float upX = 0.;
	static float upY = 1.;
	static float upZ = 0.;
	
	if(dir == ZOOM_UP)
	{
		eyeZ+=0.05;
	} 
	else
	{
		eyeZ-=0.05;
	}
	
	gluLookAt(eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ);
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
    float   sh = fabs(width * 0.5f);
    float   sw = fabs(height * 0.5f);
    
    glBegin(GL_POLYGON);
		glNormal3f(-sw, -sh, .0f);
        glVertex3f(-sw, -sh, .0f);
        
        glNormal3f(+sw, -sh, .0f);
        glVertex3f(+sw, -sh, .0f);

        glNormal3f(+sw, +sh, .0f);        
        glVertex3f(+sw, +sh, .0f);
        
        glNormal3f(-sw, +sh, .0f);
        glVertex3f(-sw, +sh, .0f);
    glEnd();
}

void    drawParaCube( float inXR,
                              float inYR,
                              float inZR)
{
    float   sx = fabs(inXR * 0.5f);
    float   sy = fabs(inYR * 0.5f);
    float   sz = fabs(inZR * 0.5f);
    
    // deux rectangles sur z
    glPushMatrix();
                                        //   [->.  ]
        glTranslatef(.0f, .0f, -sz);     // ->[  .  ]
        glRotatef(180.0,0.,1.,0.); 		// rotation on y (for reflexion)
        drawRectangle2D(inXR, inYR);    // FACE 1
    glPopMatrix();
    
    glPushMatrix();                     //   [->.  ]                                    
        // demi tour sur z
        glRotatef(180.0f,0.,1.,0.);
		//glScalef(.0f, .0f, 1.0f);      // [  .<-]
        // go back
        glTranslatef(.0f, .0f, -sz);     // [  .  ]<-
        glRotatef(180.0f,0.,1.,0.);
        //glColor3f(1.0,1.0,0.); //jaune
        drawRectangle2D(inXR, inYR);    // FACE 2
    glPopMatrix();
    // deux rectangles sur x
    glPushMatrix();
        
        // quart de tour sur axe y          
        glRotatef(90.,1.0f,0.f,0.f);
        glPushMatrix();
            // go west
            glTranslatef(0.f, 0.f, -sx);        // FACE 3
            // front rectangle X
            glRotatef(180.,0.f,1.f,0.f);
            drawRectangle2D(inZR,inYR);
        glPopMatrix();
        glPushMatrix();
                // demi tour
                glRotatef(180.0f,0.,0.,1.);
                //glScalef(1.0f, .0f, .0f);
                // go east
                glTranslatef(.0f, 0.0f, sx);
                //glColor3f(0.,1.0,1.);
                drawRectangle2D(inZR, inYR);    // FACE 4
            
        glPopMatrix();
    glPopMatrix();
    
    // deux rectangles sur y
    glPushMatrix();
        // quart de tour sur axe x
        glRotatef(180., 0.f, 0.0f, 1.0f);
        glRotatef(90., 0.f, 1.0f, 0.0f);
        glPushMatrix();
            // go north
            
            glTranslatef(0.f, 0.f, sy);
            //glColor3f(0.,1.0,0.5);
            drawRectangle2D(inXR, inZR);        // FACE 5
        glPopMatrix();
        glPushMatrix();
			glRotatef(180.0f,1.0,0.,0.);
            // demi tour
            //glScalef(0.0f, -1.0f, 0.0f);
            // go south
            glTranslatef(0.0f, 0.f, sy);
            //glColor3f(0.75,1.0,.0);
            drawRectangle2D(inXR, inZR);        // FACE 6
        glPopMatrix();
    glPopMatrix();
}

void drawTable(float inXR,float inYR,float inZR,float footWidth,float footHeight)
{
	
	glColor3f(0.,0.25,0.51);
	drawParaCube(inXR,inYR,inZR);

	glPushMatrix();
        glTranslatef(-(inYR/2.) + footWidth/2.,0.f, 0.f);
        glTranslatef(0.f,0.f,inZR/2. - footWidth/2.);
        glRotatef(90.0,1.0,0.f,0.f);
        glTranslatef(0.,0.,footHeight/2.);
        drawParaCube(footWidth,footWidth,footHeight); // foot 1 clock wise
        glTranslatef(0.,-inZR + footWidth,0.);
        drawParaCube(footWidth,footWidth,footHeight); // foot 2 clock wise
        glTranslatef(inYR - footWidth,0.,0.);
        drawParaCube(footWidth,footWidth,footHeight); // foot 3 clock wise
        glTranslatef(0.,inZR - footWidth,0.);
        drawParaCube(footWidth,footWidth,footHeight); // foot 4 clock wise
    glPopMatrix();
	glColor3f(1.f, 1.f, 1.f);
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
 * Traitement liŽ aux actions du menu
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
	glutIdleFunc(rotateLight);
	
	glutMainLoop();
	
	return 0;
}
