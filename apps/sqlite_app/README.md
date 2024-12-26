jkennedy@mango:~/workspace/cfs/libs/sqlite/sqlite-autoconf-3470000$ sudo make install
[sudo] password for jkennedy: 
make[1]: Entering directory '/home/jkennedy/workspace/cfs/libs/sqlite/sqlite-autoconf-3470000'
 /usr/bin/mkdir -p '/usr/local/lib'
 /bin/bash ./libtool   --mode=install /usr/bin/install -c   libsqlite3.la '/usr/local/lib'
libtool: install: /usr/bin/install -c .libs/libsqlite3.so.0.8.6 /usr/local/lib/libsqlite3.so.0.8.6
libtool: install: (cd /usr/local/lib && { ln -s -f libsqlite3.so.0.8.6 libsqlite3.so.0 || { rm -f libsqlite3.so.0 && ln -s libsqlite3.so.0.8.6 libsqlite3.so.0; }; })
libtool: install: (cd /usr/local/lib && { ln -s -f libsqlite3.so.0.8.6 libsqlite3.so || { rm -f libsqlite3.so && ln -s libsqlite3.so.0.8.6 libsqlite3.so; }; })
libtool: install: /usr/bin/install -c .libs/libsqlite3.lai /usr/local/lib/libsqlite3.la
libtool: install: /usr/bin/install -c .libs/libsqlite3.a /usr/local/lib/libsqlite3.a
libtool: install: chmod 644 /usr/local/lib/libsqlite3.a
libtool: install: ranlib /usr/local/lib/libsqlite3.a
libtool: finish: PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin:/sbin" ldconfig -n /usr/local/lib
----------------------------------------------------------------------
Libraries have been installed in:
   /usr/local/lib

If you ever happen to want to link against installed libraries
in a given directory, LIBDIR, you must either use libtool, and
specify the full pathname of the library, or use the '-LLIBDIR'
flag during linking and do at least one of the following:
   - add LIBDIR to the 'LD_LIBRARY_PATH' environment variable
     during execution
   - add LIBDIR to the 'LD_RUN_PATH' environment variable
     during linking
   - use the '-Wl,-rpath -Wl,LIBDIR' linker flag
   - have your system administrator add LIBDIR to '/etc/ld.so.conf'

See any operating system documentation about shared libraries for
more information, such as the ld(1) and ld.so(8) manual pages.
----------------------------------------------------------------------
 /usr/bin/mkdir -p '/usr/local/bin'
  /bin/bash ./libtool   --mode=install /usr/bin/install -c sqlite3 '/usr/local/bin'
libtool: install: /usr/bin/install -c sqlite3 /usr/local/bin/sqlite3
 /usr/bin/mkdir -p '/usr/local/include'
 /usr/bin/install -c -m 644 sqlite3.h sqlite3ext.h '/usr/local/include'
 /usr/bin/mkdir -p '/usr/local/share/man/man1'
 /usr/bin/install -c -m 644 sqlite3.1 '/usr/local/share/man/man1'
 /usr/bin/mkdir -p '/usr/local/lib/pkgconfig'
 /usr/bin/install -c -m 644 sqlite3.pc '/usr/local/lib/pkgconfig'
make[1]: Leaving directory '/home/jkennedy/workspace/cfs/libs/sqlite/sqlite-autoconf-3470000'
jkennedy@mango:~/workspace/cfs/libs/sqlite/sqlite-autoconf-3470000$ 

