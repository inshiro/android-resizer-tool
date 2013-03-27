string sd;
string od;
string var;
string sdpi;
string odpi;
string logop("Enabled");
string command;
string ver = "2.5";
string ftitle = "Android Resizer Tool v" + ver;
void menu();



void clear() {
	#if _WIN32 || _WIN64
	std::system("CLS");
	#else
	std::system("clear");
	#endif
}

#if _WIN32 || _WIN64
wstring s2ws(const string& s) {
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    wstring r(buf);
    delete[] buf;
    return r;
}

string wchartostring(WCHAR wc[]) {
    char ch[260];
    char DefChar = ' ';
    WideCharToMultiByte(CP_ACP,0,wc,-1, ch,260,&DefChar, NULL);
    string ss(ch);
	return ss;
}

string wildcard(const char* arg) {
    vector<string> list;
	WIN32_FIND_DATA fd;
	wstring stemp = s2ws(arg);
	LPCWSTR result = stemp.c_str();
	HANDLE h = FindFirstFile(result,&fd);
	if(h == INVALID_HANDLE_VALUE) return "";
	while(1) {
	list.push_back(wchartostring(fd.cFileName));
		if(FindNextFile(h, &fd) == FALSE)
		break;
	}
	for(size_t i = 0; i < list.size(); i++) // Using Index Based Accessing
		return list[i].c_str();
	for(vector<string>::const_iterator it = list.begin(); // Using const_iterator
		it != list.end(); ++it) return it->c_str();
}
#else
string wildcard(const char* arg) {
    size_t n;
    glob_t res;
    char **p;
    //chdir(root);
    glob(arg, 0, 0, &res);
    n = res.gl_pathc;
    if (n < 1) {
        return "";
    } else {
        for (p = res.gl_pathv; n; p++, n--) {
            return *p;
        }
    }
    globfree(&res);
}
int kbhit(void) {
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}
#endif

void title() {
	#if _WIN32 || _WIN64
	wstring stemp = s2ws(ftitle);
	LPCWSTR result = stemp.c_str();
	SetConsoleTitle(result);
	#else
	//printf("%c]0;%s%c", '\033', ftitle, '\007');
	cout << "\033]0;" + ftitle + "\007";
	#endif
}

void fp() {
#if _WIN32 || _WIN64
while(!_getch());
#else
fflush(stdout);
while (!kbhit());
#endif
}

/*
void cleanpath(string &f) {
	trim(f);
	if(find_first(f, "\"")) erase_all(f, "\"");
	if(is_directory(f)) {
	f = (system_complete(f).string());
	char c = *f.rbegin();
	#if _WIN32 || _WIN64
	if (c == '\\') replace_last(f, "\\", "\0");
	#else
	if (c == '/') replace_last(f, "/", "\0");
	#endif
	c = '\0';
	}
}
*/

void read(string str, const char* delim){
	vector<string> list;

	//Convert string to char*
	char * writable = new char[str.size() + 1];
	copy(str.begin(), str.end(), writable);
	writable[str.size()] = '\0';
	
	//Split string by delimiter
	strtok(writable,delim);
	while (writable != NULL) {
		list.push_back(writable);
		writable = strtok (NULL, delim);
	}
	delete[] writable; //Deletes char*

	//Analyze list vector & sets values
	if(list[0] == "convert_command") command = list[1];
	else if(list[0] == "convert_source") sdpi = list[1];
	else if(list[0] == "convert_destination") odpi = list[1];
	else if(list[0] == "convert_source_path") sd = list[1];
	else if(list[0] == "convert_destination_path") od = list[1];
	//else { cerr << "\n An error occurred while reading settings "; fp(); }
	list.clear();
}

