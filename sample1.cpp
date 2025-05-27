#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GL/glut.h>

#define MAX_PARTICLES 1000
#define WCX 640
#define WCY 480
#define RAIN 0
#define SNOW 1

float slowdown = 2.0;  // Higher value = slower particles
float zoom = -40.0;
int fall = RAIN;  // start with rain

typedef struct {
    bool alive;
    float life;
    float fade;
    float xpos, ypos, zpos;
    float vel;
    float gravity;
} particles;

particles par_sys[MAX_PARTICLES];

// Initialize a single particle
void initParticle(int i) {
    par_sys[i].alive = true;
    par_sys[i].life = 1.0f;
    par_sys[i].fade = (float)(rand() % 100) / 1000.0f + 0.003f;
    par_sys[i].xpos = (float)(rand() % 21) - 10;
    par_sys[i].ypos = 10.0f;
    par_sys[i].zpos = (float)(rand() % 21) - 10;
    par_sys[i].vel = 0.0f;
    par_sys[i].gravity = -0.8f;
}

void initParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        initParticle(i);
    }
}

void drawRain() {
    for (int i = 0; i < MAX_PARTICLES; i += 2) {
        if (par_sys[i].alive) {
            float x = par_sys[i].xpos;
            float y = par_sys[i].ypos;
            float z = par_sys[i].zpos + zoom;

            glColor3f(0.5f, 0.5f, 1.0f);
            glBegin(GL_LINES);
            glVertex3f(x, y, z);
            glVertex3f(x, y + 0.5f, z);
            glEnd();

            par_sys[i].ypos += par_sys[i].vel / (slowdown * 1000);
            par_sys[i].vel += par_sys[i].gravity;
            par_sys[i].life -= par_sys[i].fade;

            if (par_sys[i].ypos <= -10) par_sys[i].life = -1.0f;
            if (par_sys[i].life < 0.0f) initParticle(i);
        }
    }
}

void drawSnow() {
    for (int i = 0; i < MAX_PARTICLES; i += 2) {
        if (par_sys[i].alive) {
            float x = par_sys[i].xpos;
            float y = par_sys[i].ypos;
            float z = par_sys[i].zpos + zoom;

            glColor3f(1.0f, 1.0f, 1.0f);
            glPushMatrix();
            glTranslatef(x, y, z);
            glutSolidSphere(0.2f, 16, 16);
            glPopMatrix();

            par_sys[i].ypos += par_sys[i].vel / (slowdown * 1000);
            par_sys[i].vel += par_sys[i].gravity;
            par_sys[i].life -= par_sys[i].fade;

            if (par_sys[i].ypos <= -10) par_sys[i].life = -1.0f;
            if (par_sys[i].life < 0.0f) initParticle(i);
        }
    }
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (fall == RAIN)
        drawRain();
    else if (fall == SNOW)
        drawSnow();

    glutSwapBuffers();
}

void normal_keys(unsigned char key, int x, int y) {
    if (key == 'r') fall = RAIN;
    if (key == 's') fall = SNOW;
    if (key == 'q') exit(0);

    // Speed control
    if (key == '+') {
        if (slowdown > 0.5f) slowdown -= 0.1f;  // speed up (less slowdown)
    }
    if (key == '-') {
        if (slowdown < 10.0f) slowdown += 0.1f;  // slow down (more slowdown)
    }

    glutPostRedisplay();
}

void idle() {
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WCX, WCY);
    glutCreateWindow("Rain and Snow Simulation");

    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);

    // Set viewport and projection once here
    glViewport(0, 0, WCX, WCY);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)WCX / (float)WCY, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    initParticles();

    glutDisplayFunc(drawScene);
    glutKeyboardFunc(normal_keys);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
