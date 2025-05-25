#include <string>
using namespace std;

#include <QString>
#include <QDir>
#include <QFile>

#include "Resources.hpp"

Resources::Resources() {
  title = "Prayer Clock";

  prayerXml = "prayers.xml";
  icon = "prayer-clock.png";
  ui = "prayer-clock.ui";
}

void Resources::solvePath() {
  // 尝试查找 .ui 文件 (假设你已经手动创建了)
  if (QFile::exists(QString::fromStdString(ui))) {
    return;
  }

  // Qt 中获取用户数据目录的方式
  QString userDataDir = QDir::homePath() + "/.local/share/prayer-clock"; // 类似 Glib::get_user_data_dir()
  QDir dir(userDataDir);
  if (!dir.exists()) {
    dir.mkpath("."); // 创建目录
  }
  QString dest = userDataDir + "/prayers.xml";

  while (!QFile::exists(dest)) {
    // 查找 prayers.xml
    QString findXml = "../share/prayer-clock/prayers.xml";
    if (QFile::exists(findXml)) {
      QFile::copy(findXml, dest);
      break;
    }
    findXml = "/usr/local/share/prayer-clock/prayers.xml";
    if (QFile::exists(findXml)) {
      QFile::copy(findXml, dest);
      break;
    }
    findXml = "/usr/share/prayer-clock/prayers.xml";
    if (QFile::exists(findXml)) {
      QFile::copy(findXml, dest);
      break;
    }
  }
  prayerXml = dest.toStdString();

  // 查找 .ui 和 icon 文件 (假设你的 .ui 文件名和 glade 一致)
  string app = "../share/prayer-clock/";
  string pix = "../share/pixmaps/";
  if (QFile::exists(QString::fromStdString(app + ui))) {
    ui = (app + ui);
    icon = (pix + icon);
    return;
  }

  app = "/usr/local/share/prayer-clock/";
  pix = "/usr/local/share/pixmaps/";
  if (QFile::exists(QString::fromStdString(app + ui))) {
    ui = (app + ui);
    icon = (pix + icon);
    return;
  }

  app = "/usr/share/prayer-clock/";
  pix = "/usr/share/pixmaps/";
  if (QFile::exists(QString::fromStdString(app + ui))) {
    ui = (app + ui);
    icon = (pix + icon);
    return;
  }
}

bool Resources::exists(const std::string& filename) {
  return QFile::exists(QString::fromStdString(filename));
}
