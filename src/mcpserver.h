#ifndef MCPSERVER_H
#define MCPSERVER_H

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <optional>

class McpServer {
public:
	struct ToolSchema {
		std::string name;
		std::string description;
		struct Property {
			std::string name;
			std::string title;
			std::string type;
			Property() = default;
			Property(std::string name, std::string title, std::string type)
				: name(name), title(title), type(type)
			{}
		};
		std::vector<Property> properties;
		std::string title;
		std::string type;
	};

	class AbstractMcpTool {
		friend class McpServer;
	private:
		McpServer::ToolSchema schema_;
	protected:
		ToolSchema &schema() { return schema_; }
		void add_property(std::string name, std::string title, std::string type)
		{
			schema().properties.emplace_back(name, title, type);
		}
	public:
		AbstractMcpTool(std::string const &name, std::string const &description)
		{
			schema_.name = name;
			schema_.description = description;
			schema_.title = name + "Arguments";
			schema_.type = "object";
		}
		virtual std::optional<std::string> call(std::vector<std::string> const &args) = 0;
	};
private:
	struct Message {
		std::string method;
		struct Params {
			std::string protocolVersion;
			struct Capabilities {
				struct Roots {
					bool listChanged = false;
				} roots;
				struct Prompts {
					bool listChanged = false;
				} prompts;
				struct Resources {
					bool subscribe = false;
					bool listChanged = false;
				} resources;
				struct Tools {
					bool listChanged = false;
				} tools;
			} capabilities;
			struct ClientInfo {
				std::string name;
				std::string version;
			} clientInfo;
			std::string name;
			struct Argument {
				std::string name;
				std::string value;
			};
			std::vector<Argument> arguments;
		} params;
		struct Result {
			std::string protocolVersion;
			struct Capabilities {
				struct Experimental {
				} experimental;
				struct Prompts {
					bool listChanged = false;
				} prompts;
				struct Resources {
					bool subscribe = false;
					bool listChanged = false;
				} resources;
				struct Tools {
					bool listChanged = false;
				} tools;
			} capabilities;
			struct ServerInfo {
				std::string name;
				std::string version;
			} serverInfo;
			struct Tool {
				std::string name;
				std::string description;
				struct InputSchema {
					struct Property {
						std::string name;
						std::string title;
						std::string type;
					};
					std::vector<Property> properties;
					std::vector<std::string> required;
					std::string title;
					std::string type;
				} inputSchema;
			};
			std::vector<Tool> tools;
		} result;
		std::string jsonrpc;
		std::string id;
	};

public:
	void test();

	std::map<std::string, std::shared_ptr<AbstractMcpTool>> methods_;

	void install_tool(std::shared_ptr<AbstractMcpTool> const &tool);

	void run();
private:
	static Message parse_message(const std::string &line);
};

#endif // MCPSERVER_H
