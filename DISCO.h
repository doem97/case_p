#ifndef DISCO_H_INCLUDED
#define DISCO_H_INCLUDED

double FuncC(long c, double a);//1+a=b

long DISCOUpdate(long c, long l, double a);

long DISCO_renor(double a1, double a2, long c1);// previous a1,c1;next a2,c2;

/*************************OEF********************************/
long OEFUpdate(long c, long l, double a);
double FC_OEF(long c, double a);
double INV_FC_OEF(double n, double a);
long OEF_renor(double a1, double a2, long c1);// previous a1,c1;next a2,c2;

#endif // DISCO_H_INCLUDED
