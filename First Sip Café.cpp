#define _CRT_SECURE_NO_WARNINGS
#ifdef APPLE
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>      
#else
#include <windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>    
#endif
#include <stdlib.h>
#include <iostream>
#include <stdio.h> 
#include <GL/glut.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

using namespace std;


GLuint texture1;
GLuint texture2;
GLuint texture3;

int currentScene = 1;
int lastScene = 0;
GLdouble PI = 3.14159265359;
float girlX = 0.1f;
float girlY = -0.8f;
bool showCup = false;
float steamAngle = 0.0f;
float steamY = 0.0f;
bool isMusicPlaying = false;


char image1Path[] = "C:\\imagesprogect\\out.bmp";
char image2Path[] = "C:\\imagesprogect\\in.bmp";
char image3Path[] = "C:\\imagesprogect\\table.bmp";

GLuint LoadTexture(const char* filename, int width, int height)
{
    GLuint texture;
    unsigned char* data;
    FILE* file;

    // Open the image file (RAW format)
    file = fopen(filename, "rb");
    if (file == NULL)
    {
        cout << "Unable to open the image file" << endl;
        exit(0);
        return 0;
    }

    // Allocate memory for image data (RGB)
    data = (unsigned char*)malloc(width * height * 3);

    // Read image data from file
    fread(data, width * height * 3, 1, file);
    fclose(file);

    // Convert from BGR to RGB
    for (int i = 0; i < width * height; ++i)
    {
        int index = i * 3;
        unsigned char B = data[index];
        unsigned char R = data[index + 2];

        data[index] = R;
        data[index + 2] = B;
    }

    // Generate and bind texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture environment mode
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    // Set texture filtering
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set texture wrapping
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Send texture data to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, data);

    // Free memory
    free(data);

    return texture;
}

void drawBackground(GLuint texture) {
    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);

    // Bind the texture to use it
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set color to white (to keep original texture colors)
    glColor3f(1, 1, 1);

    // Draw a full-screen quad (background)
    glBegin(GL_QUADS);

    glTexCoord2f(0, 0);
    glVertex2f(-1, -1);

    glTexCoord2f(1, 0);
    glVertex2f(1, -1);

    glTexCoord2f(1, 1);
    glVertex2f(1, 1);

    glTexCoord2f(0, 1);
    glVertex2f(-1, 1);

    glEnd();

    // Disable texture after drawing
    glDisable(GL_TEXTURE_2D);
}

void drawTextBox() {
    // Set background color (beige) with transparency (alpha = 0.5)
    glColor4f(0.96f, 0.90f, 0.80f, 0.5f);

    // Draw rectangle (text background box)
    glBegin(GL_QUADS);

    glVertex2f(-0.98f, 0.96f);
    glVertex2f(-0.20f, 0.96f);
    glVertex2f(-0.20f, 0.65f);
    glVertex2f(-0.98f, 0.65f);

    glEnd();
}

void drawText(float x, float y, const char* text)
{
    // Set text color to black
    glColor3f(0, 0, 0);

    // Set text position on screen
    glRasterPos2f(x, y);

    // Draw each character one by one
    for (int i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
    int i;

    // Number of triangles used to approximate the circle
    int triangleAmount = 20;

    // Full circle angle (2 * PI)
    GLfloat twicePi = 2.0f * PI;

    // Start drawing triangle fan (filled circle)
    glBegin(GL_TRIANGLE_FAN);

    // Center of the circle
    glVertex2f(x, y);

    // Generate points around the circle
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)), // X position
            y + (radius * sin(i * twicePi / triangleAmount))  // Y position
        );
    }

    glEnd();
}

