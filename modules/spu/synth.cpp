#include "synth.hpp"

const u16 SMP_FREQ_TABLE[120]={ 
	0xF8DD	,0xF917	,0xF94F ,0xF995	,0xF9D7	,0xFA22	,0xFA68	,0xFAB2	,0xFAF8	,0xFB3E	,0xFB80, 0xFBC0,
	0xFBFC	,0xFC36	,0xFC6C	,0xFCA0	,0xFCD0	,0xFCFE	,0xFD29	,0xFD52	,0xFD78	,0xFD9C	,0xFDBF	,0xFDDF	,
	0xFDFD	,0xFE1A	,0xFE35	,0xFE4F	,0xFE67	,0xFE7E	,0xFE94	,0xFEA8	,0xFEBB	,0xFECD	,0xFEDF	,0xFEEF	,
	0xFEFE	,0xFF0C	,0xFF1A	,0xFF27	,0xFF33	,0xFF3E	,0xFF49	,0xFF53	,0xFF5D	,0xFF66	,0xFF6F	,0xFF77	,
	0xFF7E	,0xFF85	,0xFF8C	,0xFF93	,0xFF99	,0xFF9E	,0xFFA4	,0xFFA9	,0xFFAE	,0xFFB2	,0xFFB7	,0xFFBB	,
	0xFFBE	,0xFFC2	,0xFFC5	,0xFFC9	,0xFFCC	,0xFFCE	,0xFFD1	,0xFFD4	,0xFFD6	,0xFFD8	,0xFFDB	,0xFFDD	,
	/**/
	0xFFDE	,0xFFE0	,0xFFE2	,0xFFE4	,0xFFE5	,0xFFE6	,0xFFE8	,0xFFE9	,0xFFEA	,0xFFEB	,0xFFED	,0xFFEE	,//INAUDIBLE OVER TOP
	0xFFEE	,0xFFEF	,0xFFF0	,0xFFF1	,0xFFF2	,0xFFF2	,0xFFF3	,0xFFF4	,0xFFF4	,0xFFF5	,0xFFF6	,0xFFF6	,//INAUDIBLE OVER TOP
	0xFFF6	,0xFFF7	,0xFFF7	,0xFFF8	,0xFFF8	,0xFFF8	,0xFFF9	,0xFFF9	,0xFFF9	,0xFFFA	,0xFFFA	,0xFFFA	,//INAUDIBLE OVER TOP
	0xFFFA	,0xFFFB	,0xFFFB	,0xFFFB	,0xFFFB	,0xFFFB	,0xFFFC	,0xFFFC	,0xFFFC	,0xFFFC	,0xFFFC	,0xFFFC	//INAUDIBLE OVER TOP
	//0xFFFE	,0xFFFE	,0xFFFE	,0xFFFE	,0xFFFE	,0xFFFE	,0xFFFE	,0xFFFE	,0xFFFE	,0xFFFE	,0xFFFE ,0xFFFE	//INAUDIBLE OVER TOP
};

