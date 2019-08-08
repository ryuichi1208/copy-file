use strict;
use warnings;
use List::Util qw(max);
require Carp;

sub main {
    my $dir = shift || './';
    $dir =~ s/^(\w+)$/$1/;

    opendir my $dh, $dir
        or Carp::croak("ls.pl: $dir: No Such file or directory");
    my @files = map  { {name => $_.q{  }, len => length($_)}  }
                sort grep { !/^\./  }
                            readdir $dh;
                            closedir $dh;

    my $maxlen_fname = max(@{[map($_->{len}, @files)]});
    my $count_file = @files;

    if ($count_file) {
		my $cols = 4;
		my $rows = ($count_file % $cols)
	    	     ? int($count_file / $cols + 1)
	        	 : int($count_file / $cols);
		my @print_arrays = ('dummy');
		while (@files) {
			my @arr = ('dummy');
			push @arr, shift(@files) for 1 .. $rows;
			for my $i (1 .. $rows) {
				my $suff = ($i % $rows) ? ($i % $rows) : $rows;
				if ($arr[$i]) {
					if (my $diff_len = $maxlen_fname - $arr[$i]->{len}) {
						$arr[$i]->{name} = $arr[$i]->{name}.(q{ } x $diff_len);
					}
					push @{$print_arrays[$suff]}, $arr[$i]->{name};
				}
			}
		}
	    shift @print_arrays;
	    for my $print_col (@print_arrays) {
		    print "@$print_col\n";
	    }
    }
}

main(@ARGV)

__END__
