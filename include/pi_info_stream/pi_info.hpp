#ifndef SYSTEM_STATS_HPP
#define SYSTEM_STATS_HPP

#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>

inline long get_total_cpu_time(std::ifstream& file, long& idle_time) {
    std::string line;
    std::getline(file, line);
    std::istringstream ss(line);
    std::string cpu;
    long user, nice, system, idle, iowait, irq, softirq, steal;

    ss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    idle_time = idle + iowait;
    return user + nice + system + idle_time + irq + softirq + steal;
}

inline float get_cpu_usage() {
    std::ifstream file1("/proc/stat");
    long idle1;
    long total1 = get_total_cpu_time(file1, idle1);
    file1.close();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream file2("/proc/stat");
    long idle2;
    long total2 = get_total_cpu_time(file2, idle2);
    file2.close();

    long total_diff = total2 - total1;
    long idle_diff = idle2 - idle1;

    return 100.0f * (total_diff - idle_diff) / total_diff;
}

inline float get_cpu_temp() {
    std::ifstream file("/sys/class/thermal/thermal_zone0/temp");
    float temp;
    file >> temp;
    return temp / 1000.0f;
}

inline float get_memory_usage_percent() {
    std::ifstream file("/proc/meminfo");
    std::string key;
    long value;
    std::string unit;
    long total, available;

    while (file >> key >> value >> unit) {
        if (key == "MemTotal:") total = value;
        else if (key == "MemAvailable:") available = value;
    }
    return 100.0f * (total - available) / total;
}

inline float get_swap_usage_percent() {
    std::ifstream file("/proc/meminfo");
    std::string key;
    long swap_total = 0, swap_free = 0;
    long value;
    std::string unit;

    while (file >> key >> value >> unit) {
        if (key == "SwapTotal:") swap_total = value;
        else if (key == "SwapFree:") swap_free = value;
    }

    if (swap_total == 0) return 0.0f;
    return 100.0f * (swap_total - swap_free) / swap_total;
}

#endif // SYSTEM_STATS_HPP