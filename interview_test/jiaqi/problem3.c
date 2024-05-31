#include <stdio.h>
#include <stdlib.h>

// main(x,y,z,p){while(y=++x)for(z=0;z<y?z=z*8+y%8,y/=8,p=z==y|z/8==y,1:(x-++p||printf("%o\n",x))&&x%p;);}

// main(x,y,z,p) {
//     printf("x=%d, y=%d, z=%d, p=%d\n", x, y, z, p);
//     while(y = ++x)
//     for(z=0;
//         z < y ? z = z * 8 + y % 8,
//         y /= 8 , p = z == y | z / 8 == y,
//         1 : ( x - ++p || printf("%o\n",x)) && x%p;
//         );
// }

// main(x,y,z,p) {
//     printf("x=%d, y=%d, z=%d, p=%d\n", x, y, z, p);
//     while(y = ++x) {
//         // printf("---------%d\n", y);
//         for(z=0;
//             z < y ?
//             z = z * 8 + y % 8, y /= 8 , p = (z == y) | ((z / 8) == y), 1 :
//             ((x - ++p) || printf("%o\n",x)) && x % p;
//             );
//     }
// }

main(x,y,z,p) {
    printf("x=%d, y=%d, z=%d, p=%d\n", x, y, z, p);
    while(y = ++x) {
        // printf("---------%d\n", y);
        // for(z=0;
        //     z < y ?
        //     z = z * 8 + y % 8, y /= 8 , p = (z == y) | ((z / 8) == y), 1 :
        //     ((x - ++p) || printf("%o\n",x)) && x % p;
        //     );
        for (z=0;;) {
            if (z < y) {
                z = z * 8 + y % 8;
                y /= 8;
                p = (z == y) | ((z / 8) == y);
                continue;
            }
            ++p;
            if (x - p) {
                printf("%o\n",x);
            }
            if ((x - p) && (x % p)) {
                continue;
            } else {
                break;
            }
        }
    }
}

/*
Part 1:What does this code output?  Why?


Part 2:Describe what x, y, z, pare used for.
    (1) in parameters of main, x, y, z, p are declared to int.
    x is used to store count of commands parameters, always init to 1.
    (2) in body of main,
       x increase by 1, y is assigned to x. thus x = x + 1, y = x.
       z is used to store the result of z * 8 + y % 8, y /= 8, p = (z == y) | ((z / 8) == y).

Part 3:Remove the,(comma) operator.
Part 4:Remove the ternary operator.
Part 5:Rewrite the code to be easy to understand.  Add at least 3 comments
*/