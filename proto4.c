#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

typedef struct d
{
	int mon;
	int day;
}dat;

typedef struct s
{
	char usern[100];
	char pass[100];
	char car[100];
	char name[100];
	char email[100];
	float balance;
	float trans[200];
	dat  date[200];
	int n;
	
}userp;

void loginmenu(userp u);
void admloginmenu();

userp updatebalance(userp u, float am)
{
	FILE* fp1=fopen("userdatabase.dat","rb+");
	FILE* fp2=fopen("replica.dat","ab+");
	u.balance=am+u.balance;
	userp temp;
	fseek(fp1,0,SEEK_SET);
    while (fread(&temp,sizeof(userp),1,fp1)==1)
    {
    	if(strcmp(u.usern,temp.usern)==0)
    	{
    		fwrite(&u,sizeof(userp),1,fp2);
    	}
    	else
    	{
    		fwrite(&temp,sizeof(userp),1,fp2);
    	}
    }
	fclose(fp1);
	fclose(fp2);
	remove("userdatabase.dat");
	rename("replica.dat","userdatabase.dat");
	return u;

}


void creditmoney(userp u)
{
	float am;
	printf("What amount to credit: ");
	scanf("%f",&am);
	char c;
	while((c = getchar()) != '\n' && c != EOF);
	FILE* tr=fopen("Transactions.txt","a");
	time_t rawtime;
   	struct tm *info;
   	char buffer[80];
   	time( &rawtime );
   	info = localtime( &rawtime );
	u.trans[u.n]=am;
	u.date[u.n].mon=info->tm_mon+1;
	u.date[u.n].day=info->tm_mday;
	u.n=u.n+1;
	int m=info->tm_mon+1;
	fprintf(tr, "%d %d %s %f\n", info->tm_mday, m, u.usern,am );
	fclose(tr);
	u=updatebalance(u,am);
	loginmenu(u);

}


void checkbalance(userp u)
{
	printf("Your Current balance is: %f\n", u.balance);
	loginmenu(u);
}


void checktransactions(userp u)
{
	printf("Your Transactions:\n");
	int ntrans=u.n;
	int i;
	if(ntrans!=0)
	{
		for (i = 0; i < ntrans; ++i)
		{
			if(u.trans[i]!=0)
			printf("%d-%d %f\n",u.date[i].day,u.date[i].mon,u.trans[i]);
		}
	}
	loginmenu(u);

}

void loginmenu(userp u)
{
	printf("Welcome to your profile, Choose what to do:\n1-Credit money\n2-Check balance\n3-Check Transactions\n4-Logout \n");
	char choice;
	scanf("%c",&choice);
	char c;
	while((c = getchar()) != '\n' && c != EOF);
	if(choice=='1')
		creditmoney(u);
	else if(choice=='2')
		checkbalance(u);
	else if(choice=='3')
		checktransactions(u);
	else if(choice=='4')
		return;
	else
		{
			printf("INVALID CHOICE\n");
			loginmenu(u);
		}

}



void login()
{
	userp u;
	userp temp;
	char id[100];
	char password[100];
	char car[100];
	char pass[100];
	char name[100];
	printf("Login: ");
	scanf("%s",name);
	char c;
	while((c = getchar()) != '\n' && c != EOF);
	FILE *fp;
	fp=fopen("userdatabase.dat", "ab+");
	fp=fopen("userdatabase.dat", "rb+");
	int flag=0;
	do
	{

		fread(&temp,sizeof(userp),1,fp);
		if(strcmp(name,temp.usern)==0)
			{
				char* t=(char *)getpass("Password :");
				strcpy(pass,t);
				//scanf("%s",pass);
				flag=1;
				if(strcmp(temp.pass,pass)==0)
				{
					printf("Login Succeesfull\n");
					loginmenu(temp);
					fclose(fp);
					return;
				}
				else
					printf("Incorrect Password\n");	
				break;
			}

	}while(!feof(fp));
	if(flag==0)
		printf("Not registered\n");
	fclose(fp);
}

