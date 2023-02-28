/*
---------------------------------------------------------------------------------------------
M's observations at September 29
---------------------------------------------------------------------------------------------
Gracias por molestarte en mandar el código, sólo he echado una ojeada por encima. 
Como siempre, no me he molestado en comentar o analizar la 'arquitectura', solamente 
he aspectos acerca del código en si. 
Esta vez tampoco voy a decir nada acerca de los graficos o colores; 
Voy a centrarme en el codigo. Aunque es un poco largo por favor leelo con detalle,
porque es importante para que tu projecto vaya a buen termino. 
He añadido barras horizontales para dividir el texto en secciones. 
Tras leer el texto de nuevo noto que tiene un tono un poco brusco. 
Esa no es mi intención, recuerda que no te estoy echando la bronca! :)
------------------------------ 
El crt0 no limpia la sección de datos no inicializados. Esto es muy importante! 
No sigas sin arreglar esto. 
------------------------------ 
Haciendo un profiling rápido encuentro que el código pasa una cantidad de tiempo enorme 
ejecutando las funciones cKEY::Update y INT_VBlank, incluso al no hacer nada. 
------------------------------ 
Hay una ausencia total de consistencia en el estilo de todo el código, 
incluyendo nombres de funciones, estructuras etc: 

Nombres de funciones:    
	_add_1BIT    
	HEXADECIMAL    
    overloadTest    
	start_vector    
	IntHandler    
	INT_VBlank 
Nombres de métodos:    
	cINT::Init    
	GPU::safeblit    
	REGHND::dispatchMessages 
Nombres de clases y estructuras:    
	cINT    GPU    
	REGHND    t_chunk    
	PATTERN_CELL 
Nombres de enums: 
	enum E_Messages 
	enum E_DISPLAY_MAP_5 
Uso de tipos de tamaño generico cuando hacen falta tipos de tamaño especifico: 
	const unsigned short Palette[] (debe ser const u16 Palette[]) 
Nombres de instancias de objetos globales:    
	GPU gpu    
	cINT INT    
	SYS_TIMER 
Nombres de miembros:    
	VirtualScreen::data    
	GPU::MAP0    
	GPU::vs 
Nombres de tablas constantes:    
	Palette    
	TILESET_0    
	IntrSender 
Nombres y contenidos de macros:    
	PREFETCH (por cierto para que hacer una macro para esto?)    
	SystemCall(n)    
	DEFINT(a,b)    
	PROGRAM_NAME    
	_VER_M    
	_VER_m 
Codigo repetido: 
	REGHND::controlTrigger 
	REGHND::controlAdd 
	REGHND::controlSub 
Numeros magicos: 
	callbacks.c:90 (y callbacks.c en general) 
	int.cpp:59 hasta int.cpp:65 
	key.cpp:16 
	tim.cpp:28 
'Stunts' peligrosisimos: 
	regionhandler.cpp:186 
	regionhandler.cpp:202 
------------------------------ 
Hay una regla del estándar C muy clara que dice lo siguiente: 
Los nombres de identificadores que el programador escribe no deben empezar por '_' (barra baja). 
Las barras bajas al comienzo sirven para definir un 'espacio de nombres' para el compilador y compañia. 
Basicamente esta es la idea:     
	-Sin barra baja: lo que escribe el que usa C (e.g. tú)     
	-Una barra baja: lo que escribe el que programa la libreria C estandar y es interno a la librería.       
	
		Por ejemplo, si malloc usase una función interna para pedir memoria al sistema operativo, 
		el programador de esa funcion interna debería ponerle una barra baja: 
	
		void *_pedir_memoria(size_t s); 
		void *malloc(size_t s) {     
			[...]     
			void *p = _pedir_memoria(s);     
			[...] 
		}    
	
	-Dos barras bajas: lo que escribe el que programa el compilador cuando tiene que programar 
		una funcion o variable que 'genera' el compilador y es utilizada internamente para 
		implementar el estandar.
		
		Por ejemplo, va_arg y compañia en realidad en muchos sitios se escriben parecido a:
		
		#define va_start(a,b)   
		__builtin_va_start((a),(b)) 
		#define va_arg(a,b)    
		__builtin_va_arg((a),(b))    
		
		donde las 'funciones' __builtin_... las genera (por decirlo de algun modo) el compilador. 
		
		Esto incluye tambien nombres de macros enums structs etc etc 
------------------------------ 
Comentas poco y mal el codigo. 
	Cuando comentes escribe:    
	1. 	Comentarios que describan el porque del codigo (es decir, el algoritmo, la estructura de la funcion...) 
			Estos los pones al comienzo del codigo de una funcion o al comienzo de un archivo de codigo. 
			Que se lea como texto, con parrafos etc y mas bien siendo 'generoso'    
	2. 	Advertencias, qué significa un determinado número mágico, si escribes algo de forma poco ortodoxa (e.g. for(;++i<1024;))
			Estos los pones lo mas cerca posible del punto conflictivo pero que salten a la vista. 
			Yo los pongo asi:        
			
			[...]
			// direccion del puerto serie        
			*((u16 *) 0x12345678) = ...;        
			[...]
			
			o asi si son mas largos:
			
			[...]
			//        
			// Esto es asi por un bug en el compilador version X        
			// que genera un opcode erroneo        
			//        
			// en realidad pretende hacer esto:        
			//    for(i=0; i<1024; i++)        
			//
			i = -1        
			for(; ++i<1024; ) 
			[...]     
		
	3. 	Si una funcion es muy larga añade un comentario de vez en cuando para saber en que parte del proceso uno está     
	4. 	Al comienzo de cada archivo el autor, la fecha de creación y la idea general de lo que hay en el archivo de codigo. 
			No pongas el copyright. 
			Tampoco un historial de cambios: esa tarea es labor del sistema de control de versiones. 
			
Lo que debes evitar comentar:     

	1.	...lo que ya se ve que hace el codigo:
		
		// Pon a cero el contador de mensajes
		msgCount = 0;         
		
		...innecesariamente, escogiendo nombres de variables funciones, variables, etc autodescriptivos (aunque no lo sea el algoritmo) 
		Por ejemplo en regionHandler.cpp:10
			Control *control; // focused control         
		podrias cambiarlo a:         
			Control *focusControl;     
			
	2.	...en linea, porque ese tipo de comentarios puede pasar de las ochenta columnas y se torna de dificil lecturade
	3.	...utilizando caracteres extendidos IBM, a menos que estés programando en java,D,C#... o similar
	------------------------------ 
	He visto clases por ejemplo: 
		class cINT {  
			cINT() {};  
			void Init(void); 
			[...] 
		}; 
		
		class cKEY {
			cKEY() {}  
			void Init(void);   
			[...] 
		}; 
		
		class cMEM {  
			cMEM() {}  
			void Init(void);   
			[...] 
		}; 
		class REGHND {    
			REGHND() {       [...]    }    
			void load(...) {       [...]    } 
		}; 
		
	Seguro que puedes ver el patrón: Init hace el trabajo del constructor y el constructor no hace nada! 
	(excepto en REGHND, pero en el caso de REGHND puedes añadir un segundo constructor que 
	acepte un const Region *. 
------------------------------ 
Hay definiciones en archivos hpp. 
Primero lo que dice el estandar:     
	- Una declaración dice el nombre, el tipo y 'como localizar' un objeto.       
		Por ejemplo:           
			extern int var;       
		dice que en algun lugar hay una variable de tipo int que tiene el nombre var;		
		No se crea ningun objeto.    
		
	- La definicion 'crea' el objeto. 
		Las definiciones suelen ir 'juntas' a una declaracion:
			int var;       
		dice que en algun lugar hay una variable de tipo int que tiene el nombre var;
		y de paso tambien la crea en algun sitio.    
		
	- Evidentemente solo puede haber una definicion para un objeto pero puede haber      
		más de una declaracion para un objeto.    
	- Ese 'sitio' donde se crea la variable está relacionado estrechamente con el archivo
		cpp que ha compilado el compilador. 
		Es decir:        
			test.cpp:              
				int var;         
				
		el compilador lee eso y dice:             
			- El archivo test.cpp ha definido (creado) la variable var. 
			Por tanto cuando otro archivo pida la variable var le diré que está guardada 
			en el archivo test.cpp 

			test2.cpp:
				extern int var;         
				
		el compilador lee eso y dice:             
			- El archivo test2.cpp ha declarado una variable de tipo int con el nombre var.
			No sé donde está exactamente. 
			Ya me lo dirá el compilador cuando alguien la defina. 
			Pero por ahora puedo usarla porque se su nombre y su tipo 
			(cuando alguien me diga donde está ya remplazaré el nombre por su dirección) 

	- Regla importante (conocida como 'one definition rule').            
		Un objeto solo puede estar definido en un sitio 
		
Segundo, la idea de los archivos hpp/cpp:     
	- En los archivos hpp se meten las declaraciones. 
		De esta forma incluir un hpp da a conocer variables y funciones pero no las crea
	- En los archivos cpp se meten las definiciones. 
		De esta forma se les asigna la posicion, su valor inicial, el cuerpo para las funciones... 
		(Al compilador se le da los cpp, y este genera archivos objeto)    
	- Los archivos cpp son unidades individuales. 
		Es decir para compilar el proyecto se compilan los cpp uno por uno y luego se juntan todos en el ejecutable final.  
	- Incluir un archivo hpp varias veces o incluirlo en cada cpp o lo que sea no puede causar problemas. 
		Un archivo hpp 'da a conocer' variables, funciones... 
		Si los das a conocer dos veces no pasa nada, solo pierdes el tiempo ;)
	
	Tercero, la situacion con tu codigo:     
		- Hay algunos archivos cpp que no son unidades individuales:
			key.cpp, 
			mem.cpp, 
			int.cpp, 
			virtualscreen.cpp, 
			regionhandler.cpp, 
			tim.cpp, 
			callbacks.cpp, 
			modules.cpp
			
			En vez de eso los incluyes unos a otros, empezando en m4g.cpp.      
			
		- Defines objetos en las cabeceras. 
			Por ejemplo empezando en m4g.hpp:
				Basicamente recursivamente todo desde m4g.hpp:114 hasta m4g.hpp:151. 
			
		Te preguntarás que problema hay, si al fin y al cabo tal como lo tienes 
		compila bien (no hay simbolos redefinidos...) y tampoco hay que molestarse 
		con un makefile chungo: solo compila m4g.cpp y ya está. 
		Pues no. 
		Hay varios problemas:    
			1. 	Hay archivos .c y archivos .cpp. 
				Lo logico es esperar que en los archivos .c haya lenguaje C y en 
				los archivos .cpp haya lenguaje C++. Pero no. 
				Al estar todo incluido textualmente todo se compila como C++. 
				C y C++ son lenguajes distintos.    
			2. Hace el debugging más dificil. 
				Como el debugger se despiste lo mas minimo verás que todo el codigo 
				viene de m4g.cpp. 
				Y tecnicamente todos los simbolos están definidos en m4g.cpp, asi que 
				olvidate de localizar un objeto viendo en que .o esta definido.    
				
			3. Olvidate de listados en ensamblador para cada archivo .o.    
			4. No le gusta a los IDE. 
				'Ir a definicion' e 'Ir a declaracion' funcionan perfectamente cuando programas como yo digo. 
				Cuando no el IDE tiene que adivinar.    
			5. Tarda mucho mas en compilar    
			6. Mas importante aún, al hacerlo como haces has eliminado uno de los 'espacios de nombres' que están en C y C++. 
				Es el espacio de la 'unidad de traduccion'.        
				Una 'unidad de traduccion' es lo que queda despues de preprocesar un cpp.        
				Por ejemplo, las funciones static son funciones locales a una unidad de traduccion:  
					a.cpp:            
						static void func(int x) {             }        
					b.cpp:            
						static void func(Region &r) {             }           
						
					Observa como ambas funciones tienen el mismo nombre y sin embargo puedes           
					compilarlas sin problemas. Si en a.cpp llaman a func() entonces va a func() en a.cpp           
					y si en b.cpp llaman a func() entonces va a func() en b.cpp. 
					Nadie fuera de a.cpp puede llamar a func() en a.cpp.
					Con tu 'modelo' al estar todo incluido eso no es posible. 
					
					Lo que ocurrirá (en este caso) es que seguramente compile bien porque tienen listas de argumentos distintas. 
					Sin embargo ahora estás solo. 
					El compilador no te va a ayudar a que no cometas errores. 
					Podrias definir        
						c.cpp:            
							static void func(int x) {             }            
						y ahora tu programa no compila. 
						
						De hecho tienes un error de compilacion en c.cpp porque func está ya definida sin embargo no hay ninguna otra definicion en c.cpp de func! 
						Un error inexplicable. 
						Asi que tienes que buscar en todos los archivos cpp (y hpp) por una funcion con 
						el mismo nombre. Ahi va al traste la productividad.        
						
					Las funciones static no solo son una conveniencia para no tener que escoger nombres globales unicos. 
					Como una funcion estatica solo se puede acceder desde un sitio (la UT donde está definida) el 
					compilador puede hacer optimización mucho más agresiva. 
					
					Por ejemplo        
						puede hacer expansión en linea de la función y luego borrar directamente la funcion del ejecutable. 
						
------------------------------ 
Los defines en GPU::hpp tienen los nombres y comentarios mal. 
En concreto: 
	#define DISP_MODE_0             0x0000      // BG Mode 0  <---- 
	#define DISP_MODE_1             0x0001      // BG Mode 0  <---- 
	#define DISP_MODE_2             0x0002      // BG Mode 0  <---- 
	
... Además el bit 7 (DISP_FORCE_HBLANK) no fuerza HBlank. 
Más bien desconecta el LCD de la VRAM, haciendo que el LCD lea todo unos, 
provocando así dos cosas: 

1. 	La pantalla se vuelve blanca 
2. 	Se puede acceder a VRAM en cualquier momento sin el ciclo extra que hay cuando uno 
accede a VRAM mientras el LCD está accediendo a VRAM 
------------------------------ 
Tengo curiosidad porque decides borrar las memorias de video en cINT::Init. 
------------------------------ 
cKEY::Update es incomprensible. 
Además se ejecuta con interrupciones desactivadas? 
------------------------------ 
En 'm4g.cpp', el comentario "* Control asumible redraw is working, in a multitasked flavourand" es incomprensible. 
De verdad que no entiendo nada! 
'flavourand'? 
------------------------------ 
En la clase VirtualScreen he visto que hay un miembro llamado i y un miembro llamado d que, 
a juzgar por el código de load y clear, parece que están haciendo el trabajo de funciones locales? 
Además quisiera que me explicases esto:             
	i=-1;             
	for(;++i<1024;) 
	[...] 

?!?? 

que es más o menos equivalente a esto otro, 
pero muchisimo más confuso y potencialmente mas lento:   

	for(i=0; i<1024; i++) 
	[...] 

------------------------------ 
He visto que en REGHND::dispatchMessages utilizas punteros etiquetados al almacenar 
en los 4 bits superiores el tipo de mensaje. 
Supongo que estás al corriente del peligro que esto conlleva. 
También he visto que decides ejecutar tan solo 4 mensajes pero puedes almacenar hasta 1024 mensajes. 
Si vas a poner una limitación de mensajes haz que dependa del tiempo, no de un número de mensajes. 
(e.g. ejecutar tantos mensajes como quepan en un milisegundo). 1024 * 4 son 4K. 
El objeto REGHND que he visto creado en main reside en la pila. 
Eso son 4K en la pila. 
La pila reside en IWRAM. 
Por cierto la variable local 'max', para representar lo que representa, tiene un nombre extraño.
Lo mismo en el parámetro de drawViewport, que se llama 'c', en vez de (supongo) 'v'. 
La funcion drawDisplay acepta un objeto constante de tipo Display. 
No compruebas en sendMessage si está desbordando el buffer de mensajes. Pon por lo menos un assert o algo ahi. 
En load() aceptas un objeto Region constante pero luego lo asignas a un miembro de tipo puntero a Region no constante. 
------------------------------ 
El archivo int.s es un archivo compilado. 
	Lo ha autogenerado un compilador. 
	Aunque tecnicamente su contenido sea ensamblador no es un archivo de codigo fuente. 
	
																					m
---------------------------------------------------------------------------------------------
*/

#include "m4g.hpp"
#include "modules/modules.hpp"

int main(void){
	bool lock;
	
	while(1){
		PREFETCH
		INT::init();	
		MEM::init();
		SYS::init(); 
		SRAM::init();
		GPU::start();
		INT::enable(IRQ_VBLANK);
		INT::enable(IRQ_HBLANK);
		TIM0.init(0);
		//INT.Enable(IRQ_KEYPAD);
		TIM0.setup(0x0004, 1);
		TIM0.enable();

		REGHND::init();
		SYS::init();
				
		lock = false;
		
		VirtualScreen VS; // Create and bind virtual screen to the GPU
		GPU::vs = &VS;

		REGHND::load(&REGION_MAP_2_INS);
		
		LOADCONFIG(NULL, 0, 0, NULL);
		SRAM::dataRevert();
		instrumentUnpack(&VAR_INSTRUMENT);	

		while( !SYS::var_reset ){
			if(!GPU::isVblank()) {lock = false; continue;}
			if(!lock) { lock = true; continue; }							
			SYS::update();			
			REGHND::update(1);
		};
	}
	return 0;
}
