#include <QApplication>
#include "fermiplot.h"
#include "mainwindow.h"
#include <boost/program_options.hpp>
#include <stdio.h>

namespace po = boost::program_options;

int main(int argc, char *argv[])
{
    po::options_description desc("Allowed options");
    po::variables_map vm;

    try {
        desc.add_options()
            ("help,h", "Produce help message.")
            ("gui,g", "Run graphical user interface.")
            ("logft,l", "Calculate logft value. Have to specify options --Z, --Q, --t, --I, --mode.")
            ("integral,f", "Calculate fermi integral value. Have to specify --Z and --mode. Add --energy option for single value or options --from, --to, --tick for a range of values.")
            ("nucleus,z", po::value<int>(), "specify Z of daughter nucleus")
            ("energy,q", po::value<double>(), "Q energy in MeV")
            ("half-life,t", po::value<std::string>(), "half-life time")
            ("intensity,i", po::value<double>(), "intensity of the decay in %")
            ("mode,m", po::value<std::string>(), "beta decay mode (minus or plus)")
            ("beg,b", po::value<double>(), "Low end of integral range.")
            ("end,e", po::value<double>(), "High end of integral range.")
            ("step,s", po::value<double>(), "Step between integral values. Default it 0.1.")
        ;

        po::store(po::command_line_parser(argc, argv)
                  .options(desc)
                  .run(), vm);
        po::notify(vm);
    } catch (po::error& ex) {
        std::cerr << ex.what() << std::endl;
        std::cout << desc << std::endl;
        return 0;
    }

    if (vm.count("help") || vm.empty()) {
        std::cout << "Example usage:" << std::endl;
        std::cout << argv[0] << " -f -z 30 -mode minus -q 0.5" << std::endl;
        std::cout << argv[0] << " --logft --nucleus 30 --energy 0.573 --half-life 12.8h --intensity 38 --mode minus" << std::endl;
        std::cout << argv[0] << " -l -z 30 -q 0.573 -t 46080s -i 38 -m -" << std::endl;
        std::cout << argv[0] << " --logft -z 30 --energy 0.573 -t 46080 --intensity 38 --mode -" << std::endl;
        std::cout << argv[0] << " --integral -z 30 -mode minus -b 0.1 -e 1.0 -s 0.1" << std::endl;
        std::cout << argv[0] << " -f -z 30 -mode minus -q 0.5" << std::endl;
        std::cout << std::endl;
        std::cout << desc << std::endl;

        return 0;
    }

    if (vm.count("logft") || vm.count("l")) {
        if (!vm.count("z") && !vm.count("nucleus")) {
            std::cout << vm.count("nucles") << std::endl;
            std::cerr << "You need to specify value of Z!" << std::endl;
        } else if (!vm.count("q") && !vm.count("energy")) {
            std::cerr << "You need to specify value of Q!" << std::endl;
        } else if (!vm.count("t") && !vm.count("half-life")) {
            std::cerr << "You need to specify half-time!" << std::endl;
        } else if (!vm.count("i") && !vm.count("intensity")) {
            std::cerr << "You need to specify intensity of decay!" << std::endl;
        } else if (!vm.count("mode") && !vm.count("m")) {
            std::cerr << "You need to specify decay mode!" << std::endl;
        } else {
            int z = vm.count("z") ? vm["z"].as<int>() : vm["nucleus"].as<int>();
            double q = vm.count("q") ? vm["q"].as<double>() : vm["energy"].as<double>();
            std::string halftime = vm.count("t") ? vm["t"].as<std::string>() : vm["half-life"].as<std::string>();
            double intensity = vm.count("i") ? vm["i"].as<double>() / 100 : vm["intensity"].as<double>() / 100;

            double time;
            char unit[10];
            sscanf(halftime.c_str(), "%lf%s", &time, unit);

            time = convert_to_seconds(time, std::string(unit));

            std::string mode = vm.count("mode") ? vm["mode"].as<std::string>() : vm["m"].as<std::string>();

            if (validate_data(z, q, time, intensity, mode)) {
                bool positron = mode == "minus" || mode == "-" ? false : true;

                double logft_value = logft(q, z, positron, time, intensity);

                std::cout << logft_value << std::endl;
            }
        }

        return 0;
    }

    if (vm.count("integral")) {
        if (!vm.count("z") && !vm.count("nucleus")) {
            std::cerr << "You need to specify value of Z!" << std::endl;
        } else if (!vm.count("mode") && !vm.count("m")) {
            std::cerr << "You need to specify decay mode!" << std::endl;
        } else {
            int z = vm.count("z") ? vm["z"].as<int>() : vm["nucleus"].as<int>();
            if (z <= 0 || z > 102) {
                std::cout << "Z is out of bounds - must be between 1 and 101" << std::endl;
                return -1;
            }

            double from = -1.0;
            double to = -1.0;
            double step = 0.1;

            if (vm.count("beg") || vm.count("b")) {
                double beg = from = vm.count("beg") ? vm["beg"].as<double>() : vm["b"].as<double>();
                if (beg >= 0.01 && beg < 25.044) {
                    from = beg;
                } else {
                    std::cout << "Lower bound is out of range!" << std::endl;
                }
            }
            if (vm.count("end") || vm.count("e")) {
                double end = vm.count("end") ? vm["end"].as<double>() : vm["e"].as<double>();
                if (end > 0.01 && end <= 25.044) {
                    to = end;
                } else {
                    std::cout << "Higher bound is out of range!" << std::endl;
                }
            }
            if (vm.count("step") || vm.count("s")) {
                double tick = vm.count("step") ? vm["step"].as<double>() : vm["s"].as<double>();
                if (tick > 0)
                    step = tick;
                else
                    std::cout << "Step value must be positive!" << std::endl;

            }

            std::string mode = vm.count("mode") ? vm["mode"].as<std::string>() : vm["m"].as<std::string>();
            bool positron = mode == "minus" || mode == "-" ? false : true;

            if (from > 0 && to > 0 && from <= to && step > 0) {
                std::cout << "Z=" << z << ", mode=" << mode << ", beg=" << from << ", end=" << to << ", step=" << step << std::endl;
                std::cout << "Q [MeV]\tf(Z,E)" << std::endl;
                for (double i = from; i < to; i += step) {
                    double fval = f_value(i, z, positron);
                    std::cout << i << "\t" << fval << std::endl;
                }
            } else {
                if (!vm.count("energy") && !vm.count("q")) {
                    std::cerr << "You need to specify energy!" << std::endl;
                    return -1;
                }

                double energy = vm.count("q") ? vm["q"].as<double>() : vm["energy"].as<double>();

                if (energy < 0.01 || energy > 25.044) {
                    std::cout << "Energy must be between 0.01 and 25.044 MeV!" << std::endl;
                    return -1;
                }

                std::cout << energy << "\t" << f_value(energy, z, positron) << std::endl;
            }
        }

        return 0;
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
