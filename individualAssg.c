#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void sig_handler(int signo){
    if(signo == SIGINT)
       printf("This is an interrupt message from the handler.\n");
 }


int main(void){
   
    int fd1[2], fd2[2],fd3[2], fd4[2],a,b;
    pipe(fd1);
    pipe(fd2);
    pipe(fd3);
    pipe(fd4);


   if(signal(SIGINT, sig_handler) == SIG_ERR)
      printf("Failed to catch SIGINT\n");

    printf("Please enter TWO numbers for addition and subtraction:\n");
    scanf("%d", &a);
    scanf("%d", &b);
    write(fd1[1], &a, sizeof(int));
    write(fd2[1], &b, sizeof(int));
    printf("Please enter TWO numbers for multiplication and division:\n");
    scanf("%d", &a);
    scanf("%d", &b);
    write(fd3[1], &a, sizeof(int));
    write(fd4[1], &b, sizeof(int));

/*--------------------------------firstChild-----------------------------------*/

    pid_t firstChild = fork();
    if(firstChild == 0){
       int status = 0, a, b, c, d ;
       read(fd1[0], &a, sizeof(int));
       read(fd2[0], &b, sizeof(int));
       printf("First Child will Handle ADDITION and SUBTRACTION calculation.\n");
       c = a + b;
       d = a - b;
       write(fd1[1], &c, sizeof(int));
       write(fd2[1], &d, sizeof(int));    


  } //firstChild end

/*--------------------------------secondChild---------------------------------*/

  else
{

    pid_t secondChild = fork();
    if(secondChild == 0){
       int status = 0, a, b, c, d;
       read(fd3[0], &a, sizeof(int));
       read(fd4[0], &b, sizeof(int));

       printf("Second Child will handle MULTIPLICATION and DIVISION calculation.\n");
       c = a*b;
       d = a/b;

       write(fd3[1], &c, sizeof(int));
       write(fd4[1], &d, sizeof(int));

//to display output

        pid_t display = fork();
        if(display == 0){

        int sum, sub, mul, div;
        read(fd1[0], &sum, sizeof(int));
        read(fd2[0], &sub, sizeof(int));
        read(fd3[0], &mul, sizeof(int));
        read(fd4[0], &div, sizeof(int));

        printf("SUMMATION = %d\n",sum);
        printf("SUBTRACTION = %d\n",sub);
        printf("MULTIPLICATION = %d\n", mul);
        printf("DIVISION = %d\n", div);

     } //display output end

   }

 } //secondChild end


  return 0;
}


