#include <string>
using namespace std;

class Resources {
public:
  Resources();
  void solvePath();
  bool exists(const std::string& filename);

  std::string title;
  std::string prayerXml;
  std::string icon;
  std::string ui; // 在 Qt 中可能主要用于查找 .ui 文件名
};
