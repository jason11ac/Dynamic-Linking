#include "randcpuid.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dlfcn.h>

int
main (int argc, char **argv)
{
  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
	perror (argv[1]);
      else
	valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the                    
     appropriate library.  */
  void (*initialize) (void);
  unsigned long long (*rand64) (void);
  void (*finalize) (void);

  char* error = NULL; //Pointer to check for errors from dlerror()

  void* hard;
  void* soft;
  
  if (rdrand_supported()) // Use hardware functions
  {
     hard = dlopen("randlibhw.so", RTLD_NOW); //NOW mode so that symbols are resolved before dlopen returns
     if (hard == NULL)
       {
	 printf("Error opening hardware library. %s\n", dlerror());
	 exit(1); //Exit with non-zero status
       }
     initialize = dlsym(hard, "hardware_rand64_init"); //Initialize the hardware_rand64 implementation
     error = dlerror();
     if (error)
       {
	 printf("Error finding symbol for initilizing. %s\n", dlerror());
	 exit(1); //Exit with non-zero status
       }
     initialize();
     rand64 = dlsym(hard, "hardware_rand64"); //Initialize hardware symbol
     error = dlerror();
     if (error)
       {
	 printf("Error finding symbol for rand64. %s\n", dlerror());
	 exit(1); //Exit with non-zero status
       }
     finalize = dlsym(hard, "hardware_rand64_fini"); //Finalize the hardware_rand64 implementation
     error = dlerror();
     if (error)
       {
	 printf("Error finding symbol for finalizing. %s\n", dlerror());
	 exit(1); //Exit with non-zero status
       }  
      //initialize = hardware_rand64_init;
      //rand64 = hardware_rand64;
      //finalize = hardware_rand64_fini;
     finalize();
  }
  else
  {
     soft = dlopen("randlibsw.so", RTLD_NOW); //NOW mode so that symbols are resolved before dlopen returns
     if (soft == NULL)
       {
	 printf("Error opening software library. %s\n", dlerror());
	 exit(1); //Exit with non-zero status
       }
     initialize = dlsym(soft, "software_rand64_init"); //Initialize the software_rand64 implementation                                 
     error = dlerror();
     if (error)
       {
	 printf("Error opening software library. %s\n", dlerror());
	 exit(1); //Exit with non-zero status 
       }
     rand64 = dlsym(soft, "software_rand64"); //Initialize the software symbol
     error = dlerror();
     if (error)
       {
	 printf("Error finding symbol for rand64. %s\n", dlerror());
	 exit(1); //Exit with non-zero status
       }
     finalize = dlsym(soft, "software_rand64_fini"); //Finalize the software_rand64 implementation
     error = dlerror();
     if (error)
       {
	 printf("Error finding symbol for finalizing. %s\n", dlerror());
	 exit(1); //Exit with non-zero status
       } 
     //initialize = software_rand64_init;
     //rand64 = software_rand64;
     //finalize = software_rand64_fini;
  }

  //initialize ();
  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  do
    {
      unsigned long long x = rand64 ();
      size_t outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (fwrite (&x, 1, outbytes, stdout) != outbytes)
	{
	  output_errno = errno;
	  break;
	}
      nbytes -= outbytes;
    }
  while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
      finalize ();
      return 1;
    }

  if (rdrand_supported())
    {
      dlclose(hard);
    }
  else
    {
      dlclose(soft);
    }
  
  //finalize ();
  return 0;
}
