#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds[[maybe_unused]]) {
    long remainingSeconds = seconds;
    int hours = remainingSeconds / 3600;
    remainingSeconds = remainingSeconds % 3600;
    int minutes = remainingSeconds / 60;
    remainingSeconds = remainingSeconds % 60;

    string formattedTime{""};
    formattedTime += hours > 9 ? std::to_string(hours): string{"0"} + std::to_string(hours);
    formattedTime += ":";
    formattedTime += minutes > 9 ? std::to_string(minutes): string{"0"} + std::to_string(minutes);
    formattedTime += ":";
    formattedTime += remainingSeconds > 9 ? std::to_string(remainingSeconds): string{"0"} + std::to_string(remainingSeconds);
    return formattedTime; 
}