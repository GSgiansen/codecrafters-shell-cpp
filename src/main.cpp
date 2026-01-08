#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <cstdlib>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "navigation.h"
#include "quoting.h"

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
std::vector<std::string> parseCommand(const std::string& command) {
  std::vector<std::string> args;
  std::string current_arg;
  bool in_quotes = false;
  for (size_t i = 0; i < command.length(); ++i) {
    char c = command[i];
    if (c == '\'') {
      in_quotes = !in_quotes;
    } else if (c == '"') {

    }
    else if (isspace(c) && !in_quotes) {
      if (!current_arg.empty()) {
        args.push_back(current_arg);
        current_arg.clear();
      }
    } else {
      current_arg += c;
    }
  }
    if (!current_arg.empty()) {
    args.push_back(current_arg);
  }
  return args;
}

void handle_echo(std::vector<std::string> args) {
    
    for (std::string word : args) {
      std::cout << word << " ";
    }
    std::cout << "\n";
}


void handle_type(std::vector<std::string> args, std::vector<std::string> builtins) {
  std::string word = args[0];

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

void executeProgram(const std::string& path, const std::vector<std::string>& args) {
  pid_t pid = fork();
  
  if (pid == 0) {
    std::vector<char*> argv;
    for (const auto& arg : args) {
      argv.push_back(const_cast<char*>(arg.c_str()));
    }
    argv.push_back(nullptr);

    execv(path.c_str(), argv.data());
    std::cerr << "Failed to execute " << path << std::endl;
    exit(1);
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
  } else {
    std::cerr << "Fork failed" << std::endl;
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
    std::vector<std::string> args = parseCommand(command);


    if (!command.empty()) {
      std::string word = args[0];
      std::vector<std::string> ars(args.begin() + 1, args.end());
      if (word  == "exit") return 0;

      else if (word  == "echo") {
        handle_echo(ars);
      }

      else if (word == "type") {
        handle_type(ars, builtins);
      }
      else if (word == "pwd") {
        handle_pwd();
      }
      else if (word == "cd") {
        handle_cd(ars[0]);
      }

      else {
        // try to execute with the programme
        std::string full_path = find_in_path(word);
        std::string executable  = word;
        if (!full_path.empty()) {
          executeProgram(full_path, args);

        } else {
          std::cout << executable << ": command not found" << std::endl;
        }
      }
    }
  }
}
