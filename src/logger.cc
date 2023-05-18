#include "logger.hh"

#include <iostream>
#include <fstream>

void
VoidLogger::write(const std::string& str)
{
  (void) str;
}

void
BufferLogger::write(const std::string& str)
{
  this->elts_.push_back(str);
}

void
BufferLogger::save_to_file(const std::string& fpath) const
{
  std::ofstream f;
  f.open(fpath);

  if (!f.is_open())
  {
    std::cerr << "ERROR: Could not open output file: " << fpath << std::endl;
    return;
  }

  for (const std::string& elt: this->elts_)
    f << elt << std::endl;

  f.close();
}
