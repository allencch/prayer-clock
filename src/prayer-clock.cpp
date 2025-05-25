#include "prayer.h"
#include "easter.h"
#include "Resources.hpp"
#include "prayer-clock.hpp"

#include <QScreen>

int g_argc;
char** g_argv;

Resources g_res;
// #include "ui_mainwindow.h" // 假设你使用 Qt Designer 创建了 mainwindow.ui

PrayerClock::PrayerClock(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::MainWindow), prayers(new Prayers(g_res.prayerXml.c_str())), easterSeason(false) {
  ui->setupUi(this); // 设置从 mainwindow.ui 加载的界面

  mainWindow = this; // 将 mainWindow 指向自身
  prayerView = ui->prayerView; // 假设在 .ui 文件中 prayerView 的 objectName 是 prayerView
  statusBar = ui->statusbar;   // 假设在 .ui 文件中 statusbar 的 objectName 是 statusbar

  // 获取 ListView 和 Model
  QListView* listView = ui->prayersListView; // 假设在 .ui 文件中 prayersListView 的 objectName 是 prayersListView
  prayerModel = new QStandardItemModel(this);
  listView->setModel(prayerModel);
  selectModel = listView->selectionModel();

  // 处理命令行参数 (如果需要)
  QList<QString> args = QApplication::arguments();
  DateTime today;
  int argDate = 0;
  int argYear = 0, argMonth = 0, argDay = 0;
  for (int i = 1; i < args.size(); ++i) {
    if (args[i] == "-d" && i + 1 < args.size()) {
      argDate = args[i + 1].toInt();
      argYear = argDate / 10000;
      argMonth = argDate / 100 - argYear * 100;
      argDay = argDate - (argYear * 10000 + argMonth * 100);
      today.setDate(argYear, argMonth, argDay);
      break;
    }
  }
  if (argDate == 0) {
    today.getToday();
  }

  // 检查复活节季节
  Easter easter;
  easterSeason = easter.isEasterSeason(today.year, today.month, today.day);
  int diffEaster = easter.getDayFromEaster(today.year, today.month, today.day);

  // 设置标题和图标
  setWindowTitle(QString::fromStdString(g_res.title));
  setWindowIcon(QIcon(QString::fromStdString(g_res.icon)));

  // 加载祈祷文到左侧列表
  loadPrayersToListView();
  listView->setCurrentIndex(prayerModel->index(0, 0)); // 滚动到第一个条目

  // 连接列表点击信号
  connect(selectModel, &QItemSelectionModel::currentChanged, this, &PrayerClock::showPrayer);

  // 连接退出信号
  QAction* quitAction = ui->menuQuit; // 假设在 .ui 文件中 menuQuit 的 objectName 是 menuQuit
  connect(quitAction, &QAction::triggered, this, &PrayerClock::quitApp);

  // 显示默认祈祷文
  if (!prayers->list.empty()) {
    prayerView->setHtml(QString::fromStdString(prayers->list[0]->text));
  }

  // 显示当前时间
  updateTimeDisplay();

  // 根据特殊节日显示祈祷文
  checkSpecialOccasion(today);

  // 定时更新时间
  QTimer* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &PrayerClock::timeout);
  timer->start(1000);

  // 系统托盘图标
  trayIcon = new QSystemTrayIcon(QIcon(QString::fromStdString(g_res.icon)), this);
  trayIcon->setVisible(true);
  connect(trayIcon, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
      trayClicked();
    }
  });

  // 最小化信号
  // connect(this, &QMainWindow::windowStateChanged, this, &PrayerClock::minimizeWindow);

  // 关于对话框
  aboutDialog = new QDialog(this);
  aboutDialog->setWindowTitle("About");
  QVBoxLayout* aboutLayout = new QVBoxLayout(aboutDialog);
  QLabel* authorLabel = new QLabel("Catholic Prayer Clock");
  aboutLayout->addWidget(authorLabel);
  QPushButton* okButton = new QPushButton("OK");
  aboutLayout->addWidget(okButton);
  connect(okButton, &QPushButton::clicked, aboutDialog, &QDialog::close);
  QAction* aboutAction = ui->menuAbout; // 假设在 .ui 文件中 menuAbout 的 objectName 是 menuAbout
  connect(aboutAction, &QAction::triggered, this, &PrayerClock::aboutDialogShow);
  connect(aboutDialog, &QDialog::finished, this, &PrayerClock::aboutDialogClosed);

  // 删除事件
  connect(this, &QMainWindow::destroyed, this, &PrayerClock::quitApp); // 或者在 closeEvent 中处理

  // Centering window
  QScreen *screen = QApplication::primaryScreen();
  QRect screenGeometry = screen->geometry();
  int x = (screenGeometry.width() - width()) / 2;
  int y = (screenGeometry.height() - height()) / 2;
  move(x, y);
}