void reguser()
{
	userp u;
	FILE *fp;
	printf("Enter user name: ");
	scanf("%s", u.usern);
	printf("Enter car ID: ");
	scanf("%s", u.car);
	fp=fopen("userdatabase.dat", "ab+");
	fp=fopen("userdatabase.dat", "rb+");
	int flag=0;
	userp temp;
	do
	{
		fread(&temp,sizeof(userp),1,fp);
		if(strcmp(u.usern,temp.usern)==0)
		{
				flag=1;
		}
		if(strcmp(u.car, temp.car)==0)
		{
			flag=2;
		}

	}while(!feof(fp));
	if(flag==1)
	{
		printf("USERNAME ALREADY PRESENT, TRY AGAIN\n");
		fclose(fp);
		reguser();
	}
	else if(flag==2)
	{
		printf("CAR ID ALREADY REGISTERED\n");
	}
	else
	{
		fp=fopen("userdatabase.dat", "ab");
		printf("Enter user Password: ");
		scanf("%s" , u.pass);
		char c;
		while((c = getchar()) != '\n' && c != EOF);
		printf("Enter Name: ");
		fgets(u.name,100,stdin);
		printf("Enter email: ");
		scanf("%s", u.email);
		while((c = getchar()) != '\n' && c != EOF);
		u.balance=0;
		u.n=0;
		int i;
		for ( i = 0; i < 200; ++i)
		{
			u.trans[i]=0;
		}
		fwrite(&u, sizeof(userp), 1, fp);
		printf("Registered Succeesfully\n");
		
	}
	fclose(fp);


}

void regadmin()
{
	char user[100];
	char pass[100];
	char name[100];
	char pass1[100];
	char email[100];
	FILE *fp;
	printf("Enter user name: ");
	scanf("%s", user);
	fp=fopen("admindatabase.txt", "a");
	fp=fopen("admindatabase.txt", "r");
	int flag=0;
	do
	{
		fscanf(fp,"%s %s",name, pass1);
		if(strcmp(name,user)==0)
			{
				flag=1;
			}

	}while(!feof(fp));
	if(flag==1)
	{
		printf("USERNAME ALREADY PRESENT, TRY AGAIN\n");
		fclose(fp);
		regadmin();
	}
	else
	{
		fp=fopen("admindatabase.txt", "a");
		printf("Enter user Password: ");
		scanf("%s" , pass);
		printf("Enter email id: ");
		scanf("%s" , email);
		fprintf(fp, "%s %s %s\n", user , pass, email);
		printf("Succeesfully registered\n"); 
	}
	fclose(fp);


}

void daywise()
{
	int mday, month;
	char name[100];
	float tr;
	int md,m;
	FILE* fp=fopen("Transactions.txt","a");
	fp=fopen("Transactions.txt","r");
	printf("Enter the date for which the Transactions are to be fetched(Day of month then month number)\n");
	scanf("%d%d",&md,&m);
	char c;
	while((c = getchar()) != '\n' && c != EOF);
	while(!feof(fp))
	{
		fscanf(fp,"%d %d %s %f\n",&mday,&month,name,&tr);
		if(mday==md && month==m)
		{
			printf("%d-%d %s %f\n",mday,m,name,tr);
		}
	}
	fclose(fp);
	admloginmenu();
}

void alltrans()
{
	char line[1000];
	FILE* fp=fopen("Transactions.txt","a");
	fp=fopen("Transactions.txt","r");
	printf("All Transactions:\n");
	while(fgets(line,1000,fp))
	{
		printf("%s",line );
	}
	fclose(fp);
	admloginmenu();

}

void admloginmenu()
{
	printf("Welcome Administrator, Choose what to do:\n1-View Total Income\n2-View Day-wise Transactions\n3-View all Transactions\n4-Logout\n");
	char choice;
	scanf("%c",&choice);
	char c;
	while((c = getchar()) != '\n' && c != EOF);
	if(choice=='1')
	{
		float inc;
		FILE* fp=fopen("Totalincome.txt","a");
		fp=fopen("Totalincome.txt","r");
		if(fscanf(fp,"%f",&inc)==1)
			printf("Total Income: %f\n", inc);
		else
			printf("Total Income: 0\n");
		admloginmenu();
	}
	else if(choice=='2')
		daywise();
	else if(choice=='3')
		alltrans();
	else if(choice=='4')
		return;
	else
		{
			printf("INVALID CHOICE\n");
			admloginmenu();
		}
}


