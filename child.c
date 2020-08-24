#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

typedef struct {
    char* readbuffer;
    unsigned short* values;
    int count;
    unsigned int sum;
}SParams;

int cmpfunc (const void * a, const void * b) {
   return ( *(unsigned short*)a - *(unsigned short*)b );
}

void printvalues(SParams* params) {
    for(int i = 0; i < params->count; i++) {
        printf("%d", params->values[i]);
    }
    printf("\n");
    printf("sum: '%u'\n", params->sum);
}

void* convertandsum(void* arg) {
    SParams* params = (SParams*) arg;
    
    char* p;
    unsigned short* pv;

    for(p = params->readbuffer, pv = params->values; 
        p < params->readbuffer + params->count; p++, pv++) {
        *pv = ((unsigned short)*p) - 0x30; // Convertir digito en char a entero
        params->sum += *pv;
    }
    
    return NULL;
}

void* sort(void* arg) {
    SParams* params = (SParams*) arg;
    qsort(params->values, params->count, sizeof(unsigned short), cmpfunc);
    return NULL;
}

int main(int argc, char** argv) {
    printf("Iniciando proceso hijo, pid '%d'.\n", getpid());
    
    char readbuff[128];
    char* values;
    int trc = 0;
    int cz = 4;
    int rc = 0;

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <fd>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    int fd = atoi((char *)argv[1]);
    values = malloc(sizeof(char)*128);
    
    while ((rc = read(fd, readbuff, 128)) > 0) {
        if (readbuff[0] == '\n') {
            break;
        }
        else if (readbuff[rc-1] == '\n') {
            rc--;
        }
        
        if (trc + rc > cz) {
            cz = 2 * (trc + rc);
            values = realloc(values, sizeof(char)*cz);
            // printf("values readbuffer size increased to '%d'.\n", cz);
        }

        memmove(values + trc, readbuff, rc);
        trc += rc;
        
        // printf("child read '%d' bytes.\n", rc);
    }

    pthread_t t1, t2;
    pthread_attr_t tattr;
    SParams params = { values, NULL, trc, 0 };
    params.values = (unsigned short*)malloc(sizeof(unsigned short)*trc);

    if (pthread_attr_init(&tattr)) {
        fprintf(stderr, "Error iniciando atributos de thread\n");
    }

    // Convertir valores en array
    if (pthread_create(&t1, &tattr, convertandsum, (void *)&params)) {
        fprintf(stderr, "Error iniciando thread 't1'\n");
    }

    pthread_join(t1, NULL);
    printvalues(&params);

    // Ordenar valores en array
    if (pthread_create(&t2, &tattr, sort, (void *)&params)) {
        fprintf(stderr, "Error iniciando thread 't2'\n");
    }

    pthread_join(t2, NULL);
    printvalues(&params);

    printf("Proceso hijo ha terminado.\n");

    free(values);
    free(params.values);
    
    return(EXIT_SUCCESS);
}