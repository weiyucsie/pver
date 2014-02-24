#!/usr/local/bin/perl -w
use BerkeleyDB;

sub get_index_hash {
  my (%hash) = ();
  my ($bsd_ver) = split(/\./, `uname -r`, 2);
  $db  = tie (%hash, 'BerkeleyDB::Btree', { -Filename => 'INDEX.db', -Flags => DB_CREATE});
  open(H, '/usr/ports/INDEX-'.$bsd_ver) or die('INDEX read fail');
  while(($_ = <H>)) {
    if (!defined($_)) {
      last;
    }
    my(@sl) = split(/\|/);
    $sl[1] =~ s/^\/usr\/ports\///;
    $hash{$sl[0]} = $sl[1];
    $hash{$sl[1]} = $sl[0];
  }
  close(H);
  undef($db);
  untie(%hash);
}

get_index_hash();
