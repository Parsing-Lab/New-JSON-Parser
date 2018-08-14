
#include <ctime>
#include <iostream>
#include <string>


#include "njp.h"


int main(int argc, char* argv[])
{
	std::string fileName;

	{
		int lex_count = 8;
		int parse_count = 8;

		if (argc == 1) {
			std::cout << "FileName: ";
			std::getline(std::cin, fileName);
		}
		else
		{
			fileName = std::string(argv[1]);

			if (argc >= 3) {
				lex_count = atoi(argv[2]);
				parse_count = lex_count;
			}
			if (argc >= 4) {
				parse_count = atoi(argv[3]);
			}
		}

		

	//try {
		std::chrono::steady_clock::time_point a, b;

		
		for (int i = 1; i < 9; ++i) {
			//global.Remove();
			
			wiz::UserType global;
			a = std::chrono::steady_clock::now();
			{
				lex_count = i;
				parse_count = i;

				//FastJson
				wiz::LoadDataFromFileFastJson(fileName, global, lex_count, parse_count); // parallel? lexing + parallel parsing
			}
			b = std::chrono::steady_clock::now();
		

			std::cout << "time " << (double)std::chrono::steady_clock::duration(b-a).count()
			* std::chrono::steady_clock::period().num / std::chrono::steady_clock::period().den << "s" << std::endl;
		}
		std::cout << "fileName is " << fileName << std::endl;

		//wiz::SaveWizDB(global, "test3.txt", "1");
	//}
	//catch (...) {
	//	std::cout << "error occurs." << std::endl;
	//}
	}

	return 0;
}

