@rem = '
@echo off
perl.exe %R_HOME%\bin\Rdindex.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
@rem ';
# Extract R object INDEX information from Rd files.
# Usage: Rdindex [-l x] [-r y] file_1 ... file_n
# The titles are nicely formatted between the columns x and y (defaults
# are 25 and 72, respectively).

# Copyright (C) 1997-2000 Kurt Hornik and Fritz Leisch
#
# This document is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# A copy of the GNU General Public License is available via WWW at
# http://www.gnu.org/copyleft/gpl.html.  You can also obtain it by
# writing to the Free Software Foundation, Inc., 59 Temple Place,
# Suite 330, Boston, MA  02111-1307  USA.

use Getopt::Long;

my $revision = ' $Revision: 1.1.2.1 $ ';
my $version;
my $name;

$revision =~ / ([\d\.]*) /;
$version = $1;
($name = $0) =~ s|.*/||;

sub version {
  print STDERR <<END;
Rdindex $version
Copyright (C) 1997-2000 Kurt Hornik and Fritz Leisch
This is free software; see the GNU General Public Licence version 2
or later for copying conditions.  There is NO warranty.    
END
  exit 0;
}

sub usage {
  print STDERR <<END;
Usage: Rdindex.bat [options] FILES

Create an index table from FILES in R documentation format.  The titles
are nicely formatted between two column positions (defaults are 25 and
72, respectively).

Options:
  -h, --help            print short help message and exit
  -v, --version         print version info and exit
  -l, --left=X		make X the left column for title formatting
  -r, --right=Y		make Y the right column for title formatting

Email bug reports to <r-bugs\@lists.r-project.org>.
END
  exit 0; 
}

GetOptions(("h|help", "v|version", "l|left=s","r|right=s")) || &usage();

&version() if $opt_v;
&usage() if $opt_h;
$opt_l = 25 unless $opt_l;
$opt_r = 72 unless $opt_r;

my $l1 = $opt_l - 2;
my $l2 = $opt_r - $opt_l;

my $cmd = "format short =\n";
$cmd .= "@" . rep("<", $l1) . " ^" . rep("<", $l2) . "\n";
$cmd .= "\$name, \$desc\n";
$cmd .= ".\n";

eval($cmd);

$cmd = "format long =\n";
$cmd .= "@" . rep("<", $opt_r) . "\n";
$cmd .= "\$name\n";
$cmd .= ".\n";

eval($cmd);

$cmd = "format cont =\n";
$cmd .= "~" . rep(" ", $l1) . " ^" . rep("<", $l2) . "\n";
$cmd .= "\$desc\n";
$cmd .= ".\n";

eval($cmd);
    
undef $/;

foreach $arg (@ARGV) {
  foreach $file (glob($arg)) {
  if (!open(FILE, $file)) {
    print STDERR "File $file not found\n";
  }
  else {
    $_ = <FILE>;
    m/\\name\{([^\}]*)\}/;
    $name = $1;
    $l = length($name);
    if($l <= $l1){
	$~ = short;
    }
    else{
	$~ = long;
    }
    /\\title\{\s*([^\}]+)\s*\}/s;
    $desc = $1;
    write;
    $~ = cont;
    while($desc){
	write;
    }
  }
}
}

sub rep {
    my $retval = "";
    for($k=0;$k<$_[1];$k++){
	$retval .= $_[0];
    }
    $retval;
}

### Local Variables: ***
### mode: perl ***
### perl-indent-level: 4 ***
### End: ***
__END__
:endofperl
