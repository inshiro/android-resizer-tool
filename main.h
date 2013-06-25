string sd;
string od;
string sdpi;
string odpi;
string logop("Enabled");
string command;
string ver = "2.7";
string title = "Android Resizer Tool v" + ver;
int called(0);
int amountofdpis(5);
void menu();
void convert();

void clear() {
	#if _WIN32 || _WIN64
	system("CLS");
	#else
	system("clear");
	#endif
}

size_t pstrlen(string s1) {
	return strlen(s1.c_str());
}

bool create_dir(string s1) {
	#if _WIN32 || _WIN64
		if( mkdir(s1.c_str()) ) return true;
	#else
		if( mkdir(s1.c_str(), 0777) ) return true;
	#endif
	return false;
}

string extension(string path) {
	if ( path.substr(path.find_last_of(".") + 1) == path ) return path; //No extension/dot in string
	path = path.substr(path.find_last_of(".") + 1);
	path = path.insert(0, ".");
	return path;
}

bool find(string s1, string s2){
	for(size_t i=0; i < pstrlen(s1); i++) s1[i] = toupper(s1[i]);
	for(size_t i=0; i < pstrlen(s2); i++) s2[i] = toupper(s2[i]);
	//Search (using strstr source code)
    register char *a, *b;
	char *substringc = (char*)malloc(pstrlen(s2)+1);
	char *stringc = (char*)malloc(pstrlen(s1)+1);
	
	if (substringc) (string)substringc = s2.c_str();//strcpy(substringc, pstrlen(s2), s2.c_str());
	if (stringc) (string)stringc = s1.c_str();//strcpy(stringc, pstrlen(s2), );
    b = substringc;
    if (*b == 0) { free(substringc); free(stringc); return false; }
    for ( ; *stringc != 0; stringc += 1) {
	if (*stringc != *b) continue;
	a = stringc;
	while (1) {
	    if (*b == 0) { free(substringc); free(stringc); return true; }
	    if (*a++ != *b++) {
		break;
	    }
	}
	b = substringc;
    }
    free(substringc);
	free(stringc);
	return false;
}

void trim(string &s1) {
	char* str = (char*)s1.c_str();
	//s1 = &str;
	char *end;
	// Trim leading space
	while(isspace(*str)) str++;
	//if(*str == 0) return str; // All spaces?
	end = str + pstrlen(str) - 1; // Trim trailing space
	while(end > str && isspace(*end)) end--;
	*(end+1) = 0; // Write new null terminator
	s1 = str;
}

bool exists(string s1) {
	if ( access(s1.c_str(), 0) != -1 ) return true;
	return false;
}

bool direxists(string s1) {
	struct stat sb;
	if( stat(s1.c_str(),&sb) != -1 )
		if( (sb.st_mode & S_IFMT) == S_IFDIR ) return true;
	return false;
}

bool erase(string s1) {
	if ( remove(s1.c_str()) !=0 ) return false;
	else return true;
}

string current_path() {
	/*int PATHMAX = PATH_MAX;
	if(PATH_MAX == 260) PATHMAX = 1024;
	char cwd[PATHMAX];
	if ( getcwd(cwd, sizeof(cwd)) ) return string(cwd).c_str();
	else return string(cwd).c_str();
	*/

	char cCurrentPath[FILENAME_MAX];
	if ( GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)) ) {
		cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
		return (string)cCurrentPath;
	}
	return NULL;
}

bool compare(string str1, string str2) {
	for(size_t i=0; i < pstrlen(str1); i++) str1[i] = tolower(str1[i]);
	for(size_t i=0; i < pstrlen(str2); i++) str2[i] = tolower(str2[i]);
	/*for(size_t i=0; i < pstrlen(str1); i++) str1[i] = toupper(str1[i]);
	for(size_t i=0; i < pstrlen(str2); i++) str2[i] = toupper(str2[i]);
	const char *s1 = str1.c_str();
	const char *s2 = str2.c_str();
	while (*s1 == *s2++) {
		if (*s1++ == 0) return false;
	}
	if( (*(unsigned char *) s1 - *(unsigned char *) --s2) != 0 ) return true;
	*/
	if( strcmp(str1.c_str(), str2.c_str()) != 0 ) return true;
	//if( str1 == str2 ) return true;
	return false;
}

