#include "stopsign.h"
#include "verticalroad.h"
#include "mainwindow.h"

StopSign::StopSign(QWidget* parent) : QOpenGLWidget(parent)
{
    stopped = true;
    change = 1;
}

void StopSign::initializeGL()
{
    openGLFunctions = QOpenGLContext::currentContext()->functions();
}

void StopSign::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setAttribute(Qt::WA_AlwaysStackOnTop);
    glClearColor(0.0,0.0,0.0,0.0);
    glLoadIdentity();


    glBegin(GL_POLYGON);
        glColor4f(1,0,0,change);
        glVertex2f(-1,0.5);
        glVertex2f(-0.5,1);
        glVertex2f(0.5,1);
        glVertex2f(1,0.5);
        glVertex2f(1,-0.5);
        glVertex2f(0.5,-1);
        glVertex2f(-0.5,-1);
        glVertex2f(-1,-0.5);

    glEnd();
}

void StopSign::mousePressEvent(QMouseEvent *parent)
{
    Q_UNUSED(parent);

    stopped = !stopped;
    if(stopped) {
        change = 1;
    } else {
        change = 0.7;
    }
    update();
}

bool StopSign::isClicked()
{
    return stopped;
}

void StopSign::set(bool active)
{
    stopped = active;
    if(stopped) {
        change = 1;
    } else {
        change = 0.7;
    }
    update();
}
