#ifndef LOGGER_HH_
#define LOGGER_HH_

# include <vector>
# include <string>

class Logger
{
public:
  virtual ~Logger() = default;

  virtual void write(const std::string& str) = 0;
};

class VoidLogger: public Logger
{
public:
  virtual ~VoidLogger() = default;

  virtual void write(const std::string& str);
};

class BufferLogger: public Logger
{
public:
  BufferLogger() = default;
  virtual ~BufferLogger() = default;

  virtual void write(const std::string& str);

  void save_to_file(const std::string& path) const;

  const std::vector<std::string>& get() const { return this->elts_; }
protected:
  std::vector<std::string> elts_;
};

class DirectFileLogger: public Logger
{
public:
  DirectFileLogger(std::ofstream& ofs);
  virtual ~DirectFileLogger() = default;

  virtual void write(const std::string& str);
protected:
  std::ofstream& ofs_;
};

#endif /// !LOGGER_HH
