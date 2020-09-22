#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
  //read file
  FILE *fp1 = fopen(argv[1],"r");
  fseek(fp1,0,SEEK_END);
  int len_str;
  len_str = ftell(fp1);
  fseek(fp1,0,SEEK_SET);
  char buff[1];

  //get Occ
  int olen = len_str/1024;
  if(olen*1024<len_str) olen++;
  int **occ = (int **)malloc(olen * sizeof(int *));
  char *aim = "\nACGT";
  int matching[100] = {0};
  int position;
  for(int i=0;i<5;++i)
  {
    matching[aim[i]] = 0;
  }
  int n=0;
  int m=1;
  while(n<olen)
  {
    int left = 1024;
    if(len_str-1024*n<1024) left = len_str-1024*n;
    char curchar[left+1];
    fread(curchar, left, sizeof(char), fp1);
    curchar[left]='\0';
    for(int i=0;i<left;++i)
    {
      char ch=curchar[i];
      if(ch=='\n')position=i;
      matching[ch]++;
    }
    occ[n] = (int *)malloc(4 * sizeof(int));
    occ[n][0] = matching['A'];
    occ[n][1] = matching['C'];
    occ[n][2] = matching['G'];
    occ[n][3] = matching['T'];     
    n++;
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
  
  //search
  char *buffer=(char *)malloc(sizeof(int));;
  while(fgets(buffer, len_str, stdin))
  {
    int leni = strlen(buffer)-1;
    char c = buffer[leni-1];
    int fposition = ctable[c];
    int lposition = ctable[c] + matching[c] - 1;
    //printf("%c: %d,%d\n",c,fposition,lposition);
    while(fposition<=lposition && leni-->=2)
    {
      int fop = fposition/1024;
      int fend = (fop+1)*1024;
      if(fend>len_str) fend=len_str;
      int lop = lposition/1024;
      int lend = (lop+1)*1024;
      if(lend>len_str) lend=len_str;
      c = buffer[leni-1];
      int i,j;
      int focc = 0;
      int locc = 0;
      int fcount = 0;
      int lcount = 0;
      int p;
      if(c=='A') p=0;
      if(c=='C') p=1;
      if(c=='G') p=2;
      if(c=='T') p=3;
      //printf("%d %d",p,fop);
      fseek(fp1,fposition,SEEK_SET);
      for(i=fposition;i<fend;++i)
      {
        fread(buff, 1, sizeof(char), fp1);
        if(buff[0]==c)
        {
          fcount++;
        }
      }
      focc = occ[fop][p] - fcount;
      fseek(fp1,lposition+1,SEEK_SET);
      for(i=lposition+1;i<lend;++i)
      {
        fread(buff, 1, sizeof(char), fp1);
        if(buff[0]==c)
        {
          lcount++;
        }
      }
      locc = occ[lop][p] - lcount;
      //printf("%c: fp:%d,fend:%d,focc:%d,fcount:%d\n",c,fposition,fend,occ[fop][p] ,fcount);
      //printf("%c: lp:%d,lend:%d,locc:%d,lcount:%d\n",c,lposition,lend,occ[lop][p] ,lcount);
      fposition = focc + ctable[c];
      lposition = locc + ctable[c] - 1;
    }
    if(fposition>lposition) printf("%d\n",0);
    else printf("%d\n",lposition-fposition+1);
  }
 
  //free memory
  free(occ);
  fclose(fp1);

  return 0;
}
