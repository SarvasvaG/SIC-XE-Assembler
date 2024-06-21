## GETTING STARTED
• Ensure the current directory contains the “assembler.cpp” file and “tables.hpp” file.

• Name the .txt file containing your SIC/XE program as “program.txt” and place it in the current directory.

• Run the “assembler.cpp” file using the terminal.

• The following files are created as a result-
1) “intermediate_file.txt” – Intermediate File generated from pass1.
2) “obj_program.txt” – Object Program generated from pass2.
3)  “assembly_listing.txt” – Listing File generated from pass2 and also the errors generated in each pass1 and pass2.

## HIGH-LEVEL DESIGN
The following are the data structures used:

• LITTAB: Literal Table, where each entry contains the following information- error flag, set of addresses for a given label, block number of the latest literal for a given label, 
length in bytes, decimal value of the label, flag indicating if the label has been 
assigned address.

• BLOCKTAB: Block Table for program blocks, where each entry contains the following 
information- start address block, length of block, id of block.

• SYMTAB: Symbol Table, where each entry contains the following information- error 
flag, address of the label, block containing the label, whether the value specified 
by/present in label is of the type relative/absolute.

• OPTAB: Opcode Table, where each entry has the following information: the type of 
format of the instruction and opcode of the instruction.

• ASMDIR: A set containing all the unique Assembler directives supported.

• TextRecord: A class containing various utility functions and attributes for handling 
text records in pass2.

• used_BASE: A Boolean denoting, if BASE has been specified by the programmer or 
not.

Other variables used globally include:
• base: a number holding the current base to be used for base-relative addressing.

• fin, fout, fout2: Various input and output file streams used for taking input and 
pushing outputs.

• label, opcode, operand: Three strings denoting the current instruction specified by 
“line” variable.

• LOCCTR and save_LOCCTR: Location Counter and its previous values are useful for 
handling ORG.

• current_block_number and number_of_blocks: Denoting information regarding 
program blocks.

• block_mapping: Mapping of block id to it’s name.

## WORKING OF ASSEMBLER

The main functions which execute the assembler are: load_BLOCKS(), load_OPTAB(), 
pass1(), pass2() and main().

The following cpp files are responsible for fulfilling the purpose:-

• tables.cpp – It contains tables described in High-Level Design.

• assembler.cpp -  

1) **pass1 function**:
It will generate a symbol table.
It will generate an intermediate file for pass2.

2) **pass2 function**:
It will generate a listing file containing the input assembly code and address, 
block number, object code of each instruction.
It will also generate an object program including following type of record: H, 
T, M and E types.
Errors are displayed in the listing file at the end