void settings() {
clear();
if(!exists("settings.ini")) 
	cout << "\n   == settings.ini not found! ==\n\n Press [ENTER] to set it to Defaults";
else if(exists("settings.ini")) 
	cout << "\n Press [ENTER] to set it to Defaults";

	cout << "\n -----------------------------------"
	     << "\n\n Source DPI: ";
	getline(cin, sdpi);
	cout << "\n Output DPI: ";
	getline(cin, odpi);
	cout << "\n Source Folder: ";
	getline(cin, sd);
	cout << "\n Output Folder: ";
	getline(cin, od);
	
	string convert = cd + "\\other\\convert.exe";
	#if _WIN32 || _WIN64
	command = convert + " -resize %3$s -unsharp 0x1 %1$s %2$s";
	#else
	command = "convert -resize %3$s -unsharp 0x1 %1$s %2$s";
	#endif
	if(sdpi.empty()) sdpi = "xhdpi";
	if(odpi.empty()) odpi = "mdpi,ldpi,hdpi";
	if(sd.empty()) sd = "source";
	if(od.empty()) od = "output";
	ofstream zfile;
	zfile.open("settings.ini");
	zfile << "convert_command=" << command << "\n"; //Overwrites first file
	zfile.close();
	
	ofstream ofile;
	ofile.open("settings.ini", ofstream::app); //Appends to file
	ofile << "convert_source=" << sdpi << "\n";
	ofile << "convert_destination=" << odpi << "\n";
	ofile << "convert_source_path=" << sd << "\n";
	ofile << "convert_destination_path=" << od << "\n";
	ofile << "tmp_dir=temp\n";
	ofile << "xhdpi=2\n";
	ofile << "hdpi=1.5\n";
	ofile << "mdpi=1\n";
	ofile << "ldpi=0.75\n";
	ofile.close();
}

void checksettings() {
	if(exists("settings.ini")) {
		char* str = new char[256]; //256 max number of chars to hold
		ifstream infile;
		infile.open("settings.ini");
		for(int i=1; i < 6; i++) {
		infile.getline(str, 256);
		read(str, "=");
		}
		delete[] str;
		infile.close();
	}
	else if(!exists("settings.ini")) settings();
}

void log_color() {
	#if _WIN32 || _WIN64
		if(logop == "Enabled")
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		else SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << logop;
		SetConsoleTextAttribute(hConsole, saved_colors);
	#else
		if(logop == "Enabled") cout << "\033[1;32m" << logop << "\033[0m";
		else cout << "\033[1;31m" << logop << "\033[0m";
	#endif
}

string getdate() {
//Get Current Date
date current_date(day_clock::local_day());
      date::ymd_type ymd = current_date.year_month_day();
      greg_weekday wd = current_date.day_of_week();
//Get Current Time
string suffix;
time_t now = time(0);
struct tm *current = localtime(&now);
if(current->tm_hour > 11) suffix = "PM";
else if(current->tm_hour < 12) suffix = "AM";
ostringstream oss;
oss		<< wd.as_long_string() << " "
		<< ymd.month.as_long_string() << " "
		<< ymd.day << " " << ymd.year << "  "
		<< current->tm_hour-12 << ":" << current->tm_min << ":" << current->tm_sec
		<< " " << suffix;
string l = oss.str();
return l;
}

void checks() {
	if(!exists("settings.ini")) menu();
	#if _WIN32 || _WIN64
	if(!exists(cd + "\\other\\convert.exe")) {
	cerr << "\n Error: ";
	SetConsoleTextAttribute(hConsole, 0 | 7*16);
	cerr << "other\\convert.exe";
	SetConsoleTextAttribute(hConsole, saved_colors);
	cerr << " not found. ";
	fp();
	menu();
	}
	if(!exists(cd + "\\other\\tee.exe")) {
	cerr << "\n Error: ";
	SetConsoleTextAttribute(hConsole, 0 | 7*16);
	cerr << "other\\tee.exe";
	SetConsoleTextAttribute(hConsole, saved_colors);
	cerr << " not found. ";
	fp();
	menu();
	}
	#else
	if(!exists("/usr/bin/convert")) {
	cerr << "\n ImageMagick not found. ";
	fp();
	menu();
	}
	#endif
	if(wildcard("*.jar") == "") {
	cerr << "\n Jar file not found. ";
	fp();
	menu();
	}
	if(!is_directory(sd)) create_directory(sd);
	if(!is_directory(od)) create_directory(od);
	unsigned long pic_count = 0;
	string z;
	#if _WIN32 || WIN64
	z = cd + "\\" + path(sd).stem().string();
	#else
	z = cd + "/" + path(sd).stem().string();
	#endif
    directory_iterator end_iter;
	for (directory_iterator dir_itr(z); dir_itr != end_iter; ++dir_itr)
		if (is_regular_file(dir_itr->status())) {
		path s = dir_itr->path().filename();
		if(iequals(s.extension().string(), ".PNG")) ++pic_count;
		else if(iequals(s.extension().string(), ".JPG")) ++pic_count;
		else if(iequals(s.extension().string(), ".JPEG")) ++pic_count;
		else if(iequals(s.extension().string(), ".BMP")) ++pic_count;
		else if(iequals(s.extension().string(), ".GIF")) ++pic_count;
		else if(iequals(s.extension().string(), ".JFIF")) ++pic_count;
		else if(iequals(s.extension().string(), ".JP2")) ++pic_count;
		else if(iequals(s.extension().string(), ".EXIF")) ++pic_count;
		else if(iequals(s.extension().string(), ".TIF")) ++pic_count;
		else if(iequals(s.extension().string(), ".TIFF")) ++pic_count;
		else if(iequals(s.extension().string(), ".RAW")) ++pic_count;
		else if(iequals(s.extension().string(), ".WEBP")) ++pic_count;
		else if(iequals(s.extension().string(), ".DDS")) ++pic_count;
		else if(iequals(s.extension().string(), ".TGA")) ++pic_count;
		else if(iequals(s.extension().string(), ".THM")) ++pic_count;
		}
	if(pic_count == 0) {
	#if _WIN32 || _WIN64
	cerr << "\n No pictures found in: ";
	SetConsoleTextAttribute(hConsole, 0 | 7*16);
	cerr << sd;
	SetConsoleTextAttribute(hConsole, saved_colors);
	#else
	cerr << "\n No pictures found in: \033[7;7m" << sd << "\033[0m";
	#endif
	fp();
	menu();
	}
}

