#include <iostream>
#include <filesystem>
#include <pybind11/embed.h>
namespace py = pybind11;
namespace fs = std::filesystem;

const fs::path MODULES = "modules";
const fs::path BUILTIN = MODULES / "builtins";
const fs::path ZIPPED = BUILTIN / "modules.pkg";

const char* MAIN_MODULE = "main";
const char* MAIN_FUNC = "entry";
const char* EMBEDDED_MODULE_NAME = "pyapp";

int main(int argc, char* argv[])
{
	auto workingDir = fs::current_path();
	Py_SetPath((workingDir / ZIPPED).wstring().c_str());

	py::scoped_interpreter interpreter;

	std::vector<std::string> vecArg(argv, argv + argc);
	auto sys = py::module::import("sys");
	sys.attr("argv") = vecArg;
	sys.attr("dont_write_bytecode") = true;

	auto insertPath = sys.attr("path").attr("insert");
	insertPath(1, (workingDir / BUILTIN).wstring());
	insertPath(2, (workingDir / MODULES).wstring());

	try {
		auto pyapp = py::module::import(EMBEDDED_MODULE_NAME);
		sys.attr("meta_path").attr("append")(pyapp.attr("CustomImporter")());
		//py::module::import("other");

		//py::exec(startup);
		py::module::import(MAIN_MODULE).attr(MAIN_FUNC)();
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}