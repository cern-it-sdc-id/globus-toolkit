#! /usr/bin/env perl 

#
# Portions of this file Copyright 1999-2005 University of Chicago
# Portions of this file Copyright 1999-2005 The University of Southern California.
#
# This file or a portion of this file is licensed under the
# terms of the Globus Toolkit Public License, found at
# http://www.globus.org/toolkit/download/license.html.
# If you redistribute this file, with or without
# modifications, you must include this notice in the file.
#


=head1 globus-ftp-client-exist-test

    Tests to exercise the existence checking of the client library.

=cut

use strict;
use POSIX;
use Test;
use FtpTestLib;

my $test_exec = './globus-ftp-client-exist-test';
my @tests;

my $gpath = $ENV{GLOBUS_LOCATION};

if (!defined($gpath))
{
    die "GLOBUS_LOCATION needs to be set before running this script";
}

@INC = (@INC, "$gpath/lib/perl");

my ($proto) = setup_proto();
my ($source_host, $source_file, $local_copy) = setup_remote_source();

sub check_existence
{
    my ($errors,$rc) = ("",0);
    my ($old_proxy);
    my $src_url = shift;
    my $existence_rc = shift;

    my $command = "$test_exec -s $src_url >/dev/null 2>&1";
    $errors = run_command($command, $existence_rc);
    if($errors eq "")
    {
        ok('success', 'success');
    }
    else
    {
        $errors = "\n# Test failed\n# $command\n# " . $errors;
        ok($errors, 'success');
    }
}

if(source_is_remote())
{
    print "using remote source, skipping check_existence()\n";
}
else
{
    my $emptydir = POSIX::tmpnam();
    my @test_dirs;

    if(!defined($ENV{'FTP_TEST_BACKEND'}))
    {
        mkdir $emptydir, 0755;
    }

    push(@test_dirs, $emptydir);
    push(@test_dirs, '/no-such-file');
    push(@test_dirs, '/etc');
    push(@test_dirs, '/');
    push(@test_dirs, '/etc/group');

    foreach('/etc/group', '/', '/etc', '/no-such-file', $emptydir)
    {
        my $exists_rc = stat($_) ? 0 : 1;
        
        push(@tests, "check_existence('$proto$source_host$_', $exists_rc);");
    }

    if(defined($ENV{FTP_TEST_RANDOMIZE}))
    {
        shuffle(\@tests);
    }

    if(@ARGV)
    {
        plan tests => scalar(@ARGV);
        
        foreach (@ARGV)
        {
            eval "&$tests[$_-1]";
        }
    }
    else
    {
        plan tests => scalar(@tests);
        
        foreach (@tests)
        {
            eval "&$_";
        }
    }

    rmdir $emptydir;
}
