#ifndef IO_READ_POSIX_MEMORY_MAPPED_CONNECTION_H
#define IO_READ_POSIX_MEMORY_MAPPED_CONNECTION_H

#include <cstdlib>
#include <fcntl.h>
#include <sys/mman.h>

namespace io {
namespace detail {

class MemoryMappedConnection
{
public:

  MemoryMappedConnection(int fd, int size)
    : size_(size)
  {
#ifdef MAP_POPULATE
    map_ = (char*) ::mmap(0, size, PROT_READ, MAP_SHARED | MAP_POPULATE, fd, 0);
#else
    map_ = (char*) ::mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
#endif

#if defined(POSIX_MADV_SEQUENTIAL) && defined(POSIX_MADV_WILLNEED)
    ::posix_madvise((void*) map_, size, POSIX_MADV_SEQUENTIAL | POSIX_MADV_WILLNEED);
#endif
  }

  ~MemoryMappedConnection()
  {
    if (map_ != MAP_FAILED)
      ::munmap(map_, size_);
  }

  bool open()
  {
    return map_ != MAP_FAILED;
  }

  operator char*() const
  {
    return map_;
  }

private:
  char* map_;
  int size_;
};

} // namespace detail
} // namespace io

#endif /* IO_READ_POSIX_MEMORY_MAPPED_CONNECTION_H */
