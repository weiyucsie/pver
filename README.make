��l�ƽsĶ�]�w�G
�w�]db�W�٬�/usr/ports/INDEX-{OS�D�n�����s��}.db
�p/usr/ports/INDEX-7.db
�i�����ϥ�portupgrade�إߪ�database

make INDEX_DB_FILE="INDEX.db"
���wINDEX.db��database�ɮ�
�ϥ�pver�e���ϥ�pver_dbgen���͹�����database�ɮ�
�w�]��INDEX_DB_FILE�ҫ��w���ɮסA
pver_dbgen�᭱�Ĥ@�ӰѼƤ]�i�H���wdatabase�ɮצW��

make -DSORT
���͵��G�̷�package�W�ٱƧ�

make -DBSD_DB
�ϥ�FreeBSD���ت�BerkeleyDB 1.85�ӽsĶ

make -DPKGNG
�ݭnsqlite3
Ū�� LOCAL_SQLITE_PATH ( PKGDIR "local.sqlite" ) �Ө��o���a�� package �C��

make clean
�M���Ҧ����ͪ��ɮ�(�tdatabase�ɮ�)

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
