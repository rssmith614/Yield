#include "road.h"

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

void Road::setPreset(Road::RoadPreset preset, Road::Direction direction)
{
    std::normal_distribution<> dist_a(4000,1500);   // normal distribution with mean 4s and standard deviation 1.5s
    std::normal_distribution<> dist_b(3000,500);
    std::normal_distribution<> dist_c(6000,2000);

    this->preset = preset;
    this->direction = direction;

    // hard-coded traffic flow
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
    case RAND_U:
        for (int i=0; i < 50; i++) {
            gaps.push_back(QRandomGenerator::global()->bounded(500,4000));  // uniform distribution 0.5s to 4s
        }
        break;
    case DISABLED:
        gaps = {};
    }

    if (preset != DISABLED) spawnTimer->start(gaps[0]);
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

    // glClearColor doesnt have active alpha channel by default, this allows roads to
    // be transparent at the cost of always drawing the cars on top of everything
    setAttribute(Qt::WA_AlwaysStackOnTop);
    glClearColor(0,0,0,0);
}

void Road::paintGL()
{
    // paint every car
    for (Car* car : cars) {
        drawCar(car);
    }
}

void Road::drawCar(Car* car)
{
    glBegin(GL_POLYGON);
    glColor3f(car->getColor()->redF(), car->getColor()->greenF(), car->getColor()->blueF());    // grab the car's color
    for (std::vector<Vertex>::iterator vertex = car->vertices.begin(); vertex != car->vertices.end(); ++vertex) // draw every vertex beloning to the car as a polygon
    {
        glVertex2f((vertex->x) + car->offsetX, (vertex->y) + car->offsetY);
    }
    glEnd();
}

void Road::spawnCar()
{
    if (preset == DISABLED) return;

    cars.insert(cars.begin(), createCar());
    currentCar++;
    // loop through gaps
    if (currentCar == (int) gaps.size()) currentCar = 0;
    // update next spawn time
    spawnTimer->setInterval(gaps[currentCar]);
}
