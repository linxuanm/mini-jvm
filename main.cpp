#include <cxxopts.hpp>

#include "core/trace.h"

#include <iostream>

struct ArgParseError {
  bool ok = true;
  std::string arg_name;
  std::string msg;
};

void process_trace_options(const cxxopts::ParseResult &res) {
#ifndef ENABLE_TRACE
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
#endif
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

  process_trace_options(result);

  return 0;
}