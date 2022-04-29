#ifndef STOPSIGN_H
#define STOPSIGN_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class StopSign : public QOpenGLWidget
{
    Q_OBJECT

public:
    StopSign(QWidget* parent=nullptr);
    void mousePressEvent(QMouseEvent* parent=nullptr);
    bool isClicked();

    void set(bool active);

protected:
    void paintGL();
    void initializeGL();

private:
    QOpenGLFunctions *openGLFunctions;
    bool stopped;
    float change;
};

#endif // STOPSIGN_H
