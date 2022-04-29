#include "road.h"
#include <vector>
Road::Road(QWidget* parent) : QOpenGLWidget(parent)
{
    currentCar = 0;
    setPreset(DISABLED, RIGHT);

    timer = new QTimer(this);
    timer->start(33);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCars()));
}

Road::RoadPreset Road::getPreset() const {
    return m_preset;
}

void Road::setPreset(Road::RoadPreset preset, Road::Direction direction) {
    // hard-coded traffic flow
    // H_ prefix - horizontal traffic, spatial gaps
    // V_ prefix - vertical traffic, temporal gaps
    m_preset = preset;
    switch(m_preset) {
    case H_A:
        m_gaps = {0.5,1};
        break;
    case H_B:
        m_gaps = {0.05, 1.0};
        break;
    case V_A:
        m_gaps = {2000, 5000};
        break;
    case V_B:
        m_gaps = {1000, 4000};
        break;
    case DISABLED:
        m_gaps = {};
    }

    m_direction = direction;
}

void Road::initializeGL() {
    openGLFunctions = QOpenGLContext::currentContext()->functions();
}

void Road::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    setAttribute(Qt::WA_AlwaysStackOnTop);
    glClearColor(0.0,0.0,0.0,0.0);
    glLoadIdentity();

    for (Car* car : cars) {
        drawCar(car);
    }

}

void Road::halt() {
    for (Car* car : cars) {
        car->notifyCollision();
    }
}

void Road::drawCar(Car* car) {
    glClear(GL_DEPTH_BUFFER_BIT);
//    glBegin(GL_QUADS);
//        glColor3f(car->getColor()->redF(), car->getColor()->greenF(), car->getColor()->blueF());
//        glVertex2f(car->getX(), 1.0);
//        glVertex2f(car->getX(), 1.0-Car::w);
//        glVertex2f(car->getX() + Car::l, 1.0-Car::w);
//        glVertex2f(car->getX()+Car::l, 1.0);
//    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(car->getColor()->redF(), car->getColor()->greenF(), car->getColor()->blueF());
//    qDebug() << "drawing" << car->vertices.size() << "vertices" << (car->getX());
    for (std::vector<Vertex>::iterator vertex = car->vertices.begin(); vertex != car->vertices.end(); ++vertex)
      {
        glVertex2f((vertex->x) + car->offsetX, (vertex->y) + car->offsetY);
      }
    glEnd();
//    qDebug() << (Car::MovementType) car->getMovement();
}

Car* Road::createCar() {
    // construct new car
    Car* car = new Car(0.01f, (m_direction == RIGHT) ? Car::RIGHT : Car::LEFT);

    return car;
}

void Road::spawnCar() {
    // this is what actually disables a road
    if (m_preset == DISABLED) return;

    cars.insert(cars.begin(), createCar());
    currentCar++;
    // loop through gaps
    if (currentCar == (int) m_gaps.size()) currentCar = 0;
}

bool Road::readyToSpawn() {
    if (cars.size() == 0)
        return true;

    // distance from edge depends on which direction the car is driving
    if (m_direction == RIGHT) {
        if (cars[0]->getX() > (-1 + m_gaps[currentCar]))
            return true;
    } else {
        if (cars[0]->getX() < (1 - m_gaps[currentCar]))
            return true;
    }

    return false;
}

void Road::updateCars() {
    // for every car currently on the road...
    for (size_t i=0; i < cars.size(); i++)  {

        // tell the car where it is
        updateRelativeLoc(cars[i]);

        // if it's off-screen, free the pointer and pop it from the queue
        if (cars[i]->getRelativeLoc() == Car::OFF_SCREEN) {
            delete cars[i];
            cars.pop_back();
        }

        // if the car is too close to the car in front of it (only happens in the event of a collision)
        if (cars.size() > 1 && i<cars.size()-1) {
            if(carsTooClose(cars[i], cars[i+1])) {
                cars[i]->setBlocked(true);
            } else {
                cars[i]->setBlocked(false);
            }
        }
    }

    // for spatial gaps we have to check positions
    if (readyToSpawn())
        spawnCar();
}

void Road::updateRelativeLoc(Car* car) {
    // car's relative location is dependent on its movement direction
    // -1 is where a right-moving car spawns, but -1 is off-screen for left-moving car
    if (car->getMovement() == Car::RIGHT) {
        if (car->getX() - Car::l > 1)
            car->setLoc(Car::OFF_SCREEN);
    } else {
        if (car->getX() + Car::l < -1)
            car->setLoc(Car::OFF_SCREEN);
    }
}

bool Road::carsTooClose(Car* behind, Car* front) {
    if (behind->getMovement() == Car::RIGHT) {
        // x is left side of car
        if (behind->getX() > front->getX() - Car::l - 0.05)
            return true;
        else
            return false;
    } else {
        if (behind->getX() < front->getX() + Car::l + 0.05)
            return true;
        else
            return false;
    }
}
