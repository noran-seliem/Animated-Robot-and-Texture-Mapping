#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "imageloader.h"
#include "glm.h"

using namespace std;


static int elbow[] = { 0, 0 };
static int arm[] = { 0, 0 };
static int armZ[] = { 0, 0 };
static int legX[] = { 0, 0 };
static int legZ[] = { 0, 0 };
static int knee[] = { 0, 0 };
static int fingers[] = { 0, 0 };
static int flang[] = { 0, 0 };
double eye[] = { 0, 0, 5.0 };
double center[] = { 0.0, 0.0, 0.0 };
double up[] = { 0, 1.0, 0 };
double speed = 0.2;
int pressed = 0;
int limitForwardBackward = 0;
int limitUpDown = 0;
int limitRightLeft = 0;
char wavingState = 1;   // 1 for left waving and -1 for right waving
char wavingCtr = 0;     // every 20 counts it switches mode (wavingState)
char trainingAnimation = 0;
char helloAnimation = 0;
char fortniteAnimation = 0;
char fortniteFlage = 0;
char helloFlag = 0;
float BallX = 0;
float BallY = -2;
float BallZ = 1.2;
int ballCtr = 0;
int moving, startx, starty;
char ballAnimation = 0;
GLMmodel* ballonFoot = glmReadOBJ("objects/soccerball.obj");
GLMmodel* Tree = glmReadOBJ("objects/birch.obj");
//Setting White Light Source RGBA
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light_diffuse[] = { 0.5, 0.5, 0.5,1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_position[] = { 0.5,5.0, 0.0, 1.0 };
GLfloat lightPos1[] = { -0.5,-5.0,-2.0, 1.0 };
// Material Properties
GLfloat mat_amb_diff[] = { 0.643, 0.753, 0.934, 1.0 };
GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat shininess[] = { 100.0 };
GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId); //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);               //The actual pixel data
    return textureId; //Returns the id of the texture
}

GLfloat angle = 0.0;   /* in degrees */
GLfloat angle2 = 0.0;   /* in degrees */
GLuint _textureId; //The id of the texture
GLuint _textureId1; //The id of the texture

void timer(int);

void init(void)
{
    // Flip light switch

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    // assign light parameters
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

    // Material Properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    GLfloat lightColor1[] = { 1.0f, 1.0f,  1.0f, 1.0f };
    // Lighting Properties
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
    glEnable(GL_NORMALIZE);

    gluPerspective(140.0, (GLfloat)1024 / (GLfloat)869, 1.0, 150.0);

    Image* image = loadBMP("images/floor6.bmp");
    _textureId = loadTexture(image);
    delete image;

    //Enable smooth shading
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
}
void DrawModel(GLMmodel* Object, GLfloat Scale, GLfloat Angle)
{

    if (!Object) exit(0);
    glmUnitize(Object);
    glmFacetNormals(Object);
    glmVertexNormals(Object, Angle);
    glmScale(Object, Scale);
    glmDraw(Object, GLM_SMOOTH|GLM_MATERIAL);
}
//Initializes 3D rendering
void initRendering(const char* bmbImg, GLuint& textureId) {
    // Loading a Texture
    Image* image = loadBMP(bmbImg);
    textureId = loadTexture(image);
    delete image;

    //Enable smooth shading
    glShadeModel(GL_SMOOTH);

    // Enable Depth buffer
    glEnable(GL_DEPTH_TEST);
}


void choose_texture(int id)
{
    /*
     * Description:
     *     This function is responsible for changing the texture of the floor
     *     using a drop menu by the right click on the mouse.
     */
    switch (id)
    {
    case 1:
        initRendering("images/floor1.bmp", _textureId);
        break;

    case 2:
        initRendering("images/floor2.bmp", _textureId);
        break;

    case 3:
        initRendering("images/floor3.bmp", _textureId);
        break;

    case 4:
        initRendering("images/floor4.bmp", _textureId);
        break;

    case 5:
        initRendering("images/floor5.bmp", _textureId);
        break;

    case 6:
        initRendering("images/floor6.bmp", _textureId);
        break;

    default:
        break;
    }
    glutPostRedisplay();
}


