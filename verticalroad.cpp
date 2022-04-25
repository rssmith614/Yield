#include "verticalroad.h"

int VerticalRoad::clearedCars = 0;

VerticalRoad::VerticalRoad(QWidget* parent) : Road(parent)
{
    stopped = false;

    // I don't like that this is hard-coded
    intersectionLoc = -0.18;

    spawnTimer = new QTimer(this);
    spawnTimer->start(gaps.size() != 0 ? gaps[0] : 0);

    // spawn eligibility is based on time, not position so we define it here
    connect(spawnTimer, SIGNAL (timeout()), this, SLOT (spawnCar()));
    setAttribute(Qt::WA_TranslucentBackground, true);

}

void VerticalRoad::setPaused(bool paused) {
    this->paused = paused;
    if (paused) {
        for (Car* car : cars) {
            car->setBlocked(true);
        }
        disconnect(timer, SIGNAL(timeout()), this, SLOT(updateCars()));
        disconnect(spawnTimer, SIGNAL (timeout()), this, SLOT (spawnCar()));
    } else {
        for (Car* car : cars) {
            car->setBlocked(false);
        }
        connect(timer, SIGNAL(timeout()), this, SLOT(updateCars()));
        connect(spawnTimer, SIGNAL (timeout()), this, SLOT (spawnCar()));
    }
}

void VerticalRoad::initializeGL() {
    openGLFunctions = QOpenGLContext::currentContext()->functions();
    glClearColor(0,0,0.5,0.5);
}

void VerticalRoad::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    setAttribute(Qt::WA_AlwaysStackOnTop);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glLoadIdentity();

    for (Car* car : cars) {
        drawCar(car);
    }

}

void VerticalRoad::drawCar(Car* car) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glBegin(GL_QUADS);
        glColor4f(car->getColor()->redF(), car->getColor()->greenF(), car->getColor()->blueF(),0.5);
        glVertex2f(1.0, car->getY());
        glVertex2f(1.0, car->getY()-Car::l);
        glVertex2f(1.0 - Car::w, car->getY() - Car::l);
        glVertex2f(1.0 - Car::w, car->getY());
    glEnd();
}

Car* VerticalRoad::createCar() {
    // construct new car based on the road's direction
    Car* car = new Car(direction == UP ? Car::UP : Car::DOWN);

    return car;
}

void VerticalRoad::spawnCar() {
    if (preset == DISABLED) return;

    cars.insert(cars.begin(), createCar());
    currentCar++;
    // loop through gaps
    if (currentCar == (int) gaps.size()) currentCar = 0;
    // update next spawn time
    spawnTimer->setInterval(gaps[currentCar]);
}

void VerticalRoad::toggleStop() {
    stopped = !stopped;
}

void VerticalRoad::updateCars() {
    // for every car currently on the road...
    for (size_t i=0; i < cars.size(); i++)  {

        // tell the car where it is
        updateRelativeLoc(cars[i]);

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

        // if it's off-screen, free the pointer and pop it from the queue
        if (cars[i]->getRelativeLoc() == Car::OFF_SCREEN) {
            clearedCars++;
            delete cars[i];
            cars.pop_back();
        }
    }

    // new cars are added with spawnTimer connection
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
    // getY is top of car, so downward moving car needs to subtract length of car
    if (car->getMovement() == Car::UP) {
        return abs(car->getY() - intersectionLoc);
    } else {
        return abs(car->getY() - Car::l + intersectionLoc);
    }
}


