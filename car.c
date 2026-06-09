#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Simulation States
#define RED 0
#define YELLOW 1
#define GREEN 2

// Environment Variables
int trafficLightState = RED;
int frameCounter = 0;
float stopLineX = 0.1f;       
float zebraStartX = -0.35f;   
float preZebraStopLineX = -0.40f; // New stop line coordinate before zebra crossing

// Multi-Agent Positions
float car1X = -0.2f;         
float car2X = -0.8f;         
float car3X = -1.5f;         

// Random Lane Offsets
float car1YOffset = -0.55f;
float car2YOffset = -0.75f;
float car3YOffset = -0.65f;

// Operational dynamic speeds
float car1Speed = 0.005f;
float car2Speed = 0.004f;
float car3Speed = 0.006f;

// Active Maximum Speeds 
float car1MaxSpeed = 0.005f;
float car2MaxSpeed = 0.0035f; 
float car3MaxSpeed = 0.0065f; 

// Structure used to dynamically sort render queues
typedef struct {
    float x;
    float y;
    float r, g, b;
} RenderCar;

float getRandomRange(float min, float max) {
    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= num_segments; i++) {
            float theta = 2.0f * M_PI * (float)i / (float)num_segments;
            float x = r * cosf(theta);
            float y = r * sinf(theta);
            glVertex2f(x + cx, y + cy);
        }
    glEnd();
}

// Updated braking window to perfectly match the new pre-zebra line alignment
float calculateLightReaction(float currentX, float maxSpeed) {
    // Stops the car nose exactly behind the new line boundary
    if (currentX >= (preZebraStopLineX - 0.25f) && currentX <= preZebraStopLineX) {
        if (trafficLightState == RED || trafficLightState == YELLOW) {
            return 0.0f; 
        }
    }
    return maxSpeed; 
}

void updateEnvironment(int value) {
    frameCounter++;

    if (trafficLightState == RED && frameCounter > 300) {
        trafficLightState = GREEN;
        frameCounter = 0;
    } else if (trafficLightState == GREEN && frameCounter > 300) {
        trafficLightState = YELLOW;
        frameCounter = 0;
    } else if (trafficLightState == YELLOW && frameCounter > 100) {
        trafficLightState = RED;
        frameCounter = 0;
    }

    car1Speed = calculateLightReaction(car1X, car1MaxSpeed);
    car2Speed = calculateLightReaction(car2X, car2MaxSpeed);
    car3Speed = calculateLightReaction(car3X, car3MaxSpeed);

    car1X += car1Speed;
    car2X += car2Speed;
    car3X += car3Speed;

    if (car1X > 1.6f) {
        car1X = getRandomRange(-2.2f, -1.4f);
        car1YOffset = getRandomRange(-0.75f, -0.55f);
        car1MaxSpeed = getRandomRange(0.003f, 0.008f); 
    }
    if (car2X > 1.6f) {
        car2X = getRandomRange(-2.2f, -1.4f);
        car2YOffset = getRandomRange(-0.75f, -0.55f);
        car2MaxSpeed = getRandomRange(0.003f, 0.008f); 
    }
    if (car3X > 1.6f) {
        car3X = getRandomRange(-2.2f, -1.4f);
        car3YOffset = getRandomRange(-0.75f, -0.55f);
        car3MaxSpeed = getRandomRange(0.003f, 0.008f); 
    }

    glutPostRedisplay(); 
    glutTimerFunc(16, updateEnvironment, 0);
}

void drawRoadAndZebraCrossing() {
    // Road Bed
    glColor3f(0.18f, 0.18f, 0.18f);
    glBegin(GL_QUADS);
        glVertex2f(-2.0f, -0.4f);
        glVertex2f(2.0f, -0.4f);
        glVertex2f(2.0f, -0.9f);
        glVertex2f(-2.0f, -0.9f);
    glEnd();

    // Center Lane Markings
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
        for(float i = -2.0f; i <= 2.0f; i += 0.5f) {
            if (i > -0.6f && i < 0.2f) continue; // Clear markings near crossing zone
            glVertex2f(i, -0.66f);
            glVertex2f(i + 0.25f, -0.66f);
            glVertex2f(i + 0.25f, -0.64f);
            glVertex2f(i, -0.64f);
        }
    glEnd();

    // Zebra Crosswalk Stripes
    glColor3f(0.95f, 0.95f, 0.95f);
    for (float y = -0.85f; y <= -0.45f; y += 0.12f) {
        glBegin(GL_QUADS);
            glVertex2f(zebraStartX, y);
            glVertex2f(zebraStartX + 0.37f, y);
            glVertex2f(zebraStartX + 0.37f, y + 0.06f);
            glVertex2f(zebraStartX, y + 0.06f);
        glEnd();
    }

    // NEW line drawn cleanly right before the zebra crosswalk starts
    glColor3f(0.95f, 0.95f, 0.95f);
    glBegin(GL_QUADS);
        glVertex2f(preZebraStopLineX - 0.03f, -0.9f);
        glVertex2f(preZebraStopLineX, -0.9f);
        glVertex2f(preZebraStopLineX, -0.4f);
        glVertex2f(preZebraStopLineX - 0.03f, -0.4f);
    glEnd();

    // Structural Stop Line after the crossing
    glBegin(GL_QUADS);
        glVertex2f(stopLineX - 0.03f, -0.9f);
        glVertex2f(stopLineX, -0.9f);
        glVertex2f(stopLineX, -0.4f);
        glVertex2f(stopLineX - 0.03f, -0.4f);
    glEnd();
}

