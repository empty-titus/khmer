// Simple C++ implementation of the 'load-graph' Python script.
// Author: Eric A. McDonald


#include <cstring>
#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <unistd.h>
#include <error.h>
#include <getopt.h>

// #include <string>

#include "hashbits.hh"


using namespace std;
using namespace khmer;


static const char *	    SHORT_OPTS		= "k:N:x:";	


int main( int argc, char * argv[ ] )
{
    unsigned long	kmer_length	    = 32;
    float		ht_size_FP	    = 1.0E6;
    unsigned long	ht_count	    = 4;
    unsigned long	input_chunk_size    = 104857600;

    int			rc		    = 0;
    int			opt		    = -1;
    char *		conv_residue	    = NULL;
    string		ofile_name;
    string		ifile_name;
    FILE *		ofile		    = NULL;

    while (-1 != (opt = getopt( argc, argv, SHORT_OPTS )))
    {

	switch (opt)
	{
	case 'k':
	    kmer_length = strtoul( optarg, &conv_residue, 10 );
	    if (!strcmp( optarg, conv_residue ))
		error( EINVAL, EINVAL, "Invalid kmer length" );
	    break;
	case 'N':
	    ht_count = strtoul( optarg, &conv_residue, 10 );
	    if (!strcmp( optarg, conv_residue ))
		error( EINVAL, EINVAL, "Invalid number of hashtables" );
	    break;
	case 'x':
	    ht_size_FP = strtof( optarg, &conv_residue );
	    if (!strcmp( optarg, conv_residue ))
		error( EINVAL, EINVAL, "Invalid hashtable size" );
	    break;
	default:
	    error( 0, 0, "Skipping unknown arg, '%c'", optopt );
	}

    }

    if (optind < argc) ofile_name = string( argv[ optind++ ] );
    else error( EINVAL, 0, "Output file name required" );

    if (optind < argc) ifile_name = string( argv[ optind++ ] );
    else error( EINVAL, 0, "Input file name required" );

    HashIntoType	    ht_size		= (HashIntoType)ht_size_FP;
    unsigned int	    reads_total		= 0;
    unsigned long long int  n_consumed		= 0;
    vector<HashIntoType> ht_sizes;
    for ( unsigned int i = 0; i < ht_count; ++i )
	ht_sizes.push_back( ht_size );
    Hashbits ht( kmer_length, ht_sizes );
    ht.consume_fasta_and_tag( ifile_name, reads_total, n_consumed, NULL, NULL );
    
    //ofile = fopen( ofile_name.c_str( ), "w" );
    //if (!ofile) error( EINVAL, 0, "Failed to open output file" );

    return rc;
}


// vim: set sts=4 sw=4 tw=80:
