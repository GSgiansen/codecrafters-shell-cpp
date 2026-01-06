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

void handle_echo(std::stringstream& ss, std::string& word) {
    while (ss >> word) {
      std::cout << word << " ";
    }
    std::cout << "\n";
}
void handle_type(std::stringstream& ss, std::string& word, std::vector<std::string> builtins) {
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

void handle_pwd() {
  fs::path currentPath = fs::current_path();
  std::cout << currentPath.string() << "\n";

}

void handle_cd(std::string newPath) {
  try {
    fs::current_path(newPath);
  } catch(fs::filesystem_error const& e) {
      std::cerr << "cd: " << newPath << ": No such file or directory";
  }
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  while (true) {

    std::cout << "$ ";
    std::string command;
    std::getline(std::cin, command);
    std::vector<std::string> const builtins = {"type", "echo", "exit", "pwd", "cd"};

    if (!command.empty()) {
      std::string word;
      std::stringstream ss(command);
      ss >> word;

      if (word  == "exit") return 0;

      else if (word  == "echo") {
        handle_echo(ss, word);
      }

      else if (word == "type") {
        handle_type(ss, word, builtins);
      }
      else if (word == "pwd") {
        handle_pwd();
      }
      else if (word == "cd") {
        ss >> word;
        handle_cd(word);
      }
      else {
        // try to execute with the programme
        std::string full_path = find_in_path(word);
        std::string executable  = word;
        int argcount = 1;

        if (!full_path.empty()) {
          // found the right file, need to take the arguments
          while (ss >> word) {
             executable = executable + " " + word;
             argcount += 1;
          }
          // std::cout << "Program was passed " << argcount << " args (including program name).\n";
          int return_code = std::system(executable.c_str());

          if (return_code != 0) {
            std::cout << command << ": command not found\n";
          }
        }
        else {
            std::cout << command << ": command not found\n";

        }
      }
    }
  }
}
