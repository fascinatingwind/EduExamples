#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "scoped_input.h"

namespace
{
	bool is_exit = false;
	std::vector<IO::InputInterfacePtr> Actions;

	void CloseProgramm()
	{
		is_exit = true;
	}

	void ClearScreen()
	{
		system("cls");
	}
}

int main(int arvc, char* argv[])
{
	Actions.push_back(IO::makeScopedInput("exit", [&]() { is_exit = true; }));
	Actions.push_back(IO::makeScopedInput("clear screen", ClearScreen));

	while(!is_exit)
	{
		int i = 0;
		for(const auto& action : Actions)
		{
			std::cout << "Press : " << i << " for " << action->name() << std::endl;
			i++;
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
