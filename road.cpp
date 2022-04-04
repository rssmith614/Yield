#include "road.h"

Road::Road(QWidget* parent) : QOpenGLWidget(parent)
{
    currentCar = 0;

    timer = new QTimer(this);
    timer->start(30);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    // hard-coded traffic flow
    // if you put a number larger than 2, the traffic will basically just restart when
    // that car goes off screen (spatial gaps longer than the screen don't work with the current setup)
    // if we want the road to stay empty for a while we might have to use temporal gaps
    gaps = {0.1,0.5,0.1,0.5,0.1,0.5,0.1,0.5,0.1,0.5};
}

void Road::initializeGL() {
    openGLFunctions = QOpenGLContext::currentContext()->functions();
}

void Road::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // for every car currently on the road...
    for (size_t i=0; i < cars.size(); i++)  {
        // draw it
        drawCar(cars[i]);

        // if it's off-screen, free the pointer and pop it from the queue
        if (cars[i]->getPosition() > 1) {
            delete cars[i];
            cars.pop_back();
        }

    }

    // if the road isn't empty
    if (cars.size() > 0) {
        // if the newest car on the road is past a hard-coded position
        if (cars[0]->getPosition() > (-1 + gaps[currentCar]))
        {
            // we're good to add the next car to the road
            cars.insert(cars.begin(), createCar());
            currentCar++;
        }
        // restart the hard-coded traffic pattern
        if (currentCar == 10) currentCar = 0;
    } else {
        // add a car if the road is empty
        cars.insert(cars.begin(), createCar());
        currentCar++;
    }

}

void Road::drawCar(Car* car) {
    glBegin(GL_QUADS);
        glColor3f(car->getColor()->redF(), car->getColor()->greenF(), car->getColor()->blueF());
        glVertex2f(car->getPosition(), 1.0);
        glVertex2f(car->getPosition(), 1.0-Car::w);
        glVertex2f(car->getPosition()+Car::l, 1.0-Car::w);
        glVertex2f(car->getPosition()+Car::l, 1.0);
    glEnd();
}

Car* Road::createCar() {
    // generate random color
    QColor* color = new QColor(QRandomGenerator::global()->bounded(256),QRandomGenerator::global()->bounded(256),QRandomGenerator::global()->bounded(256));
    // construct new car
    Car* car = new Car(0.01f, color);

    return car;
}
