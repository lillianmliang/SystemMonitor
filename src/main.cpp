#include "ncurses_display.h"
#include "system.h"

#include <iostream>
#include "format.h"
void DebugElapsedTime(){
  std::cout<< Format::ElapsedTime(1)<<"\n";
}

int main() {
  System system;
  NCursesDisplay::Display(system);
  //Debug();
}