void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    // Clear Depth and Color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    gluLookAt(eye[0], eye[1], eye[2],
        center[0], center[1], center[2],
        up[0], up[1], up[2]);

    glRotatef(angle2, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);

    //floor
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);

    glNormal3f(0.0, -1.0, 0.0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-25, -2.25, 25);
    glTexCoord2f(5.0f, 0.0f);
    glVertex3f(25, -2.25, 25);
    glTexCoord2f(5.0f, 20.0f);
    glVertex3f(25, -2.25, -25);
    glTexCoord2f(0.0f, 20.0f);
    glVertex3f(-25, -2.25, -25);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // START OF RIGHT LEG
    glPushMatrix();

    glTranslatef(0.5, 0.0, 0.0);
    glRotatef((GLfloat)legZ[0], 0.0, 0.0, 1.0);
    glTranslatef(-0.5, 0.0, 0.0);

    glRotatef((GLfloat)legX[0], 1.0, 0.0, 0.0);
    glTranslatef(0.5, -0.5, 0.0);

    glPushMatrix();
    glScalef(0.5, 1.0, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();


    glTranslatef(0.0, -0.5, 0.0);
    glRotatef((GLfloat)knee[0], 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.5, 0.0);
    glPushMatrix();
    glScalef(0.5, 1.0, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glTranslatef(0.0, -0.625, 0.0);
    glPushMatrix();
    glScalef(0.75, 0.25, 0.75);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
    // END OF RIGHT LEG


    // START OF LEFT LEG
    glPushMatrix();

    glTranslatef(-0.5, 0.0, 0.0);
    glRotatef((GLfloat)legZ[1], 0.0, 0.0, 1.0);
    glTranslatef(+0.5, 0.0, 0.0);

    glRotatef((GLfloat)legX[1], 1.0, 0.0, 0.0);
    glTranslatef(-0.5, -0.5, 0.0);

    glPushMatrix();
    glScalef(0.5, 1.0, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();


    glTranslatef(0.0, -0.5, 0.0);
    glRotatef((GLfloat)knee[1], 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.5, 0.0);
    glPushMatrix();
    glScalef(0.5, 1.0, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glTranslatef(0.0, -0.625, 0.0);
    glPushMatrix();
    glScalef(0.75, 0.25, 0.75);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
    // END OF LEFT LEG


    // START OF TRUNK
    glPushMatrix();
    glTranslatef(0.0, 0.25, 0.0);
    glScalef(1.5, 0.5, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();
    // END OF TRUNK

    // START OF STOMACH
    glPushMatrix();
    glTranslatef(0.0, 1.25, 0.0);
    glScalef(1.5, 1.5, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();
    // END OF STOMACH

    // START OF HEAD
    glPushMatrix();
    glTranslatef(0.0, 2.5, 0.0);
    glutSolidSphere(0.35, 16, 16);
    glPopMatrix();
    // END OF HEAD


    // START OF RIGHT ARM

    glPushMatrix();



    glTranslatef(1.1, 1.85, 0.02);


    glRotatef((GLfloat)armZ[0], 1.0, 0.0, 0.0);


    glTranslatef(-0.375, 0, 0);
    glRotatef(arm[0], 0.0, 0.0, 1.0);
    glTranslatef(0.375, 0, 0);

    // Draw the limb
    // The shoulder is centered in origin and has 1, 1, 1 dimensions
    glPushMatrix();
    glScalef(0.75, 0.3, 0.3);
    glutSolidCube(1.0);
    glPopMatrix();

    glTranslatef(0.75, 0, 0);

    glTranslatef(-0.375, 0, 0.0);
    glRotatef((GLfloat)elbow[0], 0.0, 0.0, 1.0);
    glTranslatef(0.375, 0, 0.0);
    glPushMatrix();
    glScalef(0.75, 0.3, 0.3);
    glutSolidCube(1.0);
    glPopMatrix();

    // START OF FINGER 1
    glPushMatrix();

    glTranslatef(0.475, 0.05, 0.1125);

    glTranslatef(-0.1, 0, 0);
    glRotatef(flang[0], 0, 0, 1);
    glTranslatef(0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();



    glPushMatrix();
    glTranslatef(0.1, 0, 0);
    glTranslatef(-0.05, 0, 0);
    glRotatef(fingers[0], 0, 0, 1);
    glTranslatef(0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 1


    // START OF FINGER 2
    glPushMatrix();

    glTranslatef(0.475, 0.05, 0.0375);

    glTranslatef(-0.1, 0, 0);
    glRotatef(flang[0], 0, 0, 1);
    glTranslatef(0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();



    glPushMatrix();
    glTranslatef(0.1, 0, 0);
    glTranslatef(-0.05, 0, 0);
    glRotatef(fingers[0], 0, 0, 1);
    glTranslatef(0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 2

    // START OF FINGER 3
    glPushMatrix();

    glTranslatef(0.475, 0.05, -0.0375);
    glTranslatef(-0.1, 0, 0);
    glRotatef(flang[0], 0, 0, 1);
    glTranslatef(0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.1, 0, 0);
    glTranslatef(-0.05, 0, 0);
    glRotatef(fingers[0], 0, 0, 1);
    glTranslatef(0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 3


    // START OF FINGER 4
    glPushMatrix();

    glTranslatef(0.475, 0.05, -0.1125);

    glTranslatef(-0.1, 0, 0);
    glRotatef(flang[0], 0, 0, 1);
    glTranslatef(0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();



    glPushMatrix();
    glTranslatef(0.1, 0, 0);
    glTranslatef(-0.05, 0, 0);
    glRotatef(fingers[0], 0, 0, 1);
    glTranslatef(0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 4


    // START OF FINGER 5
    glPushMatrix();

    glTranslatef(0.475, -0.1, 0);

    glTranslatef(-0.1, 0, 0);
    glRotatef(-flang[0], 0, 0, 1);
    glTranslatef(0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.1, 0, 0);
    glTranslatef(-0.05, 0, 0);
    glRotatef(-fingers[0], 0, 0, 1);
    glTranslatef(0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 5


    glPopMatrix();


    // END OF RIGHT ARM


    // START OF LEFT ARM

    glPushMatrix();

    glTranslatef(-1.1, 1.85, 0.02);
    glRotatef((GLfloat)armZ[1], 1.0, 0.0, 0.0);
    glTranslatef(0.375, 0, 0);
    glRotatef(arm[1], 0.0, 0.0, 1.0);
    glTranslatef(-0.375, 0, 0);

    // Draw the limb
    // The shoulder is centered in origin and has 1, 1, 1 dimensions
    glPushMatrix();
    glScalef(0.75, 0.3, 0.3);
    glutSolidCube(1.0);
    glPopMatrix();

    glTranslatef(-0.75, 0, 0);

    glTranslatef(0.375, 0, 0.0);
    glRotatef((GLfloat)elbow[1], 0.0, 0.0, 1.0);
    glTranslatef(-0.375, 0, 0.0);
    glPushMatrix();
    glScalef(0.75, 0.3, 0.3);
    glutSolidCube(1.0);
    glPopMatrix();

    // START OF FINGER 1
    glPushMatrix();

    glTranslatef(-0.475, 0.05, 0.1125);

    glTranslatef(0.1, 0, 0);
    glRotatef(flang[1], 0, 0, 1);
    glTranslatef(-0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.1, 0, 0);
    glTranslatef(0.05, 0, 0);
    glRotatef(fingers[1], 0, 0, 1);
    glTranslatef(-0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 1


    // START OF FINGER 2
    glPushMatrix();

    glTranslatef(-0.475, 0.05, 0.0375);
    glTranslatef(0.1, 0, 0);
    glRotatef(flang[1], 0, 0, 1);
    glTranslatef(-0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.1, 0, 0);
    glTranslatef(0.05, 0, 0);
    glRotatef(fingers[1], 0, 0, 1);
    glTranslatef(-0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 2

    // START OF FINGER 3
    glPushMatrix();

    glTranslatef(-0.475, 0.05, -0.0375);

    glTranslatef(0.1, 0, 0);
    glRotatef(flang[1], 0, 0, 1);
    glTranslatef(-0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.1, 0, 0);
    glTranslatef(0.05, 0, 0);
    glRotatef(fingers[1], 0, 0, 1);
    glTranslatef(-0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 3


    // START OF FINGER 4
    glPushMatrix();

    glTranslatef(-0.475, 0.05, -0.1125);

    glTranslatef(0.1, 0, 0);
    glRotatef(flang[1], 0, 0, 1);
    glTranslatef(-0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.1, 0, 0);
    glTranslatef(0.05, 0, 0);
    glRotatef(fingers[1], 0, 0, 1);
    glTranslatef(-0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 4


    // START OF FINGER 5
    glPushMatrix();

    glTranslatef(-0.475, -0.1, 0);

    glTranslatef(0.1, 0, 0);
    glRotatef(-flang[1], 0, 0, 1);
    glTranslatef(-0.1, 0, 0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.1, 0, 0);
    glTranslatef(0.05, 0, 0);
    glRotatef(-fingers[1], 0, 0, 1);
    glTranslatef(-0.05, 0, 0);
    glScalef(0.1, 0.1, 0.05);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // END OF FINGER 5

    glPopMatrix();

    // END OF LEFT ARM
    glDisable(GL_COLOR_MATERIAL);
    glPushMatrix();
    glTranslatef(BallX, BallY, BallZ);
    DrawModel(ballonFoot, 0.3, 90);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(5, -2.8, -4);
    DrawModel(Tree, 9, 0);
    glPopMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glPopMatrix();
    glutSwapBuffers();

}

void crossProduct(double a[], double b[], double c[])
{
    c[0] = a[1] * b[2] - a[2] * b[1];
    c[1] = a[2] * b[0] - a[0] * b[2];
    c[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(double a[])
{
    double norm;
    norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
    norm = sqrt(norm);
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}

void rotatePoint(double a[], double theta, double p[])
{

    double temp[3];
    temp[0] = p[0];
    temp[1] = p[1];
    temp[2] = p[2];

    temp[0] = -a[2] * p[1] + a[1] * p[2];
    temp[1] = a[2] * p[0] - a[0] * p[2];
    temp[2] = -a[1] * p[0] + a[0] * p[1];

    temp[0] *= sin(theta);
    temp[1] *= sin(theta);
    temp[2] *= sin(theta);

    temp[0] += (1 - cos(theta)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
    temp[1] += (1 - cos(theta)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
    temp[2] += (1 - cos(theta)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

    temp[0] += cos(theta) * p[0];
    temp[1] += cos(theta) * p[1];
    temp[2] += cos(theta) * p[2];

    p[0] = temp[0];
    p[1] = temp[1];
    p[2] = temp[2];

}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(85.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}

void Left()
{
    // implement camera rotation arround vertical window screen axis to the left
    // used by mouse and left arrow
    rotatePoint(up, (float)22 / 7 / 20, eye);

    limitRightLeft += 1;

}

void Right()
{

    // implement camera rotation arround vertical window screen axis to the right
    // used by mouse and right arrow
    rotatePoint(up, -(float)22 / 7 / 20, eye);
    limitRightLeft -= 1;
}

void Up()
{
    // implement camera rotation arround horizontal window screen axis +ve
    // used by up arrow
    double horizontal[] = { 0, 0, 0 };

    crossProduct(eye, up, horizontal);
    normalize(horizontal);
    rotatePoint(horizontal, 3.14 / 20, eye);
    rotatePoint(horizontal, 3.14 / 20, up);
    limitUpDown += 1;

}

void Down()
{
    // implement camera rotation arround horizontal window screen axis
    // used by down arrow
    double horizontal[] = { 0, 0, 0 };
    crossProduct(eye, up, horizontal);
    normalize(horizontal);
    rotatePoint(horizontal, -3.14 / 20, eye);
    rotatePoint(horizontal, -3.14 / 20, up);
    limitUpDown -= 1;

}

void moveForward()
{
    double direction[] = { 0, 0, 0 };
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];

    eye[0] -= direction[0] * speed;
    eye[1] -= direction[1] * speed;
    eye[2] -= direction[2] * speed;
    center[0] -= direction[0] * speed;
    center[1] -= direction[1] * speed;
    center[2] -= direction[2] * speed;

    limitForwardBackward += 1;
}

void moveBack()
{

    double direction[] = { 0, 0, 0 };
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];

    eye[0] += direction[0] * speed;
    eye[1] += direction[1] * speed;
    eye[2] += direction[2] * speed;
    center[0] += direction[0] * speed;
    center[1] += direction[1] * speed;
    center[2] += direction[2] * speed;

    limitForwardBackward -= 1;
}


void specialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        if (pressed == 0) {
            if (limitForwardBackward < 10) {
                moveForward();
            }
        }
        else {
            if (limitRightLeft < 10) {
                Left();
            }
        }
        break;
    case GLUT_KEY_RIGHT:
        if (pressed == 0) {
            if (limitForwardBackward > -10) {
                moveBack();
            }
        }
        else {
            if (limitRightLeft > -10) {
                Right();
            }
        }
        break;
    case GLUT_KEY_UP:
        if (limitUpDown < 8)
        {
            Up();
        }
        break;
    case GLUT_KEY_DOWN:
        if (limitUpDown > -8) {
            Down();
        }
        break;
    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        // LEGS MOVEMENT
    case 'x':
        if (legX[1] >= -70) {
            legX[1] = (legX[1] - 5) % 360;
            glutPostRedisplay();


        }
        break;

    case 'X':
        if (legX[1] <= 70) {
            legX[1] = (legX[1] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'z':
        if (legZ[1] >= -70) {
            legZ[1] = (legZ[1] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'Z':
        if (legZ[1] <= 70) {
            legZ[1] = (legZ[1] + 5) % 360;
            glutPostRedisplay();
        }

        break;


    case 'n':
        if (legX[0] >= -50) {
            legX[0] = (legX[0] - 5) % 360;
            glutPostRedisplay();


        }
        break;

    case 'N':
        if (legX[0] <= 50) {
            legX[0] = (legX[0] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'm':
        if (legZ[0] >= -70) {
            legZ[0] = (legZ[0] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'M':
        if (legZ[0] <= 70) {
            legZ[0] = (legZ[0] + 5) % 360;
            glutPostRedisplay();
        }

        break;

        // KNEES MOVEMENT
    case 'k':
        if (knee[0] >= -30) {
            knee[0] = (knee[0] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'K':
        if (knee[0] <= 30) {
            knee[0] = (knee[0] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 's':
        if (knee[1] >= -80) {
            knee[1] = (knee[1] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'S':
        if (knee[1] <= 80) {
            knee[1] = (knee[1] + 5) % 360;
            glutPostRedisplay();
        }
        break;

        // ARMS MOVEMENT
    case 'c':
        if (arm[1] < 90) {
            arm[1] = (arm[1] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'C':
        if (arm[1] > -90) {
            arm[1] = (arm[1] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'j':
        if (arm[0] < 90) {
            arm[0] = (arm[0] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'J':
        if (arm[0] > -90) {
            arm[0] = (arm[0] - 5) % 360;
            glutPostRedisplay();
        }
        break;


        // ELBOWS MOVEMENT
    case 'v':
        if (elbow[1] < 130) {
            elbow[1] = (elbow[1] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'V':
        if (elbow[1] > -120) {
            elbow[1] = (elbow[1] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'b':
        if (elbow[0] < 130) {
            elbow[0] = (elbow[0] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'B':
        if (elbow[0] > -120) {
            elbow[0] = (elbow[0] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'U':
        if (armZ[0] < 90) {
            armZ[0] = (armZ[0] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'u':
        if (armZ[0] > -90) {
            armZ[0] = (armZ[0] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'D':
        if (armZ[1] < 90) {
            armZ[1] = (armZ[1] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'd':
        if (armZ[1] > -90) {
            armZ[1] = (armZ[1] - 5) % 360;
            glutPostRedisplay();
        }
        break;


    case 'q':
        if (flang[1] < 20) {
            flang[1] = (flang[1] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'Q':
        if (flang[1] > -20) {
            flang[1] = (flang[1] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'w':
        if (fingers[1] < 20) {
            fingers[1] = (fingers[1] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'W':
        if (fingers[1] > -20) {
            fingers[1] = (fingers[1] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'i':
        if (flang[0] < 20) {
            flang[0] = (flang[0] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'I':
        if (flang[0] > -20) {
            flang[0] = (flang[0] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'o':
        if (fingers[0] < 20) {
            fingers[0] = (fingers[0] + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'O':
        if (fingers[0] > -20) {
            fingers[0] = (fingers[0] - 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'p':
        ballAnimation = 0;
        if (helloAnimation)
        {
            arm[0] = 0;
            elbow[0] = 0;
            arm[1] = 0;
            elbow[1] = 0;
            glutPostRedisplay();
            helloAnimation = 0;
        }
        else if (trainingAnimation)
        {
            trainingAnimation = 0;
            armZ[0] = 0;
            armZ[1] = 0;
            arm[0] = 0;
            arm[1] = 0;
            elbow[0] = 0;
            elbow[1] = 0;
            glutPostRedisplay();

        }
        else if (ballAnimation == 1)
        {
            ballAnimation = 0;
            legX[0] = 0;
            legX[1] = 0;
            BallX = 0;
            BallY = -2;
            glutPostRedisplay();
        }
        else {
            fortniteAnimation = 1;
            if (!(fortniteFlage)) {
                arm[1] = -60;
                arm[0] = -40;
                elbow[1] = -100;
                elbow[0] = -80;
            }
            else {
                fortniteAnimation = 0;
                arm[1] = 0;
                arm[0] = 0;
                elbow[1] = 0;
                elbow[0] = 0;
                legZ[0] = 0;
                legZ[1] = 0;
            }
            fortniteFlage = !(fortniteFlage);
            wavingCtr = 0;
            glutPostRedisplay();
            glutTimerFunc(0, timer, 1);
        }
        break;

    case 'l':
        if (fortniteAnimation)
        {
            fortniteAnimation = 0;
            arm[1] = 0;
            arm[0] = 0;
            elbow[1] = 0;
            elbow[0] = 0;
            legZ[0] = 0;
            legZ[1] = 0;
            glutPostRedisplay();
        }
        else if (trainingAnimation)
        {
            trainingAnimation = 0;
            armZ[0] = 0;
            armZ[1] = 0;
            arm[0] = 0;
            arm[1] = 0;
            elbow[0] = 0;
            elbow[1] = 0;
            glutPostRedisplay();

        }
        else if (ballAnimation == 1)
        {
            ballAnimation = 0;
            legX[0] = 0;
            legX[1] = 0;
            BallX = 0;
            BallY = -2;
            glutPostRedisplay();
        }
        else {
            trainingAnimation = 0;
            helloAnimation = 1;
            wavingCtr = 0;
            if (helloFlag) {
                helloAnimation = 0;
                arm[0] = 0;
                arm[1] = 0;
                elbow[1] = 0;
            }
            else {
                arm[1] = -40;
                arm[0] = -90;
                elbow[1] = -80;
            }
            glutPostRedisplay();
            helloFlag = !(helloFlag);
            glutTimerFunc(0, timer, 1);
        }
        break;


    case 'r':
    case 'R':
        eye[0] = 0;
        eye[1] = 0;
        eye[2] = 5.0;
        center[0] = 0;
        center[1] = 0;
        center[2] = 0;
        up[0] = 0;
        up[1] = 1.0;
        up[2] = 0;
        angle = 0;
        angle2 = 0;
        limitRightLeft = 0;
        limitForwardBackward = 0;
        limitUpDown = 0;
        glutPostRedisplay();
        break;

    case '.':
        ballAnimation = 0;
        wavingCtr = 0;
        if (fortniteAnimation)
        {
            fortniteAnimation = 0;
            arm[1] = 0;
            arm[0] = 0;
            elbow[1] = 0;
            elbow[0] = 0;
            legZ[0] = 0;
            legZ[1] = 0;
            glutPostRedisplay();
        }
        else if (helloAnimation)
        {
            helloAnimation = 0;
            arm[0] = 0;
            arm[1] = 0;
            elbow[1] = 0;
            glutPostRedisplay();
        }
        else if (ballAnimation == 1)
        {
            ballAnimation = 0;
            legX[0] = 0;
            legX[1] = 0;
            BallX = 0;
            BallY = -2;
            glutPostRedisplay();
        }
        else {
            if (trainingAnimation == 0) {
                trainingAnimation = 1;
                armZ[0] = 90;
                armZ[1] = 90;
                elbow[0] = 100;
                elbow[1] = -100;
                glutTimerFunc(0, timer, 1);
            }
            else {
                trainingAnimation = 0;
                armZ[0] = 0;
                armZ[1] = 0;
                arm[0] = 0;
                arm[1] = 0;
                elbow[0] = 0;
                elbow[1] = 0;
                glutPostRedisplay();
            }
        }
        break;

    case 'y':
        if (fortniteAnimation)
        {
            fortniteAnimation = 0;
            arm[1] = 0;
            arm[0] = 0;
            elbow[1] = 0;
            elbow[0] = 0;
            legZ[0] = 0;
            legZ[1] = 0;
            glutPostRedisplay();
        }
        else if (helloAnimation)
        {
            helloAnimation = 0;
            arm[0] = 0;
            arm[1] = 0;
            elbow[1] = 0;
            glutPostRedisplay();
        }
        else if (trainingAnimation) {
            trainingAnimation = 0;
            armZ[0] = 0;
            armZ[1] = 0;
            arm[0] = 0;
            arm[1] = 0;
            elbow[0] = 0;
            elbow[1] = 0;
            glutPostRedisplay();
        }
        ballCtr = 0;

        if (ballAnimation == 0)
        {
            ballAnimation = 1;
            arm[1] = 50;
            arm[0] = -50;
            legX[1] = -32;
            legX[0] = 0;
            BallX = -0.4;
            BallY = -1.2;
            glutTimerFunc(0, timer, 10);
        }
        else if (ballAnimation == 1)
        {
            ballAnimation = 0;
            arm[1] = 0;
            arm[0] = 0;
            legX[0] = 0;
            legX[1] = 0;
            BallX = 0;
            BallY = -2;
            glutPostRedisplay();
        }
        break;

    case 27:
        exit(0);
        break;

    default:
        break;
    }
}

static void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            pressed = 1;
            moving = 1;
            startx = x;
            starty = y;
        }
        if (state == GLUT_UP) {
            moving = 0;
            pressed = 0;
        }
    }
}


static void motion(int x, int y)
{
    if (moving) {
        angle = angle + (x - startx);
        angle2 = angle2 + (y - starty);
        startx = x;
        starty = y;
        glutPostRedisplay();
    }
}

void timer(int value)
{
    if (value != 10) {
        if (fortniteAnimation) {
            if (fortniteFlage) {
                if (value == 1) {
                    legZ[0] += 6;
                    glutPostRedisplay();
                    wavingCtr++;
                    if (wavingCtr < 10) {
                        glutTimerFunc(20, timer, 1);
                    }
                    else {
                        wavingCtr = 0;
                        glutTimerFunc(20, timer, -1);
                    }
                }
                else if (value == -1) {
                    legZ[0] -= 6;
                    glutPostRedisplay();
                    wavingCtr++;
                    if (wavingCtr < 10) {
                        glutTimerFunc(20, timer, -1);
                    }
                    else {
                        wavingCtr = 0;
                        glutTimerFunc(20, timer, 2);
                    }
                }
                else if (value == 2) {
                    legZ[1] -= 6;
                    glutPostRedisplay();
                    wavingCtr++;
                    if (wavingCtr < 10) {
                        glutTimerFunc(20, timer, 2);
                    }
                    else {
                        wavingCtr = 0;
                        glutTimerFunc(20, timer, -2);
                    }
                }
                else if (value == -2) {
                    legZ[1] += 6;
                    glutPostRedisplay();
                    wavingCtr++;
                    if (wavingCtr < 10) {
                        glutTimerFunc(20, timer, -2);
                    }
                    else {
                        wavingCtr = 0;
                        glutTimerFunc(20, timer, 1);
                    }
                }
            }
        }
        else if (trainingAnimation) {
            if (value == 1) {
                arm[0] += 1;
                arm[1] -= 1;
                glutPostRedisplay();
                wavingCtr++;
                if (wavingCtr < 50) {
                    glutTimerFunc(10, timer, 1);
                }
                else {
                    wavingCtr = 0;
                    glutTimerFunc(10, timer, -1);
                }
            }
            else if (value == -1) {
                arm[0] -= 1;
                arm[1] += 1;
                glutPostRedisplay();
                wavingCtr++;
                if (wavingCtr < 50) {
                    glutTimerFunc(10, timer, -1);
                }
                else {
                    wavingCtr = 0;
                    glutTimerFunc(10, timer, 1);
                }
            }
        }


        if (helloAnimation) {
            if (helloFlag) {
                if (value == 1) {
                    elbow[1] -= 2;
                    glutPostRedisplay();
                    wavingCtr++;
                    if (wavingCtr < 20) {
                        glutTimerFunc(20, timer, 1);
                    }
                    else {
                        wavingCtr = 0;
                        glutTimerFunc(20, timer, -1);
                    }
                }
                else if (value == -1) {
                    elbow[1] += 2;
                    glutPostRedisplay();
                    wavingCtr++;
                    if (wavingCtr < 20) {
                        glutTimerFunc(20, timer, -1);
                    }
                    else {
                        wavingCtr = 0;
                        glutTimerFunc(20, timer, 1);
                    }
                }
            }
        }
    }
    else if (value == 10 && ballAnimation == 1)
    {
        if (ballCtr < 8)
        {
            BallX += 0.05;
            legX[1] += 4;
            BallY += 0.15;
            ballCtr++;
        }
        else if (ballCtr < 16)
        {
            BallX += 0.05;
            legX[0] -= 4;
            BallY -= 0.15;
            ballCtr++;
        }
        else if (ballCtr < 24)
        {
            BallX -= 0.05;
            legX[0] += 4;
            BallY += 0.15;
            ballCtr++;
        }
        else if (ballCtr < 32)
        {
            BallX -= 0.05;
            legX[1] -= 4;
            BallY -= 0.15;
            ballCtr++;
        }
        else if (ballCtr == 32) ballCtr = 0;
        glutTimerFunc(40, timer, 10);
        glutPostRedisplay();


    }
}




int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 700);
    glutInitWindowPosition(100, 0);
    glutCreateWindow("ROOM");
    init();
    glutMouseFunc(mouse);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMotionFunc(motion);

    glutCreateMenu(choose_texture);
    glutAddMenuEntry("Floor 1", 1);
    glutAddMenuEntry("Floor 2", 2);
    glutAddMenuEntry("Floor 3", 3);
    glutAddMenuEntry("Floor 4", 4);
    glutAddMenuEntry("Floor 5", 5);
    glutAddMenuEntry("grass", 6);
    glutAttachMenu(GLUT_RIGHT_BUTTON);



    glutMainLoop();
    return 0;
}