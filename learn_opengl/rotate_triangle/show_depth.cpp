//
//Huang,Haiqiao coded on Dec.2009
#include <iostream>
#include <stdlib.h>
#include "opencv/cv.h"
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glut.h>  
using namespace std;
float imgdata[500][500];
int w = 0;
int h = 0;
int xx = 0, yy = 0;
float angle_x = 0;
float angle_y = 180;

int mouseState = 0;
cv::Point mouseLastClick;

float scalar = 50;//scalar of converting pixel color to float coordinates
void renderScene(void) {

    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();// Reset the coordinate system before modifying
    gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    //glTranslated(-2.50, -2.50, 0);
    glRotatef(-180, 0.0, 0.0, 1.0); //rotate about the z axis
    glRotatef(angle_y, 0.0, 1.0, 0.0); //rotate about the y axis
    glRotatef(angle_x, 1.0, 0.0, 0.0); //rotate about the x axis

    float imageCenterX = w*.5;
    float imageCenterY = h*.5;
    float x, y, z;
    glPointSize(1.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            // color interpolation
            glColor3f(1 - imgdata[i][j] / 255, imgdata[i][j] / 255, imgdata[i][j] / 255);
            x = ((float)j - imageCenterX) / scalar;
            y = ((float)i - imageCenterY) / scalar;
            z = imgdata[i][j] / scalar;
            glVertex3f(x, y, z);
        }
    }
    glEnd();
    glFlush();
    glutSwapBuffers();
}
void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}
void displayDisparity(IplImage* disparity){
    double xyscale = 100;
    int j = 0;
    int i = 0;
    CvScalar s;

    //accessing the image pixels
    for (i = 0; i < h; i++){
        for (j = 0; j < w; j++){
            s = cvGet2D(disparity, i, j);
            imgdata[i][j] = s.val[0];//for disparity is a grey image.
        }
    }
}

void onMouseClick(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        switch (button)
        {
        case GLUT_LEFT_BUTTON: mouseState = 1; break;
        case GLUT_MIDDLE_BUTTON: mouseState = 3; break;
        case GLUT_RIGHT_BUTTON: mouseState = 2; break;
        default: break;
        }
        mouseLastClick.x = x;
        mouseLastClick.y = y;

        glutSetCursor(GLUT_CURSOR_CYCLE);
    }
    else if (state == GLUT_UP)
    {
        mouseState = 0;
        glutSetCursor(GLUT_CURSOR_INHERIT);
    }
}

static inline cv::Mat createRotation(const cv::Vec3f & _axis, float angle)
{
    cv::Vec3f axis = cv::normalize(_axis);
    float si = sinf(angle);
    float co = cosf(angle);

    cv::Mat ret = cv::Mat::ones(3, 3, CV_32FC1);

    ret *= co;
    for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) ret.at<float>(c, r) += (1.0f - co) * axis[c] * axis[r];

    cv::Mat skewmat = cv::Mat::zeros(3, 3, CV_32FC1);
    skewmat.at<float>(1, 0) = -axis[2];
    skewmat.at<float>(0, 1) = axis[2];
    skewmat.at<float>(2, 0) = axis[1];
    skewmat.at<float>(0, 2) = -axis[1];
    skewmat.at<float>(2, 1) = axis[0];
    skewmat.at<float>(1, 2) = -axis[0];
    skewmat *= si;
    ret += skewmat;

    return ret;
}

void onMouseMove(int x, int y)
{
    if (mouseState == 0) return;

    cv::Point movement;
    movement.x = x - mouseLastClick.x;
    movement.y = y - mouseLastClick.y;

    mouseLastClick.x = x;
    mouseLastClick.y = y;

    if ((movement.x == 0) && (movement.y == 0)) return;

    static const float scale_rotation = 0.005f;
    static const float scale_translation = 0.0025f;

    //if (xx != 0 && x!=xx){
    //    angle_x = atan((x-xx) / xx)*180/M_PI;
    //    printf("x: %d\n", x);
    //}
    //if (yy != 0 && y!=yy){
    //    angle_y = atan((y - yy) / yy) * 180 / M_PI;
    //    printf("y: %d\n", y);
    //}
}

void onNormalKeyboard(unsigned char key, int x, int y)
{
    if (key == 'q' || key == 27)
        exit(0);
    else if (key == 'r')
    {
        angle_y = -180;
        angle_x = 0;
        glutPostRedisplay();
    }
}

int main(int argc, char **argv)
{
    char* filename = argv[1];
    IplImage* imgGrey = cvLoadImage(filename, 0); //read image as a grey one
    if (imgGrey == NULL){
        cout << "No valid image input." << endl;
        char c = getchar();
        return 1;
    }
    w = imgGrey->width;
    h = imgGrey->height;
    displayDisparity(imgGrey);
    cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
    cvShowImage("original", imgGrey);
    //------------------OpenGL-------------------------
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);
    glutCreateWindow("3D disparity image");
    
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);

    glutReshapeFunc(reshape);

    glutKeyboardFunc(onNormalKeyboard);
    
    // mouse callback to control scene rotation
    glutMouseFunc(onMouseClick);
    glutMotionFunc(onMouseMove);

    glutMainLoop();

    cvWaitKey(0);
    //release opencv stuff.
    cvReleaseImage(&imgGrey);
    cvDestroyWindow("Original");

    return 0;
}