void drawFrontGirl()
{
    // Back Hair
    glColor3f(0.60f, 0.40f, 0.25f);

    glBegin(GL_POLYGON);
    glVertex2f(0.35f, 0.58f);
    glVertex2f(0.28f, 0.40f);
    glVertex2f(0.30f, 0.10f);
    glVertex2f(0.70f, 0.10f);
    glVertex2f(0.72f, 0.40f);
    glVertex2f(0.65f, 0.58f);
    glEnd();


    // Dress
    glColor3f(0.65f, 0.72f, 0.55f);

    glBegin(GL_POLYGON);
    glVertex2f(0.34f, 0.15f);
    glVertex2f(0.66f, 0.15f);
    glVertex2f(0.72f, -0.65f);
    glVertex2f(0.28f, -0.65f);
    glEnd();


    // Face
    glColor3f(1.0f, 0.90f, 0.82f);
    drawFilledCircle(0.50f, 0.36f, 0.14f);


    // Neck
    glColor3f(1.0f, 0.90f, 0.82f);

    glBegin(GL_POLYGON);
    glVertex2f(0.46f, 0.24f);
    glVertex2f(0.54f, 0.24f);
    glVertex2f(0.54f, 0.15f);
    glVertex2f(0.46f, 0.15f);
    glEnd();


    // Front Hair
    glColor3f(0.60f, 0.40f, 0.25f);

    glBegin(GL_POLYGON);
    glVertex2f(0.44f, 0.55f);
    glVertex2f(0.56f, 0.55f);
    glVertex2f(0.60f, 0.44f);
    glVertex2f(0.40f, 0.44f);
    glEnd();


    // Left Eye
    glColor3f(0.0f, 0.0f, 0.0f);
    drawFilledCircle(0.46f, 0.40f, 0.01f);

    // Right Eye
    drawFilledCircle(0.54f, 0.40f, 0.01f);


    // Mouth (Smile)
    glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_LINE_STRIP);
    glVertex2f(0.47f, 0.32f);
    glVertex2f(0.50f, 0.31f);
    glVertex2f(0.53f, 0.32f);
    glEnd();


    // Left Arm
    glColor3f(1.0f, 0.90f, 0.82f);

    glBegin(GL_POLYGON);
    glVertex2f(0.34f, 0.12f);
    glVertex2f(0.26f, -0.20f);
    glVertex2f(0.31f, -0.22f);
    glVertex2f(0.40f, 0.08f);
    glEnd();


    // Right Arm
    glBegin(GL_POLYGON);
    glVertex2f(0.66f, 0.10f);
    glVertex2f(0.74f, -0.20f);
    glVertex2f(0.69f, -0.22f);
    glVertex2f(0.60f, 0.08f);
    glEnd();


    // Left Leg
    glBegin(GL_POLYGON);
    glVertex2f(0.44f, -0.65f);
    glVertex2f(0.48f, -0.65f);
    glVertex2f(0.48f, -0.88f);
    glVertex2f(0.44f, -0.88f);
    glEnd();


    // Right Leg
    glBegin(GL_POLYGON);
    glVertex2f(0.52f, -0.65f);
    glVertex2f(0.56f, -0.65f);
    glVertex2f(0.56f, -0.88f);
    glVertex2f(0.52f, -0.88f);
    glEnd();


    // Left Shoe
    glColor3f(0.85f, 0.80f, 0.72f);

    glBegin(GL_POLYGON);
    glVertex2f(0.42f, -0.88f);
    glVertex2f(0.49f, -0.88f);
    glVertex2f(0.49f, -0.95f);
    glVertex2f(0.42f, -0.95f);
    glEnd();


    // Right Shoe
    glBegin(GL_POLYGON);
    glVertex2f(0.51f, -0.88f);
    glVertex2f(0.58f, -0.88f);
    glVertex2f(0.58f, -0.95f);
    glVertex2f(0.51f, -0.95f);
    glEnd();
}

