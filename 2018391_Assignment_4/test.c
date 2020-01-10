#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <fcntl.h>

int main(){
    FILE *fptr;
    fptr = fopen("textToBeEncrypted.txt","r");
    if(fptr==NULL){
        printf("couldn't locate textToBeEncrypted.txt make sure that it is present in this directory only");
        printf("\nexiting...\n");
        exit(-1);
    }
    char ch = fgetc(fptr);
    int numberOfCharacters = 0;
    char fileData[1024];
    memset(fileData,0,1024);
    while(ch!=EOF){
        fileData[numberOfCharacters] = ch;
        ch=fgetc(fptr);
        numberOfCharacters++;
    }
    printf("Data written in file is : %s\n",fileData);
    fclose(fptr);
    char retrivedData[1024];
    memset(retrivedData,0,1024);

    int encyptionDevice = open("/dev/encdev", O_RDWR);
    int decryptionDevice = open("/dev/decdev", O_RDWR);
    
    if(encyptionDevice==-1 ||decryptionDevice==-1){
        printf("missing encdev or decdev\n");
        printf("Exiting now ...");
        exit(-1);
    }
    //Writing Input to Encryption Device
    write(encyptionDevice, fileData,numberOfCharacters);
    
    //Reading encypted output from Encruption Device
    read(encyptionDevice,retrivedData,sizeof(retrivedData));
    FILE *encFile;
    encFile = fopen("encryptedData.txt","w");

    fwrite(retrivedData,sizeof(retrivedData),sizeof(retrivedData),encFile);

    printf("Encrypted Data : %s\n",retrivedData);


    //giving encrypted data to decryptor
    write(decryptionDevice,retrivedData,strlen(retrivedData));


    //Creating fresh new Array
    char decryptedData[1024];
    memset(decryptedData,0,1024);

    //getting decrypted data
    read(decryptionDevice,decryptedData,sizeof(decryptedData));

    printf("Decrypted Data : %s\n",decryptedData);
    
    FILE *decFile;
    decFile = fopen("decryptedData.txt","w");

    char dataToWriteToFile[numberOfCharacters];
    
    for(int i=0;i<numberOfCharacters;i++){
        dataToWriteToFile[i]='\0';
    }

    strcat(dataToWriteToFile,decryptedData);

    fwrite(dataToWriteToFile,sizeof(char),sizeof(dataToWriteToFile),decFile);
    
    fclose(decFile);
    return 0;
}
