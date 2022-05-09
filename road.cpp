#include "road.h"
#include <vector>

float Road::speed = 0.015;

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

void Road::clear()
{
    cars.clear();
}

Road::RoadPreset Road::getPreset() const
{
    return preset;
}

void Road::setPreset(Road::RoadPreset preset, Road::Direction direction)
{
    std::normal_distribution<> dist_a(4000,1500);
    std::normal_distribution<> dist_b(3000,500);
    std::normal_distribution<> dist_c(6000,2000);

    // hard-coded traffic flow
    this->preset = preset;
    switch(preset) {
    case RAND_A:
        for (int i=0; i < 50; i++) {
            gaps.push_back(abs(dist_a(*QRandomGenerator::global())));
        }
        break;
    case RAND_B:
        for (int i=0; i < 50; i++) {
            gaps.push_back(abs(dist_b(*QRandomGenerator::global())));
        }
        break;
    case RAND_C:
        for (int i=0; i < 50; i++) {
            gaps.push_back(abs(dist_c(*QRandomGenerator::global())));
        }
        break;
    case FIXED:
        gaps = {1000, 4000};
        break;
    case DISABLED:
        gaps = {};
    }

    this->direction = direction;
}

void Road::setPaused(bool paused)
{
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

void Road::initializeGL()
{
    openGLFunctions = QOpenGLContext::currentContext()->functions();
    glClearColor(0,0,0,0);
}

void Road::paintGL()
{
    setAttribute(Qt::WA_AlwaysStackOnTop);
    glLoadIdentity();

    for (Car* car : cars) {
        drawCar(car);
    }

}

void Road::drawCar(Car* car)
{
    glBegin(GL_POLYGON);
    glColor3f(car->getColor()->redF(), car->getColor()->greenF(), car->getColor()->blueF());
    for (std::vector<Vertex>::iterator vertex = car->vertices.begin(); vertex != car->vertices.end(); ++vertex)
    {
        glVertex2f((vertex->x) + car->offsetX, (vertex->y) + car->offsetY);
    }
    glEnd();
}

Car* Road::createCar()
{
    // construct new car
    Car* car = new Car(direction == RIGHT ? Car::RIGHT : Car::LEFT, speed);

    return car;
}

void Road::spawnCar()
{
    // this is what actually disables a road
    if (preset == DISABLED) return;

    cars.insert(cars.begin(), createCar());
    currentCar++;
    // loop through gaps
    if (currentCar == (int) gaps.size()) currentCar = 0;
    // update next spawn time
    spawnTimer->setInterval(gaps[currentCar]);
}

void Road::updateCars()
{
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
}

void Road::updateRelativeLoc(Car* car)
{
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

bool Road::carsTooClose(Car* behind, Car* front)
{
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