void drawBackGirl()
{
    // Dress
    glColor3f(0.65f, 0.72f, 0.55f);

    glBegin(GL_POLYGON);
    glVertex2f(0.34f, 0.15f);
    glVertex2f(0.66f, 0.15f);
    glVertex2f(0.72f, -0.65f);
    glVertex2f(0.28f, -0.65f);
    glEnd();


    // Back Hair
    glColor3f(0.60f, 0.40f, 0.25f);

    glBegin(GL_POLYGON);
    glVertex2f(0.35f, 0.68f);
    glVertex2f(0.28f, 0.45f);
    glVertex2f(0.30f, 0.05f);
    glVertex2f(0.70f, 0.05f);
    glVertex2f(0.72f, 0.45f);
    glVertex2f(0.65f, 0.68f);
    glEnd();


    // Left Arm
    glColor3f(1.0f, 0.90f, 0.82f);

    glBegin(GL_POLYGON);
    glVertex2f(0.34f, 0.12f);
    glVertex2f(0.26f, -0.20f);
    glVertex2f(0.31f, -0.22f);
    glVertex2f(0.40f, 0.08f);
    glEnd();


    // Right Arm
    glBegin(GL_POLYGON);
    glVertex2f(0.66f, 0.10f);
    glVertex2f(0.74f, -0.20f);
    glVertex2f(0.69f, -0.22f);
    glVertex2f(0.60f, 0.08f);
    glEnd();


    // Left Leg
    glBegin(GL_POLYGON);
    glVertex2f(0.44f, -0.65f);
    glVertex2f(0.48f, -0.65f);
    glVertex2f(0.48f, -0.88f);
    glVertex2f(0.44f, -0.88f);
    glEnd();


    // Right Leg
    glBegin(GL_POLYGON);
    glVertex2f(0.52f, -0.65f);
    glVertex2f(0.56f, -0.65f);
    glVertex2f(0.56f, -0.88f);
    glVertex2f(0.52f, -0.88f);
    glEnd();


    // Left Shoe
    glColor3f(0.85f, 0.80f, 0.72f);

    glBegin(GL_POLYGON);
    glVertex2f(0.42f, -0.88f);
    glVertex2f(0.49f, -0.88f);
    glVertex2f(0.49f, -0.95f);
    glVertex2f(0.42f, -0.95f);
    glEnd();


    // Right Shoe
    glBegin(GL_POLYGON);
    glVertex2f(0.51f, -0.88f);
    glVertex2f(0.58f, -0.88f);
    glVertex2f(0.58f, -0.95f);
    glVertex2f(0.51f, -0.95f);
    glEnd();
}

void drawCup3D()
{
    GLUquadric* q = gluNewQuadric();

    glPushMatrix();

    // Disable lighting to keep solid color
    glDisable(GL_LIGHTING);

    // Cup body color (brown)
    glColor3f(0.58f, 0.45f, 0.32f);

    // Rotate to make the cup upright
    glRotatef(-90, 1, 0, 0);

    // Cup body (cylinder)
    gluCylinder(q, 0.022f, 0.032f, 0.08f, 40, 40);

    // Cup base (bottom disk)
    gluDisk(q, 0, 0.022f, 40, 1);

    // Coffee surface (top disk)
    glTranslatef(0, 0, 0.08f);
    glColor3f(0.15f, 0.08f, 0.05f);
    gluDisk(q, 0, 0.028f, 40, 1);

    glPopMatrix();

    gluDeleteQuadric(q);
}

void drawSteam()
{
    // Enable transparency (blending)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Steam color (light gray with transparency)
    glColor4f(0.9f, 0.9f, 0.9f, 0.5f);

    // Draw multiple steam bubbles
    for (int i = 0; i < 3; i++)
    {
        glPushMatrix();

        // Move each bubble upward with animation
        glTranslatef(0.0f, steamY + i * 0.05f, 0.0f);

        // Draw sphere (steam particle)
        glutSolidSphere(0.02f + i * 0.01f, 10, 10);

        glPopMatrix();
    }

    // Disable blending after drawing
    glDisable(GL_BLEND);
}

