#include <iostream>
#include <string>
#include <locale>
#include "simpubli_directories.h"
#include "main_resource.h"
#include "municipios.h"

int main(int argc, char* argv[]) {

	const int caesar_key = 7;

	std::locale locale_pt_BR("pt_BR.UTF8");
	std::locale::global(locale_pt_BR);

	executePathConfiguration();
	executeDBConfiguration();

	int arg = 0;
	int option = -1;
	bool cont = true;

	if (argc > 1) {
		option = inputVerify(argv[1]);
		arg = 1;
	}

	while (cont) {

		int get_result = readOption(option, cont, arg);

		if (get_result == 0) {
			cont = true;
		}
		else {
			cont = false;
		}
	}

	return 0;
}