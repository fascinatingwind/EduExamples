#include "input_interface.h"

#include <iostream>
#include <utility>

namespace IO
{
	template<typename Func>
	class ExitInput : public InputInterface
	{
		Func Action;
		public:
			template<typename F>
			ExitInput(F func) : Action(std::forward<F>(func)) {}
			~ExitInput() = default;

			std::string name() const override { return "exit"; };

			void makeAction() const override
			{
				std::string answer;
				std::cout << "Exit from application [Y/N]" << std::endl;
				std::cin >> answer;
				if(answer == "Y")
					Action();
			}
	};

	template<typename Func>
	std::shared_ptr<ExitInput<Func>> makeExitInput(Func func)
	{
		return std::make_shared<ExitInput<Func>>(func);
	}
}
