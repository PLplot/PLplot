# vim:sw=4 sta showmatch


use strict;
use vars qw($VERSION @ISA);
$VERSION = '0.01';

# Preloaded methods go here.

package XML::Handler::Templates::Node;

sub new {
    my ($class, $type, $saxhash, $parent) = @_;
    $saxhash->{type} = $type;
    $saxhash->{parent} = $parent;
    $saxhash->{_ext} = {};
    return bless($saxhash, $class);
}

# Element name
sub name {
    my ($self) = @_;
    if($self->{type} eq 'element') {
	return $self->{Name};
    } else {
	return undef;
    }
}

sub within {
    my ($self,$name) = @_;
    for ($self = $self->{parent}; $self; $self = $self->{parent}) {
	return $self if ($self->name eq $name);
    }

    return undef;
}

sub in {
    my ($self,$name) = @_;
    if ($self->{parent} and $self->{parent}->name eq $name) {
	return $self->{parent};
    } else {
	return undef;
    }
}

sub ext { return shift->{_ext} }
sub parent { return shift->{parent} }

# one of document, element, text, processing-instruction, comment, whitespace
sub type { return shift->{type} }

package XML::Handler::Templates;




sub new {
    my ($class, $userdata) = @_;
    my $self = {
	rules => {},
	mode => [ '' ],
	userdata => $userdata
    };
    return bless($self, $class);
}

#
# Rule parsing
#

#sub parse_patterntext {
#    my ($pattern) = @_;
#    my @return;
#
#    my @alternatives = split(/|/, $pattern);
#    foreach my $a (@alternatives) {
#	my @steps = split(/\//, $a);
#	push(@return, \@steps);
#    }
#
#    return @return;
#}

sub add_rule {
    my $sub = pop;
#    my ($self, $patterntext, $mode, $priority) = @_;
#    
#    my @patterns = parse_patterntext($patterntext);
#
#    # Init hashes if not there already
#    if(!defined $self->{rules}->{$mode}) {
#	$self->{rules}->{$mode} = {
#	    t_text	=> {},
#	    t_pi	=> {},
#	    t_comment	=> {},
#	    t_elemopen	=> {},
#	    t_elemclose	=> {},
#	};
#    }
#
#    foreach my $pat (@patterns) {
#	my $laststep = $pat->[$#{$pat}];
#	my $entry = [ $pat, $sub, $priority ];
#
#	if($laststep eq 'text()') {
#	    $self->{rules}->{$mode}->{t_text}->{$patterntext} = $entry;
#	} elsif($laststep eq 'processing-instruction()') {
#	    $self->{rules}->{$mode}->{t_pi}->{$patterntext} = $entry;
#	} elsif($laststep eq 'comment()') {
#	    $self->{rules}->{$mode}->{t_comment}->{$patterntext} = $entry;
#	} elsif($laststep =~ /<$/) {
#	    $self->{rules}->{$mode}->{t_elemopen}->{$patterntext} = $entry;
#	} elsif($laststep =~ />$/) {
#	    $self->{rules}->{$mode}->{t_elemclose}->{$patterntext} = $entry;
#	} else {
#	    die "Unknown pattern type!";
#	}
#    }

    my ($self, $pattern, $mode) = @_;
    $mode = '' if !defined($mode);

    # Init hashes if not there already
    if(!defined $self->{rules}->{$mode}) {
	$self->{rules}->{$mode} = {
	    t_elemopen	=> {},
	    t_elemclose	=> {},
	    t_sdata => {},
	};
    }

    if($pattern eq 'text()') {
        $self->{rules}->{$mode}->{t_text} = $sub;
    } elsif($pattern eq 'processing-instruction()') {
        $self->{rules}->{$mode}->{t_pi} = $sub;
    } elsif($pattern eq 'comment()') {
        $self->{rules}->{$mode}->{t_comment} = $sub;
    } elsif($pattern eq '*<') {
        $self->{rules}->{$mode}->{t_elemopen}->{''} = $sub;
    } elsif($pattern eq '*>') {
        $self->{rules}->{$mode}->{t_elemclose}->{''} = $sub;
    } elsif($pattern eq 'sdata()') {
        $self->{rules}->{$mode}->{t_sdata}->{''} = $sub;
    } elsif($pattern eq '/<') {
        $self->{rules}->{$mode}->{t_docstart} = $sub;
    } elsif($pattern eq '/>') {
        $self->{rules}->{$mode}->{t_docend} = $sub;
    } elsif($pattern =~ /^(.+)<$/) {
        $self->{rules}->{$mode}->{t_elemopen}->{$1} = $sub;
    } elsif($pattern =~ /^(.+)>$/) {
        $self->{rules}->{$mode}->{t_elemclose}->{$1} = $sub;
    } elsif($pattern =~ /^\|(.+)\|$/) {
        $self->{rules}->{$mode}->{t_sdata}->{$1} = $sub;
    } else {
        die "Unknown pattern type!";
    }
}