// This function plays the door bell sound when the customer enters the coffee shop.
void playDoorBell() {
    PlaySound(TEXT("C:\\imagesprogect\\bell.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

// This function plays background cafe music.
// The sound is played in a loop using SND_LOOP to create a continuous ambient effect.
void playCafeMusic() {
    PlaySound(TEXT("C:\\imagesprogect\\cafe.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void stopMusic() {
    PlaySound(NULL, 0, 0);
}

void update(int value) {
    // Move steam upward
    steamY += 0.01f;

    // Reset position to create a looping effect
    if (steamY > 0.25f)
        steamY = 0.0f;

    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}

void specialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        girlX -= 0.05f;
        break;

    case GLUT_KEY_RIGHT:
        girlX += 0.05f;
        break;

    case GLUT_KEY_UP:
        girlY += 0.05f;
        break;

    case GLUT_KEY_DOWN:
        girlY -= 0.05f;
        break;
    }

    glutPostRedisplay();
}

void drawScene1() {
    glPushMatrix();
    glTranslatef(girlX, girlY, 0.0f);
    glScalef(0.4f, 0.4f, 1.0f);
    drawBackGirl();
    glPopMatrix();
}

void drawScene2() {
    glPushMatrix();
    glTranslatef(girlX, girlY, 0.0f);
    glScalef(0.4f, 0.4f, 1.0f);
    drawBackGirl();
    glPopMatrix();
}

void drawScene3()
{
    if (currentScene == 3)
    {
        // Cup + Steam
        // Draw the cup and its steam only if showCup is true
        if (showCup)
        {
            glPushMatrix();

            // Cup on the table
            glTranslatef(0.0f, 0.1f, 0.0f);
            glRotatef(60, 1, 0, 0);
            glTranslatef(0.0f, 0.7f, 0.4f);
            glScalef(1.5f, 1.5f, 1.5f);
            drawCup3D();
            glPopMatrix();

            // Steam
            glPushMatrix();
            glTranslatef(0.0f, 0.2f, 0.0f);
            drawSteam();
            glPopMatrix();
        }

        // the girl
        glPushMatrix();
        glTranslatef(girlX, girlY, 0.0f);
        glScalef(0.4f, 0.4f, 1.0f);
        drawBackGirl();
        glPopMatrix();
    }
}

void drawScene4() {
    glPushMatrix();

    // Move the girl (and the cup moves with her)
    glTranslatef(girlX, girlY, 0.0f);
    glScalef(0.4f, 0.4f, 1.0f);
    drawFrontGirl();
    glPushMatrix();

    // Position the cup relative to the girl's body (hand area)
    glTranslatef(0.25f, -0.2f, 0.0f);
    glScalef(1.9f, 1.9f, 1.9f);
    drawCup3D();
    glPopMatrix();
    glPopMatrix();
}

void drawSteam();

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Play sound according to the scene
    if (currentScene == 1)
    {
        if (isMusicPlaying)
        {
            stopMusic();
            isMusicPlaying = false;
        }
    }
    else if (currentScene == 2 || currentScene == 3 || currentScene == 4)
    {
        if (!isMusicPlaying)
        {
            playCafeMusic();
            isMusicPlaying = true;
        }
    }
    switch (currentScene) {

    case 1:
        drawBackground(texture1);
        drawScene1();
        drawTextBox();
        drawText(-0.95f, 0.90f, "After a long day, Sarah went to");
        drawText(-0.95f, 0.82f, "First Sip Cafe to buy her");
        drawText(-0.95f, 0.74f, "delicious coffee.");
        break;

    case 2:
        drawBackground(texture2);
        drawScene2();
        drawTextBox();
        drawText(-0.95f, 0.90f, "Sarah entered the cafe,");
        drawText(-0.95f, 0.82f, "walked to the cashier,");
        drawText(-0.95f, 0.74f, "and ordered black coffee.");
        break;

    case 3:
        drawBackground(texture3);
        drawScene3();
        drawTextBox();
        drawText(-0.95f, 0.90f, "While her coffee was being prepared,");
        drawText(-0.95f, 0.82f, "Sarah enjoyed the cozy place");
        drawText(-0.95f, 0.74f, "and the wonderful aroma of coffee,");
        drawText(-0.95f, 0.66f, "and soon her coffee was ready.");
        break;

    case 4:
        drawBackground(texture2);
        drawScene4();
        drawTextBox();
        drawText(-0.95f, 0.90f, "Sarah left the cafe happily");
        drawText(-0.95f, 0.82f, "with her coffee and said,");
        drawText(-0.95f, 0.74f, "The coffee is delicious.");
        drawText(-0.95f, 0.66f, "I will visit this place again.");
        break;
    }

    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1':
        currentScene = 1;
        break;
    case '2':
        currentScene = 2;
        break;
    case '3':
        currentScene = 3;
        break;
    case '4':
        currentScene = 4;
        break;
    case 'c':
    case 'C':
        showCup = true;
        break;
    case 's':
    case 'S':
        if (currentScene == 1)
            playDoorBell();
        break;
    }

    glutPostRedisplay();
}

void init() {
    glClearColor(1, 1, 1, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    texture1 = LoadTexture(image1Path, 1280, 832);
    if (texture1 == -1)
        cout << "Error loading texture 1" << endl;

    texture2 = LoadTexture(image2Path, 1280, 832);
    if (texture2 == -1)
        cout << "Error loading texture 2" << endl;

    texture3 = LoadTexture(image3Path, 1280, 851);
    if (texture3 == -1)
        cout << "Error loading texture 3" << endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(900, 600);
    glutCreateWindow("First Sip Café");

    init();

    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMainLoop();
    return 0;
}