#if _WIN32 || _WIN64
//#define fendl	endl
#define ngetch() _getch()
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
void colourtext(bool err, string message, int colour) {
	if( err ) {
		SetConsoleTextAttribute(hConsole, 0 | colour);
		cerr << message;
		SetConsoleTextAttribute(hConsole, saved_colors);
	}
	else if( !err ) {
		SetConsoleTextAttribute(hConsole, 0 | colour);
		cout << message;
		SetConsoleTextAttribute(hConsole, saved_colors);
	}
}
/*
string wchartostring(WCHAR wc[]) {
    char ch[260];
    char DefChar = ' ';
    WideCharToMultiByte(CP_ACP,0,wc,-1, ch,260,&DefChar, NULL);
    string ss(ch);
	return ss;
}
*/
string wildcard(string arg) {
    vector<string> list;
	WIN32_FIND_DATA fd;
	//wstring stemp = s2ws(arg);
	//LPCWSTR result = stemp.c_str();
	HANDLE h = FindFirstFile( arg.c_str(), &fd );
	if(h == INVALID_HANDLE_VALUE) return "";
	string temp;
	while(true) {
		list.push_back(fd.cFileName);
		if(FindNextFile(h, &fd) == FALSE)
		break;
	}
	//for(size_t i = 0; i < list.size(); i++) // Using Index Based Accessing
	//	temp = list[i].c_str();
	for(vector<string>::const_iterator it = list.begin(); // Using const_iterator
		it != list.end(); ++it) temp = it->c_str();
	list.clear();
	return temp;
}
const char *complete(string path) {
    DWORD  retval=0;
    TCHAR  buffer[4096]=TEXT(""); 
    TCHAR  buf[4096]=TEXT(""); 
    TCHAR** lppPart={NULL};
	const char *s;
    if( GetFullPathName(path.c_str(),
                 4096,
                 buffer,
                 lppPart) != 0) { s = buffer; return s; }
	return NULL;
}

int findJava() {
	HKEY hKey;
	LONG dwRegOPenKey = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\JavaSoft\\Java Development Kit\\"), 0, KEY_READ, &hKey);
	TCHAR    achKey[255];   // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys=0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 

	DWORD i, retCode;
	
	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	// Enumerate the subkeys, until RegEnumKeyEx fails.
	int final = 0;
	final = exists("C:\\WINDOWS\\system32\\java.exe");
	if (cSubKeys) {
		for (i=0; i<cSubKeys; i++) {
			cbName = 255;
			retCode = RegEnumKeyEx(hKey, i,
				achKey, 
				&cbName, 
				NULL, 
				NULL, 
				NULL, 
				&ftLastWriteTime); 
			if (retCode == ERROR_SUCCESS) final++;
			}
		}
	RegCloseKey(hKey);
	hKey = 0;
	if( getenv("JAVA_HOME") != NULL ) final++;
	DIR *dir = opendir("C:\\Program Files\\Java");
	struct dirent *ent;
	struct stat sb;
	if ( dir != NULL ) {
		while ((ent = readdir (dir)) != NULL ) {
			if( compare(ent->d_name,".") && compare(ent->d_name, "..") ) {
				if( stat("C:\\Program Files\\Java", &sb) == -1 ) break;
				if( (sb.st_mode & S_IFMT) == S_IFDIR )
					if( direxists((string)"C:\\Program Files\\Java\\" + ent->d_name) ) final++;
			}
		}
	}
	dir = opendir("C:\\Program Files (x86)\\Java");
	if ( dir != NULL ) {
		while ((ent = readdir (dir)) != NULL ) {
			if( compare(ent->d_name,".") && compare(ent->d_name, "..") ) {
				if( stat("C:\\Program Files (x86)\\Java", &sb) == -1 ) break;
				if( (sb.st_mode & S_IFMT) == S_IFDIR )
					if( direxists((string)"C:\\Program Files (x86)\\Java\\" + ent->d_name) ) final++;
			}
		}
	}
	closedir(dir);
	//if( final != 0 ) final = true;
	return final;
}
#else
//#define fendl	endl << endl
string complete(string s1) {
	char buf[PATH_MAX + 1];
	char *res = realpath((char *)s1.c_str(), buf);
	//if (realpath((char *)s1, buf)) return buf;
	//return (char *)s1;
	return res;
}
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
int wkbhit () {
	struct timeval tv;
	fd_set rdfs;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO(&rdfs);
	FD_SET (STDIN_FILENO, &rdfs);

	select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
	return FD_ISSET(STDIN_FILENO, &rdfs);

}
int kbhit() {
	fflush(stdout);
	int ch;
	static struct termios oldt, newt;
	//changemode
	tcgetattr( STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt);
	while ( !wkbhit() ){
		//putchar('.');
	}
	ch = getchar();
	//changemode
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}
#define ngetch() kbhit()
#endif

