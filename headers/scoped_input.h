#include "input_interface.h"

#include <iostream>
#include <utility>

namespace IO
{
	template<typename Func>
	class ScopedInput : public InputInterface
	{
		Func Action;
		std::string name_;
	public:
		template<typename F>
		ScopedInput(std::string_view name, F&& func)
			: Action(std::forward<F>(func)), name_({ name.data(), name.size() })
		{}

		std::string name() const override { return name_; };

		void makeAction() const override
		{
			Action();
		}
	};

	template<typename Func>
	std::shared_ptr<ScopedInput<Func>> makeScopedInput(std::string_view name, Func&& func)
	{
		return std::make_shared<ScopedInput<Func>>(name, func);
	}

}