void convert() {
checks();
const char* log("art_log.txt");
clear();
cout << '\n';
if(logop == "Enabled") {
	ofstream ofile;
	ofile.open(log, ofstream::app);
	ofile << "\n\n                    " << getdate();
	ofile << "\n====================================================================\n";
	ofile.close();
	filtering_ostream file;
    file.push(file_sink(log, ios_base::app));
    // Tee filter instance (will be copied into each filter stream)
    const tee_filter<ostream> teeFilter(file);
    filtering_ostream out; // out tee
    out.push(teeFilter);
    out.push(cout);
	#if _WIN32 || _WIN64
	std::system(("java -jar " + wildcard("*.jar") +
					" convert_source "  + sdpi +
					" convert_destination "  + odpi +
					" convert_source_path "  + sd +
					" convert_destination_path " + od + "|other\\tee.exe " + log).c_str());
	#else
	std::system(("java -jar " + wildcard("*.jar") +
					" convert_source "  + sdpi +
					" convert_destination "  + odpi +
					" convert_source_path "  + sd +
					" convert_destination_path " + od + "|tee -a " + log).c_str());
	#endif
	ofile.open(log, ofstream::app);
	ofile << "====================================================================";
	ofile.close();
	
	//Read log & redirect to screen
	/*ifstream ifile;
	ifile.open(log);
	string temp;
	getline(ifile, temp, '\0');
	cout << temp;
	ifile.close();
	temp.clear();*/
}
else if(logop == "Disabled") {
	std::system(("java -jar " + wildcard("*.jar") +
						" convert_source "  + sdpi +
						" convert_destination "  + odpi +
						" convert_source_path "  + sd +
						" convert_destination_path " + od
						).c_str());
	}
	cout << "\n\nPress any key to continue . . . ";
	fp();
}

void menu() {
	#if _WIN32 || _WIN64
	title();
	std::system("MODE CON:COLS=106 LINES=30");
	#endif
	string i;
	if(!var.empty()) var.clear();
	while(true) {
		checksettings();
		clear();
		if(!i.empty()) i.clear();
		cout	<< "\n Current Directory: " << current_path().string()
				<< "\n\n Settings"
				<< "\n --------"
				<< "\n Command: " << command << '\n'
				<< "\n Source DPI: " << sdpi
				<< "\n Output DPI: " << odpi
				<< "\n\n Directories"
				<< "\n -----------"
				<< "\n Source: " << sd
				<< "\n Output: " << od
				<< "\n\n\n 0 - Logging: ";
				log_color();
		cout	<< "\n\n 1 - Convert NOW!"
				<< "\n\n 2 - Modify Settings"
				<< "\n\n x - Exit"
				<< "\n\n\n Enter selection: ";
		getline(cin,i);
		if(i.empty()) {
			//Do nothing. You pressed enter
		}
		else if(i == "1") convert();
		else if(i == "0")
			if(logop == "Enabled") logop = "Disabled"; else logop = "Enabled";
		else if(i == "2") settings();
		else if(i == "x" || i == "X") {
			#if _WIN32 || _WIN64
			SetConsoleTitle(_T("C:\\WINDOWS\\system32\\cmd.exe"));
			std::system("MODE CON:COLS=82 LINES=25");
			cout << "\nGoodbye.\n";
			#else
			cout << "\n Goodbye.\n\n";
			#endif
			exit(EXIT_SUCCESS);
		}
		else {
			cerr << "\n Invalid selection! ";
			fp();
		}
	}
}
