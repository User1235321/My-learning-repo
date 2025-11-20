#define MAXBUFF 80
#define PERM 0666

typedef struct mem {
    int segment;
    char buff[MAXBUFF];
} Message;

// Ожидание начала выполнения клиента
static struct sembuf proc_wait[1] = {{1, -1, 0}};

// Уведомление сервера о том, что клиент начал работу  
static struct sembuf proc_start[1] = {{1, 1, 0}};

// Блокирование разделяемой памяти (2 операции)
static struct sembuf mem_lock[2] = {{0, 0, 0}, {0, 1, 0}};

// Освобождение ресурса
static struct sembuf mem_unlock[1] = {{0, -1, 0}};
