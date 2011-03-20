main()
{
long f1,f2;
int i;
f1=f2=1;
for(i=1;i<=20;i++)
{ printf("%12ld %12ld",f1,f2);
if(i%2==0) printf("\n");
f1=f1+f2; 
f2=f1+f2; 
}
}

