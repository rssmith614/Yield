#include "background.h"

qreal Background::lineLength = 0.1;
qreal Background::lineGap = 0.25;

Background::Background(QWidget* parent) : QOpenGLWidget(parent)
{

}

void Background::init(qreal roadStart, qreal roadWidth) {
    roadX = roadY = (roadStart/this->geometry().width())*2 - 1;

    this->roadWidth = roadWidth/this->geometry().width()*2;

    pad = 10./this->geometry().width()*2;
}

void Background::initializeGL() {
    openGLFunctions = QOpenGLContext::currentContext()->functions();
    glClearColor(33/255.,140/255.,33/255.,1); // green
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
