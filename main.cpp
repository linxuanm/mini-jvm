#include <cxxopts.hpp>

#include "core/trace.h"

#include <iostream>

int main(int argc, char *argv[]) {
  cxxopts::Options options("Mini JVM",
                           "JVM implementation for research purposes");
  options.add_options()
      ("t,trace", "Tracing options", cxxopts::value<std::string>());
  const auto result = options.parse(argc, argv);

#ifndef ENABLE_TRACE
  Trace::TraceOptions opts;
  for (const auto i : result["trace"].as<std::string>()) {
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
  Trace::configure(opts);
#endif

  return 0;
}