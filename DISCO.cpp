#include "allmyfile.h"

double FuncC(long c, double a)//1+a=b
{
	double x = (pow( (1+a), (double)c)-1)/a;
	return x;
}


//double ReFuncC(double n, double a)
//{
//	double x = (log(n*a+1) ) / (log(a + 1) );
//	return x;
//}

long DISCOUpdate(long c, long l, double a)
{
	double v = (double)(rand()/(double)RAND_MAX);

	double delta = log(a*l/(double)pow((1 + a), c)+1)/log(1+a);

	double pd = delta - (long)delta;

	if(v <= pd)
		return (long)(delta+1);
	else
		return (long)(delta);
}

long DISCO_renor(double a1, double a2, long c1)// previous a1,c1;next a2,c2;
{
    double v = (double)(rand()/(double)RAND_MAX);//generate a random number 0<x<1
    long c2;
    double c2_temp;
    c2_temp = log(a2/a1*(pow(1+a1, (double)c1)-1)+1.0)/log(1+a2);
    double pd = c2_temp - (long)c2_temp;
    if(v <= pd)
    {
        c2 = (long)(c2_temp+1);
    }
    else
    {
        c2 = (long)c2_temp;
    }
    return c2;
}

/*************************OEF********************************/

long OEFUpdate(long c, long l, double a)
{
    //a equals to 2e2 in the optimal estimation function
    double v = (double)(rand()/(double)RAND_MAX);

    double f_c = FC_OEF(c, a);//f(c)
    double delta = INV_FC_OEF((double)l+f_c, a) - c;
    delta = floor(delta);//delta
    double f_c_d = FC_OEF(c + (long)delta, a);//f(c+d);
    double f_c_d_1 = FC_OEF(c + (long)delta + 1, a);//f(c+d+1);
    double temp_1 = (double)l + f_c - f_c_d;
    double temp_2 = f_c_d_1 - f_c_d;
    double pd = temp_1/temp_2;

    if(v <= pd)
		return (long)(delta+1);
	else
		return (long)(delta);
}

double FC_OEF(long c, double a)
{
    double x = (pow( (1+a), (double)c)-1)/a*(2+a)/2;
    return x;
}

double INV_FC_OEF(double n, double a)
{
    double temp_1 = log(2*a*n + 2 + a)-log(2 + a);
    double temp_2 = log(1 + a);
    double x = temp_1/temp_2;
    return x;
}

long OEF_renor(double a1, double a2, long c1)// previous a1,c1;next a2,c2;
{
    double v = (double)(rand()/(double)RAND_MAX);//generate a random number 0<x<1
    long c2;
    double c2_temp;
    c2_temp = log(a2*(2+a1)/a1/(2+a2)*(pow(1+a1, (double)c1)-1)+1.0)/log(1+a2);
    double pd = c2_temp - (long)c2_temp;
    if(v <= pd)
    {
        c2 = (long)(c2_temp+1);
    }
    else
    {
        c2 = (long)c2_temp;
    }
    return c2;
}




