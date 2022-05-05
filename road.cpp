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

    setAttribute(Qt::WA_AlwaysStackOnTop);
    glClearColor(0,0,0,0);
}

void Road::paintGL()
{
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
