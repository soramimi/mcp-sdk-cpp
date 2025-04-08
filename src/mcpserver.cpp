#include "mcpserver.h"
#include <iostream>
#include "jstream.h"

static inline std::string getline()
{
	std::string line;
	std::getline(std::cin, line);
	return line;
}

void McpServer::test()
{
	if (1) {
		std::string line = R"---({"method": "initialize","params": {"protocolVersion": "2024-11-05","capabilities": {"roots": {"listChanged": true}},"clientInfo": {"name": "mcp","version": "0.1.0"}},"jsonrpc": "2.0","id": 0}})---";
		Message request = parse_message(line);
		return;
	}
	if (0) {
		std::string line = R"---({"jsonrpc": "2.0","id": 0,"result": {"protocolVersion": "2024-11-05","capabilities": {"experimental": {},"prompts": {"listChanged": false},"resources": {"subscribe": false,"listChanged": false},"tools": {"listChanged": false}},"serverInfo": {"name": "Demo","version": "1.6.0"}}})---";
		Message request = parse_message(line);
		return;
	}
	if (0) {
		std::string line = R"---({"method": "notifications/initialized","jsonrpc": "2.0","params": null})---";
		Message request = parse_message(line);
		return;
	}
	if (0) {
		std::string line = R"---({"method": "tools/list","jsonrpc": "2.0","id": 1})---";
		Message request = parse_message(line);
		return;
	}
	if (0) {
		std::string line = R"---({"jsonrpc": "2.0","id": 1,"result": {"tools": [{"name": "add","description": "Add two numbers","inputSchema": {"properties": {"a": {"title": "A","type": "integer"},"b": {"title": "B","type": "integer"}},"required": ["a","b"],"title": "addArguments","type": "object"}},{"name": "light_switch","description": "Turn on or turn off the light","inputSchema": {"properties": {"on": {"title": "On","type": "boolean"}},"required": ["on"],"title": "light_switchArguments","type": "object"}}]}})---";
		Message request = parse_message(line);
		return;
	}
	if (0) {
		std::string line = R"---({"method":"tools/call","params":{"name":"add","arguments":{"a":3,"b":4}},"jsonrpc":"2.0","id":25})---";
		Message request = parse_message(line);
		return;
	}
}

void McpServer::install_tool(const std::shared_ptr<AbstractMcpTool> &tool)
{
	std::string name = tool->schema().name;
	methods_[name] = tool;
}

void McpServer::run()
{
	while (1) {
		std::string line = getline();

		Message request = parse_message(line);
		if (request.method == "initialize") {
			std::vector<char> vec;
			jstream::Writer writer([](char const *p, int n){
				fwrite(p, 1, n, stdout);
			});
			writer.enable_indent(false);
			writer.enable_newline(false);
			writer.object({}, [&](){
				writer.string("jsonrpc", "2.0");
				writer.number("id", atoi(request.id.c_str()));
				writer.object("result", [&](){
					writer.string("protocolVersion", "2024-11-05");
					writer.object("capabilities", [&](){
						writer.object("experimental", [&](){});
						writer.object("prompts", [&](){
							writer.boolean("listChanged", false);
						});
						writer.object("resources", [&](){
							writer.boolean("subscribe", false);
							writer.boolean("listChanged", false);
						});
						writer.object("tools", [&](){
							writer.boolean("listChanged", false);
						});
					});
					writer.object("serverInfo", [&](){
						writer.string("name", "Demo");
						writer.string("version", "1.6.0");
					});
				});
			});
			std::string_view view(vec.data(), vec.size());
			puts(std::string(view).c_str());
			continue;
		}
		if (request.method == "notifications/initialized") {
			continue;
		}
		if (request.method == "tools/list") {
			std::vector<char> vec;
			jstream::Writer writer([&](char const *p, int n){
				vec.insert(vec.end(), p, p + n);
			});
			writer.enable_indent(false);
			writer.enable_newline(false);
			writer.object({}, [&](){
				writer.string("jsonrpc", "2.0");
				writer.number("id", atoi(request.id.c_str()));
				writer.object("result", [&](){
					writer.array("tools", [&](){
						for (auto const &method : methods_) {
							writer.object({}, [&](){
								std::shared_ptr<AbstractMcpTool> const &tool = method.second;
								ToolSchema const &schema = tool->schema();
								writer.string("name", schema.name);
								writer.string("description", schema.description);
								writer.object("inputSchema", [&](){
									writer.object("properties", [&](){
										for (auto const &p : schema.properties) {
											writer.object(p.name, [&](){
												writer.string("title", p.title);
												writer.string("type", p.type);
											});
										}
									});
									writer.array("required", [&](){
										for (auto const &p : schema.properties) {
											writer.string(p.name);
										}
									});
									writer.string("title", schema.title);
									writer.string("type", schema.type);
								});
							});
						}
					});
				});
			});
			std::string_view view(vec.data(), vec.size());
			puts(std::string(view).c_str());
			continue;
		}
		if (request.method == "resources/list") {
			std::vector<char> vec;
			jstream::Writer writer([&](char const *p, int n){
				vec.insert(vec.end(), p, p + n);
			});
			writer.enable_indent(false);
			writer.enable_newline(false);
			writer.object({}, [&](){
				writer.string("jsonrpc", "2.0");
				writer.number("id", atoi(request.id.c_str()));
				writer.object("result", [&](){
				});
			});
			std::string_view view(vec.data(), vec.size());
			puts(std::string(view).c_str());
			continue;
		}
		if (request.method == "prompts/list") {
			std::vector<char> vec;
			jstream::Writer writer([&](char const *p, int n){
				vec.insert(vec.end(), p, p + n);
			});
			writer.enable_indent(false);
			writer.enable_newline(false);
			writer.object({}, [&](){
				writer.string("jsonrpc", "2.0");
				writer.number("id", atoi(request.id.c_str()));
				writer.object("result", [&](){
				});
			});
			std::string_view view(vec.data(), vec.size());
			puts(std::string(view).c_str());
			continue;
		}
		if (request.method == "tools/call") {
			// call function
			bool isError = true;
			std::string content_text;
			auto method_pair = methods_.find(request.params.name);
			if (method_pair != methods_.end()) {
				std::shared_ptr<AbstractMcpTool> const &tool = method_pair->second;
				size_t argc = tool->schema().properties.size();
				std::vector<std::string> args(argc);
				for (size_t i = 0; i < argc; i++) {
					args[i] = request.params.arguments[i].value;
				}
				auto ret = tool->call(args);
				if (ret) {
					content_text = *ret;
					isError = false;
				} else {
					isError = true;
				}
			} else {
				isError = true;
			}
			// return result
			std::vector<char> vec;
			jstream::Writer writer([&](char const *p, int n){
				vec.insert(vec.end(), p, p + n);
			});
			writer.enable_indent(false);
			writer.enable_newline(false);
			writer.object({}, [&](){
				writer.string("jsonrpc", "2.0");
				writer.number("id", atoi(request.id.c_str()));
				writer.object("result", [&](){
					if (!isError) {
						writer.array("content", [&](){
							writer.object({}, [&](){
								writer.string("type", "text");
								writer.string("text", content_text);
							});
						});
					}
					writer.boolean("isError", isError);
				});
			});
			std::string_view view(vec.data(), vec.size());
			puts(std::string(view).c_str());
			continue;
		}
	}
}