#
# Modes
#

sub push_mode {
    my ($self, $mode) = @_;
    push(@{$self->{mode}}, $mode);
    return $self->{mode}->[$#{$self->{mode}}-1];
}

sub mode {
    my ($self) = @_;
    return $self->{mode}->[$#{$self->{mode}}];
}

sub pop_mode {
    my ($self) = @_;
    return pop(@{$self->{mode}});
}

#
# Lowercase element and attribute names 
# Usually used for SGML parsing.
#

sub lc_identifiers {
    my ($self, $setting) = @_;
    my $oldsetting = $self->{lc};
    $self->{lc} = $setting if defined $setting;
    return $oldsetting;
}

#
# Locators
#

sub set_document_locator {
    my ($self, $arg) = @_;
    $self->{locator} = $arg->{Locator};
}

sub get_locator {
    my ($self) = @_;
    return $self->{locator};
}

#
# Standard handlers
#

sub start_document {
    my ($self) = @_;

    my $doc = XML::Handler::Templates::Node->new(
		'document', {}, undef);
    $self->{current_node} = $doc;
    
    &{($self->{rules}->{$self->mode}->{t_docstart} || sub{})}
	($self->{userdata}, $doc, $self);
}

sub end_document {
    my ($self) = @_;

    my $doc = $self->{current_node};
    $self->{current_node} = undef;
    
    return &{($self->{rules}->{$self->mode}->{t_docend} || sub{})}
	($self->{userdata}, $doc, $self);
}

sub start_element {
    my ($self, $arg) = @_;

    if($self->{lc}) {
	$arg->{Name} = lc($arg->{Name});
	foreach my $a (keys %{$arg->{Attributes}}) {
	    $arg->{Attributes}->{$a} = lc($arg->{Attributes}->{$a});
	}
    }

    my $elem = XML::Handler::Templates::Node->new(
		    'element', $arg, $self->{current_node});
		    
    $self->{current_node} = $elem;

    $elem->{_lastmode} = $self->mode;
        
    my $elemrules = $self->{rules}->{$self->mode}->{t_elemopen};
    &{($elemrules->{$arg->{Name}} || $elemrules->{''} || sub{})}
       ($self->{userdata}, $elem, $self);
}
    
sub end_element {
    my ($self, $arg) = @_;

    my $elem = $self->{current_node};

    # We always enter the end element handler with the same mode
    # as we had entered the start element handler.
    # In most cases this is the more sane behavior, even though
    # it's inconsistent.

    my $elemrules = $self->{rules}->{$elem->{_lastmode}}->{t_elemclose};
    &{($elemrules->{$elem->{Name}} || $elemrules->{''} || sub{})}
       ($self->{userdata}, $elem, $self);

    $self->{current_node} = $elem->parent;
}

sub characters {
    my ($self, $arg) = @_;

    my $textnode = XML::Handler::Templates::Node->new(
		    'text', $arg, $self->{current_node});
    
    &{($self->{rules}->{$self->mode}->{t_text} || sub{})}
	($self->{userdata}, $textnode, $self);
}

sub processing_instruction {
    my ($self, $arg) = @_;

    my $pi = XML::Handler::Templates::Node->new(
		    'processing-instruction', $arg, $self->{current_node});
    
    &{($self->{rules}->{$self->mode}->{t_pi} || sub{})}
	($self->{userdata}, $pi, $self);
}

sub ignorable_whitespace {
    my ($self, $arg) = @_;

    my $textnode = XML::Handler::Templates::Node->new(
		    'whitespace', $arg, $self->{current_node});
    
    &{($self->{rules}->{$self->mode}->{t_text} || sub{})}
	($self->{userdata}, $textnode, $self);
}

sub comment {
    my ($self, $arg) = @_;

    my $comment = XML::Handler::Templates::Node->new(
		    'comment', $arg, $self->{current_node});
    
    &{($self->{rules}->{$self->mode}->{t_comment} || sub{})}
	($self->{userdata}, $comment, $self);
}

#
# SDATA entities (SGML)
#

sub internal_entity_ref {
    my ($self, $arg) = @_;

    my $sdata = XML::Handler::Templates::Node->new(
		    'sdata', $arg, $self->{current_node});

    my $sdatarules = $self->{rules}->{$self->mode}->{t_sdata};
    &{($sdatarules->{$arg->{Name}} || $sdatarules->{''} || sub{})}
       ($self->{userdata}, $sdata, $self);
}

# FIXME: Write a man page.

# Autoload methods go after =cut, and are processed by the autosplit program.

1;
__END__

