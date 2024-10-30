#include "vm/parse/ByteParser.h"

void ByteParser::trace_bytes(const std::string &tag, u32 n) const {
  trace_bytes(tag, get_pc(), n);
}

void ByteParser::trace_bytes(const std::string &tag, pc_t pos, u32 n) const {
  if (pos >= buf->size())
    return;
  Trace::ss << Color::BRIGHTCYAN << format_pc(pos) << ": " << Color::DEFAULT;
  const auto limit = std::min(static_cast<u64>(pos + n), buf->size());
  for (auto i = pos; i < limit; i++)
    Trace::ss << fmt::format("{:02X} ", (*buf)[i]);

  Trace::col_2(Trace::ss.str(), tag);
  Trace::ss.clear();
  Trace::ss.str("");
}