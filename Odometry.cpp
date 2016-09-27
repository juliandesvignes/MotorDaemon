//
// Created by discord on 26/09/16.
//

#include <fcntl.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <BlackGPIO.h>
#include "Odometry.hpp"

#define AL 0
#define BL 1
#define AR 2
#define BR 3

long Odometry::leftTicks;
long Odometry::rightTicks;
//uint8_t Odometry::firstChanL; //1 : chanA ; 2 : chanB
//uint8_t Odometry::firstChanR; //1 : chanA ; 2 : chanB
int Odometry::valueAL;
int Odometry::valueBL;
int Odometry::valueAR;
int Odometry::valueBR;

Odometry::Odometry(uint8_t chanAL, uint8_t chanBL, uint8_t chanAR, uint8_t chanBR)
{
    system((std::string("echo ")+std::to_string(chanAL)+std::string(" > /sys/class/gpio/export")).c_str());
    system((std::string("echo ")+std::to_string(chanBL)+std::string(" > /sys/class/gpio/export")).c_str());
    system((std::string("echo ")+std::to_string(chanAR)+std::string(" > /sys/class/gpio/export")).c_str());
    system((std::string("echo ")+std::to_string(chanBR)+std::string(" > /sys/class/gpio/export")).c_str());

    system((std::string("echo in > /sys/class/gpio/gpio")+std::to_string(chanAL)+std::string("/direction")).c_str());
    system((std::string("echo in > /sys/class/gpio/gpio")+std::to_string(chanBL)+std::string("/direction")).c_str());
    system((std::string("echo in > /sys/class/gpio/gpio")+std::to_string(chanAR)+std::string("/direction")).c_str());
    system((std::string("echo in > /sys/class/gpio/gpio")+std::to_string(chanBR)+std::string("/direction")).c_str());

    system((std::string("echo both > /sys/class/gpio/gpio")+std::to_string(chanAL)+std::string("/edge")).c_str());
    system((std::string("echo both > /sys/class/gpio/gpio")+std::to_string(chanBL)+std::string("/edge")).c_str());
    system((std::string("echo both > /sys/class/gpio/gpio")+std::to_string(chanAR)+std::string("/edge")).c_str());
    system((std::string("echo both > /sys/class/gpio/gpio")+std::to_string(chanBR)+std::string("/edge")).c_str());

    Odometry::leftTicks = 0;
    Odometry::rightTicks = 0;
 //   Odometry::firstChanL = 0;
 //   Odometry::firstChanR = 0;
    Odometry::valueAL = 0;
    Odometry::valueBL = 0;
    Odometry::valueAR = 0;
    Odometry::valueBR = 0;

    t = std::thread(std::bind(mainWorker, chanAL, chanBL, chanAR, chanBR), "Counter Thread");
    t.detach();
}

void Odometry::mainWorker(uint8_t chanAL, uint8_t chanBL, uint8_t chanAR, uint8_t chanBR)
{
    int fdAL = open( (std::string("/sys/class/gpio/gpio")+std::to_string(chanAL)+std::string("/value")).c_str(), O_RDONLY );
    int fdBL = open( (std::string("/sys/class/gpio/gpio")+std::to_string(chanBL)+std::string("/value")).c_str(), O_RDONLY );
    int fdAR = open( (std::string("/sys/class/gpio/gpio")+std::to_string(chanAR)+std::string("/value")).c_str(), O_RDONLY );
    int fdBR = open( (std::string("/sys/class/gpio/gpio")+std::to_string(chanBR)+std::string("/value")).c_str(), O_RDONLY );

    BlackLib::BlackGPIO   al((BlackLib::gpioName) chanAL, BlackLib::input, BlackLib::FastMode);
    BlackLib::BlackGPIO   bl((BlackLib::gpioName) chanBL, BlackLib::input, BlackLib::FastMode);
    BlackLib::BlackGPIO   ar((BlackLib::gpioName) chanAR, BlackLib::input, BlackLib::FastMode);
    BlackLib::BlackGPIO   br((BlackLib::gpioName) chanBR, BlackLib::input, BlackLib::FastMode);

    struct pollfd pfd[4];

    pfd[AL].fd = fdAL;
    pfd[AL].events = POLLPRI;
    pfd[AL].revents = 0;

    pfd[BL].fd = fdBL;
    pfd[BL].events = POLLPRI;
    pfd[BL].revents = 0;

    pfd[AR].fd = fdAR;
    pfd[AR].events = POLLPRI;
    pfd[AR].revents = 0;

    pfd[BR].fd = fdBR;
    pfd[BR].events = POLLPRI;
    pfd[BR].revents = 0;

    while (true)
    {
        poll(pfd, 4, -1);

        if (pfd[AL].revents != 0) {
            get_lead(fdAL, AL);
            onTickChanALeft();
        }
        if (pfd[BL].revents != 0) {
            get_lead(fdBL, BL);
            onTickChanBLeft();
        }
        if (pfd[AR].revents != 0) {
            get_lead(fdAR, AR);
            onTickChanARight();
        }
        if (pfd[BR].revents != 0) {
            get_lead(fdBR, BR);
            onTickChanBRight();
        }

        usleep(1);
    }
}

long Odometry::getLeftValue() {
    return Odometry::leftTicks;
}

long Odometry::getRightValue() {
    return Odometry::rightTicks;
}

void Odometry::onTickChanALeft(void)
{
  /*  if(firstChanL == 0)
    {
        firstChanL = 1;
    }
    else if(firstChanL == 2)
    {
        firstChanL = 0;
        leftTicks--;
    }*/

    if(valueAL == valueBL)
    {
        leftTicks--;
    }
}

void Odometry::onTickChanBLeft(void)
{
  /*  if(firstChanL == 0)
    {
        firstChanL = 2;
    }
    else if(firstChanL == 1)
    {
        firstChanL = 0;
        leftTicks++;
    }*/

    if(valueAL == valueBL)
    {
        leftTicks++;
    }
}

void Odometry::onTickChanARight(void)
{
  /*  if(firstChanR == 0)
    {
        firstChanR = 1;
    }
    else if(firstChanR == 2)
    {
        firstChanR = 0;
        rightTicks--;
    }*/

    if(valueAR == valueBR)
    {
        rightTicks--;
    }
}

void Odometry::onTickChanBRight(void)
{
   /* if(firstChanR == 0)
    {
        firstChanR = 2;
    }
    else if(firstChanR == 1)
    {
        firstChanR = 0;
        rightTicks++;
    }*/

    if(valueAR == valueBR)
    {
        rightTicks++;
    }
}

void Odometry::get_lead(int fd, uint8_t chan) //chan : 0=AL, 1=BL, 2=AR, 3=BR
{
    lseek(fd, 0, 0);

    char buffer[24];
    int size = read(fd, buffer, sizeof(buffer));

    if(chan == AL)
    {
        if (size != -1) {
            buffer[size] = NULL;
            valueAL = atoi(buffer);
        }
        else {
            valueAL = -1;
        }
    } else if(chan == BL) {
        if (size != -1) {
            buffer[size] = NULL;
            valueBL = atoi(buffer);
        }
        else {
            valueBL = -1;
        }
    } else if(chan == AR) {
        if (size != -1) {
            buffer[size] = NULL;
            valueAR = atoi(buffer);
        }
        else {
            valueAR = -1;
        }
    } else if(chan == BR) {
        if (size != -1) {
            buffer[size] = NULL;
            valueBR = atoi(buffer);
        }
        else {
            valueBR = -1;
        }
    }

}

