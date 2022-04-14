#include "background.h"

qreal Background::lineLength = 0.1;
qreal Background::lineGap = 0.25;

Background::Background(QWidget* parent) : QOpenGLWidget(parent)
{

}

void Background::init(qreal roadStart, qreal roadWidth) {
    roadX = roadY = (roadStart/this->geometry().width())*2 - 1;
//    qDebug() << road_x;
    this->roadWidth = roadWidth/this->geometry().width()*2;
//    qDebug() << this->roadWidth;
    pad = 10./this->geometry().width()*2;
//    qDebug() << pad;
}

void Background::initializeGL() {
    openGLFunctions = QOpenGLContext::currentContext()->functions();
    glClearColor(0.13,0.55,0.13,1);


}

void Background::paintGL() {
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

    glLineWidth(2);
    for (int i=0; (-1+lineGap) + ((lineGap+lineLength)*i) < 1; i++) {
//        qDebug() << "drawing horizontal line #" << i;
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
