#ifndef IO_READ_POSIX_FILE_CONNECTION_H
#define IO_READ_POSIX_FILE_CONNECTION_H

#include <cstddef>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

namespace io {
namespace detail {

class FileConnection
{
public:

  typedef int FileDescriptor;

  FileConnection(const char* path, int flags = O_RDONLY)
  {
    fd_ = ::open(path, flags);
  }

  ~FileConnection()
  {
    if (open())
      ::close(fd_);
  }

  bool open()
  {
    return fd_ != -1;
  }

  bool size(int* pSize)
  {
    struct stat info;
    if (::fstat(fd_, &info) == -1)
      return false;

    *pSize = info.st_size;
    return true;
  }

  operator FileDescriptor() const
  {
    return fd_;
  }

private:
  FileDescriptor fd_;
};


} // namespace detail
} // namespace io

#endif /* IO_READ_POSIX_FILE_CONNECTION_H */
