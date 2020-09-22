#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main()
{
  char buffer[255]; 
  char buff[255];
  int num[255]={0};
  int nmbs;
  int sum = 0;
  int n = 0;
  double low;

  //get data
  while(fgets(buffer, (sizeof buffer / sizeof buffer[0]), stdin))
  {
    if(!isdigit(buffer[0]))
    {
      nmbs = atoi(&buffer[2]);
      buff[n] = buffer[0];
      sum += nmbs;
      num[buffer[0]] = nmbs;
      n++;
    }
    else
    {
      low = atof(&buffer[0]);
    }
    
  }

  // get range
  double nm;
  double left[255]={0};
  double right[255]={0};

  for(int i=0;i<n;i++)
  {
    nm = num[buff[i]];
    if(i == 0)
    {
      left[buff[i]] = 0;
      right[buff[i]] = nm/sum;
    }
    else
    {
      nm = num[buff[i]];
      left[buff[i]] = right[buff[i-1]];
      right[buff[i]] = right[buff[i-1]] + nm/sum;
    }

    
  }
  for(int i=0;i<sum;i++)
  {
    int j = 0;
    while(right[buff[j]] <= low +0.000001)
    {
      j++;
    }
    printf("%c",buff[j]);
    double range = right[buff[j]] - left[buff[j]];
    low = (low - left[buff[j]])/range;


  }

  return 0;
}
