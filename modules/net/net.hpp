#ifndef NET_HPP
#define NET_HPP

#include "../../agb.h"
#include "../../data/control.hpp"

typedef enum eNetMode {
	NET_GBA,
	NET_IBM,
	NET_SYNC_SLAVE,
	NET_SYNC_MASTER,
} NetMode;

class Net {
	public:
		static NetMode netmode;
		static void init();
		static void setMode(NetMode mode); 
		static void clear();
		static void update();
		static void recvSync();
		static void sendSync();
		static void gbaSync();
		static void ibmSync();
		static void alterMode(Control *c, bool bigstep, bool add, u32 *pointer);
};

#endif
