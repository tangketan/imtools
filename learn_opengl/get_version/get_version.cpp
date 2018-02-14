#include<Windows.h>  
#include <stdio.h>  
#include <GL/glut.h>  

// show a white triangle
void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.5, -0.5, 0.0);
    glVertex3f(0.5, 0.0, 0.0);
    glVertex3f(0.0, 0.5, 0.0);
    glEnd();
    glFlush();
}

void changeSize(int w, int h) {

    //     ��ֹ�������߶�Ϊ0
    // (        ��������ô��ڿ��Ϊ0).
    if (h == 0)
        h = 1;

    float ratio = 1.0* w / h;

    //     ��λ��ͶӰ����
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //     �����ӿڴ�СΪ�������ڴ�С
    glViewport(0, 0, w, h);

    //     ������ȷ��ͶӰ����
    gluPerspective(45, ratio, 1, 1000);
    //    ����������ģ����ͼ����
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, -1.0, 0.0f, 1.0f, 0.0f);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Version");


#pragma region show version string
    const GLubyte* name = glGetString(GL_VENDOR); //���ظ���ǰOpenGLʵ�ֳ��̵�����  
    const GLubyte* biaoshifu = glGetString(GL_RENDERER); //����һ����Ⱦ����ʶ����ͨ���Ǹ�Ӳ��ƽ̨  
    const GLubyte* OpenGLVersion = glGetString(GL_VERSION); //���ص�ǰOpenGLʵ�ֵİ汾��  
    const GLubyte* gluVersion = gluGetString(GLU_VERSION); //���ص�ǰGLU���߿�汾  

    printf("OpenGLʵ�ֳ��̵����֣�%s\n", name);
    printf("��Ⱦ����ʶ����%s\n", biaoshifu);
    printf("OpenGLʵ�ֵİ汾�ţ�%s\n", OpenGLVersion);
    printf("OGLU���߿�汾��%s\n", gluVersion);
#pragma endregion show version string

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutMainLoop();

    return 0;
}