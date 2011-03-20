void ppp(unsigned long int k)
{ union aa
  { unsigned int a[2];
    unsigned long int b;
    } c,*p;
    p=&c;
    c.b=k;
    printf("\n k=%lx",k);
    printf("\n low=%0x  high=%0x",p->a[0],p->a[1]);
    }
    main()
     { unsigned long int w;
	w=0x12345678;
	ppp(w);
	}