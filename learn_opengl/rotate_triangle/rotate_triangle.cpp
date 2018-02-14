#include<Windows.h>  
#include <stdio.h>  
#include <GL/glut.h>  

// show a white triangle
float angle = 0.0;
float red = 1.0, blue = 1.0, green = 1.0;
void renderScene(void) {
    //    ע�����������������Ȼ�������

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    //    ���浱ǰģ����ͼ����

        glPushMatrix();
        
    glRotatef(angle, 0.0, 1.0, 0.0);

    // glColor3f    ���û��������ε���ɫ��
        glColor3f(red, green, blue);

    glBegin(GL_TRIANGLES);
    glVertex3f(-0.5, -0.5, 0.0);
    glVertex3f(0.5, 0.0, 0.0);
    glVertex3f(0.0, 0.5, 0.0);
    glEnd();



    //    ������ջ
        glPopMatrix();

    //     ����������
        glutSwapBuffers();

        //     ��angle�Զ����ӡ�
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
    const GLubyte* name = glGetString(GL_VENDOR); //���ظ���ǰOpenGLʵ�ֳ��̵�����  
    const GLubyte* biaoshifu = glGetString(GL_RENDERER); //����һ����Ⱦ����ʶ����ͨ���Ǹ�Ӳ��ƽ̨  
    const GLubyte* OpenGLVersion = glGetString(GL_VERSION); //���ص�ǰOpenGLʵ�ֵİ汾��  
    const GLubyte* gluVersion = gluGetString(GLU_VERSION); //���ص�ǰGLU���߿�汾  

    printf("OpenGLʵ�ֳ��̵����֣�%s\n", name);
    printf("��Ⱦ����ʶ����%s\n", biaoshifu);
    printf("OpenGLʵ�ֵİ汾�ţ�%s\n", OpenGLVersion);
    printf("OGLU���߿�汾��%s\n", gluVersion);
#pragma endregion show version string

    //     �����ó������ʱ����renderScene��
    glutIdleFunc(renderScene);

    glutReshapeFunc(changeSize);
    
    glutKeyboardFunc(processNormalKeys);

    glutSpecialFunc(processSpecialKeys);

    glutMainLoop();

    return 0;
}