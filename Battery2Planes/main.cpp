/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */
#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include"plane.h"
#include"planeflock.h"
#include"config.h"

using namespace std;


#define MAXTIMERS    50    //最大时钟数
#define PI   3.1416f
#define CIRSLICE 30
int winHeight;
int winWidth;
PlaneFlock Mission1;
int sign=0;
int frameNum=0;
/* GLUT callback Handlers */
int  GameState=0; //0:游戏正在进行，1:炮台赢,2:飞机赢

static void resize(int width, int height);
int getFPS();
void drawString(const char* str,int x,int y);
static void drawing();
static void WinOrLost();
static void display(void);
static void key(unsigned char key, int x, int y);
static void idle(void);
void DrawCircle(float x, float y, float r);

static void tmrProduceHeavyPlanes(int value); //重型飞机
static void tmrProduceLightPlanes(int value); //轻型飞机
static void tmrProduceRedTriPlanes(int value);//三倍速飞机

TimerNode timerList[MAXTIMERS];
int ntn=0;  //时钟数量
Pos2D airport[MAXTIMERS];
int nap=0;

void DrawAirport(float r)
{
    for(int i=0; i < nap; i++)
    {
        DrawCircle(airport[i].x,airport[i].y,0.05f);
    }
}
void DrawCircle(float x, float y, float r)
{
    static float f=0.0f;
    f+=0.01f;
    int i;
    float fscale = (float)winWidth / winHeight;
    glColor3f(1.0f,0.0f,0.0f);
    glBegin(GL_LINES);
    for(int i=0;i<CIRSLICE;i++)
        glVertex2f(x+r*cos(2*PI/CIRSLICE*i+f),y+r*sin(2*PI/CIRSLICE*i+f)*fscale);
    glEnd();

//    glPointSize(3);
//    glBegin(GL_POINTS);
//    glVertex2f(ctPos.x,ctPos.y);
//    glEnd();

}

void InitTimers()
{
    //LoadConfig("E:\\UniqueStudioTest\\Battery2Planes\\config.txt");
    ntn = ConfigFileParser("E:\\UniqueStudioTest\\Battery2PlanesV2\\Battery2Planes\\config.txt",timerList,airport,&nap,MAXTIMERS);

    for(int i=0;i<ntn;i++)
    {
        timerList[i].id = i;
        switch(timerList[i].pt){
            case HPT:
                glutTimerFunc(timerList[i].millis,tmrProduceHeavyPlanes,timerList[i].id);
                break;
            case LPT:
                glutTimerFunc(timerList[i].millis,tmrProduceLightPlanes,timerList[i].id);
                break;
            case RPT:
                glutTimerFunc(timerList[i].millis,tmrProduceRedTriPlanes,timerList[i].id);
                break;
        }
    }
}
static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;
    winHeight=height;
    winWidth=width;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}
int getFPS()
{
    static int FPS = 0;
    static int lastFPS=0;
    static int tempFPStime=0;

     FPS++;

     int nowtime = GetTickCount();
     if (nowtime - tempFPStime >= 1000){
          lastFPS = FPS;
          tempFPStime = nowtime;
          FPS = 0;
     }
    return lastFPS;
}

void selectFont(int size, int charset, const char* face) {
    HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
        charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
    HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
    DeleteObject(hOldFont);
}
void drawString(const char* str,int x,int y) {
    static int isFirstCall = 1;
    static GLuint lists;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, winWidth-1, 0, winHeight-1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    //从这里开始处理二维画面
      selectFont(48, ANSI_CHARSET, "Comic Sans MS");  //字体可以在系统文件里寻找
      glColor3f(1.0f, 0.0f, 0.0f);
      glRasterPos2i(x, winHeight-y); //x与y是屏幕坐标，左下角为(0,0)

    if( isFirstCall ) { // 如果是第一次调用，执行初始化
                         // 为每一个ASCII字符产生一个显示列表
         isFirstCall = 0;

         // 申请MAX_CHAR个连续的显示列表编号
         lists = glGenLists(MAXCHAR);

         // 把每个字符的绘制命令都装到对应的显示列表中
         wglUseFontBitmaps(wglGetCurrentDC(), 0, MAXCHAR, lists);
     }
     // 调用每个字符对应的显示列表，绘制每个字符
    for(; *str!='\0'; ++str)
         glCallList(lists + *str);

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}
static void drawing()
{

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    //gluOrtho2D(0, winWidth-1, 0, winHeight-1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();


    glColor3f(1.0f,1.0f,1.0f);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.8, -0.8); //点P1,左下角
    glVertex2f(-0.8, +0.8);  //点P2,左上角
    glVertex2f(+0.8, +0.8);   //点P3,右上角
    glVertex2f(+0.8, -0.8); //点P4,右下角
    glEnd();

    // Mission1.UpdatePosition();
    glColor3f(1.0f,0.0f,0.0f);
    glPointSize(10);
    glBegin(GL_POINTS);
    glVertex2f(-0.803,0);
    glEnd();
    glColor3f(1.0f,1.0f,1.0f);
    glPointSize(2);
    Mission1.planesDisplay();


    DrawAirport(0.1f);

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

