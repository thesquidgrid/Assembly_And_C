// make the following symbol avalible outside this file
.global my_asm_16bitset
.global my_asm_16bitclr
.global my_asm_16bitcheck


//function to set a bitmask
my_asm_16bitset:
    ORRS R0, R1             // R0 = R0 | R1 :)
    BX LR                   //return branch back - program counter callback

//function to clear bit based on bitmask
my_asm_16bitclr:
    MVNS R1, R1             // R1 = NOT(R1)
    ANDS R0, R1             // R0 = R1 AND R0
    BX LR

//function to check if a bit is in a certain position based on a bitmask
my_asm_16bitcheck:
    ANDS R0, R1             // R0 = R1 & R0
    CMP R0, R1              // compare R0 and R1
    BEQ ret_true            // if R0 and R1 are equal, jump to tag ret_true. If not ignore line
    MOVS R0, #0             // move value 0 to R0
    BX LR                   //Jump out of function
    
ret_true: //tag if R0 and R1 are = to eachother
    MOVS R0, #1             //move value 1 to R0
    BX LR                   //Jump out of function

.END
