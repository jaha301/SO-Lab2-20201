#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>

#include <sys/wait.h>


int main(int argc, char *argv[])
{  char *bin = "/bin/";
   
    char **path;
    char mensajeError[25]= "An error has occurred\n";
        
    path = (char **)malloc(3 * sizeof(char *));
    path[0] = bin;
    path[1] = NULL;
    
//Interactivo
    if (argc == 1)
  { while (1)
        {   int forks[100];
            forks[0] = 777;
            printf("wish> ");
            
            char *line = NULL;
           
            size_t len = 0;
            ssize_t read;
            if ((read = getline(&line, &len, stdin)) == -1)
            {write(STDERR_FILENO, mensajeError, strlen(mensajeError));
                continue;}
            line[strlen(line) - 1] = '\0';
    
            char *aux;
            char *aux2;
            char *aux3;
	     char *ex = strdup(line);
            int comand_exists = 0;

            for (int j = 0; (aux = strsep(&ex, " ")) != NULL; j++)
            {for (int k = 0; (aux2 = strsep(&aux, "\t")) != NULL; k++)
                { for (int k = 0; (aux3 = strsep(&aux2, "&")) != NULL; k++)
                    { if (strcmp(aux3, "") != 0)
                        { comand_exists = 1;}}}
            }
            if (comand_exists == 0) {continue;}
            char *argumento[100];
            char *palabra;
            
            for (int i = 0; (palabra = strsep(&line, "&")) != NULL; i++)
            {if (strcmp(palabra, "") != 0 && strcmp(palabra, "\t") != 0)
                {
                    argumento[i] = palabra;
                    argumento[i + 1] = NULL;
                }
                else{ i = i - 1; };  };

            for (int i = 0; argumento[i] != NULL; i++)
            {
		 char *argumento2[100];
                char *FILE = NULL;
                
                char *FILE2 = NULL;
                char *palabra2;
                char *palabra3;

                int redireccion = 0;
                int error = 0;

                char *args = strdup(argumento[i]);

                for (int j = 0; (palabra2 = strsep(&args, ">")) != NULL; j++)
                { if (j == 0)
                    { if (strcmp(palabra2, "") != 0){ argumento[i] = palabra2; }
                        else
                        { error = 1; } }
                    else if (j == 1)
                    {
                        redireccion = 1;
                        FILE = palabra2;
                        if (FILE == NULL){ error = 1; }
                    }

		  else   {error = 1; break;}
                }if (redireccion == 1)
                {if (FILE != NULL)
                    { int unFILE = 0;
                        char *palabra6;
                        char *palabra7;

               	   for (int j = 0; (palabra6 = strsep(&FILE, " ")) != NULL; j++)
                        {
                            for (int k = 0; (palabra7 = strsep(&palabra6, "\t")) != NULL; k++)
                            { if (strcmp(palabra7, "") != 0)
                                {  if (unFILE == 0)
                                    { unFILE = 1;
                                        FILE2 = palabra7;
                                        j = j + 1;
                                    } else
                                    {error = 1; break; }
                                }}
                        }
                    }
                }

                if (error == 1 || (FILE2 == NULL && redireccion == 1))
                { write(STDERR_FILENO, mensajeError, strlen(mensajeError));
                    break;
                }
                for (int j = 0; (palabra2 = strsep(&argumento[i], " ")) != NULL; j++)
                {
                    for (int k = 0; (palabra3 = strsep(&palabra2, "\t")) != NULL; k++)
                    {
                        if (strcmp(palabra3, "") != 0)
                        {   argumento2[j] = palabra3;
                            argumento2[j + 1] = NULL;
                            j = j + 1;
                        } }
                        j = j - 1; };

                if (i == 0)
                {
                    if (strcmp(argumento2[0], "exit") == 0)
                    { if (argumento2[1] == NULL)
                        {   exit(0);
                            break;}
                        else
                        { write(STDERR_FILENO, mensajeError, strlen(mensajeError));
                            break; }
                    };
                    if (strcmp(argumento2[0], "cd") == 0)
                    {if (chdir(argumento2[1]) == -1)
                        { write(STDERR_FILENO, mensajeError, strlen(mensajeError)); }

                        break;
                    };

                    if (strcmp(argumento2[0], "path") == 0)
                    {
                        if (argumento2[1] == NULL)
                        {
                            path[0] = NULL;
                        }
                        free(path);
                        path = (char **)malloc(sizeof(char *));
                        for (int k = 1; argumento2[k] != NULL; k++)
                        {  char *letra = strdup(argumento2[k]);
                            char pathActi[200] = "./";
                            char pathActi2[200] = "";
                            char *auxult;
                            char *barraInclinada = "/";
                            if (letra[0] == 47)
                            {
                                strcat(pathActi2, argumento2[k]);
                                strcat(pathActi2, barraInclinada);
                                auxult = strdup(pathActi2);
                            }
                            else
                            {
                                strcat(pathActi, argumento2[k]);
                                strcat(pathActi, barraInclinada);
                                auxult = strdup(pathActi);
                            }
                            path[k - 1] = strdup(auxult);
                            path[k] = NULL;
                        }
                        break;
                    };
                };
                 int a = 1;
                for (int j = 0; path[j] != NULL; j++)
                {  char *path2 = strdup(path[j]);
                   strcat(path2, argumento2[0]);
                   if (access(path2, F_OK) == 0)
                    {  a = 0;
                        int rc = fork();
                         forks[i] = rc;
                        forks[i + 1] = 777;
                         if (rc == 0)
                        {    if (redireccion == 1)
                            {     close(STDOUT_FILENO);
                                open(FILE2, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
                                execvp(path2, argumento2);
                                exit(0);
                            }else
                            { execv(path2, argumento2);
                                exit(0);}
                        }
                        break;
                    }
                };
                 if (a == 1){write(STDERR_FILENO, mensajeError, strlen(mensajeError));}
            };
            int status;
            for (int f = 0; forks[f] != 777; f++)
            {
                waitpid(forks[f], &status, 0);
            }
        };
    };

  
  
    //batch
   if (argc == 2)
    { FILE *fp = fopen(argv[1], "r");
        
        if (fp == NULL)
        { write(STDERR_FILENO, mensajeError, strlen(mensajeError));
            exit(1);
        };
        FILE *fp2 = fopen(argv[1], "r");
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        while ((read = getline(&line, &len, fp)) != -1)
        {
            int forks[100];
            forks[0] = 777;
            if (strcmp(line, "") != 0)
            {
                line[strlen(line) - 1] = '\0';
		     char *ex = strdup(line);
		     char *aux;
                char *aux2;
                char *aux3;
		    int comand_exists = 0;
                for (int j = 0; (aux = strsep(&ex, " ")) != NULL; j++)
                {
                    for (int k = 0; (aux2 = strsep(&aux, "\t")) != NULL; k++)
                    {
                        for (int k = 0; (aux3 = strsep(&aux2, "&")) != NULL; k++)
                        {
                            if (strcmp(aux3, "") != 0) { comand_exists = 1;}
                        }
                    }
                } if (comand_exists == 0) { continue; } 
                 char *palabra;
                char *argumento[100];
                for (int i = 0; (palabra = strsep(&line, "&")) != NULL; i++)
                {
                    if (strcmp(palabra, "") != 0 && strcmp(palabra, "\t") != 0)
                    {  argumento[i] = palabra;
                        argumento[i + 1] = NULL;}
                    else{ i = i - 1;};
                };

                for (int i = 0; argumento[i] != NULL; i++)
                {
                   char *argumento2[100];
                    char *palabra2;
                    char *palabra3;
                    char *FILE = NULL;
                    char *FILE2 = NULL;
                    int redireccion = 0;
                    int error = 0;

                    char *args = strdup(argumento[i]);

                    for (int j = 0; (palabra2 = strsep(&args, ">")) != NULL; j++)
                    {
                        if (j == 0)
                        {
                            if (strcmp(palabra2, "") != 0)
                            { argumento[i] = palabra2;}
                            else{error = 1; }
                        } else if (j == 1)
                        { redireccion = 1;
                            FILE = palabra2;
                            if (FILE == NULL)
                            {
                                error = 1;
                            }
                        } else
                        {  error = 1;
                            break; }
                    } if (redireccion == 1)
                    {  if (FILE != NULL)
                        {   int unFILE = 0;
                            char *palabra6;
                            char *palabra7;

                              for (int j = 0; (palabra6 = strsep(&FILE, " ")) != NULL; j++)
                            {
                                for (int k = 0; (palabra7 = strsep(&palabra6, "\t")) != NULL; k++)
                                {   if (strcmp(palabra7, "") != 0)
                                    {    if (unFILE == 0)
                                        { FILE2 = palabra7;
                                        	unFILE = 1;
                                              j = j + 1;
                                        }  else  {  error = 1; 
                                         break;
                                        }
                                    }
                                }
                            }
                        }
                    }
			  if (error == 1 || (FILE2 == NULL && redireccion == 1))
                    {    write(STDERR_FILENO, mensajeError, strlen(mensajeError));
                        break;
                    } for (int j = 0; (palabra2 = strsep(&argumento[i], " ")) != NULL; j++)
                    {    for (int k = 0; (palabra3 = strsep(&palabra2, "\t")) != NULL; k++)
                        {   if (strcmp(palabra3, "") != 0)
                            {    argumento2[j] = palabra3;
                                argumento2[j + 1] = NULL;
                                j = j + 1;
                            }
                        }
                        j = j - 1;
                    };
		      if (i == 0)
                    {
                    
                       if (strcmp(argumento2[0], "exit") == 0)
                        {
                            if (argumento2[1] == NULL){exit(0);
                                break;}
                            else {write(STDERR_FILENO, mensajeError, strlen(mensajeError));
                                break;
                            }
                        };

                        if (strcmp(argumento2[0], "cd") == 0)
                        {    if (chdir(argumento2[1]) == -1)
                            {    write(STDERR_FILENO, mensajeError, strlen(mensajeError));
                            }
                            break;
                        };

                        if (strcmp(argumento2[0], "path") == 0)
                        {   if (argumento2[1] == NULL)
                            {
                                path[0] = NULL;
                            }
                            free(path);
                            path = (char **)malloc(sizeof(char *));

                            for (int k = 1; argumento2[k] != NULL; k++)
                            {    char *letra = strdup(argumento2[k]);
			      char pathActi[200] = "./";
                                char pathActi2[200] = "";
                                  char *barraInclinada = "/";
                                  char *auxult;
                              
				      if (letra[0] == 47)
                                {   strcat(pathActi2, argumento2[k]);
                                    strcat(pathActi2, barraInclinada);
                                    auxult = strdup(pathActi2);
                                }     else
                                {
                                    strcat(pathActi, argumento2[k]);
                                    strcat(pathActi, barraInclinada);
                                    auxult = strdup(pathActi);
                                }   path[k - 1] = strdup(auxult);
                                path[k] = NULL;
                            }
                            break;
                        };
                    };
                    int a = 1;
                     for (int j = 0; path[j] != NULL; j++)
                    {   char *path2 = strdup(path[j]);
			   strcat(path2, argumento2[0]);
			  if (access(path2, F_OK) == 0)
                        {   a = 0;
                            int rc = fork();
                              forks[i] = rc;
                            forks[i + 1] = 777;
                             if (rc == 0)
                            {   if (redireccion == 1)
                                {   close(STDOUT_FILENO);
                                    open(FILE2, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
                                    execvp(path2, argumento2);
                                    exit(0);
                                } else{
                                    execv(path2, argumento2);
                                    exit(0);
                                }
                            }break;
                        }
                    };
                       if (a == 1) {write(STDERR_FILENO, mensajeError, strlen(mensajeError));}
                };
            };
            
             int status;
            for (int f = 0; forks[f] != 777; f++)
            {waitpid(forks[f], &status, 0); }
        };
        fclose(fp);
        fclose(fp2);
        exit(0);
    };    
     write(STDERR_FILENO, mensajeError, strlen(mensajeError));
    exit(1);
}
