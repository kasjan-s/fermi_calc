#include <QApplication>
#include "fermiplot.h"
#include "mainwindow.h"
#include <boost/program_options.hpp>
#include <stdio.h>

namespace po = boost::program_options;

double convert_to_seconds(double time, std::string unit) {
    if (unit == "y" || unit == "year" || unit == "years")
        time *= 3600 * 24 * 365;
    else if (unit == "d" || unit == "day" || unit == "days")
        time *= 3600 * 24;
    else if (unit == "h" || unit == "hour" || unit == "hours")
        time *= 3600;
    else if (unit == "m" || unit == "min" || unit == "minutes" || unit == "mins")
        time *= 60;
    else if (unit == "ms")
        time /= 1000;
    else if (unit == "s" || unit == "second" || unit == "seconds")
        time = time;
    else {
        std::cerr << "Unknown time unit. Assuming seconds." << std::endl;
    }
    return time;
}

int main(int argc, char *argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("gui", "run graphical user interface")
        ("logft", "calculate logft value")
        ("Z", po::value<int>(), "specify Z of daughter nucleus")
        ("Q", po::value<double>(), "Q energy in MeV")
        ("t", po::value<std::string>(), "half-life time")
        ("I", po::value<double>(), "intensity of the decay in %")
        ("mode", po::value<std::string>(), "beta decay mode (minus, plus or EC)")
    ;

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
              options(desc).run(), vm);
    po::notify(vm);

    if (vm.count("logft")) {
        if (!vm.count("Z")) {
            std::cerr << "You need to specify value of Z!" << std::endl;
        } else if (!vm.count("Q")) {
            std::cerr << "You need to specify value of Q!" << std::endl;
        } else if (!vm.count("t")) {
            std::cerr << "You need to specify half-time!" << std::endl;
        } else if (!vm.count("I")) {
            std::cerr << "You need to specify intensity of decay!" << std::endl;
        } else if (!vm.count("mode")) {
            std::cerr << "You need to specify decay mode!" << std::endl;
        } else {
            int z = vm["Z"].as<int>();
            double q = vm["Q"].as<double>();

            std::string halftime = vm["t"].as<std::string>();
            double time;
            char unit[10];
            sscanf(halftime.c_str(), "%lf%s", &time, unit);

            time = convert_to_seconds(time, std::string(unit));

            double intensity = vm["I"].as<double>() / 100;
            std::string mode = vm["mode"].as<std::string>();

            bool positron = mode == "minus" ? false : true;

            if (mode == "EC") {
                q -= ELECTRON_RMASS_MEV * 2;
            }
            std::cout << q << " " << z << " " << positron << " " << time << " " << intensity << std::endl;
            double logft_value = logft(q, z, positron, time, intensity);

            std::cout << logft_value << std::endl;
        }
    }

    if (vm.count("gui")) {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return a.exec();
    } else {
        return 0;
    }
}