void drawTrafficLight() {
    float poleX = stopLineX + 0.08f;

    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(poleX, -0.4f);
        glVertex2f(poleX + 0.02f, -0.4f);
        glVertex2f(poleX + 0.02f, 0.4f);
        glVertex2f(poleX, 0.4f);
    glEnd();

    glColor3f(0.05f, 0.05f, 0.05f);
    glBegin(GL_QUADS);
        glVertex2f(poleX - 0.05f, 0.05f);
        glVertex2f(poleX + 0.07f, 0.05f);
        glVertex2f(poleX + 0.07f, 0.55f);
        glVertex2f(poleX - 0.05f, 0.55f);
    glEnd();

    float bulbX = poleX + 0.01f;
    float radius = 0.04f;

    if (trafficLightState == RED) glColor3f(1.0f, 0.0f, 0.0f); else glColor3f(0.2f, 0.0f, 0.0f);
    drawCircle(bulbX, 0.44f, radius, 32);

    if (trafficLightState == YELLOW) glColor3f(1.0f, 0.9f, 0.0f); else glColor3f(0.2f, 0.2f, 0.0f);
    drawCircle(bulbX, 0.30f, radius, 32);

    if (trafficLightState == GREEN) glColor3f(0.0f, 1.0f, 0.0f); else glColor3f(0.0f, 0.2f, 0.0f);
    drawCircle(bulbX, 0.16f, radius, 32);
}

void drawCustomCar(float xPos, float r, float g, float b, float yOffset) {
    glPushMatrix();
    glTranslatef(xPos, yOffset, 0.0f); 

    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(-0.25f, -0.09f);
        glVertex2f(0.12f, -0.09f);
        glVertex2f(0.10f, 0.02f);
        glVertex2f(-0.25f, 0.04f);
    glEnd();

    glColor3f(0.1f, 0.1f, 0.1f); 
    glBegin(GL_QUADS);
        glVertex2f(-0.18f, 0.03f);
        glVertex2f(0.02f, 0.02f);
        glVertex2f(-0.02f, 0.13f);
        glVertex2f(-0.14f, 0.13f);
    glEnd();

    glColor3f(0.4f, 0.6f, 0.8f);
    glBegin(GL_QUADS);
        glVertex2f(-0.07f, 0.04f);
        glVertex2f(0.01f, 0.03f);
        glVertex2f(-0.02f, 0.11f);
        glVertex2f(-0.07f, 0.11f);
    glEnd();

    glColor3f(0.05f, 0.05f, 0.05f); 
    drawCircle(-0.15f, -0.12f, 0.055f, 24); 
    drawCircle(0.04f, -0.12f, 0.055f, 24);  

    glPopMatrix();
}

void display() {
    glClearColor(0.85f, 0.85f, 0.85f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawRoadAndZebraCrossing();
    drawTrafficLight();
    
    RenderCar queue[3];
    queue[0] = (RenderCar){car1X, car1YOffset, 0.9f, 0.1f, 0.1f}; 
    queue[1] = (RenderCar){car2X, car2YOffset, 0.1f, 0.5f, 0.9f}; 
    queue[2] = (RenderCar){car3X, car3YOffset, 0.9f, 0.6f, 0.1f}; 

    // Bubble Sort based on depth layout alignment
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2 - i; j++) {
            if (queue[j].y < queue[j + 1].y) {
                RenderCar temp = queue[j];
                queue[j] = queue[j + 1];
                queue[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        drawCustomCar(queue[i].x, queue[i].r, queue[i].g, queue[i].b, queue[i].y);
    }

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (w <= h)
        glOrtho(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w, -1.0, 1.0);
    else
        glOrtho(-1.0 * (GLfloat)w / (GLfloat)h, 1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0, -1.0, 1.0);
        
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    srand(time(NULL)); 
    
    car1X = getRandomRange(-0.2f, 0.0f);
    car2X = getRandomRange(-0.8f, -0.5f);
    car3X = getRandomRange(-1.5f, -1.1f);
    
    car1MaxSpeed = getRandomRange(0.004f, 0.008f);
    car2MaxSpeed = getRandomRange(0.004f, 0.008f);
    car3MaxSpeed = getRandomRange(0.004f, 0.008f);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(960, 540); 
    glutCreateWindow("AI Traffic - Pre-Crossing Stop Line Layout");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    glutTimerFunc(0, updateEnvironment, 0);

    glutMainLoop();
    return 0;
}