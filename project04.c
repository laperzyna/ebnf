#include "project04.h"
#include <stdlib.h>
#include <math.h>

double pow(double base, double exponent) {
  if (exponent == 0) {
    return 1;
  }

  double ans = base * 1;//this calculates the start value
  for(int i = 0; i < exponent-1; i++ ) {
    ans *= base;
  }
  return ans;
}

void printNumber(int val, int base, int numBits) {
  if (base == 16) {
    //in hex each column can hold from 0 to 15 which is actually /4 bits
    numBits /= 4;
  }
  
  if (base == 10) {
    printf("%d\n", val);
  } else if (base == 2 || base == 16) {
    if (base == 2) {
      printf("0b");
      } else {
      printf("0x");     
    }
    //biggest number is 32bit
    char output[numBits + 1]; //null terminating
    //since array is populating from back to front
    //put the null terminating at the end
    //fill the entire array with 0's
    for(int i = 0 ; i < numBits; i++) {
      output[i] = '0';
    }
    output[numBits] = '\0';
    int curIdx = numBits-1;
    //this makes an unsigned int
    u_int value;
    //when the integer comes in negative the first bit is a 1,
    //which messes up the math below when trying to get a remainder.
    //need this number to seem positive without changing any of
    //the bits. To do that cast this integer as an unsigned 
    //integer, which tells the computer it should not take the first bit 
    //1 as a negative
    if (val < 0) {
      value = (unsigned int) val;
    } else {
      value = val;
    }
    while (value != 0) {
      //get the remainder when dividing
      int remainder = value % base;
      if (0 <= remainder && remainder <= 9) {
         //put this number into the output array
        //convert this number to a char by adding '0'
        output[curIdx--] = remainder + '0';
      } else if (remainder > 9) {
       //print the hex values 10- A   15 - F
       //from looking at the ascii table, can convert the numeric 
       //value 10 to 'A' by adding 55
        output[curIdx--] = remainder + 55;       
      }
      //update the number
      value = value / base;    
    }  
    printf("%s\n", output);    
  }  
}

long doOperation(long num1, long num2, enum parse_oper_enum OP ) {
  if (OP == OP_PLUS) {
    return num1 + num2;
  } else if (OP == OP_MINUS) {
    return num1 - num2;
  } else if (OP == OP_MULT) {
    return num1 * num2;
  } else if (OP == OP_DIV) {
    return num1 / num2;
  } else if (OP == OP_AND) {  
    return num1 & num2;
  } else if (OP == OP_OR) {
    return num1 | num2;
  } else if (OP == OP_XOR) {
    return num1 ^ num2;
  } else if (OP == OP_SHIFT_LEFT) {
    return num1 << num2;
  } else if (OP == OP_SHIFT_RIGHT) {
    return num1 >> num2;
  } else {
    printf("ERROR: unknown operator when evaluating the math\n");
    return 0;
  }
}

long evalTree(struct parse_node_st * curNode) {
  if (curNode->type == EX_OPER2) {
    //get the result from left 
    long resultLeft = evalTree(curNode->oper2.left);
    long resultRight = evalTree(curNode->oper2.right);
    return doOperation(resultLeft, resultRight,curNode->oper2.oper);
  } else if (curNode->type == EX_INTVAL) {
  		return curNode->intval.value;
  } else {
    //oper1 should only be - or ~
    long intVal = evalTree(curNode->oper1.operand);
    if (curNode->oper1.oper == OP_MINUS) {
      return -1 * intVal;
    } else {
      return ~intVal;
    }
  }
}

int main(int argc, char **argv) {
    struct scan_table_st scan_table; // table of tokens
    struct parse_table_st parse_table; // table of parse nodes
    struct parse_node_st *parse_tree; // tree (pointers only) of parse nodes
    char input[SCAN_INPUT_LEN];
    int len;
    int base = 10;
    int width = 32; //default number print width is 16 bits
    int foundEArg = 0;
    if (argc != 3 && argc != 5) {
      printf("usage: ./project04 -e \"expr\" -b base\n");
      printf("-b is optional\n");
      printf("example: ./project04 -e \"1 + 2\" -b 10\n" );
      exit(-1);
    }
    for(int i = 1; i < argc; i++) {
      //if current argument is -e
      if (strcmp(argv[i],"-e") == 0) {
      	foundEArg = 1;
        //read next argument as the expression to use
        strncpy(input, argv[i+1], SCAN_INPUT_LEN);
        len = strnlen(input, SCAN_INPUT_LEN);
      } else  if (strcmp(argv[i],"-b") == 0){        
        base = atoi(argv[i+1]);  
      } else  if (strcmp(argv[i],"-w") == 0){        
        width = atoi(argv[i+1]);
      } 
    }
  //check if -e was not found
  if (foundEArg == 0) {
    printf("did not find e argument!\n");
    printf("usage: ./project04 -e \"expr\" -b base\n");
    printf("-b is optional\n");
    printf("example: ./project04 -e \"1 + 2\" -b 10\n" );
    exit(-1);
  }
    scan_table_init(&scan_table);
    scan_table_scan(&scan_table, input, len);
    // You may uncomment this if you need to debug the scanner 
    // but leave it commented for "grade test" since scanner output
    // is not part of the expected output for lab03
    // scan_table_print(&scan_table);
    parse_table_init(&parse_table);
    parse_tree = parse_program(&parse_table, &scan_table);
    //Evaluate the tree
    long ans = evalTree(parse_tree);
    printNumber(ans,base, width);
    return 0;
}
