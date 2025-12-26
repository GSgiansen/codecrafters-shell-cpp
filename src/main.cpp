#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  while (true) {

    std::cout << "$ ";
    std::string command;
    std::getline(std::cin, command);
    std::vector<std::string> builtins = {"type", "echo", "exit"};

    if (!command.empty()) {
      std::string word;
      std::stringstream ss(command);
      ss >> word;

      if (word  == "exit") return 0;

      else if (word  == "echo") {
        while (ss >> word) {
          std::cout << word << " ";
        }
        std::cout << "\n";
      }

      else if (word == "type") {
        ss >> word;
        if (std::find(builtins.begin(), builtins.end(), word) != builtins.end()) {
            std::cout << word << " is a shell builtin\n";
        } else {
          std::cout << word << ": not found\n";

        }

      }
      else {

        std::cout << command << ": command not found\n";
      }
    }
  }
}
