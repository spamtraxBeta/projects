#! /usr/bin/perl


use strict;
use Data::Dumper;
use JSON::Parse 'parse_json';

my $state_on = "on";
my $proto = "elro_800_switch";
my $configTable = 
[
	# protocol, system, unit, state,                #origin,                   repeats,           commands
	#           code    code
	# Button A
	[$proto, 21,     1,   [\&cmp_not,   $state_on], [\&cmp_equal, "receiver"], [\&cmp_equal, 1], ["mpc stop;"] ],

	# Button B
	[$proto, 21,     2,   [\&cmp_equal, $state_on], [\&cmp_equal, "receiver"], [\&cmp_equal, 1], ["mpc play;"]],
	[$proto, 21,     2,   [\&cmp_not,   $state_on], [\&cmp_equal, "receiver"], [\&cmp_equal, 1], ["mpc stop;"]],
	[$proto, 21,     2,   [\&cmp_equal, $state_on], [\&cmp_equal, "receiver"], [\&cmp_equal, 3], ["mpc clear;", "mpc load default;", "mpc play"]],
	
	# Button C
	[$proto, 21,     4,   [\&cmp_equal, $state_on], [\&cmp_equal, "receiver"], [\&cmp_equal, 1], ["mpc prev;"]],
	[$proto, 21,     4,   [\&cmp_not,   $state_on], [\&cmp_equal, "receiver"], [\&cmp_equal, 1], ["mpc next;"]],
	
	# Button D
	[$proto, 21,     8,   [\&cmp_equal, $state_on], [\&cmp_equal, "receiver"], [\&cmp_equal, 1], ["mpc volume -5;"]],
	[$proto, 21,     8,   [\&cmp_not,   $state_on], [\&cmp_equal, "receiver"], [\&cmp_equal, 1], ["mpc volume +5;"]],
	
	
];


use constant
{
	IDX_Protocol => 0,
	IDX_SysCode  => 1,
	IDX_UnitCode => 2,
	IDX_State	 => 3,
	IDX_Origin   => 4,
	IDX_Repeats  => 5,
	IDX_Commands => 6
};

sub cmp_equal()
{
	my $input0 = shift;
	my $input1 = shift;
	
	return $input0 eq $input1;	
}

sub cmp_not()
{
	my $input0 = shift;
	my $input1 = shift;
	
	return $input0 ne $input1;	
}


&main();
sub main()
{
    my $data = "";
    my $blockCounter = 0;

    
    while (<STDIN>)
    {
        foreach my $char (split //, $_)
        {
            $data .= $char;
        
            if ($char eq "{")
            {
                $blockCounter++;
            }
            elsif ($char eq "}")
            {
                $blockCounter--;
            }
            
            
            if ($blockCounter == 0)
            {
                if ($data =~ /^\s*$/)
                {
                    # ignore whitespace only
                }
                else
                {
                    &processData($data);
                }

                $data = "";
            }
            else
            {

            }
        }
    }
}

sub processData()
{
    my $json = shift;
    my $perl = parse_json ($json);

    my $protocol = $perl->{'protocol'};
    
    my $message = $perl->{'message'};
    
    my $systemCode = $message->{'systemcode'};
    my $unitCode = $message->{'unitcode'};
    my $state = $message->{'state'};

    my $repeats = $perl->{'repeats'};
	my $origin  = $perl->{'origin'};

	
    for (my $i = 0; $i < scalar(@$configTable); ++$i)
    {
		my $config = $configTable->[$i];
		
		my $cfgProtocol = $config->[IDX_Protocol];
		my $cfgSysCode  = $config->[IDX_SysCode];
		my $cfgUnitCode = $config->[IDX_UnitCode];
		my $cfgState    = $config->[IDX_State];
		my $cfgOrigin   = $config->[IDX_Origin];
		my $cfgRepeats  = $config->[IDX_Repeats];
		my $cfgCommands = $config->[IDX_Commands];
		
		my $cfgStateFunc = $cfgState->[0];
		my $cfgStateExpected = $cfgState->[1];
		
		my $cfgOriginFunc = $cfgOrigin->[0];
		my $cfgOriginExpected = $cfgOrigin->[1];
		
		my $cfgRepeatsFunc = $cfgRepeats->[0];
		my $cfgRepeatsExpected = $cfgRepeats->[1];
		
		if
		(
				($protocol eq $cfgProtocol)
				&&
				($cfgSysCode eq $systemCode)
				&&
				($cfgUnitCode eq $unitCode)
				&&
				(&$cfgStateFunc($state, $cfgStateExpected))
				&&
				(&$cfgOriginFunc($origin, $cfgOriginExpected))
				&&
				(&$cfgRepeatsFunc($repeats, $cfgRepeatsExpected))
		)
		{
			print Dumper $perl;
			foreach my $cmd (@$cfgCommands)
			{
				&command($cmd);
			}
		}
		
    }  
}


sub command()
{
    my $cmd = shift;
    
    print $cmd, "\n";
    #system($cmd);
    
    my $result = `$cmd`;
    print "RESULT: $result";
    print "\n";
    
    return $result;
}
