/* 
 * File:   uebung01_bsp03.c
 * Author: nina
 *
 * Created on 5. März 2014, 16:49
 */

#include <stdlib.h>     // error handling
#include <stdio.h>      // file io
#include <math.h>       // sqrt()
#include <string.h>

#define NQUESTIONS 20   ///< Number of questions

const int answers[20]={2,4,2,3,3,1,4,1,2,1,3,1,1,4,4,1,1,4,2,3};

static unsigned int randomNumber=2;

unsigned int myrand() 
{
    randomNumber = ((7*randomNumber+4)%17);
    return randomNumber;
}

int numberRightAnswersPerson(const char *filename)
{
    FILE* fCurrent;                         // File Handle
    int cq=0;                                  // Buffers for file parsing
    int cansw=0;                               // storage answer
    int rightAnswersPerson=0;
    
        fCurrent = fopen(filename, "r");
        if( fCurrent == NULL ) 
        {
            return -1;
        }

        // READ FILE
        do 
        {
            cq = fgetc(fCurrent); //stores question
                        
            if(cq>='A' && cq<='T')
            {
                cansw = fgetc(fCurrent)-'0'; //stores answer
                 if(answers[cq-'A']==cansw) 
                 {
                     ++rightAnswersPerson; 
                 }
            }
        } while (cq!=EOF);

        // CLOSE FILE
        fclose(fCurrent);
        
        return rightAnswersPerson;
}

int numberRightAnswersAllPersons(const char *filename, int *sumRightAnswers)
{
    FILE* fCurrent;                         // File Handle
    int cq=0;                                  // Buffers for file parsing
    int cansw=0;                               // storage answer
    
    fCurrent = fopen(filename, "r");
    if( fCurrent == NULL ) 
    {
        return -1;
    }
    // READ FILE
    do
    {
        cq = fgetc(fCurrent); //stores question
                        
        if(cq>='A' && cq<='T')
        {
            cansw = fgetc(fCurrent)-'0'; //stores answer
            if(answers[cq-'A']==cansw)
            {
                sumRightAnswers[cq-'A']+=1;
            }
        }
    } while (cq!=EOF);

    // CLOSE FILE
    fclose(fCurrent);
        
    return 0;
}


double calcAvg(int *r, int n)
{
    int s=0;
    
    for(int i=0;i<n;++i)
    {
        s+=r[i];
    }
    return (double)s/n;
}


double calcStdDev(double avg, int *r, int n)
{
    double s=0;
    
    for(int i=0;i<n;++i)
    {
        s+=(r[i]-avg)*(r[i]-avg);
    }
    return sqrt(s/(n-1));
}


int cmpIntAsc(const void * a, const void * b) 
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    if (x < y) return -1;
    else if (x > y) return 1;
    else return 0;
}


double calcMedian(int *r, int n)
{
    int *a = (int *)malloc(n * sizeof(int));
    double m;
    
    memcpy(a,r,n*sizeof(int));

    qsort(a,n,sizeof(int),cmpIntAsc); //sorts array, zB a[0],a[1] -> a und b in cmp-funktion
    
    if(n%2!=0)
    {
        m=(double)r[n/2];
    }
    else
    {
        m=(double)(r[n/2]+r[n/2+1])/2.0;
    }
    
    if(a!=0)
    {
        free(a);
        a=0;
    }
    
    return m;
}



int main(int argc, char** argv) 
{
    // DECLARATIONS
    unsigned int nFile;                     // file Counters
    
    int numParticipants=argc-1;
    
    // CRUDE ARGUMENT CHECKING
    if(argc<2) fputs("Error: No argument supplied!\n", stderr);
    
    ///////////////////////////////////////////////////////////////////
    // 3a) how many right answered question a single participant have? 
    ///////////////////////////////////////////////////////////////////
    FILE *f;
    int *r=(int *)malloc(numParticipants*sizeof(int)); 
    if(r==0)
    {
        fputs("Malloc returned null!\n",stderr);
        return -1;
    }
    
    f=fopen("rightanswers_pp.txt","w");
    fprintf(f,"number of right answers per participant:\n");
    for(int i=0;i<numParticipants;++i) 
    {
        r[i]=numberRightAnswersPerson(argv[i+1]);
        fprintf(f,"participant %d : %d \n", i+1, r[i]); //write right answer per participant in file
    }
    fclose(f);        
        
    ///////////////////////////////////////////////////////////////////
    // 3c)1) average value, deviation value, median
    ///////////////////////////////////////////////////////////////////

    
    //average
    
    double rAvg=calcAvg(r,numParticipants);
    
    printf("average how many right answered question per person %lf\n",rAvg);
    
    //standard deviation
    printf("standard deviation: %lf\n",calcStdDev(rAvg,r,numParticipants));
        
    //median
    printf("median: %lf\n",calcMedian(r,numParticipants));
    printf("\n");
    
    ///////////////////////////////////////////////////
    // 3b) calculate right answers of all per question
    ///////////////////////////////////////////////////
    
    FILE *f2;
    
    int sumRightAnswers[NQUESTIONS];
    for(int i=0;i<NQUESTIONS;++i)
    {
        sumRightAnswers[i]=0;
    }
    
    f2=fopen("rightanswers_all.txt","w");
    fprintf(f2,"number of all right answers per question:\n");
    for(int i=0;i<numParticipants; ++i) 
    {
        if (numberRightAnswersAllPersons(argv[i+1],sumRightAnswers)!=0)
        {
            perror("Error reading file");
        }
    }
    
    for(int i=0;i<NQUESTIONS;++i)
    {
        fprintf(f2,"%c : %d\n",'A'+i,sumRightAnswers[i]);
    }
    fclose(f2);
    
    //average how many persons answered a single question right

    double av_all=calcAvg(sumRightAnswers,NQUESTIONS);
    printf("average how many persons answered a single question right: %lf\n",av_all);
    
    //standard deviation
    printf("standard deviation: %lf\n",calcStdDev(av_all,sumRightAnswers,NQUESTIONS));
    
    //median
    printf("median: %lf\n",calcMedian(sumRightAnswers,NQUESTIONS));
    
    printf("%d ",myrand()%4+1);
    printf("%d ",myrand()%4+1);
    
    if(r!=0)
    {
        free(r); 
        r=0;
    }

    return EXIT_SUCCESS;
}