void admlogin()
{
	char id[100];
	char password[100];
	char pass[100];
	char name[100];
	char email[100];
	printf("Login: ");
	scanf("%s",name);
	char c;
	while((c = getchar()) != '\n' && c != EOF);
	FILE *fp;
	fp=fopen("admindatabase.txt", "a");
	fp=fopen("admindatabase.txt", "r");
	int flag=0;
	do
	{
		fscanf(fp,"%s %s %s",id,password,email);
		if(strcmp(name,id)==0)
			{
				char* t=(char *)getpass("Password :");
				strcpy(pass,t);
				flag=1;
				if(strcmp(password,pass)==0)
				{
					printf("Login Succeesfull\n");
					fclose(fp);
					admloginmenu();
					return;
				}
				else
					printf("Incorrect Password\n");	
				break;

			}

	}while(!feof(fp));
	if(flag==0)
		printf("Not registered\n");
	fclose(fp);
}


void systemfun()
{
	char id[100];      //for reading car id from the user
 	userp u;      //for reading structure from file
	printf("Enter the car id: ");
	scanf("%s",id);
	char c;
	while((c = getchar()) != '\n' && c != EOF);   //flushing the input stream
	FILE* fp=fopen("userdatabase.dat","ab");
	fp=fopen("userdatabase.dat","rb");
	int flag=0;
	while(!feof(fp))
	{
		fread(&u,sizeof(userp),1,fp);
		if(strcmp(u.car,id)==0)
		{
			flag=1;
			break;
		}

	}
	if(flag==0)
		printf("CAR NOT REGISTERED\n");
	else
	{
		float toll=-30;
		if((u.balance+toll)>=0)
		{
			time_t rawtime;
   			struct tm *info;
   			time( &rawtime );
   			info = localtime( &rawtime );
			u.trans[u.n]=toll;
			u.date[u.n].mon=info->tm_mon+1;
			u.date[u.n].day=info->tm_mday;
			u.n=u.n+1;
			updatebalance(u, toll);
			FILE* tr=fopen("Transactions.txt","a");
			fprintf(tr, "%d %d %s %f\n", info->tm_mday, info->tm_mon+1,u.usern,toll);
			fclose(tr);
			FILE* tot;
			tot=fopen("Totalincome.txt","a");
			tot=fopen("Totalincome.txt","r");
			float currinc;
			if(fscanf(tot,"%f",&currinc)==1)
				currinc=currinc-toll;
			else
				currinc=-toll;
			tot=fopen("Totalincome.txt","w");
			fprintf(tot, "%f\n",currinc);
			fclose(tot);
		}
		else
		{
			printf("Not enough money in e-wallet. Manual payment necessary\n");
		}
	}
	fclose(fp);
}

int main()
{
	system("clear");
	while(1)
	{
		int n,flag=0;
		char yn;
		printf("****WELCOME TO THE MAIN MENU OF ONLINE TOLL PLAZA**** \nPlease choose an option: 1-Admin, 2-User, 3-System, 4-Exit Program\n");
		scanf("%d",&n);
		char c;
		switch(n)
		{

			case 1:printf("Do you have an account(Y/N):");
					while((c = getchar()) != '\n' && c != EOF);
					scanf("%c",&yn);
					if(yn=='Y')
						admlogin();
					else if(yn=='N')
						regadmin();
					else
					{
						printf("INVALID CHOICE\n");	
					}
					break;
			case 2: printf("Do you have an account(Y/N):");
					while((c = getchar()) != '\n' && c != EOF);
					scanf("%c",&yn);
					if(yn=='Y')
						login();
					else if(yn=='N')
						reguser();
					else
					{
					printf("INVALID CHOICE\n");
					}
					break;
			case 3: systemfun();
					break; 

			case 4: flag=1;
					break;

			default: printf("INVALID CHOICE\n");
				 	break;
		}
		if(flag==1)
			break;
	}		

	
}
