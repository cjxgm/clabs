 main()
  {  int x,y,z;
    for(x=0;x<=100;x++)
      for(y=0;y<=100;y++)
	{z=100-x-y;
	  if(15*x+9*y+z==300)
	  printf("x=%-8dy=%-8dz=%-8d\n",x,y,z);
	  }
	  }