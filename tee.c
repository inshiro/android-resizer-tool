/* tee.c
*	Splits standard input stream to standard output stream and
*	one or more output files.
*
* http://david.tribble.com/src/tee.c
*
* Notice
*	Copyright ©1985-2006 by David R Tribble, all rights reserved.
*
* History
*	1.0 1985-04-15 drt.
*	First revision.
*
*	1.1 1987-10-15 drt.
*	File modes are now binary.
*
*	1.2 1988-01-11 drt.
*	Uses setvbuf() for stdin.
*
*	1.4 2006-06-26 drt.
*	Added command line options '-a', '-n'.
*	Added 'opt_nlflush' and 'opt_append' flags.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int opt_append  = 0;	// Append to output files

void tee(int numfiles, char *filename[]) {
    const char *mode = "wb"; //Output file mode
    FILE *fp[300]; // Output file pointers
    int i; // File counter
    if (opt_append) mode = "ab";

    for (i = 0;  i < numfiles && i < 300;  ++i) {
        fp[i] = fopen(filename[i], mode);
        if (fp[i] == NULL)
            fprintf(stderr, "tee: Can't write: %s\n", filename[i]);
    }
    for (;;) { // Split the input to several output files
        int	c = getchar(); // Input/output char
        if (c == EOF) break;
        for (i = 0;  i < numfiles && i < 300;  ++i) // Send char to output files
            if (fp[i] != NULL) putc(c, fp[i]);
        putchar(c); // Send char to stdout
        if (c == '\n') { // Handle flushing after newlines
            for (i = 0;  i < numfiles && i < 300;  ++i) // Flush output files
                if (fp[i] != NULL) fflush(fp[i]);
            fflush(stdout); // Flush stdout
        }
    }
    // Close the files
    for (i = 0;  i < numfiles && i < 300;  ++i) {
        if (fp[i] == NULL)
            fclose(fp[i]);
    }
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    char invbuf[10*1024]; // Stdin buffer
    // Parser command line options
    while (argc > 1 && argv[1][0] == '-') {
        if (strcmp(argv[1], "-a") == 0) opt_append = 1;
        else if (strcmp(argv[1], "--append") == 0) opt_append = 1;
        else if (strcmp(argv[1], "--version") == 0) goto version;
        //else if (strcmp(argv[1], "-n") == 0) opt_nlflush = 1;
        else goto usage;
        argc--, argv++;
    }
    if (argc < 2 || isatty(fileno(stdin))) goto usage; // Check command line args

#ifdef O_BINARY
    // Open stdin in binary mode
    if (setmode(fileno(stdin), O_BINARY) == -1) {
        fprintf(stderr, "tee: Can't set mode on standard input\n");
        return EXIT_FAILURE;
    }
#endif

    setvbuf(stdin, invbuf, _IOFBF, sizeof invbuf);

#ifdef O_BINARY
    // Open stdout in binary mode
    if (setmode(fileno(stdout), O_BINARY) == -1) {
        fprintf(stderr, "tee: Can't set mode on standard output\n");
        return EXIT_FAILURE;
    }
#endif
    
    tee(argc-1, argv+1); // Split output into one or more file streams
    return EXIT_SUCCESS;

	usage:
		printf("Usage: tee [OPTION]... [FILE]...");
		printf("\nCopy standard input to each FILE, and also to standard output.");
		printf("\n\n  -a, --append              append to the given FILEs, do not overwrite");
		printf("\n      --help     display this help and exit");
		printf("\n      --version  output version information and exit\n");
		return EXIT_FAILURE;
	
	version:
		printf("Copyright (C) 1985-2006 David R Tribble\n");
		return EXIT_FAILURE;
}
