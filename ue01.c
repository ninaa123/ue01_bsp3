/* 
 * File:   uebung01_bsp03.c
 * Author: nina
 *
 * Created on 5. März 2014, 16:49
 */

#include <stdlib.h>     // error handling
#include <stdio.h>      // file io
#include <math.h>       // sqrt()

#define NQUESTIONS 20   ///< Number of questions

const int answers[20]={2,4,2,3,3,1,4,1,2,1,3,1,1,4,4,1,1,4,2,3};

unsigned int myrand() 
{
    static unsigned int randomNumber=2;
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
    
    for(int i=1;i<(n+1);++i)
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


double calcMedian(int *r, int totalnumb)
{
    int *a = (int *)malloc(totalnumb * sizeof(int));
    int *b = (int *)malloc(totalnumb * sizeof(int));

    qsort(r,totalnumb,sizeof(int),cmpIntAsc); //sorts array

    if((sizeof(r)/sizeof(r[0]))%2!=0)
    {
        return r[(sizeof(r)/(2*sizeof(r[0])))];
    }
    else
    {
        int k = (sizeof(r)/(2*sizeof(r[0])));
        return (double) (r[k-1]+r[k])/2;
    }
}



int main(int argc, char** argv) 
{
    // DECLARATIONS
    unsigned int nFile;                     // file Counters
    
    int numParticipants = argc - 1;
    
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
    
    f=fopen("f.txt","w");
    fprintf(f,"number of right answers per participant:\n");
    for(int i = 0; i < numParticipants; ++i) 
    {
        r[i]=numberRightAnswersPerson(argv[i+1]);
        fprintf(f,"participant %c : %d \n", i+1, r[i]); //write right answer per participant in file
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
    
    FILE *rightanswers_all;
    
    int sumRightAnswers[NQUESTIONS];
    for(int i=0;i<NQUESTIONS;++i)
    {
        sumRightAnswers[i]=0;
    }
    
    rightanswers_all=fopen("rightanswers_all.txt","w");
    fprintf(rightanswers_all,"number of all right answers per question:\n");
    for(nFile=1; nFile<((unsigned int)argc); ++nFile) 
    {
        if (numberRightAnswersAllPersons(argv[nFile],sumRightAnswers)!=0)
        {
            perror("Error reading file");
        }
    }
    
    for(int i=0;i<NQUESTIONS;++i)
    {
        fprintf(rightanswers_all,"%c : %d\n",'A'+i,sumRightAnswers[i]);
    }
    fclose(rightanswers_all);
    
    int sum2=0;
    for(int i=0;i<(NQUESTIONS);++i)
    {
        sum2+=sumRightAnswers[i];
    }
    printf("%d\n",sum2);
    
    int sum3=0;
    for(int i=1;i<(NQUESTIONS+1);++i)
    {
        sum3+=sumRightAnswers[i];
    }
    printf("%d\n",sum3);
    
    //average how many persons answered a single question right

    double av_all=calcAvg(sumRightAnswers,NQUESTIONS);
    printf("average how many persons answered a single question right: %lf\n",av_all);
    
    //standard deviation
    printf("standard deviation: %lf\n",calcStdDev(av_all,sumRightAnswers,NQUESTIONS));
    
    //median
    printf("median: %lf\n",calcMedian(sumRightAnswers,NQUESTIONS));
    
    printf("%d ",myrand());
    printf("%d ",myrand());
    
    if(r!=0)
    {
        free(r); 
        r=0;
    }

    return EXIT_SUCCESS;
}

