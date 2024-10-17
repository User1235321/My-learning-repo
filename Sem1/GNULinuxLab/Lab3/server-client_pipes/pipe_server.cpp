/* Программа  pipe_server.cpp */
#include"p.h"
int main(void)
{
int n, done, dummyfifo, privatefifo, publicfifo;
static char buffer[PIPE_BUF];
FILE *fin;
struct message msg; 
   /* Созданиеочередитипа public FIFO */
mknod(PUBLIC, S_IFIFO | 0666, 0);
   /* Открыть public FIFO начтениеизапись */
if (((publicfifo=open(PUBLIC, O_RDONLY))==-1) ||(dummyfifo=open(PUBLIC, O_WRONLY | O_NDELAY))==-1){
    perror(PUBLIC);
        exit(1);
            }
            /* Сообщениеможнопрочитатьиз public конвейера */
while(read(publicfifo, (char *) &msg, sizeof(msg))>0){
    n = done = 0; /* Очисткасчетчиков | флагов */
        do{/* Попыткаоткрытия private FIFO */
        if ((privatefifo=open(msg.fifo_name, O_WRONLY | O_NDELAY))==-1)
        sleep(3);/* Задержкаповремени  */
        else{/* Открытиеуспешно */
         fin = popen(msg.cmd_line, "r");/* Исполнение shell cmd, полученнойотклиента */
                write(privatefifo, "\n", 1); /* Подготовкаочередноговывода */    while((n=read(fileno(fin), buffer, PIPE_BUF))>0){ 
                write(privatefifo, buffer, n);/* Выводв private FIFO кклиенту */
                memset(buffer, 0x0, PIPE_BUF);/* Очисткабуфера */
                }
                pclose(fin);
                close(privatefifo);
                done = 1;/* Записьпроизведенауспешно */
                    }
                    }while(++n<5 && !done);
                    if(!done)/* Указаниенанеудачныйисход */ 
                       write(fileno(stderr), "\nNOTE: SERVER ** NEVER ** accessed private FIFO\n", 48);    }
                       return 0;
}
                       

