#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>          
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/un.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
//Leonardo David Camacho Garcia
//16650228
#define MAXDATASIZE 512
#define PORT 3551
#define BACKLOG 2
void menu(int *fd2);
void main()
{

   struct sockaddr_in client;  //direccion del cliente 
   struct sockaddr_in server; //direccion del server
   int fd, fd2;

   int sin_size;
   if ((fd=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {  //llamada al socket
      printf("Error en el socket()!\n");
      exit(-1);
   }
   server.sin_family = AF_INET;         
   server.sin_port = htons(PORT); 
   server.sin_addr.s_addr = INADDR_ANY; //obtener nuestra ip
   bzero(&(server.sin_zero),8); //agregar ceros

   if(bind(fd,(struct sockaddr*)&server,
           sizeof(struct sockaddr))==-1) {
      printf("Error en bind() \n");
      exit(-1);
   }     

   if(listen(fd,BACKLOG) == -1) {  //obtener listen
      printf("Error en listen()\n");
      exit(-1);
   }

   while(1) {
      sin_size=sizeof(struct sockaddr_in);
      if ((fd2 = accept(fd,(struct sockaddr *)&client,
                        &sin_size))==-1) {
         printf("Error en accept()\n");
         exit(-1);
      }
      menu(&fd2);
      close(fd2);
   }
}

void menu(int *fd2){
	char* fs_name = "/Leonardo/adminredes/scoket/muchotexto.txt";
        FILE *fs = fopen(fs_name, "r");
	char mensaje[1000];
   	char limpiar[10];
	int opc,tam,opc_msg,opc_a,opc_com,cantidad,archivo;
	strcpy(mensaje,"Te conectaste al Servidor\nElige la opción deseada\n 1.-Enviar mensaje \n2.-Enviar archivo \n3.-Ejecutar comando \n4.-Salir");
	write(*fd2,mensaje,strlen(mensaje));
	read(*fd2,&opc,sizeof(opc));
	switch(opc){
		case 1:
			while(1){
				printf("\n1.-Enviar mensaje \n2.-Recibi mensajer \n3.-Salir\n");
                        	scanf("%d",&opc_msg);
                        	if(opc_msg==1){
					strcpy(mensaje,"");
					printf("\n[SERVIDOR]-Introduce el mensaje que deseas enviar: ");
                                	gets(mensaje);
                                	gets(mensaje);
                                	opc=strlen(mensaje);
                                	write(*fd2,&opc,sizeof(opc));
                                	write(*fd2,mensaje,strlen(mensaje));
				}else if(opc_msg==2){
                                	//Recibir mensajes
					read(*fd2,&tam,sizeof(tam));
                                        read(*fd2,mensaje,tam);
                                        mensaje[tam]='\0';
                                        printf("\n[CLIENTE]-Mensaje: %s\n",mensaje);
                       		}else{
                                	break;
                        	}
			}
		break;
		case 2: //envio de archivos
			while(1){
				printf("\n1.-Leer\n 2.-Salir\n");
				scanf("%d",&opc_a);
				if(opc_a==1){
					while (1) {
    						int bytes_read = read(fs, mensaje, sizeof(mensaje));
						void *p = mensaje;
    						while (bytes_read > 0) {
        						int bytes_written = write(*fd2, p, bytes_read);
        						if (bytes_written <= 0) {
        						}
        						bytes_read -= bytes_written;
        						p += bytes_written;
    						}
						system("cat muchotexto.txt");
						break;
					}
				}else{
					exit(0);
				}
			}
		break;
		case 3:
			while(1){
                                printf("\n1.-Recibir \n2.-Enviar Resultado\n 3.-Salir\n");
                                scanf("%d",&opc_com);
                                if(opc_com==1){
                                        strcpy(mensaje,"");
                                        read(*fd2,&tam,sizeof(tam));
                                        read(*fd2,mensaje,tam);
                                        mensaje[tam]='\0';
                                        printf("\n[CLIENTE]-Comando: %s\n",mensaje);
                                }else if(opc_com==2){
					opc=strlen(mensaje);
                                        write(*fd2,&opc,sizeof(opc));
                                        write(*fd2,mensaje,strlen(mensaje));
				}else{
					exit(0);
                                        break;
                                }
                        }
		break;
		case 4:
		exit(0);
	}
        //Leonardo David Camacho Garcia
        //16650228
	printf("\n La opcion seleccionaste fue: %d",opc);
}
