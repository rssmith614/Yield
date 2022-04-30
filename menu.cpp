#include "menu.h"
Menu::Menu(){

}
Menu::Menu(QWidget* parent) : QOpenGLWidget(parent)
{

}
//Menu::Menu(QWidget* parent) : QOpenGLWidget(parent)
//{

//}
void Menu::initializeGL() {
    openGLFunctions = QOpenGLContext::currentContext()->functions();
    glClearColor(0,0,0,0);
}

void Menu::paintGL(){
    setAttribute(Qt:: WA_AlwaysStackOnTop);

}
