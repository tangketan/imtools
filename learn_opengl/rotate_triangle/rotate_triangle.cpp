#include<Windows.h>  
#include <stdio.h>  
#include <GL/glut.h>  

// show a white triangle
float angle = 0.0;
float red = 1.0, blue = 1.0, green = 1.0;
void renderScene(void) {
    //    注意我们这里清除了深度缓冲区。

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    //    保存当前模型视图矩阵。

        glPushMatrix();
        
    glRotatef(angle, 0.0, 1.0, 0.0);

    // glColor3f    设置绘制三角形的颜色。
        glColor3f(red, green, blue);

    glBegin(GL_TRIANGLES);
    glVertex3f(-0.5, -0.5, 0.0);
    glVertex3f(0.5, 0.0, 0.0);
    glVertex3f(0.0, 0.5, 0.0);
    glEnd();



    //    弹出堆栈
        glPopMatrix();

    //     交换缓冲区
        glutSwapBuffers();

        //     让angle自动增加。
                angle++;
                Sleep(10);

}

void processNormalKeys(unsigned char key, int x, int y) {

    if (key == 27 || key=='q')
        exit(0);
    else if (key == 'r') {
        int mod = glutGetModifiers();
        if (mod == GLUT_ACTIVE_ALT){
            red = 0.0;
            blue = 1.0;
            green = 1.0;
        }
        else{
            red = 1.0;
            blue = 0;
            green = 0;
        }
    }
}

void changeSize(int w, int h) {

    //     防止除数即高度为0
    // (        你可以设置窗口宽度为0).
    if (h == 0)
        h = 1;

    float ratio = 1.0* w / h;

    //     单位化投影矩阵。
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //     设置视口大小为增个窗口大小
    glViewport(0, 0, w, h);

    //     设置正确的投影矩阵
    gluPerspective(45, ratio, 1, 1000);
    //    下面是设置模型视图矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, -1.0, 0.0f, 1.0f, 0.0f);
}

void processSpecialKeys(int key, int x, int y) {

    switch (key) {
    case GLUT_KEY_F1:
        red = 1.0;
        green = 0.0;
        blue = 0.0; break;
    case GLUT_KEY_F2:
        red = 0.0;
        green = 1.0;
        blue = 0.0; break;
    case GLUT_KEY_F3:
        red = 0.0;
        green = 0.0;
        blue = 1.0; break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Version");


#pragma region show version string
    const GLubyte* name = glGetString(GL_VENDOR); //返回负责当前OpenGL实现厂商的名字  
    const GLubyte* biaoshifu = glGetString(GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台  
    const GLubyte* OpenGLVersion = glGetString(GL_VERSION); //返回当前OpenGL实现的版本号  
    const GLubyte* gluVersion = gluGetString(GLU_VERSION); //返回当前GLU工具库版本  

    printf("OpenGL实现厂商的名字：%s\n", name);
    printf("渲染器标识符：%s\n", biaoshifu);
    printf("OpenGL实现的版本号：%s\n", OpenGLVersion);
    printf("OGLU工具库版本：%s\n", gluVersion);
#pragma endregion show version string

    //     这里让程序空闲时调用renderScene，
    glutIdleFunc(renderScene);

    glutReshapeFunc(changeSize);
    
    glutKeyboardFunc(processNormalKeys);

    glutSpecialFunc(processSpecialKeys);

    glutMainLoop();

    return 0;
}