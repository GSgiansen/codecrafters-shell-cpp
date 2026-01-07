
#include <iostream>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;



void handle_pwd() {
  fs::path currentPath = fs::current_path();
  std::cout << currentPath.string() << "\n";

}

void handle_cd(std::string newPath) {
  const char* path_env = std::getenv("HOME");
  std::string cpp_home = path_env;
  if (newPath == "~") {
    try {
      fs::current_path(cpp_home);
    } catch (fs::filesystem_error const& e) {
    }
  }
  else {
    try {
      fs::current_path(newPath);
    } catch(fs::filesystem_error const& e) {
        std::cerr << "cd: " << newPath << ": No such file or directory\n";
    }
  }
}

