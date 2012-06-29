/*
   a
  ---
f| g |b   
  ---
e|   |c   
  ---
   d
*/
#define Sega    0x01
#define Segb    0x02
#define Segc    0x40
#define Segd    0x20
#define Sege    0x10
#define Segf    0x04
#define Segg    0x08

#define Num0    (Sega + Segb + Segc + Segd + Sege + Segf       )
#define Num1    (           Segb + Segc                                       )
#define Num2    (Sega + Segb + Segd + Sege + Segg)
#define Num3    (Sega + Segb + Segc + Segd               + Segg)
#define Num4    (           Segb + Segc               + Segf + Segg)
#define Num5    (Sega            + Segc + Segd + Segf + Segg)
#define Num6    (Sega            + Segc + Segd + Sege + Segf + Segg)
#define Num7    (Sega + Segb + Segc                            )
#define Num8    (Sega + Segb + Segc + Segd + Sege + Segf + Segg)
#define Num9    (Sega + Segb + Segc + Segd        + Segf + Segg)
#define Numblk  (0                                             )

INT8U  Num_code[11] = {Num0,Num1,Num2,Num3,Num4,Num5,Num6,Num7,Num8,Num9,Numblk};
