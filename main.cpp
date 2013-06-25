#ifdef _WIN32
	#define _CRT_SECURE_NO_WARNINGS
	#include <windows.h>
	#include <conio.h>
	#include <tchar.h>
	#include <direct.h>
	#include <io.h>
	#include <sys/utime.h>
	#define GetCurrentDir _getcwd
	#if defined(_MSC_VER)
		#define access _access
		#define rmdir _rmdir
		#define chdir _chdir
		#define unlink _unlink
		#define utimbuf _utimbuf
		#define utime _utime
		#define mkdir _mkdir
	#endif
	WORD GetConsoleTextAttribute (HANDLE hCon) {
		CONSOLE_SCREEN_BUFFER_INFO con_info;
		GetConsoleScreenBufferInfo(hCon, &con_info);
		return con_info.wAttributes;
	}
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	const int saved_colors = GetConsoleTextAttribute(hConsole);
#else
	#include <termios.h>
	#include <sys/time.h>
	#include <glob.h> // For wildcard
	#include <unistd.h>
	#include <utime.h>
	#define GetCurrentDir getcwd
#endif
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <vector>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <climits>
#include <cerrno>
#include <dirent.h>
using namespace std;
string cd;
#include "main.h"

void usage(string file) {
	cout	<< "\nUsage: " << file << " [OPTION]"
			<< "\nAdjust Android Densities, including .9.PNGs"
			<< "\n\n   -h, --help       Display this help message"
			<< "\n   -c, --convert    Resize/convert source folder"
			<< "\n   -v, --version    Display tool version"
			<< "\n   setup            Modify settings";
	#if _WIN32 || _WIN64
	cout << endl;
	#else
	cout << endl << endl;
	#endif
	exit(EXIT_SUCCESS);
}

string stem(string path) {
	string filename;
	size_t pos = path.find_last_of("/\\");
	if(pos != string::npos)
	filename.assign(path.begin() + pos + 1, path.end());
	else filename = path;
	while(filename.find_last_of(".") != string::npos) {
	filename = filename.substr(0, filename.find_last_of("."));
	}
	return filename;
}

int main(int ac, char* av[]) {
	cd = av[0];
	#if _WIN32 || _WIN64
	if( complete(cd) == cd.c_str() ) {
		if ( !find(cd, ".exe") ) { cd += ".exe"; cd = complete(cd); }
	}
	else cd = complete(cd);
	#else
	cd = complete(cd.c_str());
	#endif
	cd = cd.substr( 0, cd.find_last_of("/\\") );
	chdir(cd.c_str());
	if(ac == 1) {
		#if _WIN32 || _WIN64
		ftitle();
		std::system("MODE CON:COLS=106 LINES=30");
		#endif
		menu();
	}
	string a = av[1];
	trim(a);
	if (a == "-v" || a == "--version") {
		#if _WIN32 || _WIN64
		cout << title << '\n';
		#else
		cout << title << "\n\n";
		#endif
	}
	else if (a == "-h" || a == "--help")
		usage( stem(complete(av[0])) );
	else if (a == "-c" || a == "--convert") {
		called = 2;
		convert();
	}
	else if (a == "setup") {
	called = true;
	settings();
	exit(EXIT_SUCCESS);
	}
	else {
		cerr << "Unknown option: ";
		for(int i=1; i < ac; i++) cerr << av[i] << " ";
		cerr << '\n';
		exit(1);
	}
	return 0;
}