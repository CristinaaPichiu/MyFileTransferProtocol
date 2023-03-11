/* servTCPConcTh2.c - Exemplu de server TCP concurent care deserveste clientii
   prin crearea unui thread pentru fiecare client.
 
  
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/

//COMPILARE: gcc myprog.c -o myprog -lmysqlclient -pthread

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <mysql/mysql.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>


/* portul folosit */
#define PORT 2908
#define dimensiune 1024
int conectat;
int logat[6], status[6];


/* codul de eroare returnat de anumite apeluri */
extern int errno;

typedef struct thData{
	int idThread; //id-ul thread-ului tinut in evidenta de acest program
	int cl; //descriptorul intors de accept
}thData;

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void *);

/// Functie care cauta un rezultat returnat de interogarea "interogare" si il compara cu sirul introdus ca parametru

int search_bd(char *sir, char * interogare)
{
   
   MYSQL *apel_bd;
   MYSQL *rezultat_bd;
   MYSQL_ROW inregistrare;
   
   char * server = "localhost";
   char * user = "utilizator";
   char* password = "baze10";
   char * database = "db";
   
   
   apel_bd=mysql_init(NULL);
   
   //conectarea propriu-zisa la baza de date 
   
   if(!mysql_real_connect(apel_bd, server, user, password, database, 0, NULL, 0))
   {
      fprintf(stderr, "%s\n", mysql_error(apel_bd));
      return 1;
   }
   
   //trimitere interogare 
   
   if( mysql_query(apel_bd, interogare))
   {
     fprintf(stderr, "%s\n", mysql_error(apel_bd));
     return 1;
   }
   
   rezultat_bd=mysql_use_result(apel_bd);
   
   while((inregistrare=mysql_fetch_row(rezultat_bd))!=NULL)
   {
       if(strcmp(inregistrare[0], sir)==0)
         {
          
    mysql_free_result(rezultat_bd);
    mysql_close(apel_bd);
     return 1;}
   }
    
    
    mysql_free_result(rezultat_bd);
    mysql_close(apel_bd);
    return 0;
   

}

//Functie care valideaza username ul, verifica daca acesta exista in BD 

int validare_nume(char*nume)
{
   if(search_bd(nume, "SELECT name FROM utilizatori"))
   return 1;
   return 0;
}

//Functie care valideaza parola, verifica daca aceasta exista in BD 

int validare_parola(char * parola)
{
 if(search_bd(parola, "SELECT password FROM utilizatori"))
 return 1;
 return 0;
}

//Functie care returneaza ce status are un anumit utilizator dupa numele acestuia 

int search_status( char * nume)
{
   
   MYSQL *apel_bd;
   MYSQL *rezultat_bd;
   MYSQL_ROW inregistrare;
   
   char * server = "localhost";
   char * user = "utilizator";
   char* password = "baze10";
   char * database = "db";
   
   
   apel_bd=mysql_init(NULL);
   
   //conectarea propriu-zisa la baza de date 
   
   if(!mysql_real_connect(apel_bd, server, user, password, database, 0, NULL, 0))
   {
      fprintf(stderr, "%s\n", mysql_error(apel_bd));
      exit(1);
   }
   
   //trimitere interogare 
   
   if(strcmp(nume, "cristina.pichiu")==0)
   {
   
   if( mysql_query(apel_bd, "SELECT status FROM utilizatori where name='cristina.pichiu'"))
   {
     fprintf(stderr, "%s\n", mysql_error(apel_bd));
     exit(1);
   }
   }
   else 
   if(strcmp(nume, "catalina.tanase")==0)
   {
    if( mysql_query(apel_bd, "SELECT status FROM utilizatori where name='catalina.tanase'"))
   {
     fprintf(stderr, "%s\n", mysql_error(apel_bd));
     exit(1);
   }
   }
   else 
   if(strcmp(nume, "cristian.pichiu")==0)
   {
    if( mysql_query(apel_bd, "SELECT status FROM utilizatori where name='cristian.pichiu'"))
   {
     fprintf(stderr, "%s\n", mysql_error(apel_bd));
     exit(1);
   }
   }
   else if(strcmp(nume, "andreea.popescu")==0)
   {
    if( mysql_query(apel_bd, "SELECT status FROM utilizatori where name='andreea.popescu'"))
   {
     fprintf(stderr, "%s\n", mysql_error(apel_bd));
     exit(1);
   }
   }
   else 
   if(strcmp(nume, "catalin.morariu")==0)
   {
    if( mysql_query(apel_bd, "SELECT status FROM utilizatori where name='catalin.morariu'"))
   {
     fprintf(stderr, "%s\n", mysql_error(apel_bd));
     exit(1);
   }
   }
   
   rezultat_bd=mysql_use_result(apel_bd);

   while((inregistrare=mysql_fetch_row(rezultat_bd))!=NULL)
    if(strcmp(inregistrare[0], "whitelist")==0)
       
    {
    
    mysql_free_result(rezultat_bd);
    mysql_close(apel_bd);
    return 1;}
  
    mysql_free_result(rezultat_bd);
    mysql_close(apel_bd);
      return 0;
   
}

