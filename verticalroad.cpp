#include "verticalroad.h"

VerticalRoad::VerticalRoad(QWidget* parent) : Road(parent)
{
    stopped = false;

    intersectionLoc = -0.2;
}

void VerticalRoad::initializeGL() {
    openGLFunctions = QOpenGLContext::currentContext()->functions();
}

void VerticalRoad::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setAttribute(Qt::WA_AlwaysStackOnTop);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glLoadIdentity();

    for (Car* car : cars) {
        drawCar(car);
    }

}

void VerticalRoad::drawCar(Car* car) {
    glBegin(GL_QUADS);
        glColor3f(car->getColor()->redF(), car->getColor()->greenF(), car->getColor()->blueF());
        glVertex2f(1.0, car->getY());
        glVertex2f(1.0, car->getY()-Car::l);
        glVertex2f(1.0 - Car::w, car->getY() - Car::l);
        glVertex2f(1.0 - Car::w, car->getY());
    glEnd();
}

Car* VerticalRoad::createCar() {
    // generate random color
    QColor* color = new QColor(QRandomGenerator::global()->bounded(256),QRandomGenerator::global()->bounded(256),QRandomGenerator::global()->bounded(256));
    // construct new car
    Car* car = new Car(0.01f, color, Car::UP);

    return car;
}

void VerticalRoad::toggleStop() {
    if (stopped) {
        stopped = false;
    }
    else {
        stopped = true;
    }
}

void VerticalRoad::updateCars() {
    // for every car currently on the road...
    for (size_t i=0; i < cars.size(); i++)  {

        // if the stop sign is active
        if (this->stopped) {
            // and the car is in fron of the stop sign
            if (cars[i]->isBeforeIntersection()) {
                // and within .05 of the stop sign
                if (abs(cars[i]->getY() - intersectionLoc) < 0.05) {
                    cars[i]->setStopped(true);
                } else {
                    cars[i]->setStopped(false);
                }
            // if the car has passed the stop sign, don't let the stop sign stop it
            } else {
                cars[i]->setStopped(false);
            }
        // if the stop sign is inactive, tell all the cars
        } else {
            cars[i]->setStopped(false);
        }

        // if the car is too close to the car in front of it
        if (cars.size() > 1 && i!=cars.size()-1) {
            if(cars[i]->getY() > cars[i+1]->getY() - Car::l - 0.05) {
                cars[i]->setBlocked(true);
            } else {
                cars[i]->setBlocked(false);
            }
        }

        // tell the car where it is
        if (cars[i]->getY() < intersectionLoc) {
            cars[i]->setLoc(Car::BEFORE_INTERSECTION);
        } else {
            cars[i]->setLoc(Car::AFTER_INTERSECTION);
        }

        // if it's off-screen, free the pointer and pop it from the queue
        if (cars[i]->getY() > (1+Car::w)) {
            delete cars[i];
            cars.pop_back();
        }
    }

    // if the road isn't empty
    if (cars.size() > 0) {
        // if the newest car on the road is past a hard-coded position
        if (cars[0]->getY() > (-1 + m_gaps[currentCar]))
        {
            // we're good to add the next car to the road
            cars.insert(cars.begin(), createCar());
            currentCar++;
        }
        // restart the hard-coded traffic pattern
        if (currentCar == (int) m_gaps.size()) currentCar = 0;
    } else if (m_preset != DISABLED) {
        // add a car if the road is empty
        cars.insert(cars.begin(), createCar());
        currentCar++;
    }
}
