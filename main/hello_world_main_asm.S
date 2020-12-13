/*
This little bit of code is executed in-place by one CPU, but copied to a different memory region
by the other CPU. Make sure it stays position-independent.
*/
    .text
    .align  4
    .global testasm
    .type   testasm,@function
//Args:
//a2 - buffer
//a3 - mask
//a4 - notmask
//a5 - x
//a6 - y
//a7 - z
//a8 - upper mask
testasm:
   


  entry a1, 64
   
//creation mask
    //movi a9,0xf0f0f0f0
    movi a9,129
    ssl a3
    sll a9,a9
    movi.n a4, -1
    xor a4,a9,a4
    

    movi a9,128
    ssl a3
    sll a8,a9

    movi a9,1
    ssl a3
    sll a3,a9



    l16ui a9,a2,2
    and a9,a9,a4
    BBCI a5,7,zero7
    or a9,a9,a3
    zero7:
   // movi a9,147
    s16i a9,a2,2


    l32i a9,a2,12
    and a9,a9,a4
    BBCI a5,6,zero6
    or a9,a9,a3
    zero6:
    BBCI a5,5,zero5
    or a9,a9,a8
    zero5:
    s32i a9,a2,12 


    l32i a9,a2,24
    and a9,a9,a4
    BBCI a5,4,zero4
    or a9,a9,a3
    zero4:
    BBCI a5,3,zero3
    or a9,a9,a8
    zero3:
    s32i a9,a2,24


   l32i a9,a2,36
    and a9,a9,a4
    BBCI a5,2,zero2
    or a9,a9,a3
    zero2:
    BBCI a5,1,zero1
    or a9,a9,a8
    zero1:
    s32i a9,a2,36

   l32i a9,a2,48
    and a9,a9,a4
    BBCI a5,0,zero0
    or a9,a9,a3
    zero0:
    BBCI a6,7,zero70
    or a9,a9,a8
    zero70:
    s32i a9,a2,48


    //addi a2,a2,48 

    l32i a9,a2,12+48
    and a9,a9,a4
    BBCI a6,6,zero60
    or a9,a9,a3
    zero60:
    BBCI a6,5,zero50
    or a9,a9,a8
    zero50:
    s32i a9,a2,12+48 


    l32i a9,a2,24+48
    and a9,a9,a4
    BBCI a6,4,zero40
    or a9,a9,a3
    zero40:
    BBCI a6,3,zero30
    or a9,a9,a8
    zero30:
    s32i a9,a2,24+48


   l32i a9,a2,36+48
    and a9,a9,a4
    BBCI a6,2,zero20
    or a9,a9,a3
    zero20:
    BBCI a6,1,zero10
    or a9,a9,a8
    zero10:
    s32i a9,a2,36+48

   l32i a9,a2,48+48
    and a9,a9,a4
    BBCI a6,0,zero00
    or a9,a9,a3
    zero00:
    BBCI a7,7,zero700
    or a9,a9,a8
    zero700:
    s32i a9,a2,48+48


    addi a2,a2,96 

    l32i a9,a2,12
    and a9,a9,a4
    BBCI a7,6,zero600
    or a9,a9,a3
    zero600:
    BBCI a7,5,zero500
    or a9,a9,a8
    zero500:
    s32i a9,a2,12 


    l32i a9,a2,24
    and a9,a9,a4
    BBCI a7,4,zero400
    or a9,a9,a3
    zero400:
    BBCI a7,3,zero300
    or a9,a9,a8
    zero300:
    s32i a9,a2,24


   l32i a9,a2,36
    and a9,a9,a4
    BBCI a7,2,zero200
    or a9,a9,a3
    zero200:
    BBCI a7,1,zero100
    or a9,a9,a8
    zero100:
    s32i a9,a2,36

    l16ui a9,a2,48
    and a9,a9,a4
    BBCI a7,0,zero000
    or a9,a9,a3
    zero000:
    s16i a9,a2,48 

retw  




 /*
This little bit of code is executed in-place by one CPU, but copied to a different memory region
by the other CPU. Make sure it stays position-independent.
*/
    .text
    .align  4
    .global testasm2
    .type   testasm2,@function
//Args:
//a2 - buffer
//a3 - mask
//a4 - notmask
//a5 - x
//a6 - y
//a7 - z
//a8 - upper mask
testasm2:
   


  entry a1, 64
/*
 variables
*/

