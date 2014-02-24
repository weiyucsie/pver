初始化編譯設定：
預設db名稱為/usr/ports/INDEX-{OS主要版本編號}.db
如/usr/ports/INDEX-7.db
可直接使用portupgrade建立的database

make INDEX_DB_FILE="INDEX.db"
指定INDEX.db為database檔案
使用pver前先使用pver_dbgen產生對應的database檔案
預設為INDEX_DB_FILE所指定的檔案，
pver_dbgen後面第一個參數也可以指定database檔案名稱

make -DSORT
產生結果依照package名稱排序

make -DBSD_DB
使用FreeBSD內建的BerkeleyDB 1.85來編譯

make -DPKGNG
需要sqlite3
讀取 LOCAL_SQLITE_PATH ( PKGDIR "local.sqlite" ) 來取得本地的 package 列表

make clean
清除所有產生的檔案(含database檔案)

Initial compiling setting:
Default database filename is /usr/port/INDEX-{Major OS Number}.db,
for example, /usr/ports/INDEX-7.db
You can use the database file created by portupgrade, too.

make INDEX_DB_FILE="INDEX.db"
Specify INDEX.db as the database file.
Use pver_dbgen to generate the database file before using pver.
The default database file is the file specified by INDEX_DB_FILE,
it is possible to specify the database filename by the first argument of pver_dbgen.

make -DSORT
Sort by the package name

make -DBSD_DB
Compile files by the built-in Berkeley 1.85

make -DPKGNG
Need sqlite3
Read the file LOCAL_SQLITE_PATH ( PKGDIR "local.sqlite" ) to get the local package list

make clean
Clean all the generated files(including database files)
