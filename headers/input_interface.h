#include <string>
#include <memory>

namespace IO
{
	class InputInterface
	{
		public:
			virtual std::string name() const = 0;
			virtual void makeAction() const = 0;
	};

	using InputInterfacePtr = std::shared_ptr<InputInterface>;
}
