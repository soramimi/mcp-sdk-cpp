
#include "mcpserver.h"
#include "strformat.h"
#include "toi.h"

class Tool_add : public McpServer::AbstractMcpTool {
public:
	Tool_add()
		: AbstractMcpTool("add", "Add two numbers")
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

class Tool_mul : public McpServer::AbstractMcpTool {
public:
	Tool_mul()
		: AbstractMcpTool("mul", "multiply two numbers")
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
	mcpsrv.install_tool(std::make_shared<Tool_add>());
	mcpsrv.install_tool(std::make_shared<Tool_mul>());
	mcpsrv.run();
	return 0;
}
