#include "road.h"

Road::Road(QWidget* parent) : QOpenGLWidget(parent)
{
    currentCar = 0;
    setPreset(DISABLED, RIGHT);

    timer = new QTimer(this);
    timer->start(33);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCars()));

    spawnTimer = new QTimer(this);
    spawnTimer->start(0);

    paused = false;

    connect(spawnTimer, SIGNAL (timeout()), this, SLOT (spawnCar()));
}

void Road::clear() {
    cars.clear();
}

Road::RoadPreset Road::getPreset() const {
    return preset;
}

void Road::setPreset(Road::RoadPreset preset, Road::Direction direction) {
    std::normal_distribution<> dist(4000,1500);

    // hard-coded traffic flow
    this->preset = preset;
    switch(preset) {
    case A:
        for (int i=0; i < 100; i++) {
            gaps.push_back(abs(dist(*QRandomGenerator::global())));
        }
//        qDebug() << gaps;
        break;
    case B:
        gaps = {1000, 4000};
        break;
    case DISABLED:
        gaps = {};
    }

    this->direction = direction;
}

void Road::setPaused(bool paused) {
    // only do this if it wasn't already paused
    if (paused && !this->paused) {
        for (Car* car : cars) {
            car->setBlocked(true);
        }
        disconnect(timer, SIGNAL(timeout()), this, SLOT(updateCars()));
        disconnect(spawnTimer, SIGNAL (timeout()), this, SLOT (spawnCar()));
    } else if (!paused && this->paused) {
        for (Car* car : cars) {
            car->setBlocked(false);
        }
        connect(timer, SIGNAL(timeout()), this, SLOT(updateCars()));
        connect(spawnTimer, SIGNAL (timeout()), this, SLOT (spawnCar()));
    }

    this->paused = paused;
}

void Road::initializeGL() {
    openGLFunctions = QOpenGLContext::currentContext()->functions();
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable( GL_BLEND );
    glClearColor(0.0,0.0,0.0,0.0);
}

void Road::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setAttribute(Qt::WA_AlwaysStackOnTop);
    glLoadIdentity();

    for (Car* car : cars) {
        drawCar(car);
    }

}

void Road::drawCar(Car* car) {
//    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
        glColor3f(car->getColor()->redF(), car->getColor()->greenF(), car->getColor()->blueF());
        glVertex2f(car->getX(), 1.0);
        glVertex2f(car->getX(), 1.0-Car::w);
        glVertex2f(car->getX() + Car::l, 1.0-Car::w);
        glVertex2f(car->getX()+Car::l, 1.0);
    glEnd();
}

Car* Road::createCar() {
    // construct new car
    Car* car = new Car(direction == RIGHT ? Car::RIGHT : Car::LEFT);

    return car;
}

void Road::spawnCar() {
    // this is what actually disables a road
    if (preset == DISABLED) return;

    cars.insert(cars.begin(), createCar());
    currentCar++;
    // loop through gaps
    if (currentCar == (int) gaps.size()) currentCar = 0;
    // update next spawn time
    spawnTimer->setInterval(gaps[currentCar]);
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

//    // for spatial gaps we have to check positions
//    if (readyToSpawn())
//        spawnCar();
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
