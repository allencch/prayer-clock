bin_PROGRAMS = prayer-clock
prayer_clock_SOURCES = aceaster.h aceaster.cpp acprayer.h acprayer.cpp prayer-clock.cpp
prayer_clock_CPPFLAGS = $(GTK3_CFLAGS) $(GTKMM3_CFLAGS)

prayer_clock_LDADD = $(GTK3_LIBS) $(GTKMM3_LIBS) -lexpat

if MINGW
prayer_clock_LDFLAGS = -s -mwindows
endif

if NO_TO_TRAY
prayer_clock_CPPFLAGS += -DNO_TO_TRAY
endif
