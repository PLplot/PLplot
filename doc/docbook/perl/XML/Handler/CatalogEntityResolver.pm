# vim:sw=4 sta showmatch

# $Id$

use strict;

package XML::Handler::CatalogEntityResolver;
use XML::Catalog;

sub new {
    my ($class, $catalog, $base) = @_;
    return bless([ $base, $catalog ], $class);
}

sub resolve_entity {
    my ($self, $arg) = @_;
    my $base;
    my $sysid;
    my $catalog = $self->[1];
    
    if(defined $arg->{'Base'}) { $base = $arg->{'Base'} }
    else { $base = $self->[0] }
    
    if(defined $catalog) {
	if(defined $arg->{'PublicId'}) {
	    $sysid = $catalog->resolve_public($arg->{'PublicId'});
	}
	elsif(defined $arg->{'SystemId'}) {
	    $sysid = $catalog->remap_system($arg->{'SystemId'});
	}
    } else {
	$sysid = $arg->{'SystemId'};
    }

    # XML::Parser::PerlSAX can't do anything by itself :-(
    # This is the mantra from XML::Parser
    
    my $name = $sysid;
    # Prepend base only for relative URLs
    if(defined($base)
	and not ($name =~ m!^(?:/|\w+:)!))
    {
	$name = $base . $sysid;
    }

    if ($name =~ s/^(\w+)://) {
	my $method = $1;

        unless ($method eq 'file') {
	    die "$0: resolve_entity: only file URIs supported\n";
	}
    }

    # I can't get O_RDONLY constants to work 'strict subs'
    # I'd rather use FileHandle, but PerlSAX/expat does not understand
    sysopen(ENT, $name, 0);	
    return { 'ByteStream' => *ENT };
}

1;

