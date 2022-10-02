

#include "ext.h"
#include <process.h>
#define TRI 1
#define DEV 1


int CALLWAD = 0;
int NOPROMPT = 0;
int GAMECOMPILE = 1;

extern int StratConv(char *strat);
extern void Initialise();


char *temp[64];

void Clearm(char *input,int amount)
{
	// innocent whistling of MattG here...
//	memset (input, 0, amount);
 	
 	int i;
	for (i=0;i<amount;i++)
		input[i]='\0';
	
}


int main(int argc, char *argv[])
{
//#if ALLRESIDENT
	FILE			*DB;
	char		*arg;
	char		DBFile[MAX_LINE];
	char		NewArg[MAX_LINE];
	int			NumMaps;
	int			size,loop,ERROR=0;
//#endif

	WADRERUN = 0;
	FullRec = 0;

	Initialise();

//#if GODMODEMACHINE
	//if not the final 'testmap' let's clean up, to ensure a build
 //	if ((strnicmp(argv[2],"GODMODEFINAL",12)))
 //		system("command /e:8192 /cP:\\game\\artstrat\\GODclean.bat");
//#endif

	#if DEV
	//	printf("argv1 %s argv2 %s \n",argv[1],argv[2]);


		if (!argv[1])
			printf("MUST SPECIFY A FILENAME\n");
		else
		{
			suppname = argv[1];

			if (argv[2])
			{
				if (!(strnicmp(argv[2],"SINGLE",6)))
				{
					SINGLEMAP = 1;
					GAMECOMPILE = 1;
				}
				else
				{
					if (!(strnicmp(argv[2],"GODMODEFINAL",12)))
					{
						NOPROMPT = 1;
						GAMECOMPILE = 1;
					}
					else
					{
						if (!(strnicmp(argv[2],"GODMODE",6)))
						{
							NOPROMPT = 1;
							GAMECOMPILE = 0;
							
						}
					}

				}

			}
				//IF THIRD ARGUMENT != MAX
			if (argv[3])
			{
				if ((strnicmp(argv[3],"MAX",3)))
	 				CALLWAD = 1;
			}
			else
				CALLWAD = 1;

			ReadLogErrors();

			//read in the generic sound base
		  	ReadGenericSounds();
			
			
			strcpy(CurrentLevel,argv[1] + 1);
			if (StratConv(argv[1]))
			{		

				if (!SINGLEMAP)
				{
					NotSpecifiedMap = 1;
//				#if ALLRESIDENT
					arg = argv[1] + 1;
					#if ARTIST
						sprintf(DBFile,"%s%s%s",INTDIR,"GAMEMAPS",".MDB");	
					#else
						sprintf(DBFile,"%s%s%s",STRATDIR,"GAMEMAPS",".MDB");	
					#endif
					DB=fopen(DBFile, "rb");
					fread(&NumMaps,sizeof(int),1,DB);
					//printf("num maps in dbase %d\n",NumMaps);
	
					for (loop=0;loop<NumMaps;loop++)
					{
					

						fread(&size,sizeof(int),1,DB);
						temp[loop] = (char*)malloc(size + 2);
						fread(temp[loop],sizeof(char),size,DB);
						printf("%s\n",temp[loop]);
					}
					fclose(DB);
		  
						
						//CHECK TO ENSURE WE DON'T REDO THE INPUT MAP
					for (loop=0;loop<NumMaps;loop++)
					{
						Clearm(CurrentLevel,128);
						strncpy(CurrentLevel,temp[loop],strlen(temp[loop]));
					 //	CurrentLevel = temp[loop];
						if (strncmp(CurrentLevel,arg,strlen(arg)))
						{

							//WE ONLY WISH TO BUILD MODEL AND ANIM LISTS RELEVANT TO THE CURRENTLY
							//SPECIFIED OUTPUT LEVEL, SO THIS IS THE BIT THAT FACILITATES IT

						   	//fridays addition
							//NumMods = 0;
							//NumAnims = 0;
							//DefinedStrats = 0;
							ResetVars();


							sprintf(NewArg,"@%s",CurrentLevel);
							StratConv(NewArg);
							fcloseall();


						}
					}
				   //	fclose (DB);
				}
				//fridays addition
				CleanMe();	

//			#endif
			}
			else
			{
				//ENSURE THE PROMPT CHECK IS BACK ON
				NOPROMPT = 0;
				ERROR++;
				printf("PROBLEM WITH INPUT SCRIPT %s\n",argv[1]);											
			}
		}
	#endif
//wait:
	fcloseall();

    
	if (!ERROR)
	{
		if (!((!FullRec) && (!TIMESTAMPFAIL)))
			printf("\n *** GAME REBUILD NEEDED ***\n");
	}
   
	if (WADRERUN)
		printf("\n *** NEED TO REWAD LEVELS ***\n");


  

	if ((ERROR) || (!NOPROMPT))
	{
		printf("Press key to continue\n");
		for (;;)
		{
			while (!_kbhit());
				break;
		}
	}
	#if SH4
	   	if (!ERROR)
	   	{
			if (!((!FullRec) && (!TIMESTAMPFAIL)))
				#if ARTIST
				//	system("command /e:8192 /cP:\\UTILS\\ARTCOMP.bat");
					system("P:\\UTILS\\ARTCOMP.bat");
				#else
					if (GAMECOMPILE)
#if GODMODEMACHINE
   						system("command /e:8192 /cP:\\game\\artstrat\\DOGcompdog.bat");
#else
		   //	   	_spawnl (_P_WAIT, "p:\\GAME\\ARTSTRAT\\COMPDOG.BAT","p:\\GAME\\ARTSTRAT\\COMPDOG.BAT", NULL, NULL);
				ERROR = system("command /e:8192 /cP:\\game\\artstrat\\compdog.bat");
  // ERROR = 	_spawnl (_P_WAIT, "c:\\windows\\COMMAND", "/e:8192 /cP:\\GAME\\ARTSTRAT\\COMPDOG.BAT",NULL);

#endif
					if ((WADRERUN) && (CALLWAD))
					{
						//_spawnl (_P_WAIT, "c:\\DREAMCAST\\REDDOG\\WADDER\\RELEASE\\wadder.exe",NULL, NULL);
						printf("\nNOW RUN WADDER AND GDWORKSHOP\n");
  
		
					 //	for (;;)
					 //	{
					 //		while (!_kbhit());
					 //		break;
					 //	}

						
					}
					fcloseall();
					if (!NOPROMPT)
					{
						printf("COMPILATION OVER - Press key to continue\n");
							
					for (;;)
	   
					 {
					 	while (!_kbhit());
					 	break;
					 }
					} 

				#endif
				
		
		}
	#endif
   /*

	if (ERROR)
	{
	  for (;;)
	  { 
		while (!_kbhit());
			break;
	  }


	}
	*/
	printf("DONE \n");

	return(ERROR);
}