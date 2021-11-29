#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<locale.h>
#include<time.h>

typedef struct students{
	char stdID[10], stdName[15], stdSurname[15];
	int numOfLectures, numOfCredits; //numOfLectures: ��rencinin ald��� ders say�s�, numOfCredits: ��rencinin ald��� kredi say�s�
}STD;

typedef struct lectures{
	char lecCode[10], lecName[20], tchID[10];
	int credit, quota; //quota: kontenjan
}LEC;

typedef struct teachers{
	char tchID[10], tchName[10], tchSurname[10], position[10]; //position:unvan
}TCH;

void lectureUpdate(char *, char *, char *);
void teacherUpdate(char *, char *, char *);
void studentUpdate(char *, char *);
void stdLecRecord(int, int, int, int *, char *, char *, char *);
void stdLecDelete(char *, char *, char *);
void lecByTchID_list(char *, char *);
void stdByCode_list(char *, char *);
void lecByStdID_list(char *, char *);
void printClassroom(char *, char *, char *);

int mevcut(char *, char *, char *, char *);
void deleteMember(char *, char *, char *);
int countLine(char *);

void deleteLecture(char *, char *, char *);

int main(){
	setlocale(LC_ALL,"Turkish");
	
	int choice=-1, maxDers=9, maxKredi=25, maxKota=20, count=1;
	char fstdname[10], flecname[10], ftchname[10], date[12];
	//��renciDersKay�t dosyas�nda kal�nan yeri bulmal�y�z (s�ralamadaki numaraland�rma i�in)
	count = countLine("OgrenciDersKayit.txt");
	
	printf("\n\t--- Ders Kay�t Program�na Ho�geldiniz ---\n\n\n");
	printf("�ncelikle bir ��rencinin alabilece�i maksimum ders ve kredi say�s�n� giriniz:\n Maksimum ders say�s�:");
	scanf("%d", &maxDers);
	printf("\n Maksimum kredi say�s�: ");
	scanf("%d", &maxKredi);
	printf("\n Bir dersin maksimum kontenjan�n� giriniz: ");
	scanf("%d", &maxKota);
	
	// TAR�H (OgrenciDersKayit.txt dosyas�na eklenecek)
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	sprintf(date, "%02d.%02d.%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
	
	printf("\n��renci dosyas� ad�: ");
	scanf("%s", fstdname);
	//strcpy(fstdname,"std.txt");
	printf("\nA��lan dersler dosyas�: ");
	scanf("%s", flecname);
	//strcpy(flecname,"lectures.txt");
	printf("\n��retim �yesi dosya ad�: ");
	scanf("%s", ftchname);
	//strcpy(ftchname,"teachers.txt");
		
	while(choice!=0){
		printf("\n\n\n----------------------MENU-----------------------------\n\n");
		printf(" 1- Yeni ders ekle / sil\n"); 			  			// lectureUpdate()
		printf(" 2- Yeni ��retim �yesi kayd� ekle / sil\n");   		// teacherUpdate()
		printf(" 3- Yeni ��renci kayd� ekle / sil\n"); 		  		// studentUpdate()
		printf(" 4- ��renci ders kayd� ekle\n");				    // stdLecRecord()
		printf(" 5- ��renci ders kayd� sil\n");						// stdLecDelete()
		printf(" 6- ID'si verilen ��retim �yesinin verdigi dersleri listele\n");   //lecByTchID_list()
		printf(" 7- Kodu verilen dersi alan ��rencileri listele\n");			   //stdByCode_list()
		printf(" 8- Numaras� verilen bir ��rencinin ald��� dersleri listele\n");   //lecByStdID_list()
		printf(" 9- ID'si verilen bir ��retim �yesinin verdi�i bir dersi alan ��rencilerin s�n�f listesini dosyaya yazd�r\n"); // printClassroom()
		printf(" 0 -�IKI�\n\n Yapmak istedi�iniz i�lemin numaras�n� giriniz: ");
		scanf("%d", &choice);
		
		switch(choice){
			case 1: lectureUpdate(flecname, ftchname, fstdname); break;	
			case 2: teacherUpdate(ftchname, flecname, fstdname); break;		
			case 3: studentUpdate(fstdname, flecname); break;
			case 4: stdLecRecord(maxDers, maxKredi, maxKota, &count, fstdname, flecname, date); break;
			case 5: stdLecDelete(fstdname, flecname, date); break;
			case 6: lecByTchID_list(flecname, ftchname); break;
			case 7: stdByCode_list(fstdname, flecname); break;
			case 8: lecByStdID_list(fstdname, flecname); break;
			case 9: printClassroom(ftchname, flecname, fstdname); break;
		}		
	}
	printf("\n\t\t --- PROGRAM BA�ARILI B�R �EK�LDE KAPANDI ---\n");
	return 1;
}

void lectureUpdate(char *flecname, char *ftchname, char *fstdname)
{
	FILE *flec, *ftch, *fstd, *frec;
	char recNO[5], recstdID[10], tire[5], reclecNO[10], pass[10]; //��renci-ders kay�t durumlar� i�in	
	char strLec[10], strCred[10], lecID[10]; // itoa fonksiyonu i�in (silme k�sm�nda laz�m olacak)
	int choice;
	
	printf("\nA��lan dersler listesinde yapmak istedi�iniz i�lemi se�iniz:\n\n");
	printf("1-Ders ekle\n2-Ders sil\t");
	scanf("%d", &choice);
	
	//DERS EKLE
	if(choice == 1)
	{				
	    LEC ders;		
		printf("\nEklemek istedi�iniz dersin bilgilerini giriniz:\n\n");
		printf("Ders kodu: "); scanf("%s", ders.lecCode);
		
		//DOSYADA ARAMA ��LEM� - �NCE, DERS ZATEN MEVCUT MU D�YE KONTROL ED�YORUZ		
		if(mevcut(flecname, ders.lecCode, "Bu ders zaten a��lm��.", "Ders hen�z a��lmam��. A�mak i�in gereken bilgileri girmeye devam ediniz:\n"))
		{
			printf("\n\nMen�ye d�nmek i�in herhangi bir tu�a bas�n�z: ");
			getch();
			return;  //dersin �nceden var olma durumunda veya dosyan�n a��lamamas� durumunda fonksiyon burada bitmeli
		}
		//��RET�M �YES� KAYDI VAR MI KONTROL�
		printf("Derse atanacak ��retim �yesi ID no: "); scanf("%s", ders.tchID);
		if(mevcut(ftchname, ders.tchID, "��retim �yesi kay�tl�. L�tfen a�a��daki bilgileri de giriniz:\n", "��retim �yesi kay�tl� de�il. �nce ��retim �yesi kayd� yap�lmal�.\n") != 1)
		{
			printf("\n\nMen�ye d�nmek i�in herhangi bir tu�a bas�n�z: ");
			getch();
			return;
    	}
				
		flec = fopen(flecname, "a+");
		if(flec) 
		{	
			printf("Ders ad�: "); scanf("%s", ders.lecName);				
			
			printf("Kredisi: "); scanf("%d", &ders.credit);
			
			ders.quota = 0;
						
			fprintf(flec, "\n%s %s %d %d %s", ders.lecCode, ders.lecName, ders.credit, ders.quota, ders.tchID);
			fclose(flec);
			printf("\n		Ders ba�ar�l� bir �ekilde a��ld�.\n");
        }
		else printf("\nDosya bulunamad�.\n");
		
	}	
	//DERS S�L
	else if(choice == 2)
	{	
		printf("\n��kar�lacak dersin kodu: ");
		scanf("%s", lecID);
		deleteLecture(flecname, fstdname, lecID);
	}
	printf("\n\nMen�ye d�nmek i�in herhangi bir tu�a bas�n�z: ");
	getch();
}

void studentUpdate(char *fstdname, char *flecname)
{
	FILE *fstd, *frec, *flec;
	int choice;
	
	printf("\n��lem se�iniz:\n");
	printf("\n1-��renci ekle\n2-�grenci sil\t");
	scanf("%d", &choice);
	
	//��RENC� EKLE
	if(choice==1)
	{
		STD student;		
		printf("\nEklemek istedi�iniz �grenci bilgileri:\n\n");
		printf("ID: "); scanf("%s", student.stdID);
					
		//DOSYADA ARAMA ��LEM� - ��RENC� ZATEN KAYITLI MI D�YE KONTROL ED�YORUZ
		if(mevcut(fstdname, student.stdID, "��renci zaten kay�tl�.", "��renci kayd� yap�l�yor... L�tfen di�er bilgileri de giriniz: ")) return;  //��rencinin �nceden kay�tl� olma durumunda veya dosyan�n a��lamamas� durumunda fonksiyon burada bitmeli
		
		fstd = fopen(fstdname, "a+");		
		if(fstd)
		{			
			printf("Ad: ");	scanf("%s", student.stdName);
			
			printf("Soyad: "); scanf("%s", student.stdSurname);
			
			student.numOfLectures = 0;	//yeni kaydolan hi�bir ��renci hen�z bir ders almam��t�r
			student.numOfCredits = 0;
			
			fprintf(fstd, "\n%s %s %s %d %d", student.stdID, student.stdName, student.stdSurname, student.numOfLectures, student.numOfCredits);
			fclose(fstd);
			printf("\n		��renci, ba�ar�l� bir �ekilde kaydedildi.\n\nMen�ye d�nmek i�in herhangi bir tu�a bas�n�z: ");
			getch();
		}
		else printf("Dosya bulunamad�.\n");
	}
	//��RENC� S�L
	else if(choice == 2)
	{	
		LEC lecture;
		char oID[10], strLecCred[10], strKota[10], ch;	
		char *bufferLEC, *bufLineLEC, *ptrLEC, *lineLEC;
		char *bufferREC, *ptrREC, *line;
		char recNO[5], recstdID[10], tire[5], reclecNO[10], pass[15]; //��renci-ders kay�t durumlar� i�in
		int c = 1; //sat�r sayac� (d�zg�n yazd�rabilmek i�in)	
		
		//��RENC�LER� L�STELEME
		fstd = fopen(fstdname, "r");
		if(!fstd)
		{
			printf("\nDosya bulunamad�:\n");
			return;
		}
		ch=fgetc(fstd); 
		printf("\n ��RENC�LER: \n"); 
		while(ch!=EOF){ 
	        printf("%c",ch); 
		    ch=fgetc(fstd); 
	    }
		fclose(fstd);
		
		printf("\n\nKayd� silinecek ��renci ID no: ");
		scanf("%s", oID);
		
		//DOSYADA ARAMA ��LEM� - 'YOKSA ZATEN S�L�NEMEZ' DURUMU ���N
		if(mevcut(fstdname, oID, "��renci kayd� siliniyor...", "Bu ��renci zaten kay�tl� de�il.") != 1) return; //��rencinin �nceden var olmama durumunda veya dosyan�n a��lamamas� durumunda fonksiyon burada bitmeli
		
		//ALDI�I DERS VARSA O DERS�N KONTENJANI 1 AZALMALI
		frec = fopen("OgrenciDersKayit.txt", "r");
		if(!frec)
		{
			printf("\nDosya bulunamad�:\n");
			return;
		}
		
		//ODK dosyas�n� g�ncellemek i�in	
		bufferREC = (char*)malloc(sizeof(char)*2000);
		memset(bufferREC, 0, 2000*sizeof(char));
		ptrREC = bufferREC;
		
		//LECTURE dosyas�n� g�ncellemek i�in
		bufferLEC = (char*)malloc(sizeof(char)*2000);
		bufLineLEC = (char*)malloc(sizeof(char)*100);
		
		lineLEC = (char*)malloc(sizeof(char)*100);
		line = (char*)malloc(sizeof(char)*100);
		
		while(!feof(frec))
		{
			fgets(line, 100, frec);
			if(strstr(line, oID))
			{
				printf("%s", line);
				sscanf(line, "%s %s %s %s %s", recNO, recstdID, tire, reclecNO, pass);	//ODK'da ��renci kayd� bulundu
				
				if(!strcmp(pass, "KAYITLI"))
				{
					memset(bufferLEC, 0, 2000*sizeof(char));
					ptrLEC = bufferLEC;
					flec = fopen(flecname, "r");
					
					if(!flec)
					{
						printf("\nDosya bulunamad�:\n");
						free(bufferREC);
						free(bufferLEC);
						free(bufLineLEC);
						free(lineLEC);
						free(line);
						return;
					}
					while(!feof(flec))
					{
						fgets(lineLEC, 100, flec);
						if(strstr(lineLEC, reclecNO))
						{
							printf("%s", lineLEC);
							sscanf(lineLEC, "%s %s %d %d %s", lecture.lecCode, lecture.lecName, &lecture.credit, &lecture.quota, lecture.tchID);
							
							lecture.quota--;
							
							memset(bufLineLEC, 0, 100);
							
							itoa(lecture.credit, strLecCred, 10);
							itoa(lecture.quota, strKota, 10);
							
							strcat(bufLineLEC, lecture.lecCode);
							strcat(bufLineLEC, " ");
							strcat(bufLineLEC, lecture.lecName);
							strcat(bufLineLEC, " ");
							strcat(bufLineLEC, strLecCred);
							strcat(bufLineLEC, " ");
							strcat(bufLineLEC, strKota);
							strcat(bufLineLEC, " ");
							strcat(bufLineLEC, lecture.tchID);
							if(c < countLine(flecname)) strcat(bufLineLEC, "\n");			
										
							strcpy(ptrLEC, bufLineLEC);
							ptrLEC += strlen(bufLineLEC);
						}
						else
						{
							strcpy(ptrLEC, lineLEC);
							ptrLEC += strlen(lineLEC);
						}
						c++;
					}
					fclose(flec);
					flec = fopen(flecname, "w");
					fprintf(flec, "%s", bufferLEC);
					fclose(flec);
					c = 1;  //dosyay� ba�tan sona tekrar tarayaca��m�z i�in c'yi 1'liyoruz
				}			
			}
			else
			{
				strcpy(ptrREC, line);
				ptrREC += strlen(line);
			}
		}
		fclose(frec);
		frec = fopen("OgrenciDersKayit.txt", "w");
		fprintf(frec, "%s", bufferREC);
		fclose(frec);
		
		free(bufferREC);
		free(bufferLEC);
		free(bufLineLEC);
		free(lineLEC);
		free(line);
		
		deleteMember(fstdname, oID, "\n		��renci kayd� ba�ar�l� bir �ekilde silindi.\n");
	}
}

void teacherUpdate(char *ftchname, char *flecname, char *fstdname)
{
	FILE *ftch, *flec;
	int choice, var;
	char *line;
	line = (char*)malloc(sizeof(char)*100);
	
	printf("1-��r. �yesi ekle\n2-��r. �yesi sil\t");
	scanf("%d", &choice);
	
	TCH teacher;	
	//��R. �YES� EKLE
	if(choice==1)
	{	
		printf("Eklemek istedi�iniz ��retim �yesi bilgileri:\n");
		printf("ID: ");
		scanf("%s", teacher.tchID);
	
		//DOSYADA ARAMA ��LEM�
		if(mevcut(ftchname, teacher.tchID, "��retim �yesi zaten kay�tl�.", "��retim �yesi kayd� yap�l�yor...")) return;  //��retim �yesinin �nceden kay�tl� olma durumunda veya dosyan�n a��lamamas� durumunda fonksiyon burada bitmeli
		
		ftch = fopen(ftchname, "a+");
		if(ftch)
		{	
			printf("Ad: "); scanf("%s", teacher.tchName);
			
			printf("Soyad: "); scanf("%s", teacher.tchSurname);
			
			printf("Unvan: "); scanf("%s", teacher.position);
			
			fprintf(ftch, "\n%s %s %s %s", teacher.tchID, teacher.tchName, teacher.tchSurname, teacher.position);
			fclose(ftch);
			printf("\n		��retim �yesi, ba�ar�l� bir �ekilde kaydedildi.\n\nMen�ye d�nmek i�in herhangi bir tu�a bas�n�z: ");
			getch();
		}
		else printf("Dosya bulunamad�.\n");
	}	
	//��R. �YES� S�L
	else if(choice == 2)
	{
		LEC lecture;
		char ouID[10], lecID[10];		
		printf("Kayd� silinecek ��retim �yesi ID no: ");
		scanf("%s", ouID);
		
		//DOSYADA ARAMA ��LEM�
		if(mevcut(ftchname, ouID, "��r. �yesi kayd� siliniyor...", "��r. �yesi zaten kay�tl� de�il.") != 1) return; //��r. �yesinin �nceden var olmama durumunda veya dosyan�n a��lamamas� durumunda fonksiyon burada bitmeli
		
		//��retim �yesinin verdi�i dersi buldur
		flec = fopen(flecname, "r");
		if(!flec)
		{
			printf("\nDosya bulunamad�.\n");
			return;
		}
		while(!feof(flec))
		{
			fgets(line, 100, flec);
			if(strstr(line, ouID))
			{
				sscanf(line, "%s %s %d %d %s", lecture.lecCode, lecture.lecName, &lecture.credit, &lecture.quota, lecture.tchID);
				printf("\nlecID: %s", lecture.lecCode);
				
				deleteLecture(flecname, fstdname, lecture.lecCode);
			}
		}
		fclose(flec);
		
		deleteMember(ftchname, ouID, "\n		��retim �yesi kayd� ba�ar�l� bir �ekilde silindi.\n\nMen�ye d�nmek i�in herhangi bir tu�a bas�n�z: ");	
		getch();
	}
	free(line);
}

void stdLecRecord(int maxDers, int maxKredi, int maxKota, int *pcount, char *fstdname, char *flecname, char *date)
{
	FILE *fstd, *flec, *frec;
	char stdIDRec[10], lecCodeRec[10], *line, ch, *buffer, *ptr;
	char recNO[5], recstdID[10], tire[5], reclecNO[10], pass[15]; //��renci-ders kay�t durumlar� i�in
	char *bufLine, strLec[10], strCred[10], strKota[10], strLecCred[10];   //*bufLine ile ��renci struct�n� tek bir sat�rda olu�turaca��z
	int found = 0, c = 1; //sadece do�ru yaz�m i�in sat�r say�s�n� kontrol ama�l�
	
	STD student;
	
	fstd = fopen(fstdname, "r");
	if(!fstd){
		printf("Dosya bulunamad�.");
		return;
	}
	printf("\nDers kayd� yapacak ��rencinin ID'sini giriniz: ");
	scanf("%s", stdIDRec);	
	
	//DOSYADA ARAMA ��LEM� - 'YOKSA KAYDI YAPILAMAZ' DURUMU ���N
	if(mevcut(fstdname, stdIDRec, " ", "��renci kayd� bulunamad�.\n\nMen�ye d�nmek i�in herhangi bir tu�a bas�n�z... ") != 1) 
	{
		getch();
		return;
	}
	
	line = (char*)malloc(sizeof(char)*100);
	while(!feof(fstd))
	{
		fgets(line, 100, fstd);
		if(strstr(line, stdIDRec) != NULL)
		{
			sscanf(line, "%s %s %s %d %d", student.stdID, student.stdName, student.stdSurname, &student.numOfLectures, &student.numOfCredits);
			printf("\n\n��renci bilgileri:\n ID: %s\n Ad: %s\n Soyad: %s\n Mevcut ders say�s�: %d\n Mevcut kredi say�s�: %d\n", student.stdID, student.stdName, student.stdSurname, student.numOfLectures, student.numOfCredits);
		}
	}
	fclose(fstd);
	
	//��rencinin ders say�s�n� a��p a�mad���n� kontrol ediyoruz
	if(student.numOfLectures == maxDers){
		printf("\n��renci ders say�s�n� a�makta, daha fazla ders alamaz.\n");
		free(line);
		return;
	}
	
	flec = fopen(flecname, "r");
	if(!flec){
		printf("Dosya bulunamad�.");
		free(line);
		return;
	}
			
	LEC lecture;
	
	flec = fopen(flecname, "r");
	if(!flec){
		printf("Dosya bulunamad�.\n");
		free(line);
        return;
	}	
	//mevcut dersleri ekrana yazd�rma
	ch=fgetc(flec); 
	printf("\n A�ILAN DERSLER: \n"); 
	while(ch!=EOF){ 
        printf("%c",ch); 
	    ch=fgetc(flec); 
    }
    rewind(flec);
	printf("\n\n��rencinin kaydolmak istedi�i dersin kodunu giriniz: ");
	scanf("%s", lecCodeRec);
	
	//OgrenciDersKayit.txt dosyas�nda KAYITLI kontrol�
	frec = fopen("OgrenciDersKayit.txt", "r");
	if(!frec)
	{
		printf("\nDosya bulunamad�. Tekrar deneyiniz.\n");
		return;
	}
	while(!feof(frec))
	{
		fgets(line, 100, frec);
		if(strstr(line, stdIDRec) != NULL && strstr(line, lecCodeRec) != NULL)
		{
			sscanf(line, "%s %s %s %s %s", recNO, recstdID, tire, reclecNO, pass, date);
			printf("\n\n��renci ID - Ders kodu -> �NCEK� DURUMU:  %s %s %s %s %s", recNO, recstdID, tire, reclecNO, pass, date);
			found = 1;
		}
	}
	fclose(frec);
	
	if(found == 1)
	{
		if(!strcmp(pass, "KAYITLI"))
		{
			printf("\n\n%s numaral� ��renci %s kodlu derse zaten kay�tl�.\n", stdIDRec, lecCodeRec);
			return;
		}
		else if(!strcmp(pass, "BIRAKTI"))
		{
			printf("\n%s numaral� ��renci %s kodlu dersi daha �nceden al�p b�rakm��.\n", stdIDRec, lecCodeRec);
			deleteMember("OgrenciDersKayit.txt", recNO, " ");
			(*pcount)--;
		}
	}
	
	while(!feof(flec))
	{
		fgets(line, 100, flec);
		if(strstr(line, lecCodeRec) != NULL)
		{
			sscanf(line, "%s %s %d %d %s", lecture.lecCode, lecture.lecName, &lecture.credit, &lecture.quota, lecture.tchID);
			printf("\n\nDers bilgileri:\n Ders kodu: %s\n Dersin ad�: %s\n Kredi: %d\n Mevcut kontenjan: %d\n Dersi veren ��retim �yesi ID no: %s\n", lecture.lecCode, lecture.lecName, lecture.credit, lecture.quota, lecture.tchID);
		}
	}
	fclose(flec);
	
	//KONTROLLER
	if(lecture.quota == maxKota){
		printf("\nDers kontenjan� dolu.\n");
		return;
	}
	if(student.numOfCredits + lecture.credit > maxKredi){
		printf("\n��renci kredisini a�makta, daha az kredili bir derse kaydolabilir.\n\nMen�ye d�nmek i�in herhangi bir tu�a bas�n�z... ");
		getch();		
		return;
	}
	student.numOfLectures++; //��rencinin ald��� ders say�s� artt�
	student.numOfCredits += lecture.credit; //��rencinin ald��� kredi say�s� artt�
	lecture.quota++; //dersin kontenjan� artt�
	
	fstd = fopen(fstdname, "r");
	
	buffer = (char*)malloc(sizeof(char)*2000);
	memset(buffer, 0, 2000*sizeof(char));
	ptr = buffer;
	
	bufLine = (char*)malloc(sizeof(char)*100);
	memset(bufLine, 0, 100);
	
	//��RENC� DOSYASINI YEN�LE
	while(!feof(fstd))
	{
		fgets(line, 100, fstd);
		if(strstr(line, stdIDRec) == NULL)
		{
			strcpy(ptr, line);
			ptr += strlen(line);
		}
		else
		{			
			itoa(student.numOfLectures, strLec, 10);
			itoa(student.numOfCredits, strCred, 10);
			
			strcat(bufLine, student.stdID);
			strcat(bufLine, " ");
			strcat(bufLine, student.stdName);
			strcat(bufLine, " ");
			strcat(bufLine, student.stdSurname);
			strcat(bufLine, " ");
			strcat(bufLine, strLec);
			strcat(bufLine, " ");
			strcat(bufLine, strCred);
			if(c < countLine(fstdname)) strcat(bufLine, "\n");
						
			strcpy(ptr, bufLine);
			ptr += strlen(bufLine);
		}
		c++;
	}
	fclose(fstd);
	fstd = fopen(fstdname, "w");
	fprintf(fstd, "%s", buffer);
	fclose(fstd);
	
	//ders dosyas�n� yenile	
	memset(buffer, 0, 2000*sizeof(char));
	ptr = buffer;
	memset(bufLine, 0, 100);
	
	c=1;
	flec = fopen(flecname, "r");	
	while(!feof(flec))
	{
		fgets(line, 100, flec);
		if(strstr(line, lecCodeRec) == NULL)
		{
			strcpy(ptr, line);
			ptr += strlen(line);
		}
		else
		{			
			itoa(lecture.credit, strLecCred, 10);
			itoa(lecture.quota, strKota, 10);
			
			strcat(bufLine, lecture.lecCode);
			strcat(bufLine, " ");
			strcat(bufLine, lecture.lecName);
			strcat(bufLine, " ");
			strcat(bufLine, strLecCred);
			strcat(bufLine, " ");
			strcat(bufLine, strKota);
			strcat(bufLine, " ");
			strcat(bufLine, lecture.tchID);
			if(c < countLine(flecname)) strcat(bufLine, "\n");			
						
			strcpy(ptr, bufLine);
			ptr += strlen(bufLine);
		}
		c++;
	}
	fclose(flec);
	flec = fopen(flecname, "w");
	fprintf(flec, "%s", buffer);
	fclose(flec);
	
	//��renciNO-DersKodu dosyas�na yazd�rma
	(*pcount)++;
	frec = fopen("OgrenciDersKayit.txt", "a");
	if(!frec){
		printf("Dosya bulunamad�.");
		free(buffer);
		free(bufLine);
		free(line);
		return;
	}
	fprintf(frec, "\n%d-) %s - %s KAYITLI %s", *pcount, student.stdID, lecture.lecCode, date);
	fclose(frec);	
	printf("\n		��renci ders kayd� ba�ar�l� bir �ekilde yap�ld�.\n\nMen�ye d�nmek i�in herhangi bir tu�a bas�n�z... ");
	getch();
	
	free(buffer);
	free(bufLine);
	free(line);
}

void stdLecDelete(char *fstdname, char *flecname, char *date)
{
	FILE *fstd, *flec, *frec;
	char stdIDRec[10], lecCodeRec[10], *line, ch, *buffer, *ptr, *bufLine;
	char strLec[10], strCred[10], strLecCred[10], strKota[10];    //integer de�erleri stringe �evirmek i�in
	char recNO[5], recstdID[10], tire[5], reclecNO[10], pass[10]; //��renci-ders kay�t durumlar� i�in
	int found = 0, c = 1;
	
	STD student;
	LEC lecture;
	
	fstd = fopen(fstdname, "r");
	if(!fstd){
		printf("Dosya bulunamad�.");
		return;
	}
	printf("\nDers kayd� silinecek ��rencinin ID'sini giriniz: ");
	scanf("%s", stdIDRec);	
	
	if(mevcut(fstdname, stdIDRec, " ", "��renci kayd� bulunamad�.") != 1)
	{
		printf("\n\nMen�ye d�nmek i�in herhangi bir tu�a bas�n�z... ");
		getch();
		return;	
	}
	line = (char*)malloc(sizeof(char)*100);
	while(!feof(fstd))
	{
		fgets(line, 100, fstd);
		if(strstr(line, stdIDRec) != NULL)
		{
			printf("%s", line);
			sscanf(line, "%s %s %s %d %d", student.stdID, student.stdName, student.stdSurname, &student.numOfLectures, &student.numOfCredits);
			printf("\n\n��renci bilgileri:\n ID: %s\n Ad: %s\n Soyad: %s\n Mevcut ders say�s�: %d\n Mevcut kredi say�s�: %d\n", student.stdID, student.stdName, student.stdSurname, student.numOfLectures, student.numOfCredits);
		}
	}
	fclose(fstd);
	
	flec = fopen(flecname, "r");
	if(!flec){
		printf("Dosya bulunamad�.\n");
		free(line);
        return;
	}	
	//A�ILAN DERSLER� EKRANA YAZDIRMA
	ch=fgetc(flec); 
	printf("\n A��lan dersler: \n"); 
	while(ch!=EOF){ 
        printf("%c",ch); 
	    ch=fgetc(flec); 
    }
    rewind(flec);
    
	//DERS KAYIT DURUMU KONTROL�
	frec = fopen("OgrenciDersKayit.txt", "r");
	if(!frec){
		printf("Dosya bulunamad�.\n");
        return;
	}	
	//EKRANA YAZDIRMA
	printf("\n\n ��renci-Ders kay�t/b�rakma durumlar�: \n"); 
	while(!feof(frec))
	{
		fgets(line, 100, frec);
		if(strstr(line, stdIDRec)) printf("\n%s", line); 
    }
    rewind(frec);
    
	printf("\n\n��rencinin b�rakmak istedi�i dersin kodunu giriniz: ");
	scanf("%s", lecCodeRec);
	
	if(mevcut(flecname, lecCodeRec, " ", "Bu ders hen�z a��lmam��.") != 1)
	{
		printf("\n\nMen�ye d�nmek i�in herhangi bir tu�a bas�n�z... ");
		getch();
		free(line);
		return;	
	}	
	
	while(!feof(flec))
	{
		fgets(line, 100, flec);
		if(strstr(line, lecCodeRec) != NULL)
		{
			sscanf(line, "%s %s %d %d %s", lecture.lecCode, lecture.lecName, &lecture.credit, &lecture.quota, lecture.tchID);
			printf("\n\nDers bilgileri:\n Ders kodu: %s\n Dersin ad�: %s\n Kredi: %d\n Mevcut kontenjan: %d\n Dersi veren ��retim �yesi ID no: %s\n", lecture.lecCode, lecture.lecName, lecture.credit, lecture.quota, lecture.tchID);
		}
	}
	fclose(flec);
    
    //KAYIT DURUMUNU ARAMA
    while(!feof(frec))
	{
		fgets(line, 100, frec);
		if(strstr(line, stdIDRec) != NULL && strstr(line, lecCodeRec) != NULL)
		{
			sscanf(line, "%s %s %s %s %s %s", recNO, recstdID, tire, reclecNO, pass, date);
			printf("\n\n��renci ID - Ders kodu -> �NCEK� DURUMU:  %s %s %s %s %s", recNO, recstdID, tire, reclecNO, pass);
			found = 1;
		}
	}
	fclose(frec);
	
	if(found == 0)
	{
		printf("\n %s numaral� ��renci daha �nce %s dersine kaydolmam��.", stdIDRec, lecture.lecName);
		free(line);
		return;
	}
	if(strcmp(pass, "BIRAKTI") == 0)
	{
		printf("\n��renci bu dersi daha �nceden b�rakm��.\n");
		free(line);
		return;
	}		
	else strcpy(pass, "BIRAKTI");
	
	student.numOfLectures--;
	student.numOfCredits -= lecture.credit;
	lecture.quota--;
	
	//KAYDI YEN�LEME	
	buffer = (char*)malloc(sizeof(char)*2000);
	bufLine = (char*)malloc(sizeof(char)*100);
	memset(buffer, 0, 2000*sizeof(char));
	ptr = buffer;
	memset(bufLine, 0, 100);
	
	frec = fopen("OgrenciDersKayit.txt", "r");
	while(!feof(frec))
	{
		fgets(line, 100, frec);
		if(strstr(line, stdIDRec) == NULL || strstr(line, lecCodeRec) == NULL)
		{	
			strcpy(ptr, line);
			ptr += strlen(line);					
		}
		else // ID-CODE ikilisi bu sat�rdad�r demektir
		{
			strcat(bufLine, recNO);
			strcat(bufLine, " ");
			strcat(bufLine, recstdID);
			strcat(bufLine, " ");
			strcat(bufLine, tire);
			strcat(bufLine, " ");
			strcat(bufLine, reclecNO);
			strcat(bufLine, " ");
			strcat(bufLine, pass);
			strcat(bufLine, " ");
			strcat(bufLine, date);
			if(c < countLine("OgrenciDersKayit.txt")) strcat(bufLine, "\n");			
						
			strcpy(ptr, bufLine);
			ptr += strlen(bufLine);			
		}
		c++;
	}
	fclose(frec);
	frec = fopen("OgrenciDersKayit.txt", "w");
	fprintf(frec, "%s", buffer);
	fclose(frec);
	
	//��RENC� DOSYASINI YEN�LE	
	memset(buffer, 0, 2000*sizeof(char));
	ptr = buffer;
	memset(bufLine, 0, 100);
	c = 1;
	fstd = fopen(fstdname, "r");
	while(!feof(fstd))
	{
		fgets(line, 100, fstd);
		if(strstr(line, stdIDRec) == NULL)
		{
			strcpy(ptr, line);
			ptr += strlen(line);
		}
		else
		{			
			itoa(student.numOfLectures, strLec, 10);
			itoa(student.numOfCredits, strCred, 10);
			
			strcat(bufLine, student.stdID);
			strcat(bufLine, " ");
			strcat(bufLine, student.stdName);
			strcat(bufLine, " ");
			strcat(bufLine, student.stdSurname);
			strcat(bufLine, " ");
			strcat(bufLine, strLec);
			strcat(bufLine, " ");
			strcat(bufLine, strCred);
			if(c < countLine(fstdname)) strcat(bufLine, "\n");			
						
			strcpy(ptr, bufLine);
			ptr += strlen(bufLine);
		}
		c++;
	}
	fclose(fstd);
	fstd = fopen(fstdname, "w");
	fprintf(fstd, "%s", buffer);
	fclose(fstd);
	
	//DERS DOSYASINI YEN�LE	
	memset(buffer, 0, 2000*sizeof(char));
	ptr = buffer;
	memset(bufLine, 0, 100);
	c = 1;
	flec = fopen(flecname, "r");
	while(!feof(flec))
	{
		fgets(line, 100, flec);
		if(strstr(line, lecCodeRec) == NULL)
		{
			strcpy(ptr, line);
			ptr += strlen(line);
		}
		else
		{			
			itoa(lecture.credit, strLecCred, 10);
			itoa(lecture.quota, strKota, 10);
			
			strcat(bufLine, lecture.lecCode);
			strcat(bufLine, " ");
			strcat(bufLine, lecture.lecName);
			strcat(bufLine, " ");
			strcat(bufLine, strLecCred);
			strcat(bufLine, " ");
			strcat(bufLine, strKota);
			strcat(bufLine, " ");
			strcat(bufLine, lecture.tchID);
			if(c < countLine(flecname))strcat(bufLine, "\n");			
						
			strcpy(ptr, bufLine);
			ptr += strlen(bufLine);
		}
		c++;
	}
	fclose(flec);
	flec = fopen(flecname, "w");
	fprintf(flec, "%s", buffer);
	fclose(flec);
	
	free(buffer);
	free(bufLine);
	free(line);
	
	printf("\n		Ders kay�tlar� ba�ar�l� bir �ekilde g�ncellendi.\n\nMen�ye d�nmek i�in herhangi bir tu�a bas�n�z... ");
	getch();
}


void deleteLecture(char *flecname, char *fstdname, char *lecID)
{
	FILE *flec, *ftch, *fstd, *frec;
	char recNO[5], recstdID[10], tire[5], reclecNO[10], pass[10]; //��renci-ders kay�t durumlar� i�in	
	char strLec[10], strCred[10]; // itoa fonksiyonu i�in (silme k�sm�nda laz�m olacak)
	int choice, crec = 1, cstd = 1; //crec ve cstd dosyan�n sonunda olup olmamay� kontrol eder (do�ru yaz�m i�in)
	
	STD student;
	LEC lecture;
	
	char *bufferREC, *bufferSTD, *bufLineREC, *bufLineSTD, *ptrREC, *ptrSTD, *lineREC, *lineSTD;	
	
	//DOSYADA ARAMA ��LEM� - 'YOKSA ZATEN S�L�NEMEZ' DURUMU ���N
	if(mevcut(flecname, lecID, "Ders ��kar�l�yor...\n", "Bu ders zaten a��lmam��.") != 1) return; //dersin �nceden var olmama durumunda veya dosyan�n a��lamamas� durumunda fonksiyon burada bitmeli
	
	// D��ER DOSYALARDAN DA S�LME
	//dersler dosyas�ndan, silinecek dersin bilgileri al�nmal�d�r
	flec = fopen(flecname, "r");
	if(!flec)
	{
		printf("\nDosya bulunamad�. Dolay�s�yla ders silme i�lemi yap�lamad�.\n");
		return;
	}
	lineREC = (char*)malloc(sizeof(char)*100);
	while(!feof(flec))
	{
		fgets(lineREC, 100, flec);
		if(strstr(lineREC, lecID) != NULL)
		{
			sscanf(lineREC, "%s %s %d %d %s", lecture.lecCode, lecture.lecName, &lecture.credit, &lecture.quota, lecture.tchID);
		}
	}
	fclose(flec);
	
	//dersi alan hi�bir ��renci yoksa ��renciler dosyas�nda bir de�i�ikli�e gerek de yok demektir
	if(lecture.quota == 0)
	{
		printf("\nBu dersi zaten kimse almam��.\n");
		deleteMember(flecname, lecID, "Ders ba�ar�l� bir �ekilde ��kar�ld�.");
		free(lineREC);
		return;
	}
	
	//dersleri tutmak i�in
	bufferREC = (char*)malloc(sizeof(char)*2000);
	bufLineREC =(char*)malloc(sizeof(char)*100);
	memset(bufferREC, 0, 2000*sizeof(char));
	ptrREC=bufferREC;
	
	//��rencileri tutmak i�in
	bufferSTD = (char*)malloc(sizeof(char)*2000);
	lineSTD = (char*)malloc(sizeof(char)*100);
	
	//OgrenciDersKayit.txt dosyas�n� a�
	frec = fopen("OgrenciDersKayit.txt", "r");
	if(!frec)
	{
		printf("\nDosya a��lamad�. Dolay�s�yla ders silme i�lemi yap�lamad�.\n");
		return;
	}
	
	while(!feof(frec))
	{
		fgets(lineREC, 100, frec);
		if(strstr(lineREC, lecID) == NULL)
		{	
			strcpy(ptrREC, lineREC);
			ptrREC += strlen(lineREC);					
		}
		else // CODE bu sat�rdad�r demektir
		{
			sscanf(lineREC, "%s %s %s %s %s", recNO, recstdID, tire, reclecNO, pass);
			printf("\n\n��renci ID - Ders kodu -> �NCEK� DURUMU:  %s %s %s %s %s", recNO, recstdID, tire, reclecNO, pass);
			
			if(!strcmp(pass, "KAYITLI")) //��renci kay�tl�ysa ��renciler dosyas�nda da de�i�iklikler yap�lmal�d�r
			{
				memset(bufferSTD, 0, 2000*sizeof(char));	
				ptrSTD=bufferSTD;	
				fstd = fopen(fstdname, "r");
				if(!fstd)
				{
					printf("\nDosya bulunamad�. Dolay�s�yla ders silme i�lemi yap�lamad�.\n");
					return;
				}
				cstd = 1;
				while(!feof(fstd))
				{
					fgets(lineSTD, 100, fstd);
					if(strstr(lineSTD, recstdID) == NULL)
					{
						strcpy(ptrSTD, lineSTD);
						ptrSTD += strlen(lineSTD);
					}
					else
					{   //��renciyi bulduk
						sscanf(lineSTD, "%s %s %s %d %d", student.stdID, student.stdName, student.stdSurname, &student.numOfLectures, &student.numOfCredits);
						printf("\n\n��renci bilgileri:\n ID: %s\n Ad: %s\n Soyad: %s\n Mevcut ders say�s�: %d\n Mevcut kredi say�s�: %d\n", student.stdID, student.stdName, student.stdSurname, student.numOfLectures, student.numOfCredits);
						
						student.numOfLectures--;
						student.numOfCredits -= lecture.credit;
						
						itoa(student.numOfLectures, strLec, 10);
						itoa(student.numOfCredits, strCred, 10);
						
						memset(bufLineSTD, 0, 100);
						
						strcat(bufLineSTD, student.stdID);
						strcat(bufLineSTD, " ");
						strcat(bufLineSTD, student.stdName);
						strcat(bufLineSTD, " ");
						strcat(bufLineSTD, student.stdSurname);
						strcat(bufLineSTD, " ");
						strcat(bufLineSTD, strLec);
						strcat(bufLineSTD, " ");
						strcat(bufLineSTD, strCred);
						if(cstd < countLine(fstdname)) strcat(bufLineSTD, "\n");			
									
						strcpy(ptrSTD, bufLineSTD);
						ptrSTD += strlen(bufLineSTD);
					}
					cstd++;
				}
				fclose(fstd);
				fstd = fopen(fstdname, "w");
				fprintf(fstd, "%s", bufferSTD);
				fclose(fstd);
			}
			strcpy(pass, "DERS_SILINDI");
	
			memset(bufLineREC, 0, 100);
			
			strcat(bufLineREC, recNO);
			strcat(bufLineREC, " ");
			strcat(bufLineREC, recstdID);
			strcat(bufLineREC, " ");
			strcat(bufLineREC, tire);
			strcat(bufLineREC, " ");
			strcat(bufLineREC, reclecNO);
			strcat(bufLineREC, " ");
			strcat(bufLineREC, pass);
			if(crec < countLine("OgrenciDersKayit.txt")) strcat(bufLineREC, "\n");			
						
			strcpy(ptrREC, bufLineREC);
			ptrREC += strlen(bufLineREC);			
		}
		crec++;
	}
	fclose(frec);
	frec = fopen("OgrenciDersKayit.txt", "w");
	fprintf(frec, "%s", bufferREC);
	fclose(frec);
	
	free(bufferREC);
	free(bufferSTD);
	free(bufLineREC);
	free(bufLineSTD);
	free(lineSTD);
	
	deleteMember(flecname, lecID, "\n		Ders ba�ar�l� bir �ekilde ��kar�ld�.");	
}
//DOSYADA ARAMA FONKS�YONU
int mevcut(char *fname, char *NO, char *varInfo, char *yokInfo){
	FILE *fptr;
	char *line;
	fptr = fopen(fname, "r");
	if(!fptr){
		printf("\n\t\tDosya bulunamad�.\n");
		return -1 ;
	}	
	line = (char*)malloc(sizeof(char)*100);
	while(!feof(fptr))
	{
		fgets(line, 100, fptr);
		if(strstr(line, NO) != NULL)
		{
			printf("\nBulunan bilgiler: %s", line);
			fclose(fptr);
			printf("\n		%s\n", varInfo);
			free(line);
			return 1;
		}
	}
	printf("\n		%s\n", yokInfo);
	fclose(fptr);
	free(line);
	return 0;
}
//S�LME ve TEKRAR YAZDIRMA FONKS�YONU
void deleteMember(char *fname, char *NO, char *info)
{
	FILE *fptr;
	char *buffer, *ptr, *line, *delp; //delp: sadece sat�r sonu de�i�ikli�i durumunda bo�luk b�rak�lmamas� i�in
	int c = 1, found = 0; //c: sat�r sayac�
	buffer = (char*)malloc(sizeof(char)*2000);
	line = (char*)malloc(sizeof(char)*100);
	memset(buffer, 0, 2000*sizeof(char));
	ptr=buffer;
	fptr = fopen(fname, "r");		
	if(fptr) 
	{
		while(!feof(fptr))
		{
			fgets(line, 100, fptr);
			if(strstr(line, NO) == NULL)
			{
				if(c != (countLine(fname) - 1) ||  found==1)
				{
					strcpy(ptr, line);
					ptr += strlen(line);
				}
				else
				{
					delp = strtok(line, "\n");
					strcpy(ptr, delp);
					ptr += strlen(delp);
				}
			}else found=1;
			c++;
		}
		fclose(fptr);
		fptr = fopen(fname, "w");
		fprintf(fptr, "%s", buffer);
		fclose(fptr);
		free(buffer);
		printf("\n		%s\n", info);
	}
	else printf("\nDosya bulunamad�.");
	free(buffer);
	free(line);
}

int countLine(char *fname)
{
	int count = 1;
	char c;
	FILE *fptr;
	fptr = fopen(fname, "r");
	if(!fptr)
	{
		printf("\nDosya a��lamad�.\n");
		return 0;
	}
	for(c = getc(fptr); c != EOF; c = getc(fptr))
	{
		if (c == '\n') count++; 
	}
	fclose(fptr);
	return count;
}

void lecByTchID_list(char *flecname, char *ftchname)
{
	FILE *flec, *ftch;
	char *line;
	char ouID[10];
	int found = 0;
	
	LEC lecture;
	TCH teacher;
	
	printf("\nVerdi�i dersleri g�rmek istedi�iniz ��retim �yesinin ID numaras�n� giriniz: ");
	scanf("%s", ouID);
	
	//��R. �YES� KAYIT KONTROL�
	if(mevcut(ftchname, ouID, " ", "��r. �yesi hen�z kay�tl� de�il. �nce kayd�n� yapabilirsiniz.") != 1) return; //��r. �yesinin �nceden var olmama durumunda veya dosyan�n a��lamamas� durumunda fonksiyon burada bitmeli
	
	ftch = fopen(ftchname, "r");
	if(!ftch)
	{
		printf("\nDosya bulunamad�. L�tfen tekrar deneyiniz.\n");
		return;
	}
	line = (char*)malloc(sizeof(char)*100);
	while(!feof(ftch))
	{
		fgets(line, 100, ftch);
		if(strstr(line, ouID))
		{
			sscanf(line, "%s %s %s %s", teacher.tchID, teacher.tchName, teacher.tchSurname, teacher.position);
		}
	}
	fclose(ftch);
	
	flec = fopen(flecname, "r");
	if(!flec)
	{
		printf("\nDosya bulunamad�. L�tfen tekrar deneyiniz.\n");
		return;
	}
	printf("\n%s ID numaral� ��retim �yesinin verdi�i dersler:\n\n  DERS KODU	DERS ADI\n", teacher.tchID);
	while(!feof(flec))
	{
		fgets(line, 100, flec);
		if(strstr(line, ouID))
		{
			sscanf(line, "%s %s %d %d %s", lecture.lecCode, lecture.lecName, &lecture.credit, &lecture.quota, lecture.tchID);
			printf("\n  %s  	%s\n", lecture.lecCode, lecture.lecName);
			found = 1;
		}
	}
	fclose(flec);
	if(found == 0)
	{
		printf("\n\n%s %s, hen�z herhangi bir derse atanmam��t�r.\n", teacher.tchName, teacher.tchSurname);
	}
	free(line);
}

void stdByCode_list(char *fstdname, char *flecname)
{
	FILE *fstd, *frec, *flec;
	STD student;
	char *lineREC;
	char *lineSTD;
	char recNO[5], recstdID[10], tire[5], reclecNO[10], pass[10]; //��renci-ders kay�t durumlar� i�in		
	char dersKodu[10];
	int found = 0;
	
	printf("\n��rencilerini g�rmek istedi�iniz dersin kodunu giriniz: ");
	scanf("%s", dersKodu);
	
	//DERS KAYIT KONTROL�
	if(mevcut(flecname, dersKodu, " ", "Bu ders hen�z a��lmam��.") != 1) return; //dersin �nceden a��lmama durumunda veya dosyan�n a��lamamas� durumunda fonksiyon burada bitmeli
	
	printf("\n%s kodlu dersi alan ��renciler:\n\n ��RENC� ID   ��RENC� AD-SOYAD\n", dersKodu);
	
	frec = fopen("OgrenciDersKayit.txt", "r");
	if(!frec)
	{
		printf("\nDosya bulunamad�. L�tfen tekrar deneyiniz.\n");
		return;
	}
	
	lineREC = (char*)malloc(sizeof(char)*100);
	lineSTD = (char*)malloc(sizeof(char)*100);
	while(!feof(frec))
	{
		fgets(lineREC, 100, frec);
		if(strstr(lineREC, dersKodu))
		{
			sscanf(lineREC, "%s %s %s %s %s", recNO, recstdID, tire, reclecNO, pass);
			
			if(!strcmp(pass, "KAYITLI"))
			{
				//��rencinin ad�n� da listelemek i�in ��renciler dosyas�n� a��yoruz
				fstd = fopen(fstdname, "r");
				if(!fstd)
				{
					printf("\nDosya bulunamad�. L�tfen tekrar deneyiniz.\n");
					return;
				}
				while(!feof(fstd))
				{
					fgets(lineSTD, 100, fstd);
					if(strstr(lineSTD, recstdID))
					{
						sscanf(lineSTD, "%s %s %s %d %d", student.stdID, student.stdName, student.stdSurname, &student.numOfLectures, &student.numOfCredits);
						
						printf("\n %s     %s %s\n", student.stdID, student.stdName, student.stdSurname);
						found = 1;						
					}
				}
				fclose(fstd);
			}			
		}
	}
	fclose(frec);
	
	if(found == 0)
	{
		printf("\n\n%s dersini hen�z kimse almam��t�r.\n", dersKodu);
	}	
	free(lineREC);
	free(lineSTD);
	
	printf("\n\nMen�ye d�nmek i�in herhangi bir tu�a bas�n�z: ");
	getch();
}

void lecByStdID_list(char *fstdname, char *flecname)
{
	FILE *fstd, *flec, *frec;
	char oID[10], *line, *lineLEC;
	char recNO[5], recstdID[10], tire[5], reclecNO[10], pass[10]; //��renci-ders kay�t durumlar� i�in			
	int found = 0;
	
	STD student;
	LEC lecture;
	
	printf("\nDerslerini g�rmek istedi�iniz ��renci ID numaras�n� giriniz: ");
	scanf("%s", oID);
	
	//hem ��renci varl��� kontrol� hem de ��renci bilgilerini alma i�lemi
	fstd = fopen(fstdname, "r");
	if(!fstd)
	{
		printf("\nDosya bulunamad�. L�tfen tekrar deneyiniz.\n");
		return;
	}
	
	line = (char*)malloc(sizeof(char)*100);
	lineLEC = (char*)malloc(sizeof(char)*100);
	while(!feof(fstd))
	{
		fgets(line, 100, fstd);
		if(strstr(line, oID))
		{
			sscanf(line, "%s %s %s %d %d", student.stdID, student.stdName, student.stdSurname, &student.numOfLectures, &student.numOfCredits);
			printf("\n��renci Ad-Soyad: %s %s\n", student.stdName, student.stdSurname);
			found = 1;						
		}
	}
	fclose(fstd);
	if(found == 0)
	{
		printf("\n\n%s numaral� bir ��renci kayd� bulunamad�.\n", oID);
		free(line);
		free(lineLEC);
		return;
	}
	found = 0;
	
	printf("\n %s numaral� ��rencinin ald��� dersler:\n\n  DERS KODU   DERS ADI\n", oID);
	
	//ODK KAYIT DURUMU KONTROL�
	frec = fopen("OgrenciDersKayit.txt", "r");
	if(!frec)
	{
		printf("\nDosya bulunamad�. L�tfen tekrar deneyiniz.\n");
		free(line);
		free(lineLEC);
		return;
	}
	while(!feof(frec))
	{
		fgets(line, 100, frec);
		if(strstr(line, oID))
		{
			sscanf(line, "%s %s %s %s %s", recNO, recstdID, tire, reclecNO, pass);
			
			if(!strcmp(pass, "KAYITLI"))
			{
				//dersin ad�n� da listede g�stermek i�in dersler dosyas�n� a��yoruz
				flec = fopen(flecname, "r");
				if(!flec)
				{
					printf("\nDosya bulunamad�. L�tfen tekrar deneyiniz.\n");
					return;
				}
				while(!feof(flec))
				{
					fgets(lineLEC, 100, flec);
					if(strstr(lineLEC, reclecNO))
					{
						sscanf(lineLEC, "%s %s %d %d %s", lecture.lecCode, lecture.lecName, &lecture.credit, &lecture.quota, lecture.tchID);
						printf("\n  %s  	%s\n", lecture.lecCode, lecture.lecName);
						found = 1;
					}
				}
				fclose(flec);
			}			
		}
	}
	fclose(frec);
	
	if(found == 0)
	{
		printf("\n\n%s numaral� ��renci hen�z ders almam��t�r.\n", oID);
	}
	free(line);
	free(lineLEC);
	
	printf("\n\nMen�ye d�nmek i�in herhangi bir tu�a bas�n�z: ");
	getch();
}

void printClassroom(char *ftchname, char *flecname, char *fstdname)
{
	FILE *ftch, *flec, *fstd, *frec, *fptr;
	TCH teacher;
	LEC lecture;
	STD student;
	char *buffer = (char*)malloc(sizeof(char)*2000);
	char *bufLine = (char*)malloc(sizeof(char)*100);
	char *lineREC = (char*)malloc(sizeof(char)*100);
	char *lineSTD = (char*)malloc(sizeof(char)*100);
	char recNO[5], recstdID[10], tire[5], reclecNO[10], pass[10]; //��renci-ders kay�t durumlar� i�in		
	char ouID[10], dersKodu[10], *ptr, fname[20];
	int found = 0;
	
	//KAYITLARI TUTUP DOSYAYA YAZDIRMAK ���N
	memset(buffer, 0, 2000*(sizeof(char)));
	ptr = buffer;
	
	lecByTchID_list(flecname, ftchname);
	
	printf("\nS�n�f listesini yazd�rmak i�in ��retim �yesi ID numaras�n� l�ten tekrar giriniz: ");
	scanf("%s", ouID);
	
	if(mevcut(ftchname, ouID, " ", " ") != 1)
	{
		printf("\n��retim �yesi kayd� yine bulunamad�.");
		printf("\n\nMen�ye d�nmek i�in herhangi bir tu�a bas�n�z: ");
		getch();
		free(lineREC);
		free(lineSTD);
		free(bufLine);
		free(buffer);
		return;
	}
	
	printf("\nS�n�f listesini, DERSKODU_SINIFLiSTESi.txt dosyas�na yazd�rmak istedi�iniz ders kodunu giriniz: ");
	scanf("%s", dersKodu);
	
	strcpy(ptr, ouID);
	ptr += strlen(ouID);
	
	strcpy(ptr, " numarali ogretim uyesinden ");
	ptr += strlen(" numarali ogretim uyesinden ");
	
	strcpy(ptr, dersKodu);
	ptr += strlen(dersKodu);
	
	strcpy(ptr, " kodlu dersi alan ogrenciler:\n\n");
	ptr += strlen(" kodlu dersi alan ogrenciler:\n\n");
	
	//DERS KAYIT KONTROL�
	strcpy(ptr, "OGRENCI ID   OGRENCI AD-SOYAD\n");
	ptr += strlen("OGRENCI ID   OGRENCI AD-SOYAD\n");
		
	frec = fopen("OgrenciDersKayit.txt", "r");
	if(!frec)
	{
		printf("\nDosya bulunamad�. L�tfen tekrar deneyiniz.\n");
		free(lineREC);
		free(lineSTD);
		free(bufLine);
		free(buffer);
		return;
	}
	while(!feof(frec))
	{
		fgets(lineREC, 100, frec);
		if(strstr(lineREC, dersKodu))
		{
			sscanf(lineREC, "%s %s %s %s %s", recNO, recstdID, tire, reclecNO, pass);
			
			if(!strcmp(pass, "KAYITLI"))
			{
				//��rencinin ad�n� da listelemek i�in ��renciler dosyas�n� a��yoruz
				fstd = fopen(fstdname, "r");
				if(!fstd)
				{
					printf("\nDosya bulunamad�. L�tfen tekrar deneyiniz.\n");
					free(lineREC);
					free(lineSTD);
					free(bufLine);
					free(buffer);
					return;
				}
				while(!feof(fstd))
				{
					fgets(lineSTD, 100, fstd);
					if(strstr(lineSTD, recstdID))
					{
						sscanf(lineSTD, "%s %s %s %d %d", student.stdID, student.stdName, student.stdSurname, &student.numOfLectures, &student.numOfCredits);
						
						memset(bufLine, 0, 100);
						
						strcat(bufLine, student.stdID);
						strcat(bufLine, "     ");
						strcat(bufLine, student.stdName);
						strcat(bufLine, " ");
						strcat(bufLine, student.stdSurname);
						strcat(bufLine, "\n");
						
						strcpy(ptr, bufLine);
						ptr += strlen(bufLine);
						
						found = 1;						
					}
				}
				fclose(fstd);
			}			
		}
	}
	fclose(frec);
	
	if(found == 0)
	{
		printf("\n\n%s dersini hen�z kimse almam��t�r. Dosyaya bir �ey yaz�lamad�.\n", dersKodu);
		return;
	}
	printf("\nOlu�turmak istedi�iniz s�n�f listesinin ad�n� derskodu_SINIFLiSTESi.txt format�nda giriniz: ");
	scanf("%s", fname);
	fptr = fopen(fname, "a");
	fprintf(fptr, "%s", buffer);
	fclose(fptr);
	
	free(lineREC);
	free(lineSTD);
	free(bufLine);
	free(buffer);
	
	printf("\n		DERSKODU_SINIFLiSTESi.txt dosyas�na s�n�f listesi olu�turuldu.\n\nMen�ye d�nmek i�in herhangi bir tu�a bas�n�z: ");
	getch();
}

