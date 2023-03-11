/* cliTCPIt.c - Exemplu de client TCP
   Trimite un numar la server; primeste de la server numarul incrementat.
         
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  		// mesajul trimis
  int nr=0, amParola=0;
  char buf[256];
  char parola[256];
  char sir[256], Raspuns[256];
 

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

  /* citirea mesajului */
  
  while(1)
  { 
  
  printf("\n");
  amParola=0;
  printf ("[client]Introduceti o comanda: ");
  fflush (stdout);
  
  int n=read (0, buf, sizeof(buf));
  
  buf[strlen(buf) ] = '\0';
  //if(n==0||n==-1)
    // break;
  
  printf("\n");
  /*
  if(strstr(buf, "login")==NULL && strstr(buf, "password")==NULL && strstr(buf, "help")==NULL))
   {
      
      
   }
  */
  if(strstr(buf, "password:"))
  {
    //getpass ca sa pot sa nu vad parola cand o tastez 
    
    buf[0]='\0';
    strcpy(buf, getpass("Introduceti acum parola: "));
    
   // Criptare parola: 
   
    for(unsigned i=0;i<strlen(buf);i++)
       buf[i]=buf[i]-5;
       
      //buf[strlen(buf)-1]='\0';
      
    amParola=1;
      
     
  }
  
  if(strstr(buf, "afisare_c:"))
  {
  
  
  FILE *f;
  char c;

  sir[0]='\0';
  strcpy(sir, "./Client/");
  strncat(sir+9, buf+10, strlen(buf)-10);
  sir[strlen(sir)-1]='\0';
  int eroare=0;
  
  printf("%s\n", sir);

  f = fopen(sir, "r");
  if (f == NULL) {
     printf("Eroare la deschiderea fisierului");
     eroare=1;
  }


  if(eroare==0)
  { 

  char linie[101];
  
  strcpy(linie, "");
  printf("\n");
   printf("--------------------------------------------------------------");
 
  
  while (fgets(linie, sizeof(linie), f) != NULL) {
    printf("%s\n", linie);
    
  }
  
 // printf("%s\n", Raspuns);
 printf("\n");
 printf("--------------------------------------------------------------");
 }
  fclose(f);
  
  
  }
  
  
  
  if(strstr(buf, "afisare_s:"))
  {
  
  
  FILE *f;
  char c;

  sir[0]='\0';
  strcpy(sir, "./Server/");
  strncat(sir+9, buf+10, strlen(buf)-10);
  sir[strlen(sir)-1]='\0';

  f = fopen(sir, "r");
  if (f == NULL) {
    perror("Error opening file");
    return 1;
  }

  char linie[101];
  strcpy(linie, "");
   printf("\n");
   printf("--------------------------------------------------------------");
 
 
  


  
  while (fgets(linie, sizeof(linie), f) != NULL) {
   printf("%s\n", linie);
  }
 printf("\n");
 printf("--------------------------------------------------------------");
  
 
  fclose(f);

  
  }
  
  
  //nr=atoi(buf);
  //scanf("%d",&nr);
  
  if(amParola==0)
  printf("\n[client] Am citit %s\n",buf);
  
  if(strstr(buf,"quit"))
  break;
  
  if(strstr(buf, "help"))
  printf("\nlogin:username\npassword:insert password\nlogout:username\nquit\nhelp\nrename_c:fisier1/fisier2\nrename_s:fisier1/fisier2\nremove_c:fisier\nremove_s:fisier\nls_c:fisier\nls_s:fisier\nafisare_c:fisier\nafisare_s:fisier\ndirector_curent\ncreareFisier_c:nume\ncreareFisier_s:nume\ncreareDir_c:nume\ncreareDir_s:nume\nlungime_c:fisier\nlungime_s:fisier\ncopiere_cs:fisier\ncopiere_sc:fisier\nmutare_cs:fisier\nmutare_sc:fisier");
  
  
  
  

  /* trimiterea mesajului la server */
  if (write (sd,buf, strlen(buf)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    
    

  /* citirea raspunsului dat de server 
     (apel blocant pina cind serverul raspunde) */
     
  if (read (sd, buf, 255) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
    
   //buf[strlen(buf) -1] = '\0';
   
  /* afisam mesajul primit */
  printf("\n");

  printf ("[client]: %s\n", buf);
  bzero(buf, 255);
  }

  /* inchidem conexiunea, am terminat */
  close (sd);
}
