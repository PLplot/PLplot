#!/usr/bin/perl -w
#
# Upload the deb files for PLplot to the plplot.sf.net web site
#
# Copyright (C) 2004  Rafael Laboissiere
#
# This script is part of PLplot
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published by
# the Free Software Foundation; version 2 of the License.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

sub env_default {
    my $envvar = shift;
    my $default = shift;
    $value = defined $ENV{$envvar} ? $ENV{$envvar} : $default;
    eval qq{\$$envvar = "$value"};
}

env_default ("WWW_USER", "rlaboiss");
env_default ("HOST", "shell.sf.net");
env_default ("WWW_HOST", "plplot.sf.net");
env_default ("WWW_DIR", "/home/groups/p/pl/plplot/htdocs");
env_default ("APT_DIR", "debian");
env_default ("GPGKEY", "0x4A5D72FE");

$debver = `cat /etc/debian_version`;
chomp $debver;

$md5sum = "md5sum.txt";
$md5sum_asc = "$md5sum.asc";

system "touch override";
system "dpkg-scanpackages . override | gzip -c > Packages.gz";
system "dpkg-scansources . override | gzip -c > Sources.gz";

@files = ("Release")
    if -f "Release";
@files = (@files, "Packages.gz", "Sources.gz");
@desc = ();
%sources = ();
%pkgs = ();
for $i (split ("\n\n", `gunzip -c Packages.gz`)) {
    $i =~ /Package:\s*([^\s]+)/;
    $n = $1;
    chomp $n;
    $i =~ /Version:\s*(\d+:)*([^\s]+)/;
    $v = $2;
    chomp $v;
    $pkgs{$n} = $v;
    $txt = "$n.txt";
    open (DESC, "> $txt");
    print DESC $i;
    close DESC;
    push (@desc, $txt);
    $src = ($i =~ /Source: (.*)/) ? $1 : $n;
    if (not defined $sources{$src}) {
      $sources{$src} = 1;
      open (CHG, (glob "${src}_$v*.changes")[0]);
      while (<CHG>) {
        @f = split (" ", $_);
        if ((scalar @f == 5) and ($f[0] =~ /[a-f\d]{32}/)) {
          push (@files, $f[4]);
        }
      }
    }
}

open (CSUM, "> $md5sum");
print CSUM map {`md5sum $_`} @files;
close CSUM;
unlink $md5sum_asc;
system
  "gpg --armor --detach-sign --default-key $GPGKEY --output $md5sum_asc $md5sum";

$index = "index.html";
open (INDEX, "> $index");

print INDEX <<EOF
<?xml version="1.0" encoding="iso-8859-1"?>
<!DOCTYPE html
     PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>Apt-getable repository for PLplot (Debian version $debver)</title>
<style type="text/css">
  body { color: black; background-color: white; }
  h1 { font-size: 1.5em; border-bottom: solid black 1px;
       color: #005a9c; }
  dt { font-size: 1.1em; font-weight: bold;
       color: rgb(136,0,0);
       margin-top: 1em; margin-bottom: 0.5em; }
  code { color: inherit; background-color: #e0e0e0; }
  table.pkg { border-collapse: collapse;
              border-spacing: 0px;
              border: solid #a0a0a0 1px; }
  table.footer { border-top: solid black 1px; font-size: 0.8em; }
  td.pkg { border: solid #a0a0a0 1px;
           padding: 4px; }
  th.pkg { border: solid #a0a0a0 1px;
           color: white; background-color: #a0a0a0;
           padding: 4px; }
</style>
</head>
<body>
<h1>Apt-getable Repository for PLplot (Debian version $debver)</h1>
<dl>
<dt> Available packages: </dt>
<dd>
<table class="pkg">
<tr><th class="pkg">name</th><th class="pkg">version</th></tr>
EOF
;

foreach $p (sort keys %pkgs) {
print "$p\n";
  print INDEX <<EOF
<tr><td class="pkg"><a href="$p.txt">$p</a></td>
<td class="pkg">$pkgs{$p}</td></tr>
EOF
;
}

print INDEX <<EOF
</table>
</dd>
<dt> /etc/apt/sources.list lines: </dt>

<dd>
<code>deb http://$WWW_HOST/$APT_DIR ./&nbsp;&nbsp;&nbsp;&nbsp;</code>
<br/>
<code>deb-src http://$WWW_HOST/$APT_DIR ./</code>
</dd>

<dt> Files: </dt>
<dd>
EOF
;

print INDEX map {
  $size = `du -bh $_`;
  chomp $size;
  $size =~ s/\s*([\dMKk]+)\s+.*/$1/;
  $size .= " bytes" if not ($size =~ /[MKk]/);
  "<a href=\"$_\">$_</a> ($size)\n <br/>";
} @files;

$fingerprint = `gpg --list-keys --fingerprint $GPGKEY | grep fingerprint`;

print INDEX <<EOF
</dd>
<dt> Checksums file: </dt>
<dd>
<a href="$md5sum">$md5sum</a>
(<a href="$md5sum_asc">detached GPG signature</a> signed with key
<a
href="http://wwwkeys.pgp.net:11371/pks/lookup?op=index&amp;search=$GPGKEY">$GPGKEY</a>)
</dd>
<dd>$fingerprint</dd>
</dl>
<p></p>
<table class="footer" width="100%">
<tr><td align="left">Last updated
EOF
;

print INDEX `date '+%Y-%m-%d %H:%M%z'`;

print INDEX <<EOF
</td>
<td align="right">
  <a href="http://validator.w3.org/check/referer">Valid XHTML 1.0!</a>
  &nbsp;&nbsp;
  <a href="http://jigsaw.w3.org/css-validator/check/referer">Valid CSS!</a>
</td>
</tr>
</table>
</body>
</html>
EOF
;

close (INDEX);

$pdir = "$WWW_DIR/$APT_DIR";
$ssh_cmd = "ssh $WWW_USER\@$HOST";

system "$ssh_cmd rm -rf $pdir";
system "$ssh_cmd mkdir -p $pdir";

system ("scp $index $md5sum $md5sum_asc " . join (" ", @files)
        . " " . join (" ", @desc) . " $WWW_USER\@$HOST:$pdir");

system "$ssh_cmd chgrp -R plplot $pdir";
system "$ssh_cmd chmod -R g=u $pdir";

