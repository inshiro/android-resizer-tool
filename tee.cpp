#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
//#include <fcntl.h>
#define MAXFILE 300
using namespace std;
void tee(int numfiles, char* logfile[]) {
//http://david.tribble.com/src/tee.c
    char invbuf[10*1024]; // Stdin buffer
#ifdef O_BINARY
    // Open stdin in binary mode
    if (setmode(fileno(stdin), O_BINARY) == -1) {
        cerr << "tee: Can't set mode on standard input\n";
        exit(EXIT_FAILURE);
    }
#endif
	setvbuf(stdin, invbuf, _IOFBF, sizeof invbuf);
#ifdef O_BINARY
    // Open stdout in binary mode
    if (setmode(fileno(stdout), O_BINARY) == -1) {
        cerr << "tee: Can't set mode on standard output\n";
        exit(EXIT_FAILURE);
    }
#endif
    const char* mode = "ab"; // Output file mode
    FILE * fp[MAXFILE]; // Output file pointers
    int i; // File counter
    // Open the output files
    for (i = 0;  i < numfiles && i < MAXFILE;  ++i) {
        fp[i] = fopen(logfile[i], "ab");
        if (fp[i] == NULL) cerr << "tee: Can't write: " << logfile[i] << '\n';
    }
    // Split the input to several output files
    for (;;) {
        int	c = getchar();
        if (c == EOF) break;

        // Send char to output files
        for (i = 0;  i < numfiles && i < MAXFILE;  ++i)
            if (fp[i] != NULL) putc(c, fp[i]);

        
        putchar(c); // Send char to stdout
            // Flush output files
            for (i = 0;  i < numfiles && i < MAXFILE;  ++i)
                if (fp[i] != NULL) fflush(fp[i]);
            fflush(stdout);
        //}
    }

    // Close the files
    for (i = 0;  i < numfiles && i < MAXFILE;  ++i)
		if (fp[i] == NULL) fclose(fp[i]);
    fflush(stdout);
}

int main(int argc, char* argv[]) {
	//if(argc == 1) return EXIT_FAILURE;
    tee(argc-1, argv+1);
    return EXIT_SUCCESS;

}
