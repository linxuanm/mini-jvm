#pragma once

#include "vm/parse/CodePtr.h"

class ByteParser : public CodePtr {

public:
  void trace_bytes(const std::string &tag, pc_t pos, u32 n) const;

private:

};