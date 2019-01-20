#include "kmp.h"
#include <limits>
#include <fstream>
#include <windows.h>
#include <conio.h>
using namespace std;

namespace util { /* Various uncategorised functions */
	string generateRandomText(const size_t length)
	{
		string text, letters = "abcdefghijklmnopqrstuwxyzABCDEFGHIJKLMNOPQRTUWXYZ";
		char d;
		for (size_t i = 0; i<length; i++) {
			d = letters[rand()%letters.length()];
			text.push_back(d);
		}
		return text;
	}
}

namespace kmp { /* Search related functions, meat of the program */
	vector<long> preprocess(const string &pattern)
	{
		vector<long> offsetTable;
		long j = 0, t = -1;
		offsetTable.push_back(t);
		while ( j < (long)pattern.length()-1 ) {
			while ( t > -1  &&  pattern.at(j) != pattern.at(t) ) {
				t = offsetTable.at(t);
			}
			t++; j++;
			if ( pattern.at(j) == pattern.at(t) ) {
				offsetTable.push_back( offsetTable.at(t) );
			} else {
				offsetTable.push_back(t);
			}
		}
		return offsetTable;
	}

	vector<size_t> search(const string &text, const string &pattern)
	{
		vector<size_t> matchTable;
		vector<long> next = preprocess(pattern);
		size_t i = 0;
		long j = 0;
		while ( i < text.length() ) {
			while (j > -1  &&  text.at(i) != pattern.at(j) ) {
				j = next[j];
			}

			if ((size_t)j == pattern.length()-1) {
				matchTable.push_back(i-pattern.length()+1);	// Adding one because "i-pattern.length" points to a char just before the comparasment start
				j = -1;
			}
			i++; j++; 
		}
		return matchTable;
	}
}

namespace ui { /* User interface */
	long wczytaj(const long d_gran, const long g_gran, const string message)
	{
		long x = 0;
		if (message.length() == 0) {
			cout<<"<"<<d_gran<<", "<<g_gran<<">:$ ";
		} else {
			cout<<message<<", dostępny zakres: <"<<d_gran<<", "<<g_gran<<">: ";
		}
		do {
			cin>>x;
			if (x >= d_gran && x <= g_gran) {
				break;
			} else {
				cout<<"Wartość jest nieprawidłowa, dostępny zakres: <"<<d_gran<<", "<<g_gran<<">: ";
			}
		} while (1);
		return x;
	}

	void printUsageInfo(void)
	{
		cout<<"Program wyszukuje podciągi znaków w większym ciągu znaków. Na przykład słowa w tekście."<<endl;
		cout<<"Należy przy tym wspomieć, że program nie posiada wsparcia dla wyszukiwania polskich znaków, ani wyrażeń regularnych"<<endl;
		cout<<"Wyjaśnienia znajdują się w instrukcji użytkownika."<<endl;
		cout<<"Aby wyjść z programu, wybierz opcję nie znajdującą się w menu."<<endl; 
	}

	void askForConfirmation(char &choice)
	{
		do {
			cout<<"<Y/n>:$ "; cin>>choice;
		} while (!(choice == 'Y' || choice == 'y' || choice == 'N' || choice == 'n') );
	}

	void printColored(const string str, const int color)
	{
		static HANDLE console;
		static CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		static bool needInit = 1;
		if (needInit) {
			needInit = 0;
			console = GetStdHandle(STD_OUTPUT_HANDLE);
			GetConsoleScreenBufferInfo(console, &consoleInfo);
		}
		SetConsoleTextAttribute(console, color);
		cout<<str;
		SetConsoleTextAttribute(console, consoleInfo.wAttributes);
	}

	string getText(void)
	{
		enum inputMethod {
			MANUAL = 1,
			RANDOM = 2,
			FROMFILE = 3,
			EXIT_INPUT = 4
		};
		char choice = 0;
		size_t len = 0;
		string text, path;
		bool menuLoop = 1;
		bool filePathChoiceLoop = 1;
		do {
			cout<<"# Podaj sposób wprowadzenia tekstu do przeszukania:"<<endl;
			cout<<"\t1. Z klawiatury"<<endl;
			cout<<"\t2. Wygeneruj tekst losowo"<<endl;
			cout<<"\t3. Z pliku"<<endl;
			choice = wczytaj(1, 4, "");
			switch (choice) {
				case MANUAL: 
					cout<<"Wprowadź tekst do przeszukania: ";
					cout.flush(); cin.sync(); 
					getline(cin, text);
					menuLoop = 0;
					break;
				case RANDOM:
					cout<<"Podaj długość tekstu do wygenerowania (max. "<<numeric_limits<long>::max()<<"):"<<endl;
					len = (size_t)wczytaj(0, numeric_limits<long>::max(), "");
					text = util::generateRandomText(len);
					cout<<"\n# Wygenerowany tekst: "<<endl<<text<<endl<<endl;
					menuLoop = 0;
					break;
				case FROMFILE:
					do {
						cout<<"Podaj ścieżkę pliku do otworzenia: "; cin>>path;
						text = io::getStrFromFile(path);
						if (text.length()) {
							cout<<"\n# Wczytany tekst: "<<endl<<text<<endl<<endl;
							filePathChoiceLoop = 0;
							menuLoop = 0;
						} else {
							cout<<"Otworzenie pliku nie powiodło się, lub wczytywany plik jest pusty! Czy chcesz wczytać inny?"<<endl;
							askForConfirmation(choice);
							switch (choice) {
								case 'N':
								case 'n':
									filePathChoiceLoop = 0;
									break;
							}
						}
					} while (filePathChoiceLoop);
					break;
				case EXIT_INPUT:
					cout<<"Zamykam program..."<<endl;
					exit(0);
					break;
			}
		} while (menuLoop);
		return text;
	}
	
