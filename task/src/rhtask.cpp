#include <iostream>

bool priority_command(const std::string& cmd) {
  // TODO: check if priority command
  return true;
}

bool supported_command(const std::string& cmd) {
  return ("get" == cmd || "print" == cmd || "remove" == cmd || priority_command(cmd));
}

void process_command(const std::string& cmd) {
  return;
}

int analyze_command(const std::string& cmd) {
  if("exit" == cmd) {
    return 0;
  } else if(supported_command(cmd)) {
    process_command(cmd);
    return 1;
  } else {
    return -1;
  }
}

int main(void) {
  std::string cmd;
  int run = 1;
  while(run) {
    std::cout << "Command:";
    std::cin >> cmd;
    run = analyze_command(cmd);
    if(run < 0) {
      std::cerr << "WARNING: invalid command:[" << cmd << "]" << std::endl;
    }
  }
  return 0;
}
