#include "verticalroad.h"

VerticalRoad::VerticalRoad(QWidget* parent) : Road(parent)
{
    stopped = false;

    intersectionLoc = -0.18;

    spawnTimer = new QTimer(this);
    spawnTimer->start(m_gaps.size() != 0 ? m_gaps[0] : 0);

    connect(spawnTimer, SIGNAL (timeout()), this, SLOT (spawnCar()));
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
    // construct new car
    Car* car;
    if (m_direction == UP)
        car = new Car(0.01f, Car::UP);
    else
        car = new Car(0.01f, Car::DOWN);

    return car;
}

void VerticalRoad::spawnCar() {
    if (m_preset == DISABLED) return;

    cars.insert(cars.begin(), createCar());
    currentCar++;
    if (currentCar == (int) m_gaps.size()) currentCar = 0;
    spawnTimer->setInterval(m_gaps[currentCar]);
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
            if (cars[i]->getRelativeLoc() == Car::BEFORE_INTERSECTION) {
                // and within .05 of the stop sign
                if (distanceToIntersection(cars[i]) < 0.05) {
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
        if (cars.size() > 1 && i<cars.size()-1) {
            if(carsTooClose(cars[i], cars[i+1])) {
                cars[i]->setBlocked(true);
            } else {
                cars[i]->setBlocked(false);
            }
        }

        // tell the car where it is
        updateRelativeLoc(cars[i]);

        // if it's off-screen, free the pointer and pop it from the queue
        if (cars[i]->getRelativeLoc() == Car::OFF_SCREEN) {
            // score += 1 (maybe using emit?)
            delete cars[i];
            cars.pop_back();
        }
    }

//    qDebug() << spawnTimer->interval() << spawnTimer->remainingTime();

    // if the road isn't empty
//    spawnTimer->setInterval(m_gaps[currentCar]);
//    if (cars.size() > 0) {
//        // if the newest car on the road is past a hard-coded position
//        if (cars[0]->getY() > (-1 + m_gaps[currentCar]))
//        {
//           // we're good to add the next car to the road
//            cars.insert(cars.begin(), createCar());
//            currentCar++;
//        }
//        // restart the hard-coded traffic pattern
//        if (currentCar == (int) m_gaps.size()) currentCar = 0;
//    } else if (m_preset != DISABLED) {
//        // add a car if the road is empty
//        cars.insert(cars.begin(), createCar());
//        currentCar++;
//    }
}

void VerticalRoad::updateRelativeLoc(Car *car)
{
    if (car->getMovement() == Car::UP) {
        if (car->getY() < intersectionLoc) {
            car->setLoc(Car::BEFORE_INTERSECTION);
        } else if (car->getY() - Car::l > 1) {
            car->setLoc(Car::OFF_SCREEN);
        } else {
            car->setLoc(Car::AFTER_INTERSECTION);
        }
    } else {
        if (car->getY() > -intersectionLoc) {
            car->setLoc(Car::BEFORE_INTERSECTION);
        } else if (car->getY() < -1) {
            car->setLoc(Car::OFF_SCREEN);
        } else {
            car->setLoc(Car::AFTER_INTERSECTION);
        }
    }
}

bool VerticalRoad::carsTooClose(Car *behind, Car *front)
{
    if (behind->getMovement() == Car::UP) {
        if(behind->getY() > front->getY() - Car::l - 0.05) {
            return true;
        } else {
            return false;
        }
    } else {
        if(behind->getY() < front->getY() + Car::l + 0.05) {
            return true;
        } else {
            return false;
        }
    }
}

qreal VerticalRoad::distanceToIntersection(Car *car)
{
    if (car->getMovement() == Car::UP) {
        return abs(car->getY() - intersectionLoc);
    } else {
        return abs(car->getY() - Car::l + intersectionLoc);
    }
}


