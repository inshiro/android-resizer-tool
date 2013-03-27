#ifdef _WIN32
	#define _CRT_SECURE_NO_WARNINGS
	#include <windows.h>
	#include <conio.h>
	#include <tchar.h>
	WORD GetConsoleTextAttribute (HANDLE hCon) {
		CONSOLE_SCREEN_BUFFER_INFO con_info;
		GetConsoleScreenBufferInfo(hCon, &con_info);
		return con_info.wAttributes;
	}
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	const int saved_colors = GetConsoleTextAttribute(hConsole);
#else
	#include <termios.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <glob.h> // For wildcard
#endif
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "boost/date_time/gregorian/gregorian.hpp"
using namespace std;
using namespace boost;
using namespace boost::gregorian;
using namespace boost::iostreams;
using namespace boost::filesystem;
string cd;
#include "main.h"

void usage(string file) {
	cout	<< "\nUsage: " << file << " [OPTION]"
			<< "\nResize Android Drawables, including .9.pngs"
			<< "\n\n   -h, --help       Display this help message"
			<< "\n   -c, --convert    Resize/convert source folder"
			<< "\n   -v, --version    Display tool version";
	#if _WIN32 || _WIN64
	cout << '\n';
	#else
	cout << "\n\n";
	#endif
	exit(EXIT_SUCCESS);
}

int main(int ac, char* av[]) {
	cd = system_complete(av[0]).parent_path().string();
	if(ac == 1) menu();
	string a = av[1];
	trim(a);
	if (a == "-h" || a == "--help")
		usage((string)system_complete(av[0]).stem().string());
	else if (a == "-v" || a == "--version") {
	#if _WIN32 || _WIN64
	cout << ftitle << '\n';
	#else
	cout << ftitle << "\n\n";
	#endif
	}
	else if (a == "-c" || a == "--convert") convert();
	else if (a == "setup") settings();
	else {
	cerr << "Unknown option: ";
	for(int i=1; i < ac; i++)
		 cerr << av[i] << " ";
	#if _WIN32 || _WIN64
	cerr << '\n';
	#else
	cerr << "\n\n";
	#endif
	exit(EXIT_SUCCESS);
	}
	return 0;
}
