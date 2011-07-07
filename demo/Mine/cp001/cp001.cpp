#include <GL/glut.h> 
#include <stdlib.h> 
#include <stdio.h>

#define SCREEN_W 640
#define SCREEN_H 480

// Keep track of windows changing width and height
GLfloat windowWidth;
GLfloat windowHeight;

void drawText(GLfloat x, GLfloat y, const char* text){
    
    for(const char* c = text; *c != '\0'; c++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

int frame=0;
int step=1;
bool step_init=true;
int step_len=0;
#define STEP_TO(cond, to) \
        if (cond){\
            step=to;\
            frame=0;\
            step_init=true;\
        }
#define STEP_NEXT(cond) \
        STEP_TO(cond, step+1);

GLfloat progress = 0.0f;

#define DOT(x, y)\
        glVertex2f(x, -y);
#define LINE(x1, y1, x2, y2)\
        glBegin(GL_LINE_STRIP); DOT(x1, y1); DOT(x2, y2); glEnd();
#define RECT_FILL(x1, y1, x2, y2)\
        glRectf(x1, -y1, x2, -y2);
#define RECT(x1, y1, x2, y2)\
        glBegin(GL_LINE_STRIP);\
            DOT(x1, y1); DOT(x1, y2); DOT(x2, y2); DOT(x2, y1); DOT(x1, y1);\
        glEnd();
#define PROGRESS_BAR(x1, y1, x2, y2, pct)\
        RECT(x1, y1, x2, y2);\
        if (pct>1.0f) {RECT_FILL(x1+1, y1-1, x1+(x2-x1-1)*1.0f, y2+1);}\
        else {RECT_FILL(x1+1, y1-1, x1+(x2-x1-1)*pct, y2+1);}
#define TEXT(x, y, text)\
        {\
            int line=0;\
            glRasterPos2f(x, -y);\
            for(const char* c = text; *c != '\0'; c++){\
                if (*c=='\n'){\
                    line++;\
                    glRasterPos2f(x, -y-9*line);\
                }\
                else glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);\
            }\
        }
#define TEXT2(x, y, text, from, to)\
        {\
            int line=0;\
            glRasterPos2f(x, -y);\
            for(int txt_i=from; txt_i<=to; txt_i++){\
                if (text[txt_i]=='\n'){\
                    line++;\
                    glRasterPos2f(x, -y-9*line);\
                }\
                else glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[txt_i]);\
            }\
        }

int textLen(const char* text){
    int len=0;
    for(const char* c = text; *c != '\0'; c++) len++;
    return len;
}

const char* strs[] = {"Hello, World!",
                "This is my first demoscene,\n"
                "which uses state-machine.",
                "I hope it isn't my last production,\n"
                "and I hope I will do things better.",
                "I eXerigumo Clanjor has set up\n"
                "a demoscene group called Clanjor Productions.",
                "We will make more amazing demoscenes soon...",
                "Code\n"
                "    CjXgm\n"
                "Design\n"
                "    CjXgm",
                "         Present\n"
                "Clanjor Productions",
                "           Copyleft\n"
                "eXerigumo Clanjor, 2010-2011.",
                "$"};
const char* text="";
int text_step=0;
// Called to draw scene
void RenderScene(void)
{
    float rate=0;
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    switch(step){
        case 1:
            glColor3f(1.0f, 0.0f, 0.0f);
            TEXT(-windowWidth/2,-8,"Loading...");
            glPushMatrix();
                PROGRESS_BAR(-windowWidth/2, -5, windowWidth/2, 5, progress);
            glPopMatrix();
            TEXT(windowWidth/4-windowWidth/2*progress,12,"A Clanjor Production");
            STEP_NEXT(progress>=1);
            break;
        case 2:
            glColor3f(1.0f, 0.0f, 0.0f);
            TEXT(-windowWidth/2,-8,"Done.");
            if(frame%2==0){
                glPushMatrix();
                    PROGRESS_BAR(-windowWidth/2, -5, windowWidth/2, 5, 1.0f);
                glPopMatrix();
            }
            glColor3f(1.0f, frame/12.0f, frame/12.0f);
            TEXT(windowWidth/4-windowWidth/2-frame/12.0f*windowWidth/10,12,"A Clanjor Production");
            progress=0;
            STEP_NEXT(frame>=12);
            break;
        case 3:
            if(step_init){
                step_len = 20;
                step_init = false;
            }
            rate = frame/(float)step_len;
            glColor3f(1.0f - rate, 0.0f, 0.0f);
            glPushMatrix();
                RECT_FILL(-windowWidth/2-windowWidth/2*rate, 
                          -5+(windowHeight-5)*rate, 
                          windowWidth/2+windowWidth/2*rate, 
                          5-(windowHeight-5)*rate);
            glPopMatrix();
            STEP_NEXT(frame>=step_len);
            break;
        case 4:
            if(step_init){
                step_len = 14*4;
                text = strs[text_step];
                RECT_FILL(-windowWidth, -windowHeight, windowWidth, windowHeight);
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
                glColor3f(1.0f, 0.0f, 0.0f);
                step_init = false;
            }
            rate = frame/(float)step_len;
            for (int i=0; i<(textLen(text)+1)*rate*4; i++){
                TEXT2(-windowWidth/2, 20+frame/5.0f, text, 0, i/4);
            }
            STEP_NEXT(frame>=step_len);
            break;
        case 5:
            if(step_init){
                step_len = 20;
                step_init = false;
            }
            rate = frame/(float)step_len;
            glColor3f(1.0f, rate, rate);
            TEXT(-windowWidth/2, 20+14*4/5.0f+frame/5.0f, text);
            STEP_NEXT(frame>=step_len);
            break;
        case 6:
            if(step_init){
                step_len = 80;
                step_init = false;
            }
            rate = frame/(float)step_len;
            glColor3f(1.0f - rate, 1.0f - rate, 1.0f - rate);
            TEXT(-windowWidth/2, 20+14*4/5.0f+4+frame/5.0f, text);
            STEP_NEXT(frame>=step_len);
            break;
        case 7:
            text_step++;
            if (strs[text_step]=="$"){
                STEP_NEXT(true);
                break;
            }
            STEP_TO(true, 4);
            break;
        case 8:
           if(step_init){
                step_len = 20;
                step_init = false;
            }
            rate = frame/(float)step_len;
            glColor3f(rate, rate, rate);
            glPushMatrix();
                RECT_FILL(-windowWidth, 
                          -windowHeight, 
                          windowWidth, 
                          windowHeight);
            glPopMatrix();
            STEP_NEXT(frame>=step_len);
            break;
        case 9:
            RECT_FILL(-windowWidth, 
              -windowHeight, 
              windowWidth, 
              windowHeight);
            STEP_NEXT(frame>=20);
            break;
        default:
            exit(0);
            break;
    };
    glutSwapBuffers();
}

// Called by GLUT library when idle (window not being
// resized or moved)
void TimerFunction(int value)
{
    frame++;
    progress += 0.015f;
    // Redraw the scene with new coordinates
    glutPostRedisplay();
    glutTimerFunc(33, TimerFunction, 1);
}

// Setup the rendering state
void SetupRC(void)
{
    // Set clear color to blue
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

// Called by GLUT library when the window has chanaged size
void ChangeSize(int w, int h)
{
    GLfloat aspectRatio;

    // Prevent a divide by zero
    if(h == 0)
        h = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Establish clipping volume (left, right, bottom, top, near, far)
    aspectRatio = (GLfloat)w / (GLfloat)h;
    if (w <= h) 
    {
        windowWidth = 100;
        windowHeight = 100 / aspectRatio;
        glOrtho (-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
    }
    else 
    {
        windowWidth = 100 * aspectRatio;
        windowHeight = 100;
        glOrtho (-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Main program entry point
int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(SCREEN_W, SCREEN_H);
    glutCreateWindow("LanDeXithO Production");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(33, TimerFunction, 1);

    SetupRC();

    glutMainLoop();

    return 0;
}
