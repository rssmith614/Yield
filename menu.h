#ifndef MENU_H
#define MENU_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
class Menu : public QOpenGLWidget
{
public:
    Menu();
    Menu(QWidget* parent);
//    Menu(QWidget *parent = nullptr);
//    ~Menu();

private:
    QOpenGLFunctions *openGLFunctions;
protected:
    void initializeGL();
    void paintGL();


};

#endif // MENU_H