void ftitle() {
	#if _WIN32 || _WIN64
	//wstring stemp = s2ws(title);
	//LPCWSTR result = stemp.c_str();
	SetConsoleTitle(title.c_str());
	#else
	//printf("%c]0;%s%c", '\033', title, '\007');
	cout << "\033]0;" + title + '\007';
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

void erase_all(string& subject, const string search) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != string::npos) {
         subject.replace(pos, search.length(), "\0");
         pos += pstrlen("\0");
    }
}

void replace_last(string &myString, string replace) {
	myString = myString.substr(0, myString.size()-1);
	myString += replace;
}

void cleanpath(string &f) {
	if( f.find("\"") != string::npos) erase_all(f, "\"");
	trim(f);
	if(direxists(f)) {
	f = complete(f);
	char c = *f.rbegin();
	#if _WIN32 || _WIN64
	if (c == '\\') replace_last(f, "\0");
	#else
	if (c == '/') replace_last(f, "\0");
	#endif
	c = '\0';
	}
}

string finalstem(string s1, const char* search){
	string result;
	size_t *pos = new size_t;
	*pos = s1.find_last_of(search);
	if(*pos != string::npos)
		result.assign(s1.begin() + *pos + 1, s1.end());
	else
		result = s1;
	delete pos;
	return result;
}

string finalstemdir(string str) {
	#if _WIN32 || _WIN64
	return finalstem(str, "\\");
	#else
	return finalstem(str, "/");
	#endif
}

string removeSubstring(string str, string toremove){
	unsigned *found = new unsigned;
	*found = str.find(toremove);
	if (*found!=string::npos) {
		for(size_t i=0; i < toremove.length(); i++) str.erase(str.begin()+0);
		}
	delete found;
	return str;
}

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
	delete[] writable;

	//Analyze list vector & sets values
	if(list[0] == "convert_command") command = list[1];
	else if(list[0] == "convert_source") sdpi = list[1];
	else if(list[0] == "convert_destination") odpi = list[1];
	else if(list[0] == "convert_source_path") sd = list[1];
	else if(list[0] == "convert_destination_path") od = list[1];
	list.clear();
}

bool read_dpi(string str, string search) {
	vector<string> list;
	//Convert string to char*
	char * writable = new char[str.size() + 1];
	copy(str.begin(), str.end(), writable);
	writable[str.size()] = '\0';
	
	//Split string by delimiter
	strtok( writable, "," );
	while (writable != NULL) {
		list.push_back(writable);
		writable = strtok( NULL, "," );
	}
	delete[] writable;

	//Analyze list vector & sets values
	trim(search);
	bool result = false;
	for(size_t i=0; i < list.size(); i++) {
		trim(list[i]);
		if( list[i] == search ) result = true;
	}
	list.clear();
	return result;
}

bool dfind(string x, string y) {
	trim(x);
	if( x.find(",") != string::npos )  if( read_dpi(x, y) ) return true;
	if( x.find(y) != string::npos ) {
		//Checking if it's a false alarm
		if( x.find("hdpi") != string::npos ) {
			if( x == "xhdpi" ) return false;
			else return true;
		}
	return true; //Found
	}
	return false; //Not Found
}