	string getPattern(void)
	{
		enum inputMethod {
			MANUAL = 1,
			RANDOM = 2,
			FROMFILE = 3,
			EXIT_INPUT = 4
		};
		char choice = 0;
		size_t len = 0;
		string pattern, path;
		bool menuLoop = 1;
		bool filePathChoiceLoop = 1;
		do {
			cout<<"# Podaj sposób wprowadzenia wzorca do wyszukania:"<<endl;
			cout<<"\t1. Z klawiatury"<<endl;
			cout<<"\t2. Wygeneruj wzorzec losowo"<<endl;
			cout<<"\t3. Z pliku"<<endl;
			choice = wczytaj(1, 4, "");
			switch (choice) {
				case MANUAL: 
					cout<<"Wprowadź wzorzec do wyszukania: "; 
					cout.flush(); cin.sync();
					getline(cin, pattern);
					menuLoop = 0;
					break;
				case RANDOM:
					cout<<"Podaj długość wzorca do wygenerowania (max. "<<numeric_limits<long>::max()<<"):"<<endl;
					len = (size_t)wczytaj(0, numeric_limits<long>::max(), "");
					pattern = util::generateRandomText(len);
					cout<<"\n# Wygenerowany wzorzec: "<<endl<<pattern<<endl<<endl;
					menuLoop = 0;
					break;
				case FROMFILE:
					do {
						cout<<"Podaj ścieżkę pliku do otworzenia: "; cin>>path;
						pattern = io::getStrFromFile(path);
						if (pattern.length()) {
							cout<<"\n# Wczytany wzorzec: "<<endl<<pattern<<endl<<endl;
							filePathChoiceLoop = 0;
							menuLoop = 0;
						} else {
							cout<<"Otworzenie pliku nie powiodło się, lub wczytywany plik jest pusty! Czy chcesz wczytać inny?"<<endl;
							askForConfirmation(choice);
							switch (choice) {
								case 'N':
								case 'n':
									filePathChoiceLoop = 0;
									break;
							}
						}
					} while (filePathChoiceLoop);
					break;
				case EXIT_INPUT:
					cout<<"Zamykam program..."<<endl;
					exit(0);
					break;
			}
		} while (menuLoop);
		return pattern;
	}
	
	int getMode(void)
	{
		/* enum type regarding the loaded option is defined in kmp.h, as it is used in main() */
		int choice = 0;
		cout<<"# W jaki sposób chciałbyś wyszukiwać wzorzec?"<<endl;
		cout<<"\t1. Inkrementalnie"<<endl;
		cout<<"\t2. Ciągle"<<endl;
		choice = wczytaj(1, 3, "");
		if (choice == EXIT_MODE) {
			cout<<"Zamykam program..."<<endl;
			exit(0);
		}
		return choice;
	}
	
	namespace answer {	
		void draw(const string &text, const string &pattern)
		{
			vector<size_t> matchTable = kmp::search(text, pattern);
			size_t j = 0;
			cout<<"\n# Wyniki wyszukiwania:"<<endl;
			for (size_t i = 0; i<text.length();) {
				if (!matchTable.empty()  &&  j<matchTable.size()) {
					cout<<text.substr(i, matchTable.at(j)-i);
					printColored(pattern, PURPLE);
					i+=matchTable.at(j)-i+pattern.length()-1;
					j++;
				} else if (!matchTable.empty()  &&  j>=matchTable.size()) {
					cout<<text.substr(i, text.length()-i);
					i+=text.length()-i;
				} else {
					cout<<text;
					i+=text.length();
				}
			}
			cout<<endl<<endl;
			cout<<"Ilość wystąpień wzorca: "<<matchTable.size()<<endl;
		}

		void drawIncrementally(const string &text, const string &pattern)
		{
			vector<size_t> matchTable = kmp::search(text, pattern);
			size_t j = 0;
			cout<<"\n# Wyniki wyszukiwania:"<<endl;
			for (size_t i = 0; i<text.length(); i++) {
				if (!matchTable.empty()  &&  j<matchTable.size()) {
					cout<<text.substr(i, matchTable.at(j)-i);
					printColored(pattern, PURPLE);
					i+=matchTable.at(j)-i+pattern.length()-1;
					j++;
					getch();
				} else if (!matchTable.empty()  &&  j>=matchTable.size()) {
					cout<<text.substr(i, text.length()-i);
					i+=text.length()-i;
				} else {
					cout<<text;
					i+=text.length();
				}
			}
			cout<<endl<<endl;
			cout<<"Ilość wystąpień wzorca: "<<matchTable.size()<<endl;
		}
	}
}

namespace io { /* Filesystem interface */
	string getStrFromFile(const string &path)
	{
		string str;
		ifstream file;
		do {
			file.open(path, ios::in);
			if (!file) {
				 str = "";
				 return str;
			}
		} while (!file);
		while (!file.eof() && file.peek()!='\377') { // Last byte in a file is 0xFF, we do not want to read it 
			str.push_back(file.get());
		}
		if (str.back() == '\n') str.pop_back(); // Removing the last newline should not effect the search, norrecting for multiple newlines which could be present in a file
		file.close();
		return str;
	}

	void initConsole(void) 
	{
		if (IsValidCodePage(CP_UTF8)) {
			if (!SetConsoleCP(CP_UTF8)) exit(1);
			if (!SetConsoleOutputCP(CP_UTF8)) exit(1);
		} else cout<<"Konsola nie posiada strony kodowania UTF-8, wsparcie dla wyświetlania polskich znaków jest nieaktywne."<<endl;
	}
}
