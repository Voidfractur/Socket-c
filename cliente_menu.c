#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h> 
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <stdio.h>          
//Leonardo David Camacho Garcia
//16650228
struct persona{
	char nombre[100];
	int edad;
	char dir[100];
	char cel[10];
};

#define PORT 3551   
#define MAXDATASIZE 1000   

int main(int argc, char *argv[])
{
   struct persona *agenda=NULL;
   char limpiar[10];

   int fd, numbytes,opc,opc_msg,opc_com,tam,cantidad,opc_a,archivo;       
   char buf[MAXDATASIZE];  //almacenaje de la cadena

   struct hostent *he;         
   struct sockaddr_in server;  //direccion del server
   if (argc !=2) { 
      printf("Uso: %s <Dirección IP>\n",argv[0]);
      exit(-1);
   }

   if ((he=gethostbyname(argv[1]))==NULL){       
      printf("gethostbyname() error\n");
      exit(-1);
   }

   if ((fd=socket(AF_INET, SOCK_STREAM, 0))==-1){  
      printf("socket() error\n");
      exit(-1);
   }

   server.sin_family = AF_INET;
   server.sin_port = htons(PORT); 
   server.sin_addr = *((struct in_addr *)he->h_addr); 
   bzero(&(server.sin_zero),8);

   if(connect(fd, (struct sockaddr *)&server,
      sizeof(struct sockaddr))==-1){ 
      printf("connect() error\n");
      exit(-1);
   }

   if ((numbytes=read(fd,buf,MAXDATASIZE)) == -1){  
      printf("Error en recv() \n");
      exit(-1);
   }

   buf[numbytes]='\0';

   printf("Mensaje del Servidor: %s\n",buf); 
   scanf("%d",&opc);
   write(fd,&opc,sizeof(opc));
   switch(opc){
               
      case 1: //1.-Enviar/Recibir Mensajes
         while(1){
            printf("\n1.-Enviar \n2.-Recibir \n3.-Salir\n");
            scanf("%d",&opc_msg);
            strcpy(buf,"");
            if(opc_msg==1){
               printf("\n++CLIENTE++-Escribe el mensaje que deseas mandar: ");
               gets(buf);
               gets(buf);
               opc=strlen(buf);
               write(fd,&opc,sizeof(opc));
               write(fd,buf,strlen(buf));
            }else if(opc_msg==2){
               //Recibir mensajes
               read(fd,&tam,sizeof(tam));
               read(fd,buf,tam);
               buf[tam]='\0';
               printf("\n++SERVIDOR++-Mensaje: %s\n",buf);
            }else{
               break;
            }
         }
      break;
      case 2:  //2.-Enviar Archivo
         printf("\nQue deseas realizar?\n \n1.-Enviar\n2.-Salir\n");
         scanf("%d",&opc_a);
         gets(limpiar);
            if(opc_a==1){
		char* fs_name = "/Leonardo/adminredes/scoket/muchotexto.txt";
        	printf("++CLIENTE++ Enviando al servidor  %s ", fs_name);
        	FILE *fs = fopen(fs_name, "r");
        	if(fs == NULL){
            		printf("ERROR: Archivo no encontrado %s.\n", fs_name);
            		exit(1);
        	}

        	bzero(buf, MAXDATASIZE); 
        	int fs_block_sz; 
        	while((fs_block_sz = fread(buf, sizeof(char), MAXDATASIZE, fs)) > 0){
            		if(send(fd,buf, fs_block_sz, 0) < 0){
                		fprintf(stderr, "ERROR: Error no se pudo enviar el archivo  %s. (errno = %d)\n", fs_name, errno);
                		break;
           		}
            		bzero(buf, MAXDATASIZE);
        	}
        	printf("\nArchivo enviado correctamente %s\n", fs_name);
	    }else{
               exit(0);
            }
      break;
      case 3:
	while(1){
            printf("\n1.-Enviar \n2.-Recibir\n 3.-Salir\n");
            scanf("%d",&opc_com);
            strcpy(buf,"");
            if(opc_com==1){
               printf("\n++CLIENTE++-Introduce el comando que deseas mandar: ");
               gets(buf);
               gets(buf);
               opc=strlen(buf);
               write(fd,&opc,sizeof(opc));
               write(fd,buf,strlen(buf));
            }else if(opc_com==2){
		read(fd,&tam,sizeof(tam));
               read(fd,buf,tam);
               buf[tam]='\0';
               printf("\n++SERVIDOR++-Comando: %s\n",buf);
	       system(buf);
	    }else{
	    	exit(0);
	    }
	}
      break;
      case 4:
      exit(0);
   }
   //Leonardo David Camacho Garcia
//16650228
   close(fd);
}