void dpimenu1() {
		string c;
		//string bsdpi = sdpi;
		//string bodpi = odpi;
		if( !sdpi.empty() ) sdpi.clear();
		if( !odpi.empty() ) odpi.clear();
		vector<string> dpis;
		dpis.reserve(amountofdpis);
		dpis.push_back("");
		dpis.push_back("xhdpi");
		dpis.push_back("hdpi");
		dpis.push_back("mdpi");
		dpis.push_back("ldpi");
		const char *message = "\n\t1: XHDPI\t2: HDPI\n\t3: MDPI\t\t4: LDPI\n\n ";
		int ifclause;
		cout << message << "Enter Source DPI: ";
		while(true) {
			c.clear();
			c = ngetch();
			ifclause = ( atoi(c.c_str()) > 0 && atoi(c.c_str()) < amountofdpis );
			//fflush(stdout);
			if(c == "\r" || c == "\n") { sdpi = "xhdpi"; cout << endl; break; }
			if(ifclause) {
				sdpi = dpis[atoi(c.c_str())];
				cout << c << endl;
				break;
			}
		}

		cout << message << "Enter Output DPI: ";
		while(true) {
			c.clear();
			c = ngetch();
			//fflush(stdout);
			ifclause = ( atoi(c.c_str()) > 0 && atoi(c.c_str()) < amountofdpis );
			if(c == "\r" || c == "\n") { odpi = "hdpi,mdpi,ldpi"; cout << endl; break; }
			if( ifclause && sdpi == dpis[atoi(c.c_str())] ) { cerr << "Cannot set the same value as source dpi\n"; cout << message << "Enter Output DPI: "; }
			else if( ifclause && sdpi != dpis[atoi(c.c_str())] ) {
				odpi = dpis[atoi(c.c_str())];
				cout << c << endl;
				break;
			}
		}
		dpis.clear();
}

void dpimenu2() {
		string c;
		vector<string> dpis;
		dpis.reserve(amountofdpis);
		dpis.push_back("");
		dpis.push_back("xhdpi");
		dpis.push_back("hdpi");
		dpis.push_back("mdpi");
		dpis.push_back("ldpi");
		int ifclause=0;
		int count=0;
		const char *message = "\n\t1: XHDPI\t2: HDPI\n\t3: MDPI\t\t4: LDPI\n\n More Output DPIs?: ";
		cout << message;
		while(true) {
			c.clear();
			c = ngetch();
			//fflush(stdout);
			ifclause = ( atoi(c.c_str()) > 0 && atoi(c.c_str()) < amountofdpis );
			if(c == "\r" || c == "\n" || c == "n" || c == "N") break;
			if( ifclause && dfind(odpi, dpis[atoi(c.c_str())]) ) { cerr << "Already included!\n"; cout << message; }
			else if( ifclause && sdpi == dpis[atoi(c.c_str())] ) { cerr << "Cannot set the same value as source dpi\n"; cout << message; }
			else if( ifclause && sdpi != dpis[atoi(c.c_str())] ){
				count++;
				odpi += "," + dpis[atoi(c.c_str())];
				cout << c;
				if ( count != 2 ) cout << endl << message;
				else if ( count == 2 ) break;
			}
		}
		dpis.clear();
}

void settings() {
if(called==0) clear();
	cout << "\n   == Generating settings.ini ==\n";
	cout << "\n Press [ENTER] to set it to Defaults";
	cout << "\n -----------------------------------\n";
	dpimenu1();
	dpimenu2();
	cout << "\n\n Source Folder: ";
	getline(cin, sd);
	cout << "\n Output Folder: ";
	getline(cin, od);
	
	#if _WIN32 || _WIN64
	command = "other\\convert.exe -resize %3$s %1$s %2$s";
	#else
	if(exists("other/convert")) command = "other/convert -resize %3$s %1$s %2$s";
	else command = "convert -resize %3$s %1$s %2$s";
	#endif
	if(sdpi.empty()) sdpi = "xhdpi";
	if(odpi.empty()) odpi = "mdpi,ldpi,hdpi";
	if(sd.empty()) sd = "source";
	if(od.empty()) od = "output";
	ofstream zfile;
	zfile.open((string(cd + "/settings.ini")).c_str());
	zfile << "convert_command=" << command << "\n"; //Overwrites first file
	zfile.close();
	cleanpath(sd);
	cleanpath(od);
	/*
	string shorten = finalstemdir(sd); //path(sd).stem().stem().string();
	if(is_directory(shorten)) sd = shorten;
	cleanpath(od);
	shorten = system_complete(path(od)).parent_path().string();
	if(shorten == cd) od = finalstemdir(od); //path(od).stem().stem().string();
	shorten.clear();
	
	sdpi = algorithm::to_lower_copy(sdpi);
	odpi = algorithm::to_lower_copy(odpi);*/
	ofstream ofile;
	ofile.open((string(cd + "/settings.ini")).c_str(), ofstream::app); //Appends to file
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
	if(called == 2) convert();
	else if(called) exit(EXIT_SUCCESS);
	else if(!called) menu();
}

