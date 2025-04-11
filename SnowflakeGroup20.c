#include <GL/glut.h>
#include <vector>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Global variables
int depth = 4;
std::vector<float> snowflakePoints;

// Generate Koch curve points
void generateKochPoints(std::vector<float>& points, float x1, float y1, float x2, float y2, int depth) {
    if (depth == 0) {
        // Only add the first point of the segment
        points.push_back(x1);
        points.push_back(y1);
        return;
    }

    float deltaX = x2 - x1;
    float deltaY = y2 - y1;

    float x3 = x1 + deltaX / 3;
    float y3 = y1 + deltaY / 3;

    float x4 = x1 + 2 * deltaX / 3;
    float y4 = y1 + 2 * deltaY / 3;

    float angle = atan2(deltaY, deltaX) - M_PI / 3;
    float length = sqrt(deltaX * deltaX + deltaY * deltaY) / 3;
    float x5 = x3 + length * cos(angle);
    float y5 = y3 + length * sin(angle);

    generateKochPoints(points, x1, y1, x3, y3, depth - 1);
    generateKochPoints(points, x3, y3, x5, y5, depth - 1);
    generateKochPoints(points, x5, y5, x4, y4, depth - 1);
    generateKochPoints(points, x4, y4, x2, y2, depth - 1);
}

// Generate the complete Koch snowflake outline
std::vector<float> generateKochSnowflake(int depth) {
    std::vector<float> points;

    // Base triangle
    float side = 1.5;
    float height = side * sqrt(3) / 2;

    float x1 = -side / 2;
    float y1 = -height / 3;

    float x2 = side / 2;
    float y2 = -height / 3;

    float x3 = 0;
    float y3 = 2 * height / 3;

    // Generate points for each side
    generateKochPoints(points, x1, y1, x2, y2, depth);
    generateKochPoints(points, x2, y2, x3, y3, depth);
    generateKochPoints(points, x3, y3, x1, y1, depth);

    // Add the first point again to close the loop
    points.push_back(points[0]);
    points.push_back(points[1]);

    return points;
}

// Display callback function
void display() {
    // Clear the screen to white
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw filled snowflake
    glColor3f(0.0f, 0.5f, 1.0f); // Blue color
    glBegin(GL_TRIANGLE_FAN);
    for (size_t i = 0; i < snowflakePoints.size(); i += 2) {
        glVertex2f(snowflakePoints[i], snowflakePoints[i + 1]);
    }
    glEnd();

    // Draw outline
    glLineWidth(2.0f);
    glColor3f(0.0f, 0.3f, 0.8f); // Darker blue
    glBegin(GL_LINE_LOOP);
    for (size_t i = 0; i < snowflakePoints.size(); i += 2) {
        glVertex2f(snowflakePoints[i], snowflakePoints[i + 1]);
    }
    glEnd();

    glutSwapBuffers();
}

// Keyboard callback function
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC key
        exit(0);
    }
    else if (key >= '0' && key <= '5') {
        depth = key - '0';
        snowflakePoints = generateKochSnowflake(depth);
        glutPostRedisplay();
    }
}

// Reshape callback function
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Keep aspect ratio square
    if (width <= height)
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat)height / (GLfloat)width,
            2.0 * (GLfloat)height / (GLfloat)width, -10.0, 10.0);
    else
        glOrtho(-2.0 * (GLfloat)width / (GLfloat)height,
            2.0 * (GLfloat)width / (GLfloat)height, -2.0, 2.0, -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Koch Snowflake");

    // Register callbacks
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);

    // Generate initial snowflake points
    snowflakePoints = generateKochSnowflake(depth);

    // Main loop
    glutMainLoop();
    return 0;
}