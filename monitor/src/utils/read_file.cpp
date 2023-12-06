#include "utils/read_file.h"

namespace monitor {

//读取一行的文件数据，并对一行的数据进行对空格进行切割
bool ReadFile::ReadLine(std::vector<std::string>* args) {
  std::string line;
  std::getline(ifs_, line);
  if (ifs_.eof() || line.empty()) {//当前字符串是空，或者读取到结尾了
    return false;
  }

  std::istringstream line_ss(line);//从string对象中读取字符，自动帮我们对空格切分
  while (!line_ss.eof()) {//没有读取到结尾
    std::string word;
    line_ss >> word;//一次放到word里面，并存入到args中
    args->push_back(word);
  }
  return true;
}


}  // namespace monitor