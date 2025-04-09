#include <GL/glut.h>
#include <cmath>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int RECURSION_DEPTH = 4;

void drawKochSegment(float x1, float y1, float x2, float y2, int depth) {
    if (depth == 0) {
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        return;
    }

    float dx = (x2 - x1) / 3.0;
    float dy = (y2 - y1) / 3.0;

    float xA = x1 + dx;
    float yA = y1 + dy;

    float xB = x1 + 2 * dx;
    float yB = y1 + 2 * dy;

    float midX = (x1 + x2) / 2;
    float midY = (y1 + y2) / 2;

    float px = x1 + dx * 0.5f - dy * sqrt(3.0f) / 2.0f;
    float py = y1 + dy * 0.5f + dx * sqrt(3.0f) / 2.0f;

    drawKochSegment(x1, y1, xA, yA, depth - 1);
    drawKochSegment(xA, yA, px, py, depth - 1);
    drawKochSegment(px, py, xB, yB, depth - 1);
    drawKochSegment(xB, yB, x2, y2, depth - 1);
}

void drawKochSnowflake() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);  // Blue

    float size = 0.8f;
    float x1 = -size / 2.0f;
    float y1 = -size / (2.0f * sqrt(3));
    float x2 = size / 2.0f;
    float y2 = -size / (2.0f * sqrt(3));
    float x3 = 0.0f;
    float y3 = size / sqrt(3);

    drawKochSegment(x1, y1, x2, y2, RECURSION_DEPTH);
    drawKochSegment(x2, y2, x3, y3, RECURSION_DEPTH);
    drawKochSegment(x3, y3, x1, y1, RECURSION_DEPTH);

    glEnd();
    glFlush();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // White background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.5, 1.5, -1.5, 1.5);  // Adjust projection to fit the snowflake
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Koch Snowflake");
    init();
    glutDisplayFunc(drawKochSnowflake);
    glutMainLoop();
    return 0;
}
