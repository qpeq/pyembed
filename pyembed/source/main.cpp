#include <string>
#include <iostream>
#include <filesystem>
#include <pybind11/embed.h>
namespace py = pybind11;
namespace fs = std::filesystem;

const char* MAIN_MODULE = "startup";
const char* MAIN_FUNC = "main";
const char* EMBEDDED_MODULE_NAME = "pyapp";

#if defined (_MSC_VER)
	const std::wstring DELIMITER = L";";
#else
	const std::wstring DELIMITER = L":";
#endif

void set_py_sys_path()
{
	auto working_dir = fs::current_path();
	std::vector<fs::path> sys_path {
		working_dir / "builtins" / "packages.pkg",
		working_dir / "builtins",
		working_dir / "modules"
	};

	std::wstring module_search_path;
	for (const auto &path: sys_path) {
		if (!module_search_path.empty()) {
			module_search_path.append(DELIMITER);
			module_search_path.append(path.wstring());
		}
		else {
			module_search_path = path.wstring();
		}
	}

	Py_SetPath(module_search_path.c_str());
}

int main(int argc, char* argv[])
{
	set_py_sys_path();

	py::scoped_interpreter interpreter;

	std::vector<std::string> args(argv, argv + argc);
	auto sys = py::module::import("sys");
	sys.attr("argv") = args;
	sys.attr("dont_write_bytecode") = true;

	try {
		// auto pyapp = py::module::import(EMBEDDED_MODULE_NAME);
		// sys.attr("meta_path").attr("append")(pyapp.attr("CustomImporter")());
		//py::module::import("other");

		//py::exec(startup);
		py::module::import(MAIN_MODULE).attr(MAIN_FUNC)();
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}