// functie care returneaza id-ul unui anumit utilizator dupa numele acestuia 

int search_id( char * nume)
{
   
   MYSQL *apel_bd;
   MYSQL *rezultat_bd;
   MYSQL_ROW inregistrare;
   
   char * server = "localhost";
   char * user = "utilizator";
   char* password = "baze10";
   char * database = "db";
   
   
   apel_bd=mysql_init(NULL);
   int gasit=0;
   
   //conectarea propriu-zisa la baza de date 
   
   if(!mysql_real_connect(apel_bd, server, user, password, database, 0, NULL, 0))
   {
      fprintf(stderr, "%s\n", mysql_error(apel_bd));
      exit(1);
   }
   
   //trimitere interogare 
   
   if(strcmp(nume, "cristina.pichiu")==0)
   { gasit=1;
   
   if( mysql_query(apel_bd, "SELECT id FROM utilizatori where name='cristina.pichiu'"))
   { 
     fprintf(stderr, "%s\n", mysql_error(apel_bd));
     exit(1);
   }
   }
   else 
   if(strcmp(nume, "catalina.tanase")==0)
   {  
   gasit=1;
    if( mysql_query(apel_bd, "SELECT id FROM utilizatori where name='catalina.tanase'"))
   {
     fprintf(stderr, "%s\n", mysql_error(apel_bd));
     exit(1);
   }
   }
   else 
   if(strcmp(nume, "cristian.pichiu")==0)
   { gasit=1;
    
    if( mysql_query(apel_bd, "SELECT id FROM utilizatori where name='cristian.pichiu'"))
   {
     fprintf(stderr, "%s\n", mysql_error(apel_bd));
     exit(1);
   }
   }
   else if(strcmp(nume, "andreea.popescu")==0)
   {
    gasit=1;
    if( mysql_query(apel_bd, "SELECT id FROM utilizatori where name='andreea.popescu'"))
   {
     fprintf(stderr, "%s\n", mysql_error(apel_bd));
     exit(1);
   }
   }
   else 
   if(strcmp(nume, "catalin.morariu")==0)
   { 
   gasit=1;
    if( mysql_query(apel_bd, "SELECT id FROM utilizatori where name='catalin.morariu'"))
   {
     fprintf(stderr, "%s\n", mysql_error(apel_bd));
     exit(1);
   }
   }
    
     int x=0;
   
   if(gasit==1)
   {
   rezultat_bd=mysql_use_result(apel_bd);
   inregistrare=mysql_fetch_row(rezultat_bd);
    x=atoi(inregistrare[0]);
   }

   
    //mysql_free_result(rezultat_bd);
    //mysql_close(apel_bd);
    
   return x;
}


int main ()
{ 
  struct sockaddr_in server;	// structura folosita de server
  struct sockaddr_in from;	
  int nr;
  char buf[256];		//mesajul primit de trimis la client 
  int sd;		//descriptorul de socket 
  int pid;
  pthread_t th[100];    //Identificatorii thread-urilor care se vor crea
	int i=0;
  

  /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }
  /* utilizarea optiunii SO_REUSEADDR */
  int on=1;
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  
  /* pregatirea structurilor de date */
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;	
  /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
  /* utilizam un port utilizator */
    server.sin_port = htons (PORT);
  
  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 2) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }
  /* servim in mod concurent clientii...folosind thread-uri */
  while (1)
    {
      int client;
      thData * td; //parametru functia executata de thread     
      int length = sizeof (from);

      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);

      // client= malloc(sizeof(int));
      /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
      if ( (client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
	{
	  perror ("[server]Eroare la accept().\n");
	  continue;
	}
	
        /* s-a realizat conexiunea, se astepta mesajul */
    
	// int idThread; //id-ul threadului
	// int cl; //descriptorul intors de accept

	td=(struct thData*)malloc(sizeof(struct thData));	
	td->idThread=i++;
	td->cl=client;

	pthread_create(&th[i], NULL, &treat, td);	      
				
	}//while    
};				
static void *treat(void * arg)
{		
		struct thData tdL; 
		tdL= *((struct thData*)arg);	
		printf("\n");
		printf ("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
		fflush (stdout);		 
		pthread_detach(pthread_self());		
		raspunde((struct thData*)arg);
		/* am terminat cu acest client, inchidem conexiunea */
		close (arg);
		return(NULL);	
  		
};


void raspunde(void *arg)
{
        int nr, i=0,n,lg, conectat=0,validare=0,w;
        char buf[256], ceva[256];
        char numeUser[256], Parola[256], copie[256], numeFisierVechi[256], numeFisierNou[256], sir[256];
        numeUser[0]='\0';
        copie[0]='\0';
        Parola[0]='\0';
        char raspuns[256];
        char Raspuns[256];
        raspuns[0]='\0';
        Raspuns[0]='\0';
	struct thData tdL; 
	tdL= *((struct thData*)arg);
	 
      
	while(n=read (tdL.cl, buf, 255))
	{ 
	
	buf[strlen(buf)]='\0';
	lg=strlen(buf);
	
	   // printf("%s\n", buf);
          //  printf("%d\n", strlen(buf));
           
	if (n<=0)
          {
	      printf("[Thread %d]\n",tdL.idThread);
	      perror ("Eroare la read() de la client.\n");
			
          }
        
	
	printf ("[Thread %d]Mesajul a fost receptionat...%s\n",tdL.idThread, buf);
		      
       /*pregatim mesajul de raspuns */
		      
        numeUser[0]='\0';
       
       //----------------------------------------------------------------------------LOGIN----------------------------------------------------------------------------------------------------
       
       // DACA COMANDA INTRODUSA ESE LOGIN (utilizatorul vrea sa se conecteze)
       // VALIDARE NUME UTILIZATOR
        
        if(strstr(buf, "login"))
        {  
            copie[0]='\0';
            
            
            strncat(numeUser, buf+6, lg-6);
            strcpy(copie, numeUser); 
            numeUser[strlen(numeUser)-1]='\0';
           
            copie[strlen(copie)-1]='\0';
            //printf("%s\n", numeUser);
          // printf("%s\n", copie);
           // printf("%d\n", strlen(numeUser)); 
           
           
             

            
            if(validare_nume(numeUser)==1)
                { 
                 conectat=1;
                 raspuns[0]='\0';
                 strcpy(raspuns, "Username-ul se gaseste in baza de date!");
                 
    
                }
          else
          {  
            strcpy(raspuns, "Nu ati introdus un username corect");
          }
               
                
        }
        
       else
       
        //-----------------------------------------------------------------------------------PASSWORD---------------------------------------------------------------------------------------------
        
       // VALIDARE PAROLA pentru utilizator 
       // daca comanda nu este login, password, help, rename, remove, ls etc. atunci am un sir de caractere care reprezinta parola 
       
       
      if(strstr(buf, "login")==NULL && strstr(buf, "password")==NULL && strstr(buf, "help")==NULL&&strstr(buf, "logout")==NULL&&strstr(buf, "rename_c")==NULL&&strstr(buf, "rename_s")==NULL
      &&strstr(buf, "remove_s")==NULL&&strstr(buf, "remove_c")==NULL&&strstr(buf, "ls_c")==NULL&&strstr(buf, "ls_s")==NULL&&strstr(buf, "afisare_c")==NULL&&strstr(buf, "afisare_s")==NULL
      &&strstr(buf, "director_curent")==NULL&&strstr(buf, "creareFisier_c")==NULL&&strstr(buf, "creareFisier_s")==NULL&&strstr(buf, "creareDir_c")==NULL&&strstr(buf, "creareDir_s")==NULL
      &&strstr(buf, "lungime_c")==NULL&&strstr(buf, "lungime_s")==NULL&&strstr(buf, "copiere_cs")==NULL&&strstr(buf, "copiere_sc")==NULL&&strstr(buf, "mutare_cs")==NULL&&strstr(buf,  
         "mutare_sc")==NULL)
      if(conectat==1)
      {
      
      //decriptare parola 
      
       for(unsigned i=0;i<strlen(buf);i++)
       buf[i]=buf[i]+5;
       
        
        if(validare_parola(buf))
       {
        strcpy(raspuns, "Parola introdusa este in regula. Sunteti conectat!");
        w=search_id(copie);
        logat[w]=1;
        status[w]=search_status(copie);
         
        
      }
        else 
        {strcpy(raspuns, "Verificati sintaxa si daca ati introdus in mod corect datele!");
        
         }
          
      
      }
      else if(conectat==0)
             {
             raspuns[0]='\0';
             strcpy(raspuns, "Inainte de a introduce parola trebuie sa introduceti username-ul!");
             printf("%d\n", strlen(raspuns));
             
             }
             
             
       //-----------------------------------------------------------------------------LOGOUT---------------------------------------------------------------------------------------------------
       
      //DECONECTARE UTILIZATOR 
      //in vectorul logat o sa pun 0 id ului corespunzator lui 
      
      if(strstr(buf, "logout:")&&logat[w]==1)
      {   
          strncat(numeUser, buf+7, lg-7);
          printf("%s\n", numeUser);
          numeUser[strlen(numeUser)-1]='\0';
          w=search_id(numeUser);
          logat[w]=0;
          
        
         strcpy(raspuns, "Tocmai v-ati deconectat de la contul dumneavoastra!");
      }
      else 
      if(strstr(buf, "logout")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa va puteti deconecta!");
        
        
        for(unsigned i=1;i<=5;i++)
        printf("%d ", logat[i]);
        printf("\n");
        
        
         for(unsigned i=1;i<=5;i++)
        printf("%d ", status[i]);
        printf("\n");
        
         //----------------------------------------------------------------------------RENAME----------------------------------------------------------------------------------------------------
         
      // DACA COMANDA ESTE RENAME trebuie sa redenumesc un fisier cu ajutorul functiei rename()
      //rename pentru fisierele din client
      
    if(strstr(buf, "rename_c:")&&logat[w]==1&&status[w]==1)
  {
  int i, poz=0;

  sir[0]='\0';
  strncat(sir, buf+9, strlen(buf)-9);
 
  for(i=0;i<strlen(sir);i++)
   if(sir[i]=='/')
      {poz=i; break;}
    
    numeFisierVechi[0]='\0';
    numeFisierNou[0]='\0';
    strcpy(numeFisierVechi, "./Client/");
    strcpy(numeFisierNou, "./Client/");
    
     sir[strlen(sir)-1]='\0';
    
    strncat(numeFisierVechi+9, sir, poz);
    strncat(numeFisierNou+9, sir+poz+1, strlen(sir)-poz-1);
    
     int result = rename(numeFisierVechi, numeFisierNou);
      if (result == 0) 
     {
      strcpy(raspuns, "Fisierul introdus a fost redenumit cu succes!!!");
     } 
   else
    {
      strcpy(raspuns, "Eroare de sintaxa. Fisierul introdus nu a putut fi redenumit! ");
    }
    
  }
   else 
      if(strstr(buf, "rename_c:")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa puteti efectua operatii cu fisiere!");
    else 
       if(strstr(buf, "rename_c")&&status[w]==0)
        strcpy(raspuns, "Nu aveti acces la aceste fisiere: status-->blacklist!");
        
 
  
    ///rename pentru fisierele din server
    
   if(strstr(buf, "rename_s:")&&logat[w]==1&&status[w]==1)
  {
  int i, poz=0;
  
  sir[0]='\0';
  strncat(sir, buf+9, strlen(buf)-9);
 
  for(i=0;i<strlen(sir);i++)
   if(sir[i]=='/')
      {poz=i; break;}
    
    numeFisierVechi[0]='\0';
    numeFisierNou[0]='\0';
    strcpy(numeFisierVechi, "./Server/");
    strcpy(numeFisierNou, "./Server/");
    
     sir[strlen(sir)-1]='\0';
    
    strncat(numeFisierVechi+9, sir, poz);
    strncat(numeFisierNou+9, sir+poz+1, strlen(sir)-poz-1);
    
     int result = rename(numeFisierVechi, numeFisierNou);
     if (result == 0) 
     {
      strcpy(raspuns, "Fisierul introdus a fost redenumit cu succes!!!");
     } 
   else
    {
      strcpy(raspuns, "Eroare de sintaxa. Fisierul introdus nu a putut fi redenumit! ");
    }
     
    
  }
   else 
      if(strstr(buf, "rename_s:")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa puteti efectua operatii cu fisiere!");
        else 
       if(strstr(buf, "rename_s")&&status[w]==0)
        strcpy(raspuns, "Nu aveti acces la aceste fisiere: status-->blacklist!");
        
   //---------------------------------------------------------------------------------REMOVE-----------------------------------------------------------------------------------------------
   
  //daca comanda este remove trebuie sa sterg un anumit fisier al carui nume il citesc imediat dupa comanda
  //remove pentru client
  
  
   if(strstr(buf, "remove_c:")&&logat[w]==1&&status[w]==1)
   {
     sir[0]='\0';
     strcpy(sir, "./Client/");
     strncat(sir+9, buf+9, strlen(buf)-9);
          sir[strlen(sir)-1]='\0';
          printf("%s\n", sir);
          printf("%d\n",strlen(sir));
          
    int  result = remove(sir);

   if (result == 0) 
     {
      strcpy(raspuns, "Fisierul introdus a fost eliminat cu succes!!!");
     } 
   else
    {
      strcpy(raspuns, "Eroare de sintaxa. Fisierul introdus nu a putut fi eliminat! ");
    }
    }
    else 
      if(strstr(buf, "remove_c:")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa puteti efectua operatii cu fisiere!");
     else 
       if(strstr(buf, "remove_c")&&status[w]==0)
        strcpy(raspuns, "Nu aveti acces la aceste fisiere: status-->blacklist!");
        
    
    //remove pentru server
     
    if(strstr(buf, "remove_s:")&&logat[w]==1&&status[w]==1)
   {
     sir[0]='\0';
     strcpy(sir, "./Server/");
     strncat(sir+9, buf+9, strlen(buf)-9);
          sir[strlen(sir)-1]='\0';
          printf("%s\n", sir);
          printf("%d\n",strlen(sir));
          
    int  result = remove(sir);

   if (result == 0) 
     {
      strcpy(raspuns, "Fisierul introdus a fost eliminat cu succes!!!");
     } 
   else
    {
      strcpy(raspuns, "Eroare de sintaxa. Fisierul introdus nu a putut fi eliminat! ");
    }
     
       
   }
    else 
      if(strstr(buf, "remove_s:")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa puteti efectua operatii cu fisiere!");
        
         else 
       if(strstr(buf, "remove_s")&&status[w]==0)
        strcpy(raspuns, "Nu aveti acces la aceste fisiere: status-->blacklist!");
        
        
   
    //------------------------------------------------------------------------------------LS--------------------------------------------------------------------------------------------
    
   //daca comanda este ls atunci trebuie sa afisam toate fisierele din folderul al carui nume il citesc dupa comanda
   //ls pentru client
   
  if(strstr(buf, "ls_c")&&logat[w]==1&&status[w]==1)
  {
   
  DIR *folder;
  struct dirent *directory;

  folder = opendir("./Client/"); // deschide folderul
  if (folder == NULL) {
      strcpy(Raspuns, "Eroare la afisarea fisierelor!");
    return 1;
  }
  
  
  
  strcpy(Raspuns, "Fisierele din Client sunt: ");

  while ((directory = readdir(folder)) != NULL) 
  {
   
 if(strcmp(directory->d_name, ".")!=0&&strcmp(directory->d_name,"..")!=0)
   {strcat(Raspuns, directory->d_name);
   strcat(Raspuns, " ");}
   
  }

  closedir(folder); // inchide folderul
  
  strcpy(raspuns, Raspuns);
  
  }
  else 
      if(strstr(buf, "ls_c:")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa puteti efectua operatii cu fisiere!");
    else 
       if(strstr(buf, "ls_c")&&status[w]==0)
        strcpy(raspuns, "Nu aveti acces la aceste fisiere: status-->blacklist!");
        
  
  
  //ls pentru server
  
   if(strstr(buf, "ls_s")&&logat[w]==1&&status[w]==1)
  {
   
  DIR *Folder;
  struct dirent *Directory;

  Folder = opendir("./Server/"); // deschide folderul
  if (Folder == NULL) {
    strcpy(Raspuns, "Eroare la afisarea fisierelor!");
    return 1;
  }
  
  
  strcpy(Raspuns, "Fisierele din Server sunt: ");

  while ((Directory = readdir(Folder)) != NULL)
  { 
 
 if(strcmp(Directory->d_name, ".")!=0&&strcmp(Directory->d_name,"..")!=0)
   {strcat(Raspuns, Directory->d_name);
   strcat(Raspuns, " ");}
   
  }

  closedir(Folder); // inchide folderul
  
  strcpy(raspuns, Raspuns);
  
  }
  else 
      if(strstr(buf, "ls_s:")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa puteti efectua operatii cu fisiere!");
        
    else 
       if(strstr(buf, "ls_s")&&status[w]==0)
        strcpy(raspuns, "Nu aveti acces la aceste fisiere: status-->blacklist!");
        
        
   //-------------------------------------------------------------------------------------AFISARE-------------------------------------------------------------------------------------------
  //daca comanda este afisare atunci trebuie sa afisez continutul unui anumit fisier
  //afisare FISIER DIN client 
  
  if(strstr(buf, "afisare_c:")&&logat[w]==1&&status[w]==1)
  {
  
  
          strcpy(raspuns, "A fost afisat continutul fisierului introdus de dumneavoastra");
  
  
  }
  else 
      if(strstr(buf, "afisare_c:")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa puteti efectua operatii cu fisiere!");
    else 
       if(strstr(buf, "afisare_c")&&status[w]==0)
        strcpy(raspuns, "Nu aveti acces la aceste fisiere: status-->blacklist!");
        
 
 //afisare FISIER DIN server
  
   if(strstr(buf, "afisare_s:")&&logat[w]==1&&status[w]==1)
  {
  
        strcpy(raspuns, "A fost afisat continutul fisierului introdus de dumneavoastra");
  }
   else 
      if(strstr(buf, "afisare_s:")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa puteti efectua operatii cu fisiere!");
   else 
       if(strstr(buf, "afisare_s")&&status[w]==0)
        strcpy(raspuns, "Nu aveti acces la aceste fisiere: status-->blacklist!");
        
     //----------------------------------------------------------------------------DIRECTOR CURENT----------------------------------------------------------------------------------------------- 
        
    if(strstr(buf, "director_curent")&&logat[w]==1&&status[w]==1)
    {
    
    char director[256];
    director[0]='\0';
  if (getcwd(director, sizeof(director)) != NULL)
   {
    strcat(raspuns, "Directorul in care se afla fisierele dumneavoastra este: ");
    strcat(raspuns, director);
   }
  
  else 
  {
  strcat(raspuns, "Eroare");
  }
   
  }
  
   //-----------------------------------------------------------------------------------CREARE FISIER---------------------------------------------------------------------------------------------
 if(strstr(buf, "creareFisier_c:")&&logat[w]==1&&status[w]==1)
 {
  
   FILE *fisierNou;
   
     sir[0]='\0';
     strcpy(sir, "./Client/");
     strncat(sir+9, buf+15, strlen(buf)-15);
          sir[strlen(sir)-1]='\0';
          printf("%s\n", sir);
          printf("%d\n",strlen(sir));
          
  
  fisierNou = fopen(sir, "w");
  if (fisierNou == NULL) {
    strcat(raspuns, "Nu s-a putut crea fisierul in folderul Client");
  }

 strcat(raspuns, "Fisierul a fost creat cu succes in folderul Client");

  fclose(fisierNou);
  
 }
  else 
      if(strstr(buf, "creareFisier_c:")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa puteti efectua operatii cu fisiere!");
   else 
       if(strstr(buf, "creareFisier_c")&&status[w]==0)
        strcpy(raspuns, "Nu aveti acces la aceste fisiere: status-->blacklist!");
        
    
  if(strstr(buf, "creareFisier_s:")&&logat[w]==1&&status[w]==1)
 {
  
   FILE *fisierNou;
   
     sir[0]='\0';
     strcpy(sir, "./Server/");
     strncat(sir+9, buf+15, strlen(buf)-15);
          sir[strlen(sir)-1]='\0';
          printf("%s\n", sir);
          printf("%d\n",strlen(sir));
          
  
  fisierNou = fopen(sir, "w");
  if (fisierNou == NULL) {
    strcat(raspuns, "Nu s-a putut crea fisierul in folderul Server");
  }

 strcat(raspuns, "Fisierul a fost creat cu succes in folderul Server");

  fclose(fisierNou);
  
 }
  else 
      if(strstr(buf, "creareFisier_s:")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa puteti efectua operatii cu fisiere!");
   else 
       if(strstr(buf, "creareFisier_s")&&status[w]==0)
        strcpy(raspuns, "Nu aveti acces la aceste fisiere: status-->blacklist!");
        
    
   //------------------------------------------------------------------------------------CREARE DIRECTOR--------------------------------------------------------------------------------------------
   
  if(strstr(buf, "creareDir_c:")&&logat[w]==1&&status[w]==1)
 {
  
   
   
     sir[0]='\0';
     strcpy(sir, "./Client/");
     strncat(sir+9, buf+12, strlen(buf)-12);
          sir[strlen(sir)-1]='\0';
          printf("%s\n", sir);
          printf("%d\n",strlen(sir));
          
  
     int directory = mkdir(sir, 0777);

  if (directory!= 0) {
    strcat(raspuns, "Nu s-a putut crea folderul introdus in folderul Client");
  }

 strcat(raspuns, "Folderul introdus a fost creat cu succes in folderul Client");

 
  
 }
  else 
      if(strstr(buf, "creareDir_s:")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa puteti efectua operatii cu fisiere!");
   else 
       if(strstr(buf, "creareDir_c")&&status[w]==0)
        strcpy(raspuns, "Nu aveti acces la aceste fisiere: status-->blacklist!");
        
 if(strstr(buf, "creareDir_s:")&&logat[w]==1&&status[w]==1)
 {
  
   
   
     sir[0]='\0';
     strcpy(sir, "./Server/");
     strncat(sir+9, buf+12, strlen(buf)-12);
          sir[strlen(sir)-1]='\0';
          printf("%s\n", sir);
          printf("%d\n",strlen(sir));
          
  
     int directory = mkdir(sir, 0777);

  if (directory!= 0) {
    strcat(raspuns, "Nu s-a putut crea folderul introdus in folderul Server");
  }

 strcat(raspuns, "Folderul introdus a fost creat cu succes in folderul Server");

 
  
 }
  else 
      if(strstr(buf, "creareDir_s:")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa puteti efectua operatii cu fisiere!");
   else 
       if(strstr(buf, "creareDir_s")&&status[w]==0)
        strcpy(raspuns, "Nu aveti acces la aceste fisiere: status-->blacklist!");
        
        
        
        
        
        //----------------------------------------------------------------------------LUNGIME FISIER-------------------------------------------------------------------------------------------------
        
        
         if(strstr(buf, "lungime_c:")&&logat[w]==1&&status[w]==1)
       {
        
         sir[0]='\0';
         strcpy(sir, "./Client/");
         strncat(sir+9, buf+10, strlen(buf)-10);
          sir[strlen(sir)-1]='\0';
          printf("%s\n", sir);
          printf("%d\n",strlen(sir));
          
          
    
    struct stat ceva;
    int result = stat(sir, &ceva);

    if (result == 0) {

       int size = ceva.st_size;
        

  char lungime[256];
   
  sprintf(lungime, "%d", size);
  
  printf("%s\n", lungime);

  
        strcpy(raspuns, "Dimensiunea fisierului este: ");
        strcat(raspuns, lungime);
        strcat(raspuns, " ");
        strcat(raspuns, "octeti");
        printf("Dimensiunea fișierului %s este %ld octeți.\n", sir, size);
    }

       }
       else 
      if(strstr(buf, "lungime_c")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa puteti efectua operatii cu fisiere!");
   else 
       if(strstr(buf, "lungime_c")&&status[w]==0)
        strcpy(raspuns, "Nu aveti acces la aceste fisiere: status-->blacklist!");
        
        
        
     if(strstr(buf, "lungime_s:")&&logat[w]==1&&status[w]==1)
       {
        
         sir[0]='\0';
         strcpy(sir, "./Server/");
         strncat(sir+9, buf+10, strlen(buf)-10);
          sir[strlen(sir)-1]='\0';
          printf("%s\n", sir);
          printf("%d\n",strlen(sir));
          
          
    
    struct stat ceva;
    int result = stat(sir, &ceva);

    if (result == 0) {

       int size = ceva.st_size;
        

  char lungime[256];
   
  sprintf(lungime, "%d", size);
  
  printf("%s\n", lungime);

  
        strcpy(raspuns, "Dimensiunea fisierului este: ");
        strcat(raspuns, lungime);
        strcat(raspuns, " ");
        strcat(raspuns, "octeti");
        printf("Dimensiunea fișierului %s este %ld octeți.\n", sir, size);
    }

       }
       else 
      if(strstr(buf, "lungime_s")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa puteti efectua operatii cu fisiere!");
   else 
       if(strstr(buf, "lungime_s")&&status[w]==0)
        strcpy(raspuns, "Nu aveti acces la aceste fisiere: status-->blacklist!");
        
  
       //---------------------------------------------------------------------------COPIERE-----------------------------------------------------------------------------------------------------
       
       if(strstr(buf, "copiere_cs:")&&logat[w]==1&&status[w]==1)
      {
      
      
          
          char sursa[256], destinatie[256];
          
          sursa[0]='\0';
          destinatie[0]='\0';
          
          strcpy(sursa, "./Client/");
          strcpy(destinatie, "./Server/");
          strncat(sursa+9, buf+11, strlen(buf)-11);
          strncat(destinatie+9, buf+11, strlen(buf)-11);
          sursa[strlen(sursa)-1]='\0';
          destinatie[strlen(destinatie)-1]='\0';
          
          FILE *fisier1 = fopen(sursa, "r");
          FILE *fisier2 = fopen(destinatie, "w");
 
         char *linie = malloc(dimensiune);
  
         size_t size;
         while ((size = fread(linie, 1, dimensiune, fisier1)) > 0) {
         fwrite(linie, 1, size, fisier2);
         
         
         }
         strcat(raspuns, "Fisierul a fost copiat cu succes!!");


  free(linie);
  fclose(fisier1);
  fclose(fisier2);
      
      
      }
       else 
      if(strstr(buf, "copiere_cs")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa puteti efectua operatii cu fisiere!");
   else 
       if(strstr(buf, "copiere_cs")&&status[w]==0)
        strcpy(raspuns, "Nu aveti acces la aceste fisiere: status-->blacklist!");
        
      
      
    
          if(strstr(buf, "copiere_sc:")&&logat[w]==1&&status[w]==1)
      {
      
      
          
          char sursa[256], destinatie[256];
          
          sursa[0]='\0';
          destinatie[0]='\0';
          
          strcpy(sursa, "./Server/");
          strcpy(destinatie, "./Client/");
          strncat(sursa+9, buf+11, strlen(buf)-11);
          strncat(destinatie+9, buf+11, strlen(buf)-11);
          sursa[strlen(sursa)-1]='\0';
          destinatie[strlen(destinatie)-1]='\0';
          
          FILE *fisier1 = fopen(sursa, "r");
          FILE *fisier2 = fopen(destinatie, "w");
 
         char *linie = malloc(dimensiune);
  
        size_t size;
        while ((size = fread(linie, 1, dimensiune, fisier1)) > 0) {
        fwrite(linie, 1, size, fisier2);
           
         }
         strcat(raspuns, "Fisierul a fost copiat cu succes!!");


  free(linie);
  fclose(fisier1);
  fclose(fisier2);
      
      
      }
       else 
      if(strstr(buf, "copiere_sc")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa puteti efectua operatii cu fisiere!");
   else 
       if(strstr(buf, "copiere_sc")&&status[w]==0)
        strcpy(raspuns, "Nu aveti acces la aceste fisiere: status-->blacklist!");
        
        
       //------------------------------------------------------------------------------------MUTARE--------------------------------------------------------------------------------------------
       
      if(strstr(buf, "mutare_cs")&&logat[w]==1&&status[w]==1)
      {
         
          char sursa[256], destinatie[256];
          
          sursa[0]='\0';
          destinatie[0]='\0';
          
          strcpy(sursa, "./Client/");
          strcpy(destinatie, "./Server/");
          strncat(sursa+9, buf+10, strlen(buf)-10);
          strncat(destinatie+9, buf+10, strlen(buf)-10);
          sursa[strlen(sursa)-1]='\0';
          destinatie[strlen(destinatie)-1]='\0';
          
          
          
          FILE *fisier1 = fopen(sursa, "r");
          FILE *fisier2 = fopen(destinatie, "w");
 
         char *linie = malloc(dimensiune);
  
         size_t size;
         while ((size = fread(linie, 1, dimensiune, fisier1)) > 0) {
         fwrite(linie, 1, size, fisier2);
         
         
         }
         
                
    int  result = remove(sursa);
    if (result == 0) 
     {
      strcpy(raspuns, "Fisierul introdus a fost mutat cu succes!!!");
     } 
   else
    {
      strcpy(raspuns, "Eroare de sintaxa. Fisierul introdus nu a putut fi mutat! ");
    }

  free(linie);
  fclose(fisier1);
  fclose(fisier2);
      }
        else 
      if(strstr(buf, "mutare_cs")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa puteti efectua operatii cu fisiere!");
   else 
       if(strstr(buf, "mutare_cs")&&status[w]==0)
        strcpy(raspuns, "Nu aveti acces la aceste fisiere: status-->blacklist!");
        
        
        
        
         
      if(strstr(buf, "mutare_sc")&&logat[w]==1&&status[w]==1)
      {
         
          char sursa[256], destinatie[256];
          
          sursa[0]='\0';
          destinatie[0]='\0';
          
          strcpy(sursa, "./Server/");
          strcpy(destinatie, "./Client/");
          strncat(sursa+9, buf+10, strlen(buf)-10);
          strncat(destinatie+9, buf+10, strlen(buf)-10);
          sursa[strlen(sursa)-1]='\0';
          destinatie[strlen(destinatie)-1]='\0';
          
          
          
          FILE *fisier1 = fopen(sursa, "r");
          FILE *fisier2 = fopen(destinatie, "w");
 
         char *linie = malloc(dimensiune);
  
         size_t size;
         while ((size = fread(linie, 1, dimensiune, fisier1)) > 0) {
         fwrite(linie, 1, size, fisier2);
         
         
         }
         
                
    int  result = remove(sursa);
    if (result == 0) 
     {
      strcpy(raspuns, "Fisierul introdus a fost mutat cu succes!!!");
     } 
   else
    {
      strcpy(raspuns, "Eroare de sintaxa. Fisierul introdus nu a putut fi mutat! ");
    }

  free(linie);
  fclose(fisier1);
  fclose(fisier2);
      }
        else 
      if(strstr(buf, "mutare_sc")&&logat[w]==0)
        strcpy(raspuns, "Nu sunteti conectat la sistem ca sa puteti efectua operatii cu fisiere!");
   else 
       if(strstr(buf, "mutare_sc")&&status[w]==0)
        strcpy(raspuns, "Nu aveti acces la aceste fisiere: status-->blacklist!");
        
        
       else 
       if(strstr(buf, "help"))
       {
       strcat(raspuns, "A fost afisata sintaxa comenzii help");
       }
      else
      
      
  
              raspuns[strlen(raspuns)]='\0';
         
             printf("\n");
	printf("[Thread %d]Trimitem comanda inapoi...%s\n",tdL.idThread,  raspuns);
		      
		      /* returnam mesajul clientului */
		      
	 if (write (tdL.cl, raspuns, strlen(raspuns)) <= 0)
		{
		 printf("[Thread %d] ",tdL.idThread);
		 perror ("[Thread]Eroare la write() catre client.\n");
		}
	else    {printf("\n");
		printf ("[Thread %d]Comanda a fost transmisa cu succes.\n",tdL.idThread);}
		
		 bzero(buf, 255);
	         bzero(numeUser, 255);
	         bzero(Parola, 255);
	         bzero(numeFisierVechi,256);
	         bzero(numeFisierNou,256);
	         bzero(sir, 256);
	         bzero(Raspuns, 256);
	         bzero(raspuns, 256);
	         
	         	
		}
	
		
}
