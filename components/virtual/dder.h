//
//  Header.h
//  
//
//  Created by Yves BAZIN on 22/09/2020.
//

#ifndef Header_h
#define Header_h
#include <stdio.h>

//extern int externdef;
extern int staticdef;

class dder{
public:
    dder()
    {
        //externdef=41;
        staticdef=78;
    }
    static void getvar() {
        printf("%d\n",staticdef);
       s();
    }
    static void s();
    
};

#endif /* Header_h */
