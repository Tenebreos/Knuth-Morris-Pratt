#ifndef _kmp_h_
#define _kmp_h_
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

enum searchMode {
	INCREMENTAL = 1,
	CONSTANT = 2,
	EXIT_MODE = 3
};

enum colour {
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	AQUA = 3,
	RED = 4,
	PURPLE = 5,
	YELLOW = 6,
	WHITE = 7,
	GRAY = 8, 	
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_AQUA = 11,
	LIGHT_RED = 12,
	LIGHT_PURPLE = 13,
	LIGHT_YELLOW = 14,
	BRIGHT_WHITE = 15
};

namespace util { /* Various uncategorised functions */
	std::string generateRandomText(const size_t length);
}
namespace kmp { /* Search related functions */
	std::vector<long> preprocess(const std::string &pattern);
	std::vector<size_t> search(const std::string &text, const std::string &pattern);
}
namespace ui { /* User interface */
	long wczytaj(const long d_gran, const long g_gran, const std::string message);
	void printUsageInfo(void);
	void printColored(const std::string str, const int color);
	std::string getText(void);
	std::string getPattern(void);
	int getMode(void);
	namespace answer {
		void draw(const std::string &text, const std::string &pattern);
		void drawIncrementally(const std::string &text, const std::string &pattern);
	}
}
namespace io { /* Filesystem interface */
	std::string getStrFromFile(const std::string &path);
	void initConsole(void);
}
#endif
