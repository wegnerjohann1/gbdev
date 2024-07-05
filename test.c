#include <time.h>
#include <defs.h>
#include <string.h>
#include "raygui.h"

typedef struct 
{
    int a;
    int b;
} myStruct;

myStruct structs[2] =
{
    {1, 2},
    {3, 4}
};

myStruct foo()
{
    return structs[0];
}

int main()
{
    myStruct s = foo();
    printf("%d\n", s.a);
    s.a = 2;
    printf("%d\n", s.a);
    return 0;
}