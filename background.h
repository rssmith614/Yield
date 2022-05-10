#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

class Background : public QOpenGLWidget
{
    Q_OBJECT
public:
    Background(QWidget* parent);

    void init(/*qreal roadStart, qreal roadWidth*/);

protected:
    void initializeGL();
    void paintGL();

private:
    QOpenGLFunctions *openGLFunctions;

    qreal roadWidth;
    qreal roadX, roadY;
    qreal pad;

    static qreal lineLength;
    static qreal lineGap;
};

#endif // BACKGROUND_H
