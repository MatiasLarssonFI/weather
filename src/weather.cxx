#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>

#include "weatherserver.hxx"

void run(const char* out_file_name) {
    WeatherServer server;
    if(access(out_file_name, W_OK) != 0) {
        std::cerr << "No writeable entry exists with given name, will create a new fifo.\n";
        mkfifo(out_file_name, S_IRWXU);
    }
    std::ofstream mask_f(out_file_name, std::ios::out|std::ios::app);
    while (mask_f) {
        const Weather w(server.currentWeather());
        const auto& intrp = w.interpretation;
        const auto& rec = w.record;
        std::cerr << (intrp.is_clear ? "Clear\n" : "Cloudy\n")
                << (intrp.is_sunny ? "Sunny\n" : "")
                << (rec.is_sun_up ? "Sun is up\n" : "Sun is down\n")
                << (intrp.is_windy ? "Windy\n" : "")
                << (intrp.is_rainy ? "Rainy\n" : "")
                << (intrp.is_snowy ? "Snowy\n" : "")
                << (intrp.is_hot ? "Hot\n" : "")
                << (intrp.is_cold ? "Cold\n" : "")
                << (intrp.is_freezing ? "Freezing\n" : "")
                << (intrp.is_warm ? "Warm\n" : "") << std::endl;

        mask_f << (unsigned)( (intrp.is_clear << 0) | (intrp.is_sunny << 1) | (rec.is_sun_up << 2) |
                (intrp.is_windy << 3) | (intrp.is_rainy << 4) | (intrp.is_snowy << 5) |
                (intrp.is_hot << 6) | (intrp.is_cold << 7) | (intrp.is_freezing << 8) |
                (intrp.is_warm << 9) ) << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cerr << "Failed to open output file" << std::endl;
}


int main(int argc, char* argv[]) {
    if (argc == 2) {
        run(argv[1]);
    } else {
        std::cerr << "Usage: " << argv[0] << " [output_file_name]" << std::endl;
        return 1;
    }
}
