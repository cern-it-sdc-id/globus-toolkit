#!/usr/bin/env perl

package FtpTestLib;
require Exporter;
@ISA = qw(Exporter);

@EXPORT = qw( setup_remote_source 
              setup_local_source 
              setup_remote_dest 
              source_is_remote 
              dest_is_remote 
              clean_remote_file 
              get_remote_file 
              compare_local_files );            # symbols to export by default

BEGIN { push(@INC, $ENV{GLOBUS_LOCATION} . '/lib/perl'); }

my $self = {};
use strict;

use POSIX;
use Carp;
use Sys::Hostname;
use Data::Dumper;
use File::Copy;
use Cwd;

# These are globus test support modules.
# use Globus::URL;
# use Globus::Testing::HostDB;
# use Globus::Testing::Startup;

=head1 NAME

FTPTestLib - Helper library for writing FTP tests.

=head1 SYNOPSIS

use FtpTestLib;

push_proxy($new_proxy);

pop_proxy();

compare_local_files($original, $copy);

=head1 DESCRIPTION

The FTPTestLib module provides a set of subroutes for writing tests of 
the ftp test library. The following subroutines are available:

=cut

=over 4

=item  push_proxy($)

Save the current value of the X509_USER_PROXY environment variable in
a stack and set it to the passed argument. If the argument is not defined,
then delete the proxy variable.

=cut
sub push_proxy($)
{
    push(@{$self->{proxy}}, $ENV{X509_USER_PROXY});
    if(scalar(@_) > 0)
    {
        $ENV{X509_USER_PROXY} = $_[0];
    }
    else
    {
        delete($ENV{X509_USER_PROXY});
    }
}

=pod

=item pop_proxy()

Restore the value of the X509_USER_PROXY environment variable from a
previous call to push_proxy. If the proxy was undefined when
push_proxy was called, then it will be removed from the environment.

=cut
sub pop_proxy()
{
    my $proxy = pop(@{$self->{proxy}});

    if(defined($proxy))
    {
        $ENV{X509_USER_PROXY} = $proxy;
    }
    else
    {
        delete($ENV{X509_USER_PROXY});
    }
}

=pod

=item compare_local_files($original, $copy)

Verify that a copy of a file is identical to the original.

=over 8

=item $original

The file name of the original copy of the data.

=item $copy

The file name of the new copy of the data. Any plugin-generated output
is first filtered from the copy before the comparison is made.

=back

If the files are identical, this function returns an empty string;
otherwise a shell comment-string containing information about the
differences is returned.

=cut
sub compare_local_files($$)
{
    my($a,$b) = @_;
    my $diffs;

    if(-B $a or -B $b)
    {
	$diffs = `perl -pe 's/\\[restart plugin\\].*\\n//' < $b | (cmp '$a' - 2>&1 ) | sed -e 's/^/# /'`;
    }
    else
    {
        $diffs = `perl -pe 's/\\[restart plugin\\].*\\n//' < $b | (diff '$a' - 2>&1) | sed -e 's/^/# /'`;
    }

    if($diffs ne '')
    {
        $diffs = "\n# Differences between $a and $b.\n" . $diffs;
    }
    
    return $diffs;
}

=pod

=item Testing with remote servers

The following env vars can be used to allow testing against remote servers. 
(parens denote defaults) all paths must be absolute.

FTP_TEST_SOURCE_HOST (localhost)

FTP_TEST_SOURCE_USER (current user)# used with ssh for staging

FTP_TEST_DEST_HOST (localhost)

FTP_TEST_DEST_USER (current user)  # used with ssh for staging

FTP_TEST_SOURCE_FILE (/etc/group)  # used for get-like tests and 3pt

FTP_TEST_SOURCE_BIGFILE (/bin/sh)  # used by the extended-get test

FTP_TEST_DEST_FILE (some tmpname)  # used for put-like tests and 3pt


FTP_TEST_LOCAL_FILE (/etc/group)   # used as the local source for put-like tests

FTP_TEST_LOCAL_BIGFILE (/bin/sh)   # used as the local source by the extended-put test

