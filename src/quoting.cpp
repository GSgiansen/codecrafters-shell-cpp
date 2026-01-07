#include <iostream>
#include <string>
#include <sstream>
std::string  handle_singlequote(std::string quote) {
  std::string answer;
  for (char c : quote) {
    if (c == '\'') continue;
    answer = answer + c;
  }
  return answer;


}
