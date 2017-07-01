#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <gtkmm.h>

#include "prayer.h"
#include "easter.h"

using namespace std;

int g_argc;
char** g_argv;


class Resources {
public:
  Resources() {
    title = "Prayer Clock";

    prayerXml = "prayers.xml";
    icon = "prayer-clock.png";
    glade = "prayer-clock.glade";
  }

  void solvePath() {
    //Using glade as the default standard (portable)
    if(exists(glade)) {
      return;
    }

    //Else non-portable, then copy prayers.xml to the user data dir
    //C++ cannot do the file manipulation
    string userDir = Glib::get_user_data_dir() + "/prayer-clock";
    if(!Glib::file_test(userDir,Glib::FILE_TEST_EXISTS|Glib::FILE_TEST_IS_DIR)) {
      Glib::RefPtr<Gio::File> xml = Gio::File::create_for_path(userDir);
      xml->make_directory_with_parents();
    }
    string dest = userDir + "/prayers.xml";

    while(!exists(dest)) {
      //Finding the prayers.xml
      string findXml = "../share/prayer-clock/prayers.xml";
      if(exists(findXml)) {
        Gio::File::create_for_path(findXml)->copy(Gio::File::create_for_path(dest));
        break;
      }
      findXml = "/usr/local/share/prayer-clock/prayers.xml";
      if(exists(findXml)) {
        Gio::File::create_for_path(findXml)->copy(Gio::File::create_for_path(dest));
        break;
      }
      findXml = "/usr/share/prayer-clock/prayers.xml";
      if(exists(findXml)) {
        Gio::File::create_for_path(findXml)->copy(Gio::File::create_for_path(dest));
        break;
      }
    }

    prayerXml = dest;


    //Check from the relative path, based on Unix structure
    //this works on Windows also
    string app = "../share/prayer-clock/";
    string pix = "../share/pixmaps/";
    if(exists( app + glade)) {
      glade = app + glade;
      //prayerXml = app + prayerXml;
      icon = pix + icon;
      return;
    }

    //Check from absolute path in local
    app = "/usr/local/share/prayer-clock/";
    pix = "/usr/local/share/pixmaps/";
    if(exists(app + glade)) {
      glade = app + glade;
      //prayerXml = app + prayerXml;
      icon = pix + icon;
      return;
    }

    //Check from absolute path in usr
    app = "/usr/share/prayer-clock/";
    pix = "/usr/share/pixmaps/";
    if(exists(app + glade)) {
      glade = app + glade;
      //prayerXml = app+prayerXml;
      icon = pix + icon;
      return;
    }

  }

  bool exists(string filename) {
    ifstream f(filename.c_str());
    if(f) {
      f.close();
      return true;
    }
    return false;
  }

  string title;
  string prayerXml;
  string icon;
  string glade;
};

Resources g_res;

class StringToTextbufferConverter {
public:
  StringToTextbufferConverter(string t,Glib::RefPtr<Gtk::TextBuffer> b) {
    text = t;
    buffer = b;

    //Clear text
    buffer->set_text("");

    //Convert string to textbuffer based on tag
    for(int i=0;i<text.size();i++) {
      if(!isToken(text[i])) {
        //Gtk::TextIter end = buffer->get_iter_at_offset(-1);

        buffer->insert_at_cursor(charToString(text[i]));
      }
      else {
        //Record the iter and the tag
        i = getTag(text,i);
      }
    }
  }

private:
  bool isToken(char c) {
    bool res = false;
    switch(c) {
    case '[':
      res = true;
      break;
    case ']':
      res = true;
      break;
    }
    return res;
  }

  int getTag(string& s,int i) {
    bool close = false;
    ++i; //by pass "["

    //Read until "]"
    int j = s.find_first_of("]",i) -i;
    if(isClose(s[i])) {
      tags.back().end = buffer->create_mark(buffer->get_iter_at_offset(-1));

      //Apply tag
      buffer->apply_tag_by_name(tags.back().name,tags.back().start->get_iter(),tags.back().end->get_iter());
      tags.pop_back();
    }
    else {
      Tag tag;
      tag.name = s.substr(i,j);
      tag.start = buffer->create_mark(buffer->get_iter_at_offset(-1));
      tags.push_back(tag);
    }
    i += j;


    return i;
  }

  bool isClose(char c) {
    bool res = false;
    if(c == '/')
      res = true;
    return res;
  }

