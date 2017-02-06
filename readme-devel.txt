#There are for directories as distribution:
#bin/	#for Windows binary
#deb-bin/	#Debian binary
#arch/	#Arch Linux binary
#debian/	#Debian packaging

Using mingw-cross-env,
../configure --host=i686-pc-mingw32

To package,
1. Source
2. Arch - for both source and binary
3. Debian - use the Arch binary
4. Windows
