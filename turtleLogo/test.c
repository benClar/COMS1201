    int oldY, oldX;
    oldX = getTargetX();
    oldY = getTargetY();

	    testVal(oldX,getTargetX(),"Valid: Turtle moves right along x axis only",LESS);
    testVal(oldY,getTargetY(),"Valid: Turtle y axis remains the same",EQUALS);
