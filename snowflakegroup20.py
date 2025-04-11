from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import math

# Global variables
depth = 4
snowflake_points = []


def generate_koch_points(points, x1, y1, x2, y2, depth):
    """Generate points for Koch curve recursively"""
    if depth == 0:
        # Add only the first point of the segment
        points.extend([x1, y1])
        return

    # Calculate new points
    dx = x2 - x1
    dy = y2 - y1

    x3 = x1 + dx / 3
    y3 = y1 + dy / 3

    x4 = x1 + 2 * dx / 3
    y4 = y1 + 2 * dy / 3

    angle = math.atan2(dy, dx) - math.pi / 3
    length = math.sqrt(dx * dx + dy * dy) / 3
    x5 = x3 + length * math.cos(angle)
    y5 = y3 + length * math.sin(angle)

    # Recursive calls for the four segments
    generate_koch_points(points, x1, y1, x3, y3, depth - 1)
    generate_koch_points(points, x3, y3, x5, y5, depth - 1)
    generate_koch_points(points, x5, y5, x4, y4, depth - 1)
    generate_koch_points(points, x4, y4, x2, y2, depth - 1)


def generate_koch_snowflake(depth):
    """Generate the complete Koch snowflake outline"""
    points = []

    # Base triangle
    side = 1.5
    height = side * math.sqrt(3) / 2

    x1 = -side / 2
    y1 = -height / 3

    x2 = side / 2
    y2 = -height / 3

    x3 = 0
    y3 = 2 * height / 3

    # Generate points for each side
    generate_koch_points(points, x1, y1, x2, y2, depth)
    generate_koch_points(points, x2, y2, x3, y3, depth)
    generate_koch_points(points, x3, y3, x1, y1, depth)

    # Add the first point again to close the loop
    points.extend([points[0], points[1]])

    return points


def display():
    """Display callback function"""
    # Clear the screen to white
    glClearColor(1.0, 1.0, 1.0, 1.0)
    glClear(GL_COLOR_BUFFER_BIT)

    # Draw filled snowflake
    glColor3f(0.0, 0.5, 1.0)  # Blue color
    glBegin(GL_TRIANGLE_FAN)
    for i in range(0, len(snowflake_points), 2):
        glVertex2f(snowflake_points[i], snowflake_points[i + 1])
    glEnd()

    # Draw outline
    glLineWidth(2.0)
    glColor3f(0.0, 0.3, 0.8)  # Darker blue
    glBegin(GL_LINE_LOOP)
    for i in range(0, len(snowflake_points), 2):
        glVertex2f(snowflake_points[i], snowflake_points[i + 1])
    glEnd()

    glutSwapBuffers()


def keyboard(key, x, y):
    """Keyboard callback function"""
    global depth, snowflake_points

    key = key.decode('utf-8') if isinstance(key, bytes) else key

    if key == '\x1b':  # ESC key
        sys.exit(0)
    elif key in '012345':
        depth = int(key)
        snowflake_points = generate_koch_snowflake(depth)
        glutPostRedisplay()


def reshape(width, height):
    """Reshape callback function"""
    glViewport(0, 0, width, height)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()

    # Keep aspect ratio square
    if width <= height:
        glOrtho(-2.0, 2.0, -2.0 * height / width,
                2.0 * height / width, -10.0, 10.0)
    else:
        glOrtho(-2.0 * width / height,
                2.0 * width / height, -2.0, 2.0, -10.0, 10.0)

    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()


def main():
    global snowflake_points

    # Initialize GLUT
    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB)
    glutInitWindowSize(800, 800)
    glutCreateWindow(b"Koch Snowflake")

    # Register callbacks
    glutDisplayFunc(display)
    glutKeyboardFunc(keyboard)
    glutReshapeFunc(reshape)

    # Generate initial snowflake points
    snowflake_points = generate_koch_snowflake(depth)

    # Main loop
    glutMainLoop()


if __name__ == "__main__":
    import sys

    main()