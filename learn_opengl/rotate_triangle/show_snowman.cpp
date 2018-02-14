#include <math.h>
#include <GL/glut.h>
#include <stdlib.h>

static float angle = 0.0, ratio;
static float x = 0.0f, y = 1.75f, z = 5.0f;
static float lx = 0.0f, ly = 0.0f, lz = -1.0f;
static GLint snowman_display_list;
float red = 1.0;
float green = 1.0;
float blue = 1.0;

void changeSize(int w, int h)
{

    //     防止被0除.
    if (h == 0)
        h = 1;

    ratio = 1.0f * w / h;
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //    设置视口为整个窗口大小
    glViewport(0, 0, w, h);

    //    设置可视空间
    gluPerspective(45, ratio, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x, y, z,
        x + lx, y + ly, z + lz,
        0.0f, 1.0f, 0.0f);
}

void drawSnowMan() {

    glColor3f(red, green, blue);

    //    画身体
    glTranslatef(0.0f, 0.75f, 0.0f);
    glutSolidSphere(0.75f, 20, 20);


    //     画头
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.25f, 20, 20);

    //     画眼睛
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslatef(0.05f, 0.10f, 0.18f);
    glutSolidSphere(0.05f, 10, 10);
    glTranslatef(-0.1f, 0.0f, 0.0f);
    glutSolidSphere(0.05f, 10, 10);
    glPopMatrix();

    //     画鼻子
    glColor3f(1.0f, 0.5f, 0.5f);
    glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.08f, 0.5f, 10, 2);
}

GLuint createDL() {
    GLuint snowManDL;

    //    生成一个显示列表号
    snowManDL = glGenLists(1);

    //     开始显示列表
    glNewList(snowManDL, GL_COMPILE);

    // call the function that contains 
    // the rendering commands
    drawSnowMan();

    // endList
    glEndList();

    return(snowManDL);
}

void initScene() {
    glEnable(GL_DEPTH_TEST);
    snowman_display_list = createDL();
}

void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //    画了一个地面

    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();

    //    画了36个雪人

    for (int i = -3; i < 3; i++)
    for (int j = -3; j < 3; j++) {
        glPushMatrix();
        glTranslatef(i*10.0, 0, j * 10.0);
        glCallList(snowman_display_list);;
        glPopMatrix();
    }
    glutSwapBuffers();
}

void orientMe(float ang)
{
    lx = sin(ang);
    lz = -cos(ang);
    glLoadIdentity();
    gluLookAt(x, y, z,
        x + lx, y + ly, z + lz,
        0.0f, 1.0f, 0.0f);
}

void moveMeFlat(int direction) {
    x = x + direction*(lx)*0.1;
    z = z + direction*(lz)*0.1;
    glLoadIdentity();
    gluLookAt(x, y, z,
        x + lx, y + ly, z + lz,
        0.0f, 1.0f, 0.0f);
}

void inputKey(int key, int x, int y) {

    switch (key) {
    case GLUT_KEY_LEFT:
        angle -= 0.01f;
        orientMe(angle); break;
    case GLUT_KEY_RIGHT:
        angle += 0.01f;
        orientMe(angle); break;
    case GLUT_KEY_UP:
        moveMeFlat(1); break;
    case GLUT_KEY_DOWN:
        moveMeFlat(-1); break;
    }
}

void normalKey(unsigned char key, int x, int y)
{
    if (key == 'q' || key == 27)
        exit(0);
}

#pragma region add memu to window
#define RED 1
#define GREEN 2
#define BLUE 3
#define WHITE 4

void processMenuEvents(int option) {
    //option    ，就是传递过来的value的值。
        switch (option) {
        case RED:
            red = 1.0;
            green = 0.0;
            blue = 0.0; break;
        case GREEN:
            red = 0.0;
            green = 1.0;
            blue = 0.0; break;
        case BLUE:
            red = 0.0;
            green = 0.0;
            blue = 1.0; break;
        case WHITE:
            red = 1.0;
            green = 1.0;
            blue = 1.0; break;
    }
}

void createGLUTMenus() {

    int menu;

    //     创建菜单并告诉GLUT，processMenuEvents处理菜单事件。
        menu = glutCreateMenu(processMenuEvents);

    //    给菜单增加条目
        glutAddMenuEntry("Red", RED);
    glutAddMenuEntry("Blue", BLUE);
    glutAddMenuEntry("Green", GREEN);
    glutAddMenuEntry("White", WHITE);

    //     把菜单和鼠标右键关联起来。
        glutAttachMenu(GLUT_RIGHT_BUTTON);
}

#pragma endregion

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(640, 360);
    glutCreateWindow("SnowMen from 3D-Tech");

    initScene();

    glutKeyboardFunc(normalKey);
    glutSpecialFunc(inputKey);

    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);

    glutReshapeFunc(changeSize);

    createGLUTMenus();

    glutMainLoop();

    return(0);
}