#include <cxxopts.hpp>

#include "core/trace.h"
#include "vm/loader/ClassLoader.h"

#include <iostream>

void process_trace_options(const cxxopts::ParseResult &res) {
#ifdef ENABLE_TRACE
  Trace::TraceOptions opts;
  for (const auto i : res["trace"].as<std::string>()) {
    switch (i) {
    case 'r':
      opts.runtime = true;
      break;
    case 'b':
      opts.binparse = true;
      break;
    case 'v':
      opts.validation = true;
      break;
    case 'i':
      opts.instantiation = true;
      break;
    case 'e':
      opts.execution = true;
      break;
    case 'j':
      opts.jit = true;
      break;
    default:
      std::cerr << "Unknown tracing option: '" << i << '\'' << std::endl;
      exit(1);
    }
  }
  configure(opts);
#else
  if (res.count("trace") != 0) {
    std::cerr << "Invalid argument --trace: VM is not compiled with tracing on";
    std::cerr << std::endl;
    exit(1);
  }
#endif
}

std::string process_path_option(const cxxopts::ParseResult &res) {
  if (res.count("class") == 0) {
    std::cerr << "Invalid argument --class: no class file specified";
    std::cerr << std::endl;
    exit(1);
  }
  return res["class"].as<std::string>();
}

int main(int argc, char *argv[]) {
  cxxopts::Options options("Mini JVM",
                           "JVM implementation for research purposes");
  options.add_options()
      ("class", "Class file to load", cxxopts::value<std::string>())
      ("t,trace", "Tracing options (string consisting of {r, b, v, i, e, j})",
       cxxopts::value<std::string>()->default_value(""));

  options.parse_positional({"class"});
  const auto result = options.parse(argc, argv);

  const auto path = process_path_option(result);
  process_trace_options(result);

  // TODO: proper structure
  ClassLoader loader;
  auto [kind, parse_error] = loader.load_class_file(path);
  switch (kind) {
  case CL_FileNotFound:
    std::cerr << "No such file or directory: " << path << std::endl;
    break;
  case CL_FileFormatError:
    std::cerr << "Format error: " << parse_error.err_msg << std::endl;
    break;
  case CL_Ok:
    std::cerr << "OK" << std::endl;
    break;
  }

  // TRACE_DO(B, {
  //   Trace::top_bar();
  //   Trace::center_text("Hello");
  //   Trace::mid_bar();
  //   Trace::center_text("");
  //   Trace::mid_bar_text("Section title");
  //   Trace::center_text("");
  //   Trace::col_2("Test", "More Test", Trace::ALIGN_CENTER, Trace::ALIGN_CENTER);
  //   Trace::center_text("");
  //   Trace::bottom_bar();
  // });

  return 0;
}