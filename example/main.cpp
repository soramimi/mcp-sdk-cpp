
#include "mcpserver.h"
#include "strformat.h"
#include "toi.h"

class Tool_add : public McpServer::AbstractTool {
public:
	Tool_add()
		: AbstractTool("add", "Add two numbers")
	{
		add_property("a", "A", "integer");
		add_property("b", "B", "integer");
	}
	std::optional<std::string> call(std::vector<std::string> const &args) override
	{
		if (args.size() != 2) return std::nullopt;
		int a = toi<int>(args[0]);
		int b = toi<int>(args[1]);
		int ans = a + b;
		return strformat("%d")(ans);
	}
};

class Tool_mul : public McpServer::AbstractTool {
public:
	Tool_mul()
		: AbstractTool("mul", "multiply two numbers")
	{
		add_property("a", "A", "integer");
		add_property("b", "B", "integer");
	}
	std::optional<std::string> call(std::vector<std::string> const &args) override
	{
		if (args.size() != 2) return std::nullopt;
		int a = toi<int>(args[0]);
		int b = toi<int>(args[1]);
		int ans = a * b;
		return strformat("%d")(ans);
	}
};

int main()
{
	McpServer mcpsrv;
	mcpsrv.emplace_tool<Tool_add>();
	mcpsrv.emplace_tool<Tool_mul>();
	mcpsrv.run();
	return 0;
}