void readsettings() {
	if(!exists(cd + "/settings.ini")) settings();
	else if(exists(cd + "/settings.ini")) {
		char* str = new char[1024]; //1024 max number of chars to hold
		ifstream infile;
		infile.open((string(cd + "/settings.ini")).c_str());
		for(int i=1; i < 6; i++) {
			infile.getline(str, 1024);
			read(str, "=");
		}
		delete[] str;
		infile.close();
	}
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
	string *suffix = new string;
	*suffix = "PM";
	time_t current = time(0);
	struct tm *now  = localtime( &current );
	int *hour = new int;
	*hour = now->tm_hour-12;
	if (*hour < 0) { *suffix = "AM"; *hour += 12; }
	if (*hour == 0) *hour = 12;
	vector<string> wd, month;
	wd.reserve(7);
	month.reserve(12);
	wd.push_back("Sunday");
	wd.push_back("Monday");
	wd.push_back("Tuesday");
	wd.push_back("Wednesday");
	wd.push_back("Thursday");
	wd.push_back("Friday");
	wd.push_back("Saturday");
	month.push_back("January");
	month.push_back("February");
	month.push_back("March");
	month.push_back("April");
	month.push_back("May");
	month.push_back("June");
	month.push_back("July");
	month.push_back("August");
	month.push_back("September");
	month.push_back("October");
	month.push_back("November");
	month.push_back("December");
	ostringstream oss;
	oss		<< wd[now->tm_wday] << " "
			<< month[now->tm_mon] << " "
			<< now->tm_mday << " " << now->tm_year + 1900 << "  "
			<< *hour << ":" << now->tm_min << ":" << now->tm_sec
			<< " " << *suffix;
	delete hour;
	delete suffix;
	wd.clear();
	month.clear();
	return string(oss.str()).c_str();
}

void notexist(string file, string message) {
	if(!exists(file)) {
		if(called) {
		cerr << message << '\n';
		exit(1);
		}
		if(!called) {
		cerr << "\n " << message;
		fp();
		menu();
		}
	}
}

bool checkimage(string s) {
	int final = 0;
	ifstream in(s.c_str(), ios::binary);
	if( !in.is_open()) { in.close(); return false; }
	unsigned char magic[4] = {0};
	in.read((char*)magic, sizeof(magic));
	in.close();
	if( magic[0] == 0x89 && magic[1] == 0x50 && magic[2] == 0x4e && magic[3] == 0x47 ) final++;
	else if( magic[0] == 0xff && magic[1] == 0xd8 && magic[2] == 0xff && magic[3] == 0xe0 ) final++;
	else if( magic[0] == 0x47 && magic[1] == 0x49 && magic[2] == 0x46 && magic[3] == 0x38 ) final++;
	else if( magic[0] == 0x42 && magic[1] == 0x4d ) final++;
	if( final != 0 ) return true;
	return false;
}

