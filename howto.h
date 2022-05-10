#ifndef HOWTO_H
#define HOWTO_H

#include <QWidget>
#include <QPixmap>
#include <QGraphicsView>
#include <QImage>
#include <QDir>

namespace Ui {
class HowTo;
}

class HowTo : public QWidget
{
    Q_OBJECT

public:
    explicit HowTo(QWidget *parent = nullptr);
    ~HowTo();
public slots:
    void backToMenu();

private:
    Ui::HowTo *ui;
};

#endif // HOWTO_H