  string charToString(char c) {
    string res;
    res = string(&c,1);
    return res;
  }



  string text;
  Glib::RefPtr<Gtk::TextBuffer> buffer;

  struct Tag {
    string name;
    Glib::RefPtr<Gtk::TextMark> start;
    Glib::RefPtr<Gtk::TextMark> end;
  };

  std::vector<Tag> tags;
};

class PrayerClock : public Gtk::Window {
public:
  PrayerClock(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder) : Gtk::Window(cobject) {
    //Process options
    int argDate = 0;
    int argYear,argMonth,argDay;
    for(int i=0;i<g_argc;i++) {
      if(strcmp(g_argv[i],"-d") == 0) {
        sscanf(g_argv[i+1],"%d",&argDate);
        argYear = argDate / 10000;
        argMonth = argDate / 100 - argYear*100;
        argDay = argDate - (argYear* 10000 + argMonth*100);
      }
    }

    DateTime today;
    if(argDate) {
      today.setDate(argYear,argMonth,argDay);
    }
    else {
      today.getToday();
    }

    //Check easter season
    Easter easter;
    easterSeason = easter.isEasterSeason(today.year,today.month,today.day);

    //Get easter date
    int diffEaster = easter.getDayFromEaster(today.year,today.month,today.day);


    //Get widgets
    refBuilder->get_widget("prayerView",prayerView);
    refBuilder->get_widget("mainWindow",mainWindow);
    refBuilder->get_widget("aboutDialog",aboutDialog);
    refBuilder->get_widget("statusbar",statusbar);


    //Find the resource files

    //Set title
    mainWindow->set_title(g_res.title);
    mainWindow->set_icon_from_file(g_res.icon);

    //Load prayers
    prayers = new Prayers(g_res.prayerXml.c_str());

    //List prayers in the left pane
    refBuilder->get_widget("prayersTreeView",treeview);
    Glib::RefPtr<Gtk::TreeModel> treemodel = treeview->get_model();

    Glib::RefPtr<Glib::Object> gobj = refBuilder->get_object("prayerModel");
    Glib::RefPtr<Gtk::ListStore> prayerModel = Glib::RefPtr<Gtk::ListStore>::cast_static(gobj);

    for(int i=0;i<prayers->list.size();i++) {
      Gtk::TreeIter iter = prayerModel->append();
      iter->set_value<string>(0,prayers->list[i]->title);
    }
    treeview->scroll_to_row(Gtk::TreePath("0"));


    //Connect click signal from the list
    select = treeview->get_selection();
    select->signal_changed().connect(sigc::mem_fun(*this,&PrayerClock::showPrayer));


    //Connect quit signal
    Glib::RefPtr<Gtk::ImageMenuItem> quitAction = Glib::RefPtr<Gtk::ImageMenuItem>::cast_static(refBuilder->get_object("menuQuit"));
    quitAction->signal_activate().connect(sigc::mem_fun(*this,&PrayerClock::quit));


    //Show the default prayer
    StringToTextbufferConverter(prayers->list[0]->text,prayerView->get_buffer());

    //Get the time
    string strToday = today.toString();

    //Display time
    statusbar->push(strToday);


    //Display prayer based on special occassion
    for(int i=0;i<prayers->list.size();i++) {
      if(prayers->list[i]->easter_f && prayers->list[i]->easter == diffEaster) {
        char str[10];
        sprintf(str,"%d",i);
        treeview->set_cursor(Gtk::TreePath(str));
        treeview->scroll_to_row(Gtk::TreePath(str));
      }
    }

    //Update prayer based on time
    Glib::signal_timeout().connect(sigc::mem_fun(*this,&PrayerClock::timeout),1000);

    //Tray icon signal
    Glib::RefPtr<Gtk::StatusIcon> trayicon = Glib::RefPtr<Gtk::StatusIcon>::cast_static(refBuilder->get_object("trayicon"));
    trayicon->set_from_file(g_res.icon); //Override the glade file, so that can use different path

    trayicon->signal_button_press_event().connect(sigc::mem_fun(*this,&PrayerClock::clickSignal));

#ifndef NO_TO_TRAY
    //Minimize signal
    mainWindow->signal_window_state_event().connect(sigc::mem_fun(*this,&PrayerClock::minimize));
#endif


    //About dialog
    Glib::RefPtr<Gtk::ImageMenuItem> about = Glib::RefPtr<Gtk::ImageMenuItem>::cast_static(refBuilder->get_object("menuAbout"));
    about->signal_activate().connect(sigc::mem_fun(*this,&PrayerClock::about));
    aboutDialog->signal_response().connect(sigc::mem_fun(*this,&PrayerClock::dialogDestroy));

    //Signal on delete event
    mainWindow->signal_delete_event().connect(sigc::mem_fun(*this,&PrayerClock::deleteEvent));
  }
  virtual ~PrayerClock() {
    delete prayers;
  }


protected:
  void showPrayer() {
    Gtk::TreeIter iter = select->get_selected();
    string selectedPrayer;
    Gtk::TreeRow row = *iter;
    row.get_value<string>(0,selectedPrayer);

    //Show the prayer text to the textview
    for(int i=0;i<prayers->list.size();i++) {
      if(prayers->list[i]->title == selectedPrayer) {
        Glib::RefPtr<Gtk::TextBuffer> buffer = prayerView->get_buffer();
        //buffer->set_text(prayers->list[i]->text);
        StringToTextbufferConverter(prayers->list[i]->text,prayerView->get_buffer());

        //Change the status bar
        statusbar->pop();
        statusbar->push(prayers->list[i]->title);

        break;
      }
    }
  }