PrayerClock::~PrayerClock() {
  delete prayers;
  delete ui;
}

void PrayerClock::loadPrayersToListView() {
  prayerModel->clear();
  for (const auto& prayer : prayers->list) {
    QStandardItem* item = new QStandardItem(QString::fromStdString(prayer->title));
    prayerModel->appendRow(item);
  }
}

void PrayerClock::showPrayer() {
  QModelIndex currentIndex = ui->prayersListView->currentIndex();
  if (currentIndex.isValid()) {
    QString selectedPrayerTitle = prayerModel->item(currentIndex.row())->text();
    for (const auto& prayer : prayers->list) {
      if (QString::fromStdString(prayer->title) == selectedPrayerTitle) {
        prayerView->setHtml(QString::fromStdString(prayer->text));
        statusBar->showMessage(QString::fromStdString(prayer->title));
        break;
      }
    }
  }
}

void PrayerClock::quitApp() {
  QApplication::quit();
}

void PrayerClock::timeout() {
  updateTimeDisplay();
  QTime currentTime = QTime::currentTime();
  for (int i = 0; i < prayers->list.size(); ++i) {
    if (easterSeason && prayers->list[i]->title == "Angelus") continue;
    if (!easterSeason && prayers->list[i]->title == "Regina Caeli") continue;

    for (const auto& timeStr : prayers->list[i]->time) {
      QTime prayerTime = QTime::fromString(QString::fromStdString(timeStr), "h:m:s");
      if (currentTime.hour() == prayerTime.hour() && currentTime.minute() == prayerTime.minute() && currentTime.second() == prayerTime.second()) {
        ui->prayersListView->setCurrentIndex(prayerModel->index(i, 0));
        trayClicked(); // 激活托盘图标
      }
    }
  }
}

void PrayerClock::trayClicked() {
  if (isVisible()) {
    hide();
  } else {
    showNormal();
    activateWindow(); // 确保窗口获得焦点
  }
}

// void PrayerClock::minimizeWindow(QWindowStateChangeEvent* event) {
//   if (event->oldState() != Qt::WindowMinimized && windowState() == Qt::WindowMinimized) {
//     hide();
//   }
// }
void PrayerClock::changeEvent(QEvent *event) {
  if (event->type() == QEvent::WindowStateChange) {
    QWindowStateChangeEvent *stateChangeEvent = static_cast<QWindowStateChangeEvent*>(event);
    if (stateChangeEvent->oldState() != Qt::WindowMinimized && windowState() == Qt::WindowMinimized) {
      // 窗口从非最小化状态变为最小化状态
      hide(); // 隐藏窗口
    }
    // 如果你需要处理其他状态，可以在这里添加更多条件
    // else if (stateChangeEvent->oldState() == Qt::WindowMinimized && windowState() == Qt::WindowNoState) {
    //     // 窗口从最小化恢复到正常状态
    // }
  }
  QMainWindow::changeEvent(event); // 调用基类的实现，确保其他事件被正确处理
}

void PrayerClock::aboutDialogShow() {
  aboutDialog->exec();
}

void PrayerClock::aboutDialogClosed(int result) {
  aboutDialog->hide();
}

bool PrayerClock::deleteEventHandler(QCloseEvent* event) {
  quitApp();
  return true; // 接受关闭事件
}

void PrayerClock::updateTimeDisplay() {
  QDateTime currentDateTime = QDateTime::currentDateTime();
  statusBar->showMessage(currentDateTime.toString());
}

void PrayerClock::checkSpecialOccasion(const DateTime& today) {
  Easter easter;
  int diffEaster = easter.getDayFromEaster(today.year, today.month, today.day);
  for (int i = 0; i < prayers->list.size(); ++i) {
    if (prayers->list[i]->easter_f && prayers->list[i]->easter == diffEaster) {
      ui->prayersListView->setCurrentIndex(prayerModel->index(i, 0));
      ui->prayersListView->scrollTo(prayerModel->index(i, 0));
      break;
    }
  }
}

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  // 查找资源文件
  g_res.solvePath();

  PrayerClock prayerClock;
  prayerClock.show(); // 显示主窗口

  return app.exec();
}
