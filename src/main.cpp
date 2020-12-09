#include "ncurses_display.h"
#include "system.h"

int main() {

  system("setterm -cursor off");
  System system;
  NCursesDisplay::Display(system);
}