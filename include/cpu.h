#include <defs.h>
#include <instructions.h>

typedef struct
{
    bool zero;
    bool sub;
    bool hcarry;
    bool carry;
} Flags;


typedef struct
{
    u8 a;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 h;
    u8 l;
    u16 PC;
    u16 SP; 
} cpu_registers;


typedef struct
{
    cpu_registers reg;

    //current fetch...
    u16 fetch_data;
    u16 mem_dest;
    u8 cur_opcode;
    instruction *cur_inst;

    bool halted;
    bool stepping;

} cpu_context;


void cpu_init();
bool cpu_step();