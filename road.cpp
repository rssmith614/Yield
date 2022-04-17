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
    setAttribute(Qt::WA_AlwaysStackOnTop);
    glClearColor(0.0,0.0,0.0,0.0);
    glLoadIdentity();

    for (Car* car : cars) {
        drawCar(car);
    }

}
//Road::Road(GLfloat scale, GLfloat offsetX, GLfloat offsetY)
//{
//  this->scale = scale;
//  this->offsetX = offsetX;
//  this->offsetY = offsetY;
//};

//vector<Vertex> Road::getVertices()
//{
//  return this->vertices;
//};

//Vertex Actor::getVertex(GLint i)
//{
//  return (this->vertices)[i];
//};

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
    for (std::vector<Vertex>::iterator vertex = car->vertices.begin(); vertex != car->vertices.end(); ++vertex)
      {
        glVertex2f((vertex->x) + car->offsetX, (vertex->y) + car->offsetY);
      }
    glEnd();
}

Car* Road::createCar() {
    // construct new car
    Car* car = new Car(0.01f, Car::RIGHT);

    return car;
}

void Road::updateCars() {
    // for every car currently on the road...
    for (size_t i=0; i < cars.size(); i++)  {

        // if it's off-screen, free the pointer and pop it from the queue
        if (cars[i]->getX() > 1) {
            delete cars[i];
            cars.pop_back();
        }

        // if the car is too close to the car in front of it (only happens in the event of a collision)
        if (cars.size() > 1 && i<cars.size()-1) {
            if(cars[i]->getX() > cars[i+1]->getX() - Car::l - 0.05) {
                cars[i]->setBlocked(true);
            } else {
                cars[i]->setBlocked(false);
            }
        }
    }

    // if the road isn't empty
    if (cars.size() > 0) {
        // if the newest car on the road is past a hard-coded position
//        qDebug() << cars[0]->getX();
        if (cars[0]->getX() > (-1 + m_gaps[currentCar]))
        {
//            qDebug() << "adding car";
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