McpServer::Message McpServer::parse_message(const std::string &line)
{
	char const *begin = line.c_str();
	char const *end = begin + line.size();
	Message request;
	Message::Result::Tool tool;
	Message::Result::Tool::InputSchema::Property property;

	jstream::Reader reader(begin, end);
	while (reader.next()) {
		if (reader.match("{method")) {
			request.method = reader.string();
		} else if (reader.match("{params{protocolVersion")) {
			request.params.protocolVersion = reader.string();
		} else if (reader.match("{params{**")) {
			if (reader.match("{params{capabilities{roots{listChanged")) {
				request.params.capabilities.roots.listChanged = reader.istrue();
			} else if (reader.match("{params{clientInfo{name")) {
				request.params.clientInfo.name = reader.string();
			} else if (reader.match("{params{clientInfo{version")) {
				request.params.clientInfo.version = reader.string();
			} else if (reader.match("{params{name")) {
				request.params.name = reader.string();
			} else if (reader.match("{params{arguments{*")) {
				if (reader.isvalue()) {
					Message::Params::Argument a;
					a.name = reader.key();
					a.value = reader.string();
					request.params.arguments.push_back(a);
				}
			}
		} else if (reader.match("{result{**")) {
			if (reader.match("{result{protocolVersion")) {
				request.result.protocolVersion = reader.string();
			} else if (reader.match("{result{capabilities{prompts{listChanged")) {
				request.result.capabilities.prompts.listChanged = reader.istrue();
			} else if (reader.match("{result{capabilities{resources{subscribe")) {
				request.result.capabilities.resources.subscribe = reader.istrue();
			} else if (reader.match("{result{capabilities{resources{listChanged")) {
				request.result.capabilities.resources.listChanged = reader.istrue();
			} else if (reader.match("{result{capabilities{tools{listChanged")) {
				request.result.capabilities.tools.listChanged = reader.istrue();
			} else if (reader.match("{result{serverInfo{name")) {
				request.result.serverInfo.name = reader.string();
			} else if (reader.match("{result{serverInfo{version")) {
				request.result.serverInfo.version = reader.string();
			} else if (reader.match("{result{tools[{name")) {
				tool.name = reader.string();
			} else if (reader.match("{result{tools[{description")) {
				tool.description = reader.string();
			} else if (reader.match("{result{tools[{inputSchema{properties{*{title")) {
				property.title = reader.string();
			} else if (reader.match("{result{tools[{inputSchema{properties{*{type")) {
				property.type = reader.string();
			} else if (reader.match("{result{tools[{inputSchema{properties{*")) {
				if (reader.is_start_object()) {
					property.name = reader.key();
				} else if (reader.is_end_object()) {
					tool.inputSchema.properties.push_back(property);
					property = {};
				}
			} else if (reader.match("{result{tools[{inputSchema{required[*")) {
				tool.inputSchema.required.push_back(reader.string());
			} else if (reader.match("{result{tools[{inputSchema{title")) {
				tool.inputSchema.title = reader.string();
			} else if (reader.match("{result{tools[{inputSchema{type")) {
				tool.inputSchema.type = reader.string();
			}
		} else if (reader.match("{jsonrpc")) {
			request.jsonrpc = reader.string();
		} else if (reader.match("{id")) {
			request.id = reader.string();
		}
		if (reader.is_start_object()) {
			if (reader.path() == "{result{tools[{") {
				tool = {};
			} else if (reader.path() == "{result{tools[{inputSchema{properties{") {
				property = {};
			}
		} else if (reader.is_end_object()) {
			if (reader.path() == "{result{tools[") {
				request.result.tools.push_back(tool);
				tool = {};
			}
		}
	}
	return request;
}
