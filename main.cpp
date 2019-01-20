#include "kmp.h"
#include <ctime>
using namespace std;

int main(void)
{
	string text, pattern;
	int mode;
	io::initConsole();
	srand(time(NULL));
	ui::printUsageInfo();
	text = ui::getText();
	pattern = ui::getPattern();
	mode = ui::getMode();
	switch (mode) {
		case CONSTANT:
			ui::answer::draw(text, pattern);
			break;
		case INCREMENTAL:
			ui::answer::drawIncrementally(text, pattern);
			break;
	}
	return 0;
}