static void WinOrLost()
{


     static long stTime = GetTickCount();
     static long remTime=0;

     if(GameState==0)
     {
         remTime = 40000L - GetTickCount() + stTime;
         if(Mission1.PlaneWin())
             GameState = 2;
         else if(remTime<0)
             GameState = 1;
     }


     char str[30];
     sprintf(str,"Time Remaining:%.1fs",remTime/1000.0f);
     drawString(str,10,40);

     if(GameState==2)
     {
        drawString("Plane Win!!!!",10,80);
     }
     if(GameState==1)
     {
         drawString("Emplacement Win!!!!",10,80);
     }


}
static void display(void)
{
    //const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    //const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawing();


    drawString("FPS:",winWidth-150,40);
    drawString("Keyboard: Laser:l Emp:e Lowspeed:s",winWidth/2-300,620);
    WinOrLost();
    int in;
    in=getFPS();
    //printf("%d %d %d\n",in,winHeight,winWidth);
    char out[MAXCHAR];
    itoa(in,out,10);
    drawString(out,winWidth-80,40);
    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
        case 'Q':
            exit(0);
            break;

        case 'l':

        case 'L':
            Mission1.LaserShoot();
            break;

        case 'e':
        case 'E':
            Mission1.EMPShoot();
            break;
        case 's':
        case 'S':
            Mission1.lowSpeedAttack(GetTickCount());
            break;
    }
    glutPostRedisplay();
}

static void idle(void)
{
    static TICK lastTime = 0;
    TICK tempTime = GetTickCount();
    Mission1.recoverSpeed(tempTime);
    Mission1.UpdatePosition(tempTime - lastTime);
    lastTime=tempTime;
    Mission1.RemoveDeadPlanes();


    glutPostRedisplay();
}


/*static void tmrLaserAttack(int value) //激光攻击，0.5秒1次
{
    Mission1.LaserShoot();
    glutTimerFunc(500, tmrLaserAttack, value);
}
static void tmrEMPAttack(int value) //EMP攻击，2秒1次
{
    //Mission1.PrintPlanes();
    Mission1.EMPShoot();
    //Mission1.PrintPlanes();
    //Mission1.RemoveDeadPlanes();
    //Mission1.PrintPlanes();
    glutTimerFunc(2000, tmrEMPAttack, value);
}*/

/*static void tmrLowSpeedAttack(int value)//减速弹攻击，5秒一次
{
    Mission1.lowSpeedAttack();
    glutTimerFunc(5000,tmrLowSpeedAttack,value);
}*/

/*const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };*/

/* Program entry point */

int main(int argc, char *argv[])
{
        //while(frameNum<=2400){
        glutInit(&argc, argv);
        glutInitWindowSize(1024,640);
        glutInitWindowPosition(10,10);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

        glutCreateWindow("Battle Field");

        glutReshapeFunc(resize);
        glutDisplayFunc(display);
        glutKeyboardFunc(key);

    glutIdleFunc(idle);

    glClearColor(0,0,0,1);
    /*glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);*/
    //glutTimerFunc(2000,tmrProduceHeavyPlanes,1);
    //glutTimerFunc(1000,tmrProduceLightPlanes,2);
    //glutTimerFunc(5000,tmrProduceRedTriPlanes,3);
    InitTimers();//注册事件
    //glutTimerFunc(500,tmrLaserAttack,3);//0.5秒1次
    //glutTimerFunc(2000,tmrEMPAttack,4);//2秒1次
    //glutTimerFunc(5000,tmrLowSpeedAttack,5);//5秒1次
    glutMainLoop();
    return EXIT_SUCCESS;
    //}
    //if (sign == 1)
        //printf("Planes Win!!!\n");
        //else printf("Emplacements Win!!!\n");
    //return 0;
}
static void tmrProduceHeavyPlanes(int value) //生成重型飞机
{
    //Mission1.addHeavyPlane(1);
    //glutTimerFunc(2000, tmrProduceHeavyPlanes, value);
    Mission1.addHeavyPlane(timerList[value].nplane,timerList[value].x,timerList[value].y,timerList[value].vel);
    glutTimerFunc(timerList[value].millis,tmrProduceHeavyPlanes,value);
}
static void tmrProduceLightPlanes(int value) //生成轻型飞机
{
    //Mission1.addLightPlane(3);
    //glutTimerFunc(1000, tmrProduceLightPlanes, value);
    Mission1.addLightPlane(timerList[value].nplane,timerList[value].x,timerList[value].y,timerList[value].vel);
    glutTimerFunc(timerList[value].millis,tmrProduceLightPlanes,value);
}
static void tmrProduceRedTriPlanes(int value) //生成三角飞机
{
    //Mission1.addRedTriPlane(1);
    //glutTimerFunc(timerList[value]->millis,tmrProduceRedTriPlanes,value);
    Mission1.addRedTriPlane(timerList[value].nplane,timerList[value].x,timerList[value].y,timerList[value].vel);
    glutTimerFunc(timerList[value].millis,tmrProduceRedTriPlanes,value);


}
