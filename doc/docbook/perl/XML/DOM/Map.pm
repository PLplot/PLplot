package XML::DOM::Map;

use strict;

use XML::DOM;

BEGIN {
	use Exporter ();
	use vars qw($VERSION @ISA @EXPORT);

        @ISA = qw(Exporter);
        @EXPORT = qw(&getElementsByID &node_map &node_warn &hash_append &node_map_ordered_children NMOC_NEXTSPEC NMOC_NEXTSPEC_REDO);
}

$VERSION = '0.01';

# Put some hashes together, with values from later ones
# overriding previous ones.
# This is used to implement a crude inheritence scheme
# with node_map specs.
#
sub hash_append {
	my %result = %{shift(@_)};		# Make a copy
	
	foreach my $hashref (@_) {
		foreach(keys %$hashref) {
			$result{$_} = $hashref->{$_};
		}
	}

	return \%result;
}


sub node_map {
	my $node = $_[0];
	my $spec = $_[1];

	if(!exists $spec->{''}) { $spec->{''} = sub {} };

	if($node->getNodeType == ELEMENT_NODE) {
		return &{($spec->{'elem:' . $node->getTagName()} ||
			  $spec->{'elem'} ||
			  $spec->{''})}(@_);

	} elsif($node->getNodeType == TEXT_NODE) {
		if($node->getData() =~ /[^\s]/) {
			return &{($spec->{'text'} || $spec->{''})}(@_);
		} else {
			return &{($spec->{'whitespace'} ||
				$spec->{'text'} ||
				$spec->{''})}(@_);
		}

	} elsif($node->getNodeType == ATTRIBUTE_NODE) {
		return &{($spec->{'attr:' . $node->getName()} ||
			  $spec->{'attr'} ||
			  $spec->{''})}(@_);
	
	} elsif($node->getNodeType == CDATA_SECTION_NODE) {
		return &{($spec->{'cdata'} || $spec->{''})}(@_);

	# ENTITY_REFERENCE_NODE, ENTITY_NODE
	
	} elsif($node->getNodeType == PROCESSING_INSTRUCTION_NODE) {
		return &{($spec->{'pi'} || $spec->{''})}(@_);
	
	} elsif($node->getNodeType == COMMENT_NODE) {
		return &{($spec->{'comment'} || $spec->{''})}(@_);

	}
	
	# DOCUMENT_NODE, DOCUMENT_TYPE_NODE, DOCUMENT_FRAGMENT_NODE
	# NOTATION_NODE
	# ELEMENT_DECL_NODE, ATT_DEF_NODE, XML_DECL_NODE, ATTLIST_DECL_NODE

	else {
		node_warn($node, "node_map: unrecognized node type\n");
		return &{$spec->{''}}(@_);
	}
}	
	
# Warn about unexpected markup/cdata/whatever.
#
sub node_warn
{
	my $node = shift;
	my $data = '';
	
	if($node->getNodeType == ELEMENT_NODE) {
		$data = '<';
		if($node->getParentNode()->getNodeType==ELEMENT_NODE) {
			$data .= $node->getParentNode()->getTagName() 
					. '>:<';
		}
		$data .= $node->getTagName() . ">: ";
	} elsif($node->getNodeType == TEXT_NODE) {
		$data = '<' .
			$node->getParentNode()->getTagName() . '>:"' .
			substr($node->getData(), 0, 15) . '...": ';
	}

	warn "node_warn: ", $node, ": $data", @_;
}

sub NMOC_NEXTSPEC	() { '__NMOC_NEXTSPEC__'; }
sub NMOC_NEXTSPEC_REDO	() { '__NMOC_NEXTSPEC_REDO__'; }

# Map child nodes for >1 number of specs.  
# If node_map throws NMOC_NEXTSPEC(_REDO) then switch to next spec.
# Currently, specifying additional arguments to and obtaining
# return values from mapped child nodes are not possible.
#
sub node_map_ordered_children
{
	my $node = shift;
	my $current_spec = shift;

	foreach($node->getChildNodes)
	{
		if(!defined(eval { node_map($_, $current_spec); })
			and $@)
		{
			if($@ =~ /^__NMOC_NEXTSPEC__/) {
				my $tmpspec = shift;
				$current_spec = $tmpspec if $tmpspec;
			} elsif($@ =~ /^__NMOC_NEXTSPEC_REDO__/) {
				my $tmpspec = shift;
				$current_spec = $tmpspec 
					and redo if $tmpspec;
			} else {
				# Not our exception: reraise?
				die $@;
			}
		}
	}
}

sub getElementsByID
{
	my $node = shift;
	my $list = shift or {};

	my $type = $node->getNodeType;
	if($type != ELEMENT_NODE && $type != DOCUMENT_NODE) {
		return $list;
	}
	
	if($type==ELEMENT_NODE and
		$node->getAttribute('id'))
	{
		$list->{$node->getAttribute('id')} = $node;
	}

	foreach($node->getChildNodes()) {
		$list = getElementsByID($_, $list)
	}

	return $list;
}

1;

