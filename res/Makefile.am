datadir = @datadir@/@PACKAGE@
data_DATA = prayers.dtd prayers.xml prayer-clock.png prayer-clock.desktop prayer-clock.glade
APPS_DIR=$(datadir)/../applications
ICONS_DIR=$(datadir)/../pixmaps

install-data-hook:
	mkdir -p $(APPS_DIR)
	mkdir -p $(ICONS_DIR)
	mv $(datadir)/prayer-clock.desktop $(APPS_DIR)
	mv $(datadir)/prayer-clock.png $(ICONS_DIR)
	