//a4 y
//a5 x
//a6 y1
//a7 x1


    movi a12,0x00aa00aa //AA
    movi a13,0x0000cccc //CC
    movi a14,0xf0f0f0f0 //FF
    movi a15,0x0f0f0f0f //FF2

//pretreans y
    l32i.n	a4, a2, 0
    srli	a9, a4, 7  //a9=(y>>7)
    xor     a9,a9,a4  //a9=y^(y>>7)
    and     a9,a9,a12 //a9=(y^(y>>7) & AA
    xor     a4,a9,a4
    srli    a9,a9, 7
    xor     a4,a9,a4

    srli	a9, a4, 14  
    xor     a9,a9,a4  
    and     a9,a9,a13 
    xor     a4,a9,a4
    srli    a9,a9, 14
    xor     a4,a9,a4
 
//pretranse x
    l32i.n	a5, a2, 4
    srli	a9, a5, 7  //a9=(y>>7)
    xor     a9,a9,a5  //a9=y^(y>>7)
    and     a9,a9,a12 //a9=(y^(y>>7) & AA
    xor     a5,a9,a5
    srli    a9,a9, 7
    xor     a5,a9,a5

    srli	a9, a5, 14  
    xor     a9,a9,a5  
    and     a9,a9,a13 
    xor     a5,a9,a5
    srli    a9,a9, 14
    xor     a5,a9,a5


//pretranse y1
    l32i.n	a6, a2, 8
    srli	a9, a6, 7  //a9=(y>>7)
    xor     a9,a9,a6  //a9=y^(y>>7)
    and     a9,a9,a12 //a9=(y^(y>>7) & AA
    xor     a6,a9,a6
    srli    a9,a9, 7
    xor     a6,a9,a6

    srli	a9, a6, 14  
    xor     a9,a9,a6  
    and     a9,a9,a13 
    xor     a6,a9,a6
    srli    a9,a9, 14
    xor     a6,a9,a6

//pretranse yx1
    l32i.n	a7, a2, 12
    srli	a9, a7, 7  //a9=(y>>7)
    xor     a9,a9,a7  //a9=y^(y>>7)
    and     a9,a9,a12 //a9=(y^(y>>7) & AA
    xor     a7,a9,a7
    srli    a9,a9, 7
    xor     a7,a9,a7

    srli	a9, a7, 14  
    xor     a9,a9,a7  
    and     a9,a9,a13 
    xor     a7,a9,a7
    srli    a9,a9, 14
    xor     a7,a9,a7



    //final trsn x y
     srli	a9, a4, 4
     and    a9,a9,a15
     and    a10,a5,a14
     or     a10,a9,a10 //new x
     and    a4,a4,a15
     slli   a5,a5, 4
     and    a5,a5,a14
     or     a4,a5,a4 //new y 

    //final trsn x1 y1
     srli	a9, a6, 4
     and    a9,a9,a15
     and    a11,a7,a14
     or     a11,a9,a11 //new x1
     and    a6,a6,a15
     slli   a7,a7, 4
     and    a7,a7,a14
     or     a6,a7,a6 //new y 

//x=a10
//x1=a11
//y=a4
//y1=a6
/*
 extui a9,a10,24,8
 extui a12,a11,24,8
 srli a12,a12,8
 or   a9,a9,a12
 s16i a9,a3,0

 extui a9,a10,16,8
 extui a12,a11,16,8
 srli a12,a12,8
 or   a9,a9,a12
 s16i a9,a3,10

 extui a9,a10,8,8
 extui a12,a11,8,8
 srli a12,a12,8
 or   a9,a9,a12
 s16i a9,a3,12


extui a9,a10,0,8
 extui a12,a11,0,8
 srli a12,a12,8
 or   a9,a9,a12
 s16i a9,a3,22


//y1 
 extui a9,a4,24,8
 extui a12,a6,24,8
 srli a12,a12,8
 or   a9,a9,a12
 s16i a9,a3,24

 extui a9,a4,16,8
 extui a12,a6,16,8
 srli a12,a12,8
 or   a9,a9,a12
 s16i a9,a3,34

 extui a9,a4,8,8
 extui a12,a6,8,8
 srli a12,a12,8
 or   a9,a9,a12
 s16i a9,a3,36


 extui a9,a4,0,8
 extui a12,a6,0,8
 srli a12,a12,8
 or   a9,a9,a12
 s16i a9,a3,46
*/


  retw