#include "background.h"

qreal Background::lineLength = 0.1;
qreal Background::lineGap = 0.25;

Background::Background(QWidget* parent) : QOpenGLWidget(parent)
{

}

void Background::init() {

    roadX = roadY = (255./600)*2 - 1;

    roadWidth = 40./600*2;

    pad = 10./this->geometry().width()*2;
}

void Background::initializeGL() {
    openGLFunctions = QOpenGLContext::currentContext()->functions();
    glClearColor(0.13,0.55,0.13,1); // green
}

void Background::paintGL() {
    // draw roads black
    glBegin(GL_QUADS);
        glColor3f(0,0,0);
        glVertex2f(-1,roadY-pad);
        glVertex2f(-1,roadY+(roadWidth+pad)*2);
        glVertex2f(1,roadY+(roadWidth+pad)*2);
        glVertex2f(1,roadY-pad);
    glEnd();

    glBegin(GL_QUADS);
        glColor3f(0,0,0);
        glVertex2f(roadX-pad,1);
        glVertex2f(roadX-pad,-1);
        glVertex2f(roadX+(roadWidth+pad)*2,-1);
        glVertex2f(roadX+(roadWidth+pad)*2,1);
    glEnd();

    //sidewalks
    glBegin(GL_QUADS);
        glColor3f(0.75, 0.75, 0.75);
        glVertex2f(0.2,roadY-pad);
        glVertex2f(0.2,roadY+(roadWidth+pad)-.25);
        glVertex2f(1,roadY+(roadWidth+pad)-0.25);
        glVertex2f(1,roadY-pad);

    glEnd();

    glBegin(GL_QUADS);
        glColor3f(0.75, 0.75, 0.75);
        glVertex2f(0.2,roadY-pad+0.367);
        glVertex2f(0.2,roadY+(roadWidth+pad)+.22);
        glVertex2f(1,roadY+(roadWidth+pad)+0.22);
        glVertex2f(1,roadY-pad+0.367);

    glEnd();

    glBegin(GL_QUADS);
        glColor3f(0.75, 0.75, 0.75);
        glVertex2f(-0.2,roadY-pad);
        glVertex2f(-0.2,roadY+(roadWidth+pad)-.25);
        glVertex2f(-1,roadY+(roadWidth+pad)-0.25);
        glVertex2f(-1,roadY-pad);

    glEnd();

    glBegin(GL_QUADS);
        glColor3f(0.75, 0.75, 0.75);
        glVertex2f(-0.2,roadY-pad+0.367);
        glVertex2f(-0.2,roadY+(roadWidth+pad)+.22);
        glVertex2f(-1,roadY+(roadWidth+pad)+0.22);
        glVertex2f(-1,roadY-pad+0.367);

    glEnd();

    glBegin(GL_QUADS);
        glColor3f(0.75,0.75,0.75);
        glVertex2f(roadX-pad-0.06,1);
        glVertex2f(roadX-pad-0.06,0.183);
        glVertex2f(roadX+(roadWidth+pad)*2-0.367,0.183);
        glVertex2f(roadX+(roadWidth+pad)*2-0.367,1);
    glEnd();

    glBegin(GL_QUADS);
        glColor3f(0.75,0.75,0.75);
        glVertex2f(-(roadX-pad-0.06),1);
        glVertex2f(-(roadX-pad-0.06),0.183);
        glVertex2f(-(roadX+(roadWidth+pad)*2-0.367),0.183);
        glVertex2f(-(roadX+(roadWidth+pad)*2-0.367),1);
    glEnd();

    glBegin(GL_QUADS);
        glColor3f(0.75,0.75,0.75);
        glVertex2f(roadX-pad-0.06,-1);
        glVertex2f(roadX-pad-0.06,-0.183);
        glVertex2f(roadX+(roadWidth+pad)*2-0.367,-0.183);
        glVertex2f(roadX+(roadWidth+pad)*2-0.367,-1);
    glEnd();

    glBegin(GL_QUADS);
        glColor3f(0.75,0.75,0.75);
        glVertex2f(-(roadX-pad-0.06),-1);
        glVertex2f(-(roadX-pad-0.06),-0.183);
        glVertex2f(-(roadX+(roadWidth+pad)*2-0.367),-0.183);
        glVertex2f(-(roadX+(roadWidth+pad)*2-0.367),-1);
    glEnd();

    // draw road lines
    glLineWidth(2);
    for (int i=0; (-1+lineGap) + ((lineGap+lineLength)*i) < 1; i++) {
        glBegin(GL_LINES);
            glColor3f(1,1,1);
            glVertex2f((-1+lineGap) + ((lineGap+lineLength)*i) , 0);
            glVertex2f((-1+lineGap) + ((lineGap+lineLength)*i) + lineLength, 0);
        glEnd();
    }

    for (int i=0; (-1+lineGap) + ((lineGap+lineLength)*i) < 1; i++) {
        qreal currentLinePos = (-1+lineGap) + ((lineGap+lineLength)*i);
        if (currentLinePos > -(roadY+(roadWidth+pad)*2) && currentLinePos < (roadY+(roadWidth+pad)*2)) continue;
        glBegin(GL_LINES);
            glColor3f(1,1,1);
            glVertex2f(0, currentLinePos);
            glVertex2f(0, currentLinePos + lineLength);
        glEnd();
    }

    // draw limit lines
    glBegin(GL_LINES);
        glColor3f(1,1,1);
        glVertex2f(0, -(roadY+(roadWidth+pad)*2) - 0.005);
        glVertex2f(roadX+(roadWidth+pad)*2, -(roadY+(roadWidth+pad)*2) - 0.005);
    glEnd();

    glBegin(GL_LINES);
        glColor3f(1,1,1);
        glVertex2f(-(roadX+(roadWidth+pad)*2), (roadY+(roadWidth+pad)*2) + 0.005);
        glVertex2f(0, (roadY+(roadWidth+pad)*2) + 0.005);
    glEnd();

    glLineWidth(1);
}
