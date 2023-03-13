/*------------------------------------------------------------------------------
                               Key Control Class                                
--------------------------------------------------------------------------------
 This module should hold a copy of the keyboard register, which is updated every
 time the keypad generates an interrupt. It's faster than continuously reading 
 the register, as it's wait times are higher than for mem in WRAM. 
 So it has to return KeyDown, KeyUp and KeyRepeat status for all keys.Thats all.
------------------------------------------------------------------------------*/

cKEY KEY;


// INPUT REGISTER = LRdulrsSBA (s=st)
void cKEY::init(void)
{
	*(u16*)REG_KEYCNT = 0x43FF;//Set Key Interrupt on every button
	keyrate = 40;
	retrig  = false;
}

void cKEY::update(void)
{
	R_IME=0x0;
	u8 i  = 0;
	
	keytrig = ~((*(u16*)REG_KEYINPUT));
	keyinput = keytrig;
	
	u16	kt = 0x0000,
		kd = 0x0000,
		kp = 0x0000,
		ku = 0x0000;
	u16 ct = 0x0000,
		cd = 0x0000,
		cp = 0x0000,
		cu = 0x0000;

	while(i<10)
	{
		u16 pos = 1<<i;
		ct = (keytrig  & pos)>>i;
		cd = (keydown  & pos)>>i;
		cp = (keypress & pos)>>i;
		cu = (keyup    & pos)>>i;
	
	
		if( cd)
		{
			if(!ct)cd=0;
		}

		if( ct)
		{			
			if(!cu)if(!cd)if( cp){cp=1;		}
			if( cd)if(!cu)if(!cp){cp=1;cd=0;}
			if(!cp)if(!cd)if(!cu){cd=1;		}
		}
		
		if( cu)cu=0;
		
		if( cp)
		{
			if(!ct)if(!cu)if(!cd){cu=1; cp=0;}
		}

		kt	|=	ct<<i;
		kd	|=	cd<<i;
		kp	|=	cp<<i;
		ku	|=	cu<<i;
		
		i++;
	}
	
	keytrig	= kt;
	keydown	= kd;
	keypress= kp;
	keyup	= ku;
	if(((!keytrig)&&(!keyup))&&(!keypress)&&(!keydown))SYS_QUERYKEY = false;
	R_IME=0x1;	
}

bool cKEY::down(u16 keyflag)
{
	if((keydown & keyflag)==keyflag)return(true);
	return(false);
}

bool cKEY::up(u16 keyflag)
{
	if((keyup & keyflag)==keyflag)return(true);
	return(false);
}

bool cKEY::press(u16 keyflag)
{	
	if((keypress & keyflag)==keyflag)return(true);		
	return(false);
}

bool cKEY::repeat(u8 index)
{
	u16 keyflag = (1 <<index);
	if((keypress & keyflag)==keyflag)
	{
		keytimer[index]++;
		if(keytimer[index] > keyrate)
		{
			keytimer[index]=0x0;
			return(true);
		}
		
	}
	return(false);
}

bool cKEY::activity(){
	return(keypress || keytrig || keyup );
}

void cKEY::forceNoInput(){
	while( activity() ){ update(); }
}
