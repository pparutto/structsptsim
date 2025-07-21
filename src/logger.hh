#ifndef LOGGER_HH_
#define LOGGER_HH_

# include <vector>
# include <string>

class Logger
{
public:
  virtual ~Logger() = default;

  virtual void write(const std::string& str) = 0;
  virtual bool logncoll() = 0;
};

class VoidLogger: public Logger
{
public:
  virtual ~VoidLogger() = default;

  virtual void write(const std::string& str);
  virtual bool logncoll() override { return false; };
};

class BufferLogger: public Logger
{
public:
  BufferLogger() = default;
  virtual ~BufferLogger() = default;

  virtual void write(const std::string& str);

  void save_to_file(const std::string& path) const;
  virtual bool logncoll() override { return false; };

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
  virtual bool logncoll() override { return false; };
protected:
  std::ofstream& ofs_;
};

class CollLogger: public Logger
{
public:
  CollLogger();
  virtual ~CollLogger();

  virtual void write(const std::string& str);
  virtual bool logncoll() override { return true; };

  const std::vector<std::string>& ncoll_log() const { return this->log_; };
protected:
  std::vector<std::string> log_;
};


#endif /// !LOGGER_HH
