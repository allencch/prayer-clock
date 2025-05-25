# prayer-clock
Catholic prayers reminder, contains Angelus and 3 O'clock Prayer

Migrate from http://sourceforge.net/projects/prayerclock



## Prayer Clock Changelog

2025-05-25 5.00
Remake with Qt6, AI assisted.
Breaking change!! Remove `~/.local/share/prayer-clock/prayers.xml`, so that new version will be copied here.

2015-11-05  3.0.3  
Adding CXXFLAGS to the configure.ac

2015-04-23  3.0.2  
Allows click the icon to minimize and hide the window (toggle).

2015-04-09  3.0.1  
Fix the bug that cannot always minimize to tray.

2015-03-19  3.0.0  
Use the GTK3 instead. No more support for Windows.

2012-01-16  2.0.1  
Enable the configure to "--disable-minimize-to-tray" feature. This is because GTK+ does not work very well in Mac OS X. There is no system tray icon shown, once minimize, the window is missing. Disable it during configuration, so that window will not disable from the panel.

2012-01-05  2.0.0  
It is not a great change with this version for the interface, but the engine part
-   Using gtkmm instead of GTK+ (C language)
-   Object-oriented for the Easter calculation and the prayers.xml parsing
-   Use glade file and GtkBuilder for interface for RAD
-   Better rich text format parsing algorithm, now able to use "italic"
-   Automatically save the prayers.xml in the *user data folder*, we can customize our own prayers easier
-   With the prayer-clock.png, prayer-clock.glade, and prayers.xml in a same folder, we can make it *portable*


2011-08-11  1.0.0  
-   Change the design of the icon
-   Add the icon to the GTK window
-   Change the application file hierarchy, based Linux/Unix
-   In Linux, the desktop menu item is added
-   Using autotools (autoconf and automake) for the project, not writing the makefile manually
-   Add some more prayers: Magnificat and Memorare
-   Maintain Debian package, simplify pacman package
-   Using MinGW cross compiling environment (mingw-cross-env) for Windows version

2011-06-13  0.1.7  
-   Fix the Easter season calculation

2011-04-24  0.1.6  
-   Fix the Easter season calculation
-   Remove the changelog from the source, and merge with this file
-   Add command argument for date based on PyPrayer Clock
-   Using gtk_tree_view_set_cursor() when popup the Windows, so that the sidebar is highlighted

2011-04-22  0.1.5 build 4  
-   Fix the problem that the opening prayer is appended at the end.

2011-01-12  
-   Fix the problem of final tag in RTF.
-   Fix the multiple line tag problem.

2010-12-14  0.1.5 build 3  
-   Fix the time out that does not remove the old text

2010-12-12  0.1.5 build 2  
-   Fix the time out does not convert the markup language

2010-12-07  0.1.5  
-   Add markup language to the prayers to produce rich format text.
-   The status bar will show the prayer's name once the prayer is popup

2010-10-01  minor  
-   Create a new function: acGetTmToday()

2010-09-26  0.1.4  
-   Read the icon file and XML files relatively depends on the executable binary, not depends on the 
    working directory. This is to solve the Ubuntu desktop launcher problem. 

2010-07-30  
-   Easter calculation no more using GDate to reduce dependency, separate the Easter functions to another source.

2010-05-30  
-   Minor changes on the interface, that the prayer will always scroll to top when it is selected.
-   Also add a function for relative day from the Easter, based on JPrayer Clock.

2010-05-08  
-   Fix prayer pop up which do not update the prayer, because the the left pane prayer list

2010-05-07  
-   Add Novena prayers to the Holy Spirit. Redesign the interface.

2010-03-31  
-   Add Divine Mercy Novena Prayers

2010-03-29  
-   Add "prayers" item to the file menu so that the user can choose to display different prayers.

2010-03-28  
-   Change the algorithm so that do not load all the prayers into the memory, load only when needed

2010-03-07  
-   Add stauts bar, add Regina Caeli and check for the Easter Season, using UI Manager for menu bar 


2011-04-22  0.1.5 build 4  
- Fix the problem that the opening prayer is appended at the end.

2010-12-14  0.1.5 build 3  
- Fix the time out popup that does not clear the previous text

2010-12-12  0.1.5 build 2  
- Fix the time out does not convert the markup language

2010-12-07  0.1.5  
- Add markup language to the prayers to produce rich format text.
- The status bar will show the prayer's name once the prayer is popup

2010-10-01  minor  
- Create a new function: acGetTmToday()

0.1.4 (2010-09-26)  
- Read the icon file and XML files relatively depends on the executable binary, not depends on the working directory. This is to solve the Ubuntu desktop launcher problem. 
- Windows version: Compiled with MinGW GCC 4.5.0 (previously TDM-2 mingw 4.4.1)
- Windows version: Compiled with gtk+-bundle_2.20.0 (previously gtk+-bundle_2.18.5)

0.1.3 (2010-07-30)  
- Source: Reduce the dependency of GLib for Easter day calculation
- Prayers: Several prayers added

0.1.2 (2010-05-30)  
- Minor changes on the interface, that the prayer will always scroll to top when it is selected.
- Also add a function for relative day from the Easter, based on JPrayer Clock.

0.1.1 (2010-05-08)  
- Fix pop up which do not update the prayer, because of the left pane prayer list

0.1.0 (2010-05-07)  
- Add Novena prayers to the Holy Spirit
- Redesign the interface, by moving the prayers to the side pane.

0.0.8 (2010-04-24)  
- Fix the bugs for calculation of Easter Season

0.0.7 (2010-03-31)  
- Add Divine Mercy Novena Prayers

0.0.6 (2010-03-29)  
- Add "prayers" item to the file menu so that the user can choose to display different prayers.
- Change the algorithm so that do not load all the prayers into the memory, load only when needed

0.0.4 (2010-03-07)  
- Add in Regina Caeli for Easter Season
- Add in status bar
- Using GTK UI Manager instead of Item Factory

0.0.2 (20100209)  
- Add scrollbar to the window
- Add the unit of second to the time
- Fix the time of "3 o'clock prayer"
- Fix the window so that it is not always on top

0.0.1 (20100209)  
- First release
