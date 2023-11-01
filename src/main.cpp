#include <iostream>
#include <string>
#include <vector>

#include "exit_input.h"

namespace
{
	bool is_exit = false;
	std::string input;
	std::vector<IO::InputInterfacePtr> Actions;
}

int main(int arvc, char* argv[])
{
	Actions.push_back(IO::makeExitInput([&](){ is_exit = true; }));

	while(!is_exit)
	{
		int i = 0;
		for(const auto& action : Actions)
		{
			std::cout << "Press : " << i << " for " << action->name() << std::endl;
		}
		std::cin >> i;
		if(i >= Actions.size() || i < 0)
		{
			std::cout << "Incorrect number. Please enter correct number. " << std::endl;
			continue;
		}
		Actions[i]->makeAction();
	}
	return EXIT_SUCCESS;
}
