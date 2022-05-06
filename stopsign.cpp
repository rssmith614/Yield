#include "stopsign.h"

StopSign::StopSign(QWidget* parent) : QOpenGLWidget(parent)
{
    stopped = true;
    change = 1; // transparency changes when stop sign is inactive
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

    // define vertices of an octagon
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

    toggle();
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
        change = 0.5;
    }
    update();
}

void StopSign::toggle()
{
    stopped = !stopped;
    if(stopped) {
        change = 1;
    } else {
        change = 0.5;
    }
    update();
}
