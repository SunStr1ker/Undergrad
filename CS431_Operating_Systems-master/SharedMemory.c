
#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main()
{
  int segment_id;    //ID to Shared Memory Segment
  char* shared_memory;    //Starting Address of Shared Memory  Recall char = 1 byte
  struct shmid_ds shmbuffer;
  int segment_size;
  const int shared_segment_size= 0x6400;    //bytes allocate rounded up to integer multip of page size

  //ALLOCATE SHARED MEMORY SEGMENT
  segment_id = shmget(IPC_PRIVATE, shared_segment_size, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);


  //ATTACH SHARED MEMORY SEGMENT
  shared_memory = (char*) shmat (segment_id, 0, 0);
  printf( " shared memory attached at address %p\n", shared_memory);


  //DETERMINE SHARED MEMORY SEGMENT SIZE
  shmctl (segment_id, IPC_STAT, &shmbuffer);
  segment_size = shmbuffer.shm_segsz;
  printf( "Shared Memory Segment Size = %d\n", segment_size);


  //WRITE STRING TO SHARED MEMORY SEGMENT  --> Using "SPRINTF"
  sprintf (shared_memory, "Hello!");


  //DETACH THE SHARED MEMORY SEGMENT  
  shmdt(shared_memory);


  //REATTACH SHARED MEMORY SEGMENT, AT A DIFFERENT ADDRESS!!!   
  //Specify Page Address in process Adress Space, 0x50000, to attach the shared memory
  shared_memory = (char*)shmat(segment_id,(void*)0x50000,0);
  printf("shared memory reattached at address %p\n", shared_memory);

  
  //PRINT OUT STRING FROM SHARED MEMORY
  printf( "%s\n", shared_memory);
  
  //DETACH SHARED MEMORY
  shmdt(shared_memory);

  //DEALLOCATE THE SHARED MEMORY SEGMENT
  shmctl(segment_id, IPC_RMID, 0);

  return 0;
}
