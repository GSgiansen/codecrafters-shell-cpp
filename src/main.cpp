#include <iostream>
#include <string>
#include <sstream>
int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  while (true) {

    std::cout << "$ ";
    std::string command;
    std::getline(std::cin, command);

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
      else {

        std::cout << command << ": command not found\n";
      }
    }
  }
}
