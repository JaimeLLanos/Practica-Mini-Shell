#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "parser.h"
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void *redireccionDeEntrada(tline *mandato){
	int fd;	
	if((*mandato).redirect_input != NULL){
		fd = open((*mandato).redirect_input,O_RDONLY);
		if(fd == -1){
			 printf(stderr,"%s: Error. No existe el fichero.\n",(*mandato).redirect_input);
			exit(1);
		}else{
			dup2(fd,0); //El 0 indica que es de lectura
		}
	}
}

void *redireccionDeSalida(tline *mandato){
	int fd;
	if((*mandato).redirect_output != NULL){
		fd = open((*mandato).redirect_output,O_WONLY|O_CREAT);
		if(fd == -1){
			printf(stderr,"%s: Error. Fallo al crear el fichero para escritura.",(*mandato).redirect_output);
			exit(1);
		}else{
			dup2(fd,1); //El 1 indica que es de escritura
		}
	}
	
}

void *redireccionDeError(tline *mandato){
	int fdesc;
	if((*mandato).redirect_error != NULL){
		fd = open((*mandato).redirect_error);		
		if(fd == -1){
			printf(stderr,"%s: Error. Fallo al crear el fichero para su escritura",(*mandato).redirect_error);
			exit(1);
		}else{
			dup2(fd,2); //El 2 indica que es de error
		}
	}
}

void uncomando(tline *mandatos){
	pid_t pid;
	tcommand *mandato = mandatos.commands;
	pid=fork();
	if (pid<0) {
		printf("fallo en el fork.\n");
	} else if (pid==0) {
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		redireccionDeEntrada(mandatos);
		redireccionDeSalida(mandatos);
		redireccionDeError(mandatos);
		if((*mandato).filename == NULL){
			printf(stderr,"%s: No se encuentra el mandato\n",(*mandato).filename;
			exit(1);
		}else{
			execvp((*mandato).filename,(*mandato).argv);
			printf("error execvp\n");
			exit(1);		
		}
	} else { 
		wait(NULL);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, SIG_IGN);
		} //fin padre
}//fin unmandato

void varioscomandos(){
}

int main(int argc, char* argv[]){ //inicio main
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	tline *mandatos; //variable para hacerle el tokenize
	char buffer[1024];
	if (argc==1) {
		while (fgets(buffer, 1024, stdin)) { //inicio de while
			//recogemos en buffer la variable de entrada de teclado
			printf("msh> ");			
			mandatos=tokenize(buffer);
			if ((*mandatos).ncommands==1) {
				uncomando(mandatos);
			} else if ((*mandatos).ncommands>1) {
				varioscomandos();
			}
		} //fin del while
	} else //fin del if
		printf("Error, funcion sin argumentos.\n");
} //fin del main
