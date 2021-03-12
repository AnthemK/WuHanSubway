#include "Info_masking.h"


int Encryption_file(FILE *fromfile,FILE *tofile,int cipher)
{
	int tot=0,i,qwer; 
	char nowwa=0;
	while(~fscanf(fromfile,"%c",&nowwa))
	{
		//printf("%d     ",nowwa);
		tot++;
		nowwa^=(char )char_max;
		
		fprintf(tofile,"%c",nowwa);
		//printf("%d\n",nowwa);
	}
	//printf("\n %d\n",tot);
	return tot;
}


int Decryption_file(FILE *fromfile,FILE *tofile,int cipher)
{
	rewind(fromfile);
	int tot=0,qwer=0,i;
	char  maxxx[1000000];
	char nowwa=0;
	//fscanf(fromfile,"%s",maxxx);
	//printf("%d\n\n",strlen(maxxx));
	while(~fscanf(fromfile,"%c",&nowwa))
	{
	//	printf("%d     ",nowwa);
		tot++;
		nowwa^=(char )char_max;
		
		fprintf(tofile,"%c",nowwa);
		//printf("%d\n",nowwa);
	}

	//printf("\n %d\n",tot);
	return tot;
}

int Encryption_info()
{
	int qwq=0;
//	printf("«Î ‰»Î√‹¬Î£∫"); 
//	scanf("%d",&qwq);
//	if(qwq!=11657) return 0;
	FILE *a,*b,*c,*d; 
	#ifdef debug
	a = fopen("./data/testinfo.txt","r");b = fopen("./data/enc_testinfo.txt","w");
	Encryption_file(a,b,qwq);rewind(b);fclose(a);fclose(b);
	c = fopen("./data/enc_testinfo.txt","r");d = fopen("./data/dec_testinfo.txt","w");
	Decryption_file(c,d,qwq);rewind(d);	fclose(c);fclose(d);
	#endif
	
	
	
//	a = fopen("./data/stationinfo.txt","r");b = fopen("./data/enc_stationinfo.txt","w");
//	Encryption_file(a,b);rewind(b);fclose(a);fclose(b);
	c = fopen("./data/enc_stationinfo.txt","r");d = fopen("./data/dec_stationinfo.txt","w");
	Decryption_file(c,d,qwq);rewind(d);fclose(c);fclose(d);
//	a = fopen("./data/Coordinates.txt","r");b = fopen("./data/enc_Coordinates.txt","w");
//	Encryption_file(a,b);rewind(b);fclose(a);fclose(b);
	c = fopen("./data/enc_Coordinates.txt","r");d = fopen("./data/dec_Coordinates.txt","w");
	Decryption_file(c,d,qwq);rewind(d);fclose(c);fclose(d);
//	a = fopen("./data/subwayline.txt","r");b = fopen("./data/enc_subwayline.txt","w");
//	Encryption_file(a,b);rewind(b);fclose(a);fclose(b);
	c = fopen("./data/enc_subwayline.txt","r");d = fopen("./data/dec_subwayline.txt","w");
	Decryption_file(c,d,qwq);rewind(d);	fclose(c);fclose(d);
	//*/
}

