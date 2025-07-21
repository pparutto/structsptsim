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

DirectFileLogger::DirectFileLogger(std::ofstream& ofs)
  : ofs_(ofs)
{
}

void
DirectFileLogger::write(const std::string& str)
{
  this->ofs_ << str << std::endl;
  this->ofs_.flush();
}

CollLogger::CollLogger()
  : log_()
{
}

CollLogger::~CollLogger()
{
}

void
CollLogger::write(const std::string& str)
{
  this->log_.push_back(str);
}
