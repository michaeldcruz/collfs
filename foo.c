#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <mpi.h>
#include "errmacros.h"
#include "foo.h"
#include "collfs.h"

extern int MPI_Comm_rank ( MPI_Comm comm, int *rank ) __attribute__ ((weak));

int foo(const char *path)
{
  int fd,err,rank;
  unsigned int sum,value;

  if (MPI_Comm_rank) { 
    err = MPI_Comm_rank(MPI_COMM_WORLD,&rank);CHK(err); 
  } 
  else rank = 0;
    
  fd = __collfs_open(path,O_RDONLY);
  if (fd < 0) ERR("[%d] open(\"%s\",O_RDONLY) failed",rank,path);
  sum = 0;
  while (__collfs_read(fd,&value,sizeof value) == sizeof value) {
    sum ^= value;
  }
  __collfs_close(fd);
  printf("[%d] XOR-sum of \"%s\": %x\n",rank,path,sum);
  return 0;
}