=cut

#my ($source_host, $source_file, $local_copy) = setup_remote_source($big = 0);
sub setup_remote_source(;$)
{
    my $source_host;
    my $source_file;
    my $local_copy;
    my $use_big_file = shift;
    
    $source_host = ($ENV{FTP_TEST_SOURCE_HOST} or 'localhost');
    if($use_big_file)
    {
        $source_file = ($ENV{FTP_TEST_SOURCE_BIGFILE} or '/bin/sh');
    }
    else
    {
        $source_file = ($ENV{FTP_TEST_SOURCE_FILE} or '/etc/group');
    }
    
    $local_copy = get_remote_file($source_host, $source_file, 1);
    
    return ($source_host, $source_file, $local_copy);
}

#my ($local_copy) = setup_local_source($big = 0);
sub setup_local_source(;$)
{
    my $local_copy;
    my $use_big_file = shift;
    
    if($use_big_file)
    {
        $local_copy = ($ENV{FTP_TEST_LOCAL_BIGFILE} or '/bin/sh');
    }
    else
    {
        $local_copy = ($ENV{FTP_TEST_LOCAL_FILE} or '/etc/group');
    }
    
    return ($local_copy);
}

#my ($dest_host, $dest_file) = setup_remote_dest();
sub setup_remote_dest()
{
    my $dest_host;
    my $dest_file;
    
    $dest_host = ($ENV{FTP_TEST_DEST_HOST} or 'localhost');
    $dest_file = ($ENV{FTP_TEST_DEST_FILE} or POSIX::tmpnam());

    return ($dest_host, $dest_file);
}

#bool = source_is_remote()
sub source_is_remote()
{
    return ($ENV{FTP_TEST_SOURCE_HOST} and !($ENV{FTP_TEST_SOURCE_HOST} =~ m/localhost/))
}

#bool = dest_is_remote()
sub dest_is_remote()
{
    return ($ENV{FTP_TEST_DEST_HOST} and !($ENV{FTP_TEST_DEST_HOST} =~ m/localhost/))
}


#void clean_remote_file($host, $file);
sub clean_remote_file($$)
{
    my $host = shift;
    my $file = shift;
    
    if($host =~ m/localhost/)
    {
        unlink($file);
    }
    else
    {
        my $user;

        if($ENV{FTP_TEST_DEST_USER})
        {
            $user = "$ENV{FTP_TEST_DEST_USER}\@";
        }
        else
        {
            $user = '';
        }
        
        system("ssh -q $user$host 'rm -f $file'") == 0 or die "ssh failed";
    }
}

#my $local_copy = get_remote_file($host, $file, $use_source_user = 0);
sub get_remote_file($$;$)
{
    my $host = shift;
    my $file = shift;
    my $user = shift;
    my $dest = POSIX::tmpnam();
    
    push(@{$self->{staged_files}}, $dest);
    
    if($host =~ m/localhost/)
    {
        copy($file, $dest);
    }
    else
    {
        my $user;

        if($user)
        {
            $user = $ENV{FTP_TEST_SOURCE_USER};
        }
        else
        {
            $user = $ENV{FTP_TEST_DEST_USER};
        }
        
        if($user)
        {
            $user .= '@';
        }
        else
        {
            $user = '';
        }
        
        system("scp -q -B $user$host:$file $dest") == 0 or die "scp failed";
    }

    return $dest;
}

sub ftp_commands()
{
    return ('SITE', 'TYPE', 'MODE', 'SIZE', 'DCAU', 'PROT', 'BUFSIZE',
            'OPTS', 'PASV', 'PORT', 'REST', 'RETR', 'STOR', 'ERET', 'ESTO',
            'LIST', 'NLST', 'MDTM', 'MKD', 'RMD', 'RNFR', 'RNTO', 'NOOP' );
}


=back

=cut

sub END
{
    if(exists($self->{staged_files}))
    {
        unlink(@{$self->{staged_files}});
    }
    
    if(exists($self->{host_db}))
    {
        delete $self->{host_db};
    }
}
1;
