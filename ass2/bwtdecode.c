#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
  //read file
  FILE *fp1 = fopen(argv[1],"r");
  FILE *fp2 = fopen(argv[2],"w");
  fseek(fp1,0,SEEK_END);
  int len_str;
  len_str = ftell(fp1);
  fseek(fp1,0,SEEK_SET);
  int slen = len_str/4;
  if(slen*4<len_str) slen++;
  unsigned char *last = (unsigned char *)malloc(slen * sizeof(unsigned char));
  int olen = len_str/128;
  if(olen*128<len_str) olen++;
  int **occ = (int **)malloc(olen * sizeof(int *));
  char *aim = "\nACGT";
  int matching[100] = {0};
  int position;
  int epst;
  for(int i=0;i<5;++i)
  {
    matching[aim[i]] = 0;
  }
  int n=0;
  int m=1;
  while(n<slen)
  {
    unsigned char ucurchar = 0b00000000;
    int left = 4;
    if(len_str-4*n<4) left = len_str-4*n;
    char curchar[left+1];
    fread(curchar, left, sizeof(char), fp1);
    curchar[left]='\0';
    for(int i=0;i<left;++i)
    {
      char ch=curchar[i];
      if(ch=='\n') 
      {
        position = n*4+i;
        epst = n*4+i;
        
      }
      if(ch=='C')  
      {
        ucurchar |=1 << 2*(3-i);
        ucurchar |=0 << (2*(3-i)+1);
      }
      if(ch=='G')  
      {
        ucurchar |=0 << 2*(3-i);
        ucurchar |=1 << (2*(3-i)+1);
      }  
      if(ch=='T')  
      {
        ucurchar |=1 << 2*(3-i);
        ucurchar |=1 << (2*(3-i)+1);
      }
      matching[ch]++;
    }
    if(n+1 == m*32)
    {
      occ[m-1] = (int *)malloc(4 * sizeof(int));
      occ[m-1][0] = matching['A'];
      occ[m-1][1] = matching['C'];
      occ[m-1][2] = matching['G'];
      occ[m-1][3] = matching['T'];     
      m++;
    }
    last[n] = ucurchar;
    n++;
  }
  if(m*32-n!=32 && m*32-n>0)
  {
    occ[m-1] = (int *)malloc(4 * sizeof(int));
    occ[m-1][0] = matching['A'];
    occ[m-1][1] = matching['C'];
    occ[m-1][2] = matching['G'];
    occ[m-1][3] = matching['T'];
    m++;
  } 

  //get C table
  int ctable[100] = {0};
  for(int i=0;i<5;++i)
  {
    if(i == 0) ctable[aim[i]] = 0;
    else
    {
      ctable[aim[i]] = ctable[aim[i-1]] + matching[aim[i-1]];
    }
  }
  
  //decode and output
  int haveop = 0;
  int start=0;
  char *buffer = (char *)malloc(10001*sizeof(char));
  while (haveop < len_str)
  {
    int blen=10000;
    if (len_str-haveop<10000) blen=len_str-haveop;
    
    for(int i=0;i<blen;i++)
    {
      char aimchar;
      int lposition = position/4;
      int end = (position/128+1)*32;
      if(end>slen) end=slen;
      int rposition = position%4;
      unsigned char fcheck = 0b00000000;
      unsigned char scheck = 0b00000000;
      scheck |=1 << 2*(3-rposition);
      fcheck |=1 << (2*(3-rposition)+1);
      int fint = fcheck & last[lposition];
      int sint = scheck & last[lposition];
      if(fint==0)
      {
        if(sint==0)
        {
          aimchar='A';
        }
        else
        {
          aimchar='C';
        }
        
      }
      else
      {
        if(sint==0) 
        {
          aimchar='G';
        }
        else
        {
          aimchar='T';
        }
      }
      if(start==0) aimchar='\n';start=1;
      buffer[blen-i-1] = aimchar;
      
      int count = 0;
      for(int g=lposition;g<end;++g)
      {
        for(int k=0;k<4;++k)
        {
          char checkchar;
          fcheck = 0b00000000;
          scheck = 0b00000000;
          scheck |=1 << 2*(3-k);
          fcheck |=1 << (2*(3-k)+1);
          int fint = fcheck & last[g];
          int sint = scheck & last[g];
          if(fint==0)
          {
            if(sint==0)
            {
              checkchar='A';
            }
            else
            {
              checkchar='C';
            }
            
          }
          else
          {
            if(sint==0) 
            {
              checkchar='G';
            }
            else
            {
              checkchar='T';
            }
          }
          if(checkchar==aimchar && g*4+k>position && g*4+k<len_str && g*4+k!=epst) 
          {
            count++;
          }
        }
      }
      int occnb = 0;
      int p;   
      //printf("position: %d, ",position);
      if(aimchar=='\n') position=0;
      else
      {
        if(aimchar=='A') p=0;
        if(aimchar=='C') p=1;
        if(aimchar=='G') p=2;
        if(aimchar=='T') p=3;
        int pp =position/128;
        occnb = occ[pp][p]-1;
        position = occnb + ctable[aimchar] - count;        
      }   
      //printf("%c, occnb: %d, ctable:%d, count:%d, next:%d\n",aimchar,occnb,ctable[aimchar],count,position);
    }
    buffer[blen] = '\0';
    haveop += blen;
    fseek(fp2,len_str-haveop,SEEK_SET);
    fprintf(fp2,"%s",buffer);   
  }
  
  
  
  //free memory
  free(last);
  free(occ);
  free(buffer);
  fclose(fp1);
  fclose(fp2);

  return 0;
}
