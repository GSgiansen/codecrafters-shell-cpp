#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;
std::string find_in_path(const std::string cmd) {
  const char* path_env = std::getenv("PATH");
  if (path_env == nullptr) return "";

  std::stringstream ss(path_env);
  std::string directory;

  while (getline(ss, directory, ':')) {
    std::filesystem::path full_path = std::filesystem::path(directory) / cmd;
    if (std::filesystem::exists(full_path)) {
      auto perms = std::filesystem::status(full_path).permissions();
      bool is_executable = ((perms & fs::perms::owner_exec) != fs::perms::none ||
                            (perms & fs::perms::group_exec) != fs::perms::none ||
                            (perms & fs::perms::others_exec) != fs::perms::none);

      if (is_executable) {
        return full_path.string();
      }

    }
  }
  return "";


}
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
        } 
        else {
          // scan through each directory that can be 
          std::string full_path = find_in_path(word);

          if (!full_path.empty()) {
            std::cout << word << " is " << full_path << "\n";
          }
          else {
            
            std::cout << word << ": not found\n";
          }
        }

      }
      else {

        std::cout << command << ": command not found\n";
      }
    }
  }
}
