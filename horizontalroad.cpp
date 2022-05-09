#include "horizontalroad.h"

HorizontalRoad::HorizontalRoad(QWidget* parent) : Road(parent)
{

}

Car* HorizontalRoad::createCar()
{
    // construct new car
    Car* car = new Car(direction == RIGHT ? Car::RIGHT : Car::LEFT, speed);

    return car;
}

void HorizontalRoad::updateCars()
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

void HorizontalRoad::updateRelativeLoc(Car* car)
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

bool HorizontalRoad::carsTooClose(Car* behind, Car* front)
{
    if (behind->getMovement() == Car::RIGHT) {
        // x is left side of car
        if (behind->getX() > front->getX() - Car::l - 0.08)
            return true;
        else
            return false;
    } else {
        if (behind->getX() < front->getX() + Car::l + 0.08)
            return true;
        else
            return false;
    }
}
