#include<bits/stdc++.h>
using namespace std;

struct symbol_table{
    bool error;
    int address;
    int block;
    bool relative;

    symbol_table(){
      relative=1;
    }
};

struct literal_table{
    int error;
    set<int> address;
    int blocknumber;
    int length;
    int operand_value;
    bool unassigned;

    literal_table(){
      unassigned=1;
    }
};

struct block_table{
    int start;
    int length;
    int id;
};

struct opcode_table{
    int format;
    string opcode;
};

map<string, literal_table> LITTAB;
map<string, block_table> BLOCKTAB;
map<string, symbol_table> SYMTAB;
map<string, opcode_table> OPTAB;
map<string, int> REGTAB={
  {"A",0},
  {"X",1},
  {"L",2},
  {"PC",8},
  {"SW",9},
  {"B",3},
  {"S",4},
  {"T",5},
  {"F",7}
};

set<string> ASMDIR={"START","END","BASE","NOBASE","RESW","RESB","WORD","BYTE","LTORG","USE","ORG","EQU"};
bool used_BASE=0;
int base=0;

void load_OPTAB(){
  OPTAB["ADD"].opcode="18";
  OPTAB["ADD"].format=3;

  OPTAB["+ADD"].opcode="18";
  OPTAB["+ADD"].format=4;

  OPTAB["ADDF"].opcode="58";
  OPTAB["ADDF"].format=3;

  OPTAB["+ADDF"].opcode="58";
  OPTAB["+ADDF"].format=4;

  OPTAB["ADDR"].opcode="90";
  OPTAB["ADDR"].format=2;

  OPTAB["AND"].opcode="40";
  OPTAB["AND"].format=3;

  OPTAB["+AND"].opcode="40";
  OPTAB["+AND"].format=4;

  OPTAB["CLEAR"].opcode="B4";
  OPTAB["CLEAR"].format=2;

  OPTAB["COMP"].opcode="28";
  OPTAB["COMP"].format=3;

  OPTAB["+COMP"].opcode="28";
  OPTAB["+COMP"].format=4;

  OPTAB["COMPF"].opcode="88";
  OPTAB["COMPF"].format=3;

  OPTAB["+COMPF"].opcode="88";
  OPTAB["+COMPF"].format=4;

  OPTAB["COMPR"].opcode="A0";
  OPTAB["COMPR"].format=2;

  OPTAB["DIV"].opcode="24";
  OPTAB["DIV"].format=3;

  OPTAB["+DIV"].opcode="24";
  OPTAB["+DIV"].format=4;

  OPTAB["DIVF"].opcode="64";
  OPTAB["DIVF"].format=3;

  OPTAB["+DIVF"].opcode="64";
  OPTAB["+DIVF"].format=4;

  OPTAB["DIVR"].opcode="9C";
  OPTAB["DIVR"].format=2;

  OPTAB["FIX"].opcode="C4";
  OPTAB["FIX"].format=1;

  OPTAB["FLOAT"].opcode="C0";
  OPTAB["FLOAT"].format=1;

  OPTAB["HIO"].opcode="F4";
  OPTAB["HIO"].format=1;

  OPTAB["J"].opcode="3C";
  OPTAB["J"].format=3;

  OPTAB["JEQ"].opcode="30";
  OPTAB["JEQ"].format=3;

  OPTAB["+JEQ"].opcode="30";
  OPTAB["+JEQ"].format=4;

  OPTAB["JGT"].opcode="34";
  OPTAB["JGT"].format=3;

  OPTAB["+JGT"].opcode="34";
  OPTAB["+JGT"].format=4;

  OPTAB["JLT"].opcode="38";
  OPTAB["JLT"].format=3;

  OPTAB["+JLT"].opcode="38";
  OPTAB["+JLT"].format=4;

  OPTAB["JSUB"].opcode="48";
  OPTAB["JSUB"].format=3;

  OPTAB["+JSUB"].opcode="48";
  OPTAB["+JSUB"].format=4;

  OPTAB["LDA"].opcode="00";
  OPTAB["LDA"].format=3;

  OPTAB["+LDA"].opcode="00";
  OPTAB["+LDA"].format=4;

  OPTAB["LDB"].opcode="68";
  OPTAB["LDB"].format=3;

  OPTAB["+LDB"].opcode="68";
  OPTAB["+LDB"].format=4;
  
  OPTAB["LDCH"].opcode="50";
  OPTAB["LDCH"].format=3;

  OPTAB["+LDCH"].opcode="50";
  OPTAB["+LDCH"].format=4;

  OPTAB["LDF"].opcode="70";
  OPTAB["LDF"].format=3;

  OPTAB["+LDF"].opcode="70";
  OPTAB["+LDF"].format=4;

  OPTAB["LDL"].opcode="08";
  OPTAB["LDL"].format=3;

  OPTAB["+LDL"].opcode="08";
  OPTAB["+LDL"].format=4;

  OPTAB["LDS"].opcode="6C";
  OPTAB["LDS"].format=3;

  OPTAB["+LDS"].opcode="6C";
  OPTAB["+LDS"].format=4;

  OPTAB["LDT"].opcode="74";
  OPTAB["LDT"].format=3;

  OPTAB["+LDT"].opcode="74";
  OPTAB["+LDT"].format=4;

  OPTAB["LDX"].opcode="04";
  OPTAB["LDX"].format=3;

  OPTAB["+LDX"].opcode="04";
  OPTAB["+LDX"].format=4;

  OPTAB["LPS"].opcode="D0";
  OPTAB["LPS"].format=3;

  OPTAB["+LPS"].opcode="D0";
  OPTAB["+LPS"].format=4;

  OPTAB["MUL"].opcode="20";
  OPTAB["MUL"].format=3;

  OPTAB["+MUL"].opcode="20";
  OPTAB["+MUL"].format=4;

  OPTAB["MULF"].opcode="60";
  OPTAB["MULF"].format=3;

  OPTAB["+MULF"].opcode="60";
  OPTAB["+MULF"].format=4;

  OPTAB["MULR"].opcode="98";
  OPTAB["MULR"].format=2;

  OPTAB["NORM"].opcode="C8";
  OPTAB["NORM"].format=1;

  OPTAB["OR"].opcode="44";
  OPTAB["OR"].format=3;

  OPTAB["+OR"].opcode="44";
  OPTAB["+OR"].format=4;

  OPTAB["RD"].opcode="D8";
  OPTAB["RD"].format=3;

  OPTAB["+RD"].opcode="D8";
  OPTAB["+RD"].format=4;

  OPTAB["RMO"].opcode="AC";
  OPTAB["RMO"].format=2;

  OPTAB["RSUB"].opcode="4C";
  OPTAB["RSUB"].format=3;

  OPTAB["+RSUB"].opcode="4C";
  OPTAB["+RSUB"].format=4;

  OPTAB["SHIFTL"].opcode="A4";
  OPTAB["SHIFTL"].format=2;

  OPTAB["SHIFTR"].opcode="A8";
  OPTAB["SHIFTR"].format=2;

  OPTAB["SIO"].opcode="F0";
  OPTAB["SIO"].format=1;

  OPTAB["SSK"].opcode="EC";
  OPTAB["SSK"].format=3;

  OPTAB["+SSK"].opcode="EC";
  OPTAB["+SSK"].format=4;

  OPTAB["STA"].opcode="0C";
  OPTAB["STA"].format=3;

  OPTAB["+STA"].opcode="0C";
  OPTAB["+STA"].format=4;

  OPTAB["STB"].opcode="78";
  OPTAB["STB"].format=3;

  OPTAB["+STB"].opcode="78";
  OPTAB["+STB"].format=4;

  OPTAB["STCH"].opcode="54";
  OPTAB["STCH"].format=3;

  OPTAB["+STCH"].opcode="54";
  OPTAB["+STCH"].format=4;

  OPTAB["STF"].opcode="80";
  OPTAB["STF"].format=3;

  OPTAB["+STF"].opcode="80";
  OPTAB["+STF"].format=4;

  OPTAB["STI"].opcode="D4";
  OPTAB["STI"].format=3;

  OPTAB["+STI"].opcode="D4";
  OPTAB["+STI"].format=4;

  OPTAB["STL"].opcode="14";
  OPTAB["STL"].format=3;

  OPTAB["+STL"].opcode="14";
  OPTAB["+STL"].format=4;

  OPTAB["STS"].opcode="7C";
  OPTAB["STS"].format=3;

  OPTAB["+STS"].opcode="7C";
  OPTAB["+STS"].format=4;

  OPTAB["STSW"].opcode="E8";
  OPTAB["STSW"].format=3;

  OPTAB["+STSW"].opcode="E8";
  OPTAB["+STSW"].format=4;

  OPTAB["STT"].opcode="84";
  OPTAB["STT"].format=3;

  OPTAB["+STT"].opcode="84";
  OPTAB["+STT"].format=4;

  OPTAB["STX"].opcode="10";
  OPTAB["STX"].format=3;

  OPTAB["+STX"].opcode="10";
  OPTAB["+STX"].format=4;

  OPTAB["SUB"].opcode="1C";
  OPTAB["SUB"].format=3;

  OPTAB["+SUB"].opcode="1C";
  OPTAB["+SUB"].format=4;

  OPTAB["SUBF"].opcode="5C";
  OPTAB["SUBF"].format=3;

  OPTAB["+SUBF"].opcode="5C";
  OPTAB["+SUBF"].format=4;

  OPTAB["SUBR"].opcode="94";
  OPTAB["SUBR"].format=2;

  OPTAB["SVC"].opcode="B0";
  OPTAB["SVC"].format=2;

  OPTAB["TD"].opcode="E0";
  OPTAB["TD"].format=3;

  OPTAB["+TD"].opcode="E0";
  OPTAB["+TD"].format=4;

  OPTAB["TIO"].opcode="F8";
  OPTAB["TIO"].format=1;

  OPTAB["TIX"].opcode="2C";
  OPTAB["TIX"].format=3;

  OPTAB["+TIX"].opcode="2C";
  OPTAB["+TIX"].format=4;

  OPTAB["TIXR"].opcode="B8";
  OPTAB["TIXR"].format=2;

  OPTAB["WD"].opcode="DC";
  OPTAB["WD"].format=3;

  OPTAB["+WD"].opcode="DC";
  OPTAB["+WD"].format=4;
}

void load_BLOCKS(){
  BLOCKTAB["DEFAULT"].start=0;
  BLOCKTAB["DEFAULT"].id=0;
  BLOCKTAB["DEFAULT"].length=0;
}