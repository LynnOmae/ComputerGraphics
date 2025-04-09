from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
from math import sin, cos, pi, sqrt

RECURSION_DEPTH = 4
WINDOW_SIZE = 800

# Generate a Koch curve recursively between two points
def koch_curve(p1, p2, depth):
    if depth == 0:
        return [p1, p2]
    else:
        x1, y1 = p1
        x2, y2 = p2

        dx = (x2 - x1) / 3.0
        dy = (y2 - y1) / 3.0

        a = (x1 + dx, y1 + dy)
        b = (x1 + 2*dx, y1 + 2*dy)

        # Peak of equilateral triangle
        angle = pi / 3  # 60 degrees
        px = a[0] + cos(angle) * (b[0] - a[0]) - sin(angle) * (b[1] - a[1])
        py = a[1] + sin(angle) * (b[0] - a[0]) + cos(angle) * (b[1] - a[1])
        peak = (px, py)

        # Recursively divide
        return (
            koch_curve(p1, a, depth-1)[:-1] +
            koch_curve(a, peak, depth-1)[:-1] +
            koch_curve(peak, b, depth-1)[:-1] +
            koch_curve(b, p2, depth-1)
        )

# Generate the full Koch snowflake shape
def generate_snowflake(depth):
    size = 0.9
    h = sqrt(3) / 2 * size
    p1 = (-size/2, -h/3)
    p2 = (size/2, -h/3)
    p3 = (0.0, 2*h/3)

    edge1 = koch_curve(p1, p2, depth)
    edge2 = koch_curve(p2, p3, depth)
    edge3 = koch_curve(p3, p1, depth)

    return edge1 + edge2 + edge3

def display():
    glClear(GL_COLOR_BUFFER_BIT)
    
    # Simulate gradient by blending center and edge colors
    glBegin(GL_TRIANGLE_FAN)
    glColor3f(0.7, 0.7, 1.0)  # Light blue at center
    glVertex2f(0.0, 0.0)      # Center vertex

    # Outer vertices
    glColor3f(0.0, 0.0, 1.0)  # Dark blue edges
    vertices = generate_snowflake(RECURSION_DEPTH)
    for vx, vy in vertices:
        glVertex2f(vx, vy)

    # Close the loop
    glVertex2f(vertices[0][0], vertices[0][1])
    glEnd()

    glFlush()

def init():
    glClearColor(1.0, 1.0, 1.0, 1.0)  # White background
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluOrtho2D(-1.5, 1.5, -1.5, 1.5)  # Adjust the view to fit the snowflake

def main():
    glutInit()
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB)
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE)
    glutCreateWindow(b"Koch Snowflake with Gradient")

    init()
    glutDisplayFunc(display)
    glutMainLoop()

if __name__ == "__main__":
    main()