void checks() {
	if(!exists(cd + "/settings.ini")) menu();
	#if _WIN32 || _WIN64
	if(!exists(cd + "\\other\\convert.exe")) {
		if(called) {
			cerr << "Error: ";
			colourtext( 1, "other\\convert.exe", 7*16 );
			cerr << " not found\n";
			exit(1);
		}
		cerr << "\n Error: ";
		colourtext( 1, "other\\convert.exe", 7*16 );
		cerr << " not found";
		fp();
		menu();
	}
	if(!exists(cd + "\\other\\tee.exe")) {
		if(called) {
			cerr << "Error: ";
			colourtext( 1, "other\\tee.exe", 7*16 );
			cerr << " not found\n";
			exit(1);
		}
		cerr << "\n Error: ";
		colourtext( 1, "other\\tee.exe", 7*16 );
		cerr << " not found";
		fp();
		menu();
	}
	if(!findJava()) {
		if(called) {
			cerr << "Error: ";
			colourtext( 1, "Java", 7*16 );
			cerr << " not found\n";
			exit(1);
		}
		cerr << "\n Error: ";
		colourtext( 1, "Java", 7*16 );
		cerr << " not found";
		fp();
		menu();
	}
	#else
	if(!exists("other/convert") && !exists("/usr/bin/convert")) {
		if(called) {
			cerr << "Error: 'other/convert' not found" << endl;
			exit(1);
		}
		cerr << "\n " << "Error: 'other/convert' not found";
		fp();
		menu();
	}
	notexist("other/analyze.so", "Error: 'other/analyze.so' not found");
	notexist("other/analyze.la", "Error: 'other/analyze.la' not found");
	notexist("/usr/bin/java", "Error: Java not found");
	#endif
	// Checks if input/output folders has spaces in them
	if ( sd.find(" ") != string::npos ) {
		if(called) {
		cerr << "Error: Your input folder has spaces!\n";
		exit(1);
		}
		cerr << "\n Error: Your input folder has spaces!";
		fp();
		menu();
	}
	if ( od.find(" ") != string::npos ) {
		if(called) {
		cerr << "Error: Your output folder has spaces!\n";
		exit(1);
		}
		cerr << "\n Error: Your output folder has spaces!";
		fp();
		menu();
	}
	if ( cd.find(" ") != string::npos ) {
		if(called) {
		cerr << "Error: Current Directory has spaces! Please move this tool somewhere else\n";
		exit(1);
		}
		cerr << "\n Error: Current Directory has spaces! Please move this tool somewhere else";
		fp();
		menu();
	}
	if(wildcard("*.jar") == "") {
		if(called) {
		cerr << "Error: Jar file not found\n";
		exit(1);
		}
		cerr << "\n Error: Jar file not found";
		fp();
		menu();
	}
	
	if(!direxists(sd)) create_dir(sd);
	else if(direxists(sd)) sd = complete(sd);
	if(!direxists(od)) create_dir(od);
	else if(direxists(od)) od = complete(od);
	DIR *dir;
	struct dirent *ent;
	struct stat sb;
	int pic_count = 0;
	/*string pics[] = {
		".PNG", ".JPG", ".JPEG", ".BMP", ".GIF", ".JFIF", ".JP2",
		".EXIF", ".TIF", ".TIFF", ".RAW", ".WEBP", ".DDS", ".TGA", ".THM"
	};*/
	/*vector<string> pics;
	pics.reserve(15);
	pics.push_back(".PNG");
	pics.push_back(".JPG");
	pics.push_back(".JPEG");
	pics.push_back(".BMP");
	pics.push_back(".GIF");
	pics.push_back(".JFIF");
	pics.push_back(".JP2");
	pics.push_back(".EXIF");
	pics.push_back(".TIF");
	pics.push_back(".TIFF");
	pics.push_back(".RAW");
	pics.push_back(".WEBP");
	pics.push_back(".DDS");
	pics.push_back(".TGA");
	pics.push_back(".THM");*/
	if ((dir = opendir (sd.c_str())) == NULL) { perror (""); exit(1); }
	while ((ent = readdir (dir)) != NULL ) {
		if( compare(ent->d_name,".") != 0 && compare(ent->d_name, "..") != 0 ) {
		string fullpath = sd + "/" + ent->d_name;
		fullpath = complete(fullpath);
		if( stat(fullpath.c_str(), &sb) == -1 ) break;
		if( (sb.st_mode & S_IFMT) == S_IFREG ) {
			//while (!pic_count) {
				//for (size_t i=0; i < pics.size(); i++) {
					//if(!pics[i].empty())
						//if( compare(extension(fullpath), pics[i]) )
							if( checkimage(fullpath) ) pic_count++;
					//}
				//break;
				//}
			}
		}
	}
	closedir(dir);
	//pics.clear();
	//fp();
	if(!pic_count) {
	#if _WIN32 || _WIN64
	if(called) {
		cerr << "No pictures found in: ";
		colourtext( 1, sd + "\n", 7*16 );
		exit(1);
	}
	cerr << "\n No pictures found in: ";
	colourtext( 1, sd, 7*16 );
	#else
	if(called) {
		cerr << "No pictures found in: \033[7;7m" << sd << "\033[0m\n";
		exit(1);
	}
	cerr << "\n No pictures found in: \033[7;7m" << sd << "\033[0m";
	#endif
	fp();
	menu();
	}
}

