#include <stdio.h>
#include <string.h>
 

int main()
{
  char ori_buf[255];
  char buffer[255]; 
  char buff;
  fgets(buffer, (sizeof buffer / sizeof buffer[0]), stdin);

  strcpy(ori_buf,buffer);
  //sort
  int numbs = strlen(buffer);
  for(int i=1;i<numbs;i++)
  {
    int j = i;
    while(strcmp(&buffer[j],&buffer[j-1])<0)
    {
      buff = buffer[j-1];
      buffer[j-1] = buffer[j];
      buffer[j] = buff;
      j--;
    }
  }
  // get probability
  int num[255]={0};
  double left[255]={0};
  double right[255]={0};

  for(int i=0;i<numbs;i++)
  {
    num[buffer[i]]++;
  }
  double nm;
  for(int i=0;i<numbs;i++)
  {
    nm = num[buffer[i]];
    if(i == 0)
    {
      left[buffer[i]] = 0;
      right[buffer[i]] = nm/numbs;
      printf("%c %d\n",buffer[i],num[buffer[i]]);
    }
    else
    {
      if(buffer[i] != buffer[i-1])
      {
        nm = num[buffer[i]];
        left[buffer[i]] = right[buffer[i-1]];
        right[buffer[i]] = right[buffer[i-1]] + nm/numbs;
        printf("%c %d\n",buffer[i],num[buffer[i]]);
      }
    }
  }
  // account value
  double low = 0.0;
  double high = 1.0;
  double dist;

  for(int i=0;i<numbs;i++)
  {
    if(i == 0)
    {
      low = left[ori_buf[i]];
      high = right[ori_buf[i]];
    }
    else
    {
      dist = high - low;
      low = low + dist * left[ori_buf[i]];
      high = low + dist * (right[ori_buf[i]] - left[ori_buf[i]]) ;
    }
  }
  
  printf("%.10f %.10f\n",low,high);
  return 0;
}