  void quit() {
    Gtk::Main::quit();
  }

  bool timeout() {
    //Timing
    time_t rawtime;
    time(&rawtime);

    struct tm* timeinfo;
    timeinfo = localtime(&rawtime);
    for(int i=0;i<prayers->list.size();i++) {
      if(easterSeason) {
        //Skip Angelus
        if(prayers->list[i]->title == "Angelus")
          continue;
      }
      else {
        if(prayers->list[i]->title == "Regina Caeli") {
          continue;
        }
      }
      for(int j=0;j<prayers->list[i]->time.size();j++) {
        int hour,minute,second;
        sscanf(prayers->list[i]->time[j].c_str(),"%d:%d:%d",&hour,&minute,&second);

        if(timeinfo->tm_hour == hour && timeinfo->tm_min == minute && timeinfo->tm_sec == second) {
          char str[10];
          sprintf(str,"%d",i);
          treeview->set_cursor(Gtk::TreePath(str));

          //Activate the tray
          trayClicked();
        }
      }
    }
    return true;
  }

  void trayClicked() {
    //toggle hide and show
    if(mainWindow->is_visible()) {
      mainWindow->iconify();
      mainWindow->hide();
    }
    else {
      mainWindow->deiconify();
      mainWindow->present();
    }
  }

  gboolean clickSignal(GdkEventButton *evt) {
    trayClicked();
    return true;
  }

  bool minimize(GdkEventWindowState* event) {
    if(event->changed_mask & GDK_WINDOW_STATE_ICONIFIED &&
       (event->new_window_state & GDK_WINDOW_STATE_ICONIFIED)) {
      mainWindow->hide();
    }
    return true;
  }

  void about() {
    aboutDialog->show();
  }
  void dialogDestroy(int id) {
    aboutDialog->hide();
  }

  bool deleteEvent(GdkEventAny* event) { //Because not using run(), so, need to override the delete event, else it will be only minimize
    quit();
    return false;
  }

  Prayers* prayers;
  Gtk::TreeView* treeview;
  Glib::RefPtr<Gtk::TreeSelection> select;
  Gtk::TextView* prayerView;
  Gtk::Window* mainWindow;
  Gtk::Dialog* aboutDialog;
  Gtk::Statusbar* statusbar;
  bool easterSeason;

};

int main(int argc, char** argv) {
  g_argc = argc;
  g_argv = argv;


  //Find the files based on path
  Gtk::Main kit(argc,argv);
  PrayerClock* prayerclock;

  //Find the resources
  g_res.solvePath();


  Glib::RefPtr<Gtk::Builder> builder;
  try {
    builder = Gtk::Builder::create_from_file(g_res.glade);
  }
  catch(const Glib::FileError& e) {
    cout<<e.what()<<endl;
  }
  catch(const Glib::MarkupError& e) {
    cout<<e.what()<<endl;
  }
  catch(const Gtk::BuilderError& e) {
    cout<<e.what()<<endl;
  }

  builder->get_widget_derived("mainWindow",prayerclock);

  prayerclock->show(); //Show the program
  Gtk::Main::run(); //Do not pass the *prayerclock as an argument, else hide() will close the program

  return 0;
}
