/* Motaz Faraj-1190553 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*****************************************************/
//struct for the array that saves the numbers
typedef struct aword{
    char word[100];
    char meaning[100];
} WORD;
/*****************************************************/
//struct for the hash table
typedef struct aHashTable{
    char word[1000];
    char meaning[1000];
    char state;
}HashTable;
/*****************************************************/
//this function finds the first prime number that is larger than the entered number
int FirstPrime(int n){
    while( !IsPrime(n) )
        n++;
    return n;
}
/*****************************************************/
//this function checks if a number is prime
int IsPrime(int n){
    int i;
    for( i=2; i <= n/i; i++)
        if( n%i == 0 ) return 0;
    return 1;
}
/*****************************************************/
//this function finds the sum of all char in a string
//it loops in the string adding each char acsii code to the sum
int AsciiSum(char key[100]){
    int sum = 0, i, len;
    len = strlen(key);
    for (i = 0; i < len; i++){
        sum = sum + key[i];
    }
    return sum;
}
/*****************************************************/
//this hash function finds the index of the element using Linear Probing To Solve Collision
int HashFunctionL (char key[100],int step,int Hsize){
    int sum = AsciiSum(key);
    return (((sum % Hsize)+step)%Hsize);
}
/*****************************************************/
//this hash function finds the index of the element using Quadratic Probing To Solve Collision
int HashFunctionQ (char key[100],int step,int Hsize){
    int sum = AsciiSum(key);
    return (((sum % Hsize)+(step * step))%Hsize);
}
/*****************************************************/
//this function is to search for a word in the dict
void Search(HashTable table[], char word[100], int Hsize){
    int found=0;
    for(int i=0; i<=Hsize; i++){
        if(!strcmp(table[i].word, word)){
            if(table[i].state == 'o'){
                found=1;
                printf("Found:\n");
                printf("%s:%s\n", table[i].word, table[i].meaning);
            }
            else if(table[i].state == 'd'){
                printf("The Word \'%s\' doesn't exist in the dictionary!\n", word);
            }
        }
    }
    if(found == 0){
        printf("The Word \'%s\' doesn't exist in the dictionary!\n", word);
    }
}
/*****************************************************/
//this function is to deleat a word
void Delete(HashTable table[], char word[100], int Hsize){
    int found=0;
    for(int i=0; i<=Hsize; i++){
        if(!strcmp(table[i].word, word)){
            found=1;
            printf("The Word Has Been Deleted\n");
            table[i].state = 'd';
        }
    }
    if(found == 0){
        printf("The Word \'%s\' doesn't exist in the dictionary!\n", word);
    }
}
/*****************************************************/
void UpdateMeaning(HashTable table[], char word[100], int Hsize ,char NewMeaning[100]){
    int found=0;
    for(int i=0; i<=Hsize; i++){
        if(!strcmp(table[i].word, word)){
            if(table[i].state == 'o'){
                found = 1;
                strcpy(table[i].meaning,NewMeaning);
                printf("The Word Meaning Has Been Updated\n");
                printf("%s:%s\n",table[i].word,table[i].meaning);
            }
            else if(table[i].state == 'd'){
                printf("The Word \'%s\' doesn't exist in the dictionary!\n", word);
            }
        }
    }
    if(found == 0){
        printf("The Word \'%s\' doesn't exist in the dictionary!\n", word);
    }
}
/*****************************************************/
void PrintDictionary(HashTable table[], int Hsize){
    for(int i=0;i<Hsize;i++){
        if(table[i].state == 'o' ){
            printf("%s:%s\n",table[i].word,table[i].meaning);
        }
    }

}
/*****************************************************/
void PrintDictionaryAll(HashTable table[], int Hsize){
    for(int i=0;i<Hsize;i++){
        if(table[i].state == 'o'){
            printf("%s:%s\n",table[i].word,table[i].meaning);
        }
        else if(table[i].state == 'e'){
            printf("------------Empty------------\n");
        }
    }

}
/*****************************************************/
void PrintOnFile(FILE *output,HashTable table[],int Hsize){
    for(int i=0;i<Hsize;i++){
        if(table[i].state == 'o'){
            fprintf(output,"%s:%s\n",table[i].word,table[i].meaning);
        }
    }
}
/*****************************************************/
int main(){
    FILE *fp;
    FILE *SizeRead;
    FILE *output;
    fp=fopen("dictionary.txt","r");
    SizeRead=fopen("dictionary.txt","r");
    output=fopen("dictionary_modified.txt","w");
    //declaration of int
    int i=0;
    int wordCount=0;
    int Hsize =0;
    int size = 0;
    int choice = 0;
    int step = 0;
    int index=0;
    int InTable=0;
    int crs=0;
    int crs2=0;
    double LoadFactor = 0;
    //declaration of char
    char tword[100] ,tmeaning[100];
    char line[100];
    char Sizeline[100];
    char Iword[1000][1000];
    char Imean[1000][1000];
    char Sword[100] , UWord[100] ,UMeaning[100] , DeleteWord[100];
    char InsertWord[100];
    char InsertMeaning[100];
    //declaration of arrays
    while((fgets(Sizeline, sizeof(Sizeline), SizeRead))) {
        size++;
    }
    WORD dict[size];
    Hsize = FirstPrime(size);
    HashTable table[Hsize];
    for(int s=0;s<Hsize;s++){
        table[s].state = 'e';
    }
    fclose(SizeRead);
    //menu
    while(1){
        printf("\n");
        printf("Please Select An Option\n1-Read File And Build The Dictionary\n2-Search For A Word And Display It's Meaning\n3-Update The Meaning Of An Existing Word\n4-Insert A New Word From The User With It's Meaning\n5-Print All Words In The Dictionary With Their Associated Meanings\n6-Print The Entire Hash Table Including The Empty Slots\n7-Delete A Word From The Dictionary\n8-Print The Number Of Occupied Slots, Hash Table Size And The Load Factor\n9-Save All Words Back In File Dictionary_modified.txt\n10-Exit\n");
        printf("\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);
        switch (choice){
        case 1:
                //we started by reading the file line by line and then spltting the line using strtok and saving the word into an array of struct
                //then we moved to dicide the way to solve Collision and start inserting into the hash table
                //for the load factor when the load factor becomes larger than 0.65 we deleted all the element in the hash table and reinserted them
                //with a larger hash size which equal the first prime number larger than 2*hash size
                while((fgets(line, sizeof(line), fp))) {
                    char *ptr = strtok(line,":");
                    strcpy(tword,ptr);
                    ptr = strtok(NULL,"\n");
                    strcpy(tmeaning,ptr);
                    strcpy(Iword[i],tword);
                    strcpy(Imean[i],tmeaning);
                    strcpy(dict[i].word,Iword[i]);
                    strcpy(dict[i].meaning,Imean[i]);
                    wordCount++;
                    i++;
                }
                printf("Please Choose A Way To Solve Collision Resolution:\n1-Linear Probing\n2-Quadratic Probing\n");
                printf("Enter Choice: ");
                scanf("%d", &crs);
                switch (crs){
                case 1:
                        LoadFactor =((double)InTable/(double)Hsize);
                        while(LoadFactor < 0.65){
                            for(int i=0;i<size;i++){
                            index = HashFunctionL(dict[i].word,step,Hsize);
                            while(table[index].state == 'o'){
                                step++;
                                index = HashFunctionL(dict[i].word,step,Hsize);
                            }
                            strcpy(table[index].word,dict[i].word);
                            strcpy(table[index].meaning,dict[i].meaning);
                            table[index].state='o';
                            InTable++;
                            step=0;
                            }
                            LoadFactor =((double)InTable/(double)Hsize);
                            if(LoadFactor > 0.65){
                                Hsize =FirstPrime(2*Hsize);
                                for(int s=0;s<Hsize;s++){
                                    table[s].state = 'e';
                                }
                                InTable=0;
                            }
                            if(LoadFactor <= 0.65){
                                break;
                            }
                            LoadFactor =((double)InTable/(double)Hsize);
                        }

                    break;
                case 2:
                        LoadFactor =((double)InTable/(double)Hsize);
                        while(LoadFactor < 0.65){
                            for(int i=0;i<size;i++){
                            index = HashFunctionQ(dict[i].word,step,Hsize);
                            while(table[index].state == 'o'){
                                step++;
                                index = HashFunctionQ(dict[i].word,step,Hsize);
                            }
                            strcpy(table[index].word,dict[i].word);
                            strcpy(table[index].meaning,dict[i].meaning);
                            table[index].state='o';
                            InTable++;
                            step=0;
                            }
                            LoadFactor =((double)InTable/(double)Hsize);
                            if(LoadFactor > 0.65){
                                Hsize =FirstPrime(2*Hsize);
                                for(int s=0;s<Hsize;s++){
                                    table[s].state = 'e';
                                }
                                InTable=0;
                            }
                            if(LoadFactor <= 0.65){
                                break;
                            }
                            LoadFactor =((double)InTable/(double)Hsize);
                        }

                    break;
                default:
                        printf("The Choice You Entered Is Invalid Please Enter Another\n");
                    break;

                }
            break;
        case 2:
                //entering the word you want to search for and calling the function
                printf("Enter The Word you Want To search For: ");
                scanf("\n%[^\n]%*c", Sword);
                Search(table,Sword,Hsize);

            break;
        case 3:
                //entering the word you want to update it's meaning for and calling the function
                printf("Enter The Word you Want To Update It's Meaning: ");
                scanf("\n%[^\n]%*c", UWord);
                printf("Enter The New Meaning: ");
                scanf("\n%[^\n]%*c", UMeaning);
                UpdateMeaning(table,UWord,Hsize,UMeaning);

            break;
        case 4:
                printf("Enter The Word you Want To Insert: ");
                scanf("\n%[^\n]%*c", InsertWord);
                printf("Enter The Word Meaning You Want To Insert: ");
                scanf("\n%[^\n]%*c", InsertMeaning);
                step=0;
                printf("Please Choose A Way To Solve Collision Resolution:\n1-Linear Probing\n2-Quadratic Probing\n");
                printf("Enter Choice: ");
                scanf("%d", &crs2);
                switch (crs2){
                case 1:
                        index = HashFunctionL(InsertWord,step,Hsize);
                        while(table[index].state == 'o'){
                            step++;
                            index = HashFunctionL(dict[i].word,step,Hsize);
                        }
                        strcpy(table[index].word,InsertWord);
                        strcpy(table[index].meaning,InsertMeaning);
                        table[index].state='o';
                        InTable++;
                        step=0;
                        LoadFactor =((double)InTable/(double)Hsize);
                        if(LoadFactor > 0.65){
                            Hsize =FirstPrime(2*Hsize);
                            for(int s=0;s<Hsize;s++){
                                table[s].state = 'e';
                            }
                            InTable=0;
                            LoadFactor =((double)InTable/(double)Hsize);
                            while(LoadFactor < 0.65){
                                for(int i=0;i<size;i++){
                                    index = HashFunctionL(dict[i].word,step,Hsize);
                                    while(table[index].state == 'o'){
                                        step++;
                                        index = HashFunctionL(dict[i].word,step,Hsize);
                                    }
                                    strcpy(table[index].word,dict[i].word);
                                    strcpy(table[index].meaning,dict[i].meaning);
                                    table[index].state='o';
                                    InTable++;
                                    step=0;
                                }
                                LoadFactor =((double)InTable/(double)Hsize);
                                if(LoadFactor > 0.65){
                                    Hsize =FirstPrime(2*Hsize);
                                    for(int s=0;s<Hsize;s++){
                                        table[s].state = 'e';
                                    }
                                    InTable=0;
                                }
                                if(LoadFactor <= 0.65){
                                    break;
                                }
                                LoadFactor =((double)InTable/(double)Hsize);
                            }
                            index = HashFunctionQ(InsertWord,step,Hsize);
                            while(table[index].state == 'o'){
                                step++;
                                index = HashFunctionL(dict[i].word,step,Hsize);
                            }
                            strcpy(table[index].word,InsertWord);
                            strcpy(table[index].meaning,InsertMeaning);
                            table[index].state='o';
                            InTable++;
                            step=0;
                        }

                    break;
                case 2:
                        index = HashFunctionQ(InsertWord,step,Hsize);
                        while(table[index].state == 'o'){
                            step++;
                            index = HashFunctionQ(dict[i].word,step,Hsize);
                        }
                        strcpy(table[index].word,InsertWord);
                        strcpy(table[index].meaning,InsertMeaning);
                        table[index].state='o';
                        InTable++;
                        step=0;
                        LoadFactor =((double)InTable/(double)Hsize);
                        if(LoadFactor > 0.65){
                            Hsize =FirstPrime(2*Hsize);
                            for(int s=0;s<Hsize;s++){
                                table[s].state = 'e';
                            }
                            InTable=0;
                            LoadFactor =((double)InTable/(double)Hsize);
                            while(LoadFactor < 0.65){
                                for(int i=0;i<size;i++){
                                    index = HashFunctionQ(dict[i].word,step,Hsize);
                                    while(table[index].state == 'o'){
                                        step++;
                                        index = HashFunctionQ(dict[i].word,step,Hsize);
                                    }
                                    strcpy(table[index].word,dict[i].word);
                                    strcpy(table[index].meaning,dict[i].meaning);
                                    table[index].state='o';
                                    InTable++;
                                    step=0;
                                }
                                LoadFactor =((double)InTable/(double)Hsize);
                                if(LoadFactor > 0.65){
                                    Hsize =FirstPrime(2*Hsize);
                                    for(int s=0;s<Hsize;s++){
                                        table[s].state = 'e';
                                    }
                                    InTable=0;
                                }
                                if(LoadFactor <= 0.65){
                                    break;
                                }
                                LoadFactor =((double)InTable/(double)Hsize);
                            }
                            index = HashFunctionQ(InsertWord,step,Hsize);
                            while(table[index].state == 'o'){
                                step++;
                                index = HashFunctionL(dict[i].word,step,Hsize);
                            }
                            strcpy(table[index].word,InsertWord);
                            strcpy(table[index].meaning,InsertMeaning);
                            table[index].state='o';
                            InTable++;
                            step=0;
                        }

                    break;
                default:
                        printf("The Choice You Entered Is Invalid Please Enter Another\n");
                    break;

                }

            break;
        case 5:
                PrintDictionary(table,Hsize);

            break;
        case 6:
                PrintDictionaryAll(table,Hsize);
            break;
        case 7:
                //entering the word you want to delete for and calling the function
                printf("Enter The Word You Want To Delete: ");
                scanf("\n%[^\n]%*c", DeleteWord);
                Delete(table,DeleteWord,Hsize);
                InTable--;

            break;
        case 8:
                printf("The Number Of Occupied Slots:%d\nHash Table Size:%d\nLoad Factor:%0.2f\n",InTable,Hsize,LoadFactor);

            break;
        case 9:
                PrintOnFile(output,table,Hsize);
                fclose(output);

            break;
        case 10:
                printf("The Program Has been Terminated ;)\n");
                exit(0);

        default:
                printf("The Choice You Entered Is Invalid Please Enter Another\n");

            break;
        }
    }
    return 0;
}
