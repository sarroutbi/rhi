#include <iostream>
#include <set>

struct buffer{
public:
  bool operator<(const buffer& other) const {
    return (prio) < (other.prio);
  }
  std::string dump() const {
    return std::to_string(value);
  }
  uint64_t prio;
  uint64_t value;
};

using buffer_t = struct buffer;

class Buffer {
public:
  Buffer() : m_buffer_list{}{}
  void dump_first() const {
    std::cout << m_buffer_list.begin()->dump() << std::endl;
  }
  void insert_element(const buffer_t& buffer) {
    m_buffer_list.insert(buffer);
  }
  void dump_all() const {
    for (auto const& e : m_buffer_list) {
      e.dump();
    }
  }
  void remove_first() {
    m_buffer_list.erase(m_buffer_list.begin());
  }
private:
  std::set<buffer_t> m_buffer_list;
};

bool priority_command(const std::string& cmd) {
  // TODO: check if priority command
  return true;
}

bool supported_command(const std::string& cmd) {
  return ("get" == cmd || "print" == cmd || "remove" == cmd || priority_command(cmd));
}

void process_command(const std::string& cmd, const Buffer& b) {
  if("get" == cmd) {
    b.dump_first();
  } else if("print" == cmd) {
    b.dump_all();
  }
  return;
}

int analyze_command(const std::string& cmd, const Buffer& b) {
  if("exit" == cmd) {
    return 0;
  } else if(supported_command(cmd)) {
    process_command(cmd, b);
    return 1;
  } else {
    return -1;
  }
}

int main(void) {
  std::string cmd;
  int run = 1;
  Buffer b;
  while(run) {
    std::cout << "Command:";
    std::cin >> cmd;
    run = analyze_command(cmd, b);
    if(run < 0) {
      std::cerr << "WARNING: invalid command:[" << cmd << "]" << std::endl;
    }
  }
  return 0;
}
