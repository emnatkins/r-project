@rem = '
@echo off
perl.exe %R_HOME%\bin\Rdconv.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
@rem ';
# Convert R documentation into HTML, LaTeX and text format

# Copyright (C) 1997 Friedrich Leisch
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the GNU
# General Public License for more details.
#
# A copy of the GNU General Public License is available via WWW at
# http://www.gnu.org/copyleft/gpl.html.	 You can also obtain it by
# writing to the Free Software Foundation, Inc., 59 Temple Place,
# Suite 330, Boston, MA  02111-1307  USA.

# Send any bug reports to Friedrich.Leisch@ci.tuwien.ac.at

use Getopt::Long;
# use File::Basename;
# use Cwd;

my $version = "1.14";

sub version {
  print STDERR <<END;
Rdconv $version
Copyright (C) 1997-2000 R Core Development Team
This is free software; see the GNU General Public Licence version 2
or later for copying conditions.  There is NO warranty.    
END
  exit 0;
}

sub usage {
  print STDERR <<END;
Usage: Rdconv.bat [options] FILE

Convert R documentation in FILE to other formats such as plain text,
HTML or LaTeX.

Options:
  -h, --help		print short help message and exit
  -v, --version		print version info and exit
  -d, --debug		turn on debugging (increase verbosity)
  -t, --type=TYPE	convert to format TYPE
  -o, --output-file=fn	use \`fn\' as the output file
  --os=NAME		use OS subdir \`NAME\' (unix, mac or windows)
  --OS=NAME		the same as --os.

Possible format specifications are txt (plain text), html, latex,
Sd (S documentation format), and example (extract R code in the examples).

The default is to send output to stdout, which is also given by -o -.
Using -o \"\" will choose an output filename by removing a .Rd extension
from FILE and adding a suitable extension.
  
Email bug reports to <r-bugs\@lists.r-project.org>.
END
  exit 0;
}

my $OSdir ="windows";

if($ENV{"R_HOME"}){
    $R_HOME=$ENV{"R_HOME"};
}
else{
    print "Must set R_HOME to use Rdconv\n";
    exit 1;
}

require "$R_HOME/etc/Rdconvlib.pl";

@knownoptions = ("debug|d", "type|t:s", "h|help", "v|version", 
		 "o|output-file:s", "os|OS:s");
GetOptions (@knownoptions) || &usage();
&version() if $opt_v;
&usage() if $opt_h;

$OSdir = $opt_os if $opt_os;
$out = 0;
if(defined $opt_o) {
    if(length($opt_o)) { $out = $opt_o; }
    else { $out = $ARGV[0]; $out =~ s/.Rd$//; }
}
$out = 0 if $out eq "-";

usage() if $opt_help;
print stderr "R_HOME: '$R_HOME';  cwd='$cwd'\n" if $opt_debug;

if($opt_type =~ /html/i){
    if(defined $opt_o && length($opt_o) == 0) { $out = $out . ".html"; }
    Rdconv($ARGV[0],$opt_type,$opt_debug, $out);
}
elsif($opt_type =~ /txt/i){
    Rdconv($ARGV[0],$opt_type,$opt_debug, $out);
}
elsif($opt_type =~ /Sd/i){
    if(defined $opt_o && length($opt_o) == 0) { $out = $out . ".d"; }
    Rdconv($ARGV[0],$opt_type,$opt_debug, $out);
}
elsif($opt_type =~ /latex/i){
    if(defined $opt_o && length($opt_o) == 0) { $out = $out . ".tex"; }
    Rdconv($ARGV[0],$opt_type,$opt_debug, $out);
}
elsif($opt_type =~ /example/i){
    if(defined $opt_o && length($opt_o) == 0) { $out = $out . ".R"; }
    Rdconv($ARGV[0],$opt_type,$opt_debug, $out);
} else {
    print STDERR "Unknown type: options are txt, html, latex, Sd, example\n";
}

### Local Variables: ***
### mode: perl ***
### End: ***
__END__
:endofperl
