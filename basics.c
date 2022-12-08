#include <stdio.h>
#include <stdlib.h>

struct Food
{
  int foodId;
  char amount;
  float sugarAmt;
  double calories;
};

int addTwoNumbers(int *num1, int *num2)
{
  return *num1 + *num2;
}

int main()
{
  // create this
  int lucky = 26;
  printf("address = %d \n", &lucky);

  printf("hello value = %d \n", lucky);

  int *ptr = &lucky;

  printf("hello now = %d \n", *ptr);

  printf("sum after function %d", addTwoNumbers(ptr, ptr));

  return (0);
}