bool convert_newline_file(string path) {
	struct stat s_buf;
	if (stat (path.c_str(), &s_buf) == -1) return false;
	string temppath = "./" + path + ".tmp";
	FILE *in = fopen(path.c_str(), "rb"), *out = fopen(temppath.c_str(), "wb");
	struct utimbuf ut_buf;
	int ch, prev_ch= 0;
	bool rval = false;
	if ( in == NULL ) return true;
	if ( out == NULL ) {
		fclose (in);
		return true;
	}
	int LF = 0x0A;
	int CR = 0x0D;
	
	while ((ch = getc (in)) != EOF) {
		if ( ( ch == LF ) && ( prev_ch != CR ) && ( putc( CR, out ) == EOF) || ( putc( ch, out ) == EOF) ) {
			rval = true;
			break;
		}
		prev_ch = ch;
	}
	if (fclose (in) == EOF) rval = true;
	if (fclose (out) == EOF) rval = true;
	ut_buf.actime = s_buf.st_atime;
	ut_buf.modtime = s_buf.st_mtime;
	if (utime (temppath.c_str(), &ut_buf) == -1) rval = true;
	if (unlink (path.c_str()) == -1) rval = true;
	if (rval) {
		unlink (temppath.c_str());
		return true;
	}
	if (rename (temppath.c_str(), path.c_str()) != -1) return true;
	unlink (temppath.c_str());
	return false;
}

void convert() {
	if (called || called == 2) {
		readsettings();
		called = 1;
		logop == "Enabled";
	}
	checks();
	const char* log("art-log.txt");
	string *tee = new string;
	if(!called) clear();
	cout << endl;
	try {
if(logop == "Enabled") {

	#if _WIN32 || _WIN64
	*tee = "|other\\tee.exe -a ";
	#else
	*tee = "|tee -a ";
	#endif
	ofstream ofile;
	ofile.open(log, ofstream::app);
	ofile	<< "\n                    " << getdate()
			<< "\n====================================================================\n";
	ofile.close();
	system(("java -jar " + wildcard("*.jar") +
					" convert_source "  + sdpi +
					" convert_destination "  + odpi +
					" convert_source_path "  + sd +
					" convert_destination_path " + od +
					" 2>&1 " + *tee + "art-log.txt").c_str());
	delete tee;
	ofile.open(log, ofstream::app);
	ofile << "====================================================================\n";
	ofile.close();
	#if _WIN32 || _WIN64
	convert_newline_file(log);
	#endif
}
else if(logop == "Disabled") {
	system(("java -jar " + wildcard("*.jar") +
						" convert_source "  + sdpi +
						" convert_destination "  + odpi +
						" convert_source_path "  + sd +
						" convert_destination_path " + od).c_str());
	}
	}
	catch(exception e) {
	cerr << endl << e.what();
	}
	if(called) {
	#if _WIN32 | _WIN64
	#else
	cout << endl;
	#endif
	exit(EXIT_SUCCESS);
	}
	cout << endl << "\nPress any key to continue . . . ";
	fp();
}

void menu() {
	string i;
	called = false;
	while(true) {
		readsettings();
		clear();
		if(!i.empty()) i.clear();
		cout	<< "\n Current Directory: " << current_path()
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
				<< "\n\n" << endl << " Enter selection: ";
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
			system("MODE CON:COLS=82 LINES=25");
			cout << "\nGoodbye." << endl;
			#else
			cout << "\n Goodbye.\n" << endl;
			#endif
			exit(EXIT_SUCCESS);
		}
		else {
			cerr << endl << " Invalid selection! ";
			fp();
		}
	}
}