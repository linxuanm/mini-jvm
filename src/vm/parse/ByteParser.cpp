#include "vm/parse/ByteParser.h"

void ByteParser::trace_bytes(const std::string &tag, pc_t pos, u32 n) const {
  Trace::ss << Color::BRIGHTCYAN << format_pc(pos) << ": " << Color::DEFAULT;
  for (auto i = pos; i < pos + n; i++)
    Trace::ss << fmt::format("{:02X} ", (*buf)[i]);

  Trace::col_2(Trace::ss.str(), tag);
  Trace::ss.clear();
  Trace::ss.str("");
}