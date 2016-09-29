#include <iostream>
#include <vector>
#include "../include/Odometry.hpp"
#include "../include/MotionController.hpp"

void getArgs(const std::string&, char, std::vector<std::string>&);


int main(int argc, char *argv[])
{
    MotionController motion = MotionController();
    motion.init();

    char orderC[100];
    std::string order = "";
    std::vector<std::string> args = std::vector<std::string>();

    while(order.compare("exit"))
    {
        std::cout << std::endl << "MotorDaemon Console : ";
        std::cin.getline(orderC, sizeof(orderC));
        order = std::string(orderC);
        std::cout << std::endl;

        if(!order.compare("exit")) continue;

        args.clear();
        getArgs(order, ' ', args);

        if(!args[0].compare("stop"))
        {
            motion.stop();
            continue;
        }

        else if(!args[0].compare("setksg"))
        {

            if(args.size() != 4)
            {
                std::cout << "USAGE : setksg <kp> <ki> <kd>" << std::endl;
                continue;
            }

            float kp, ki, kd;
            try
            {
                kp = std::stof(args[1]);
                ki = std::stof(args[2]);
                kd = std::stof(args[3]);
            }
            catch (std::exception const &e)
            {
                std::cout << "BAD VALUE" << std::endl;
                continue;
            }

            motion.setLeftSpeedTunings(kp, ki, kd);
            continue;
        }

        else if(!args[0].compare("setksd"))
        {

            if(args.size() != 4)
            {
                std::cout << "USAGE : setksd <kp> <ki> <kd>" << std::endl;
                continue;
            }

            float kp, ki, kd;
            try
            {
                kp = std::stof(args[1]);
                ki = std::stof(args[2]);
                kd = std::stof(args[3]);
            }
            catch (std::exception const &e)
            {
                std::cout << "BAD VALUE" << std::endl;
                continue;
            }

            motion.setRightSpeedTunings(kp, ki, kd);
            continue;
        }

        else if(!args[0].compare("setktd"))
        {

            if(args.size() != 4)
            {
                std::cout << "USAGE : setktd <kp> <ki> <kd>" << std::endl;
                continue;
            }

            float kp, ki, kd;
            try
            {
                kp = std::stof(args[1]);
                ki = std::stof(args[2]);
                kd = std::stof(args[3]);
            }
            catch (std::exception const &e)
            {
                std::cout << "BAD VALUE" << std::endl;
                continue;
            }

            motion.setTranslationTunings(kp, ki, kd);
            continue;
        }

        else if(!args[0].compare("d"))
        {

            if(args.size() != 2)
            {
                std::cout << "USAGE : d <dist>" << std::endl;
                continue;
            }

            long dist;
            try
            {
                dist = std::stol(args[1]);
            }
            catch (std::exception const &e)
            {
                std::cout << "BAD VALUE" << std::endl;
                continue;
            }

            motion.orderTranslation(dist);
            continue;
        }

        else
        {
            std::cout << "No such command" << std::endl;
        }


    }

    return 0;
}

void getArgs(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss;
    ss.str(s);
    std::string item;

    while (getline(ss, item, delim)) {
        std::cout << item << std::endl;
        elems.push_back(item);
    }
}