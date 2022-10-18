/* parse.c - parsing and parse tree construction */

#include "project04.h"

void parse_table_init(struct parse_table_st *pt) {
    pt->len = 0;
}

// "Allocate" a parse node from the table of parse nodes
struct parse_node_st * parse_node_new(struct parse_table_st *pt) {
    struct parse_node_st *np;
    np = calloc(1, sizeof(struct parse_node_st));
    pt->len += 1;
    return np;
}

void parse_error(char *err) {
    printf("parse_error: %s\n", err);
    exit(-1);
}

// These are names of operators for printing
//{OP_PLUS, OP_MINUS, OP_MULT, OP_DIV, OP_NOT, OP_OR, OP_XOR, OP_AND, OP_SHIFT_LEFT, OP_SHIFT_RIGHT};
char *parse_oper_strings[] = {"PLUS", "MINUS", "MULT", "DIV", "NOT","OR","XOR","AND","LEFT SHIFT","RIGHT SHIFT"};

// Print the dots which represent tree depth in the output
void parse_tree_print_indent(int level) {
    level *= 2;
    for (int i = 0; i < level; i++) {
        printf(".");
    }
}

// Traverse the tree recursively to print out the structs
void parse_tree_print_expr(struct parse_node_st *np, int level) {
    parse_tree_print_indent(level);
    printf("EXPR ");

    // TODO: add support for oper1
    if (np->type == EX_INTVAL) {
        printf("INTVAL %li\n", np->intval.value);
    } else if (np->type == EX_OPER2) {
        printf("OPER2 %s\n", parse_oper_strings[np->oper2.oper]);
        parse_tree_print_expr(np->oper2.left, level+1);
        parse_tree_print_expr(np->oper2.right, level+1);
    } else if (np->type == EX_OPER1) {
        printf("OPER1 %s\n", parse_oper_strings[np->oper2.oper]);
        parse_tree_print_expr(np->oper1.operand, level+1);       
    }
}

void parse_tree_print(struct parse_node_st *np) {
    parse_tree_print_expr(np, 0);
}

// Parse the "program" part of the EBNF
// A program is composed of an expression followed by EOT
struct parse_node_st * parse_program(struct parse_table_st *pt, 
                                        struct scan_table_st *st) {
    struct parse_node_st *np1;

    np1 = parse_expression(pt, st);

    if (!scan_table_accept(st, TK_EOT)) {
        parse_error("Expecting EOT");        
    }

    return np1;                                       
}

// Build the tree for expressions
// Expressions are defined in the EBNF as an operator followed
// by zero or more operator operand pairs
struct parse_node_st * parse_expression(struct parse_table_st *pt, 
                                        struct scan_table_st *st) {
    struct scan_token_st *tp;
    struct parse_node_st *np1, *np2;

  //parse the first part of the equation
    np1 = parse_operand(pt, st);

    /*
        TODO
        add cases for other OPER2 operators
    */
    while (true) {    
        tp = scan_table_get(st, 0);;
        if (tp->id == TK_PLUS) {
            scan_table_accept(st, TK_ANY);
            np2 = parse_node_new(pt);
            np2->type = EX_OPER2;
            np2->oper2.oper = OP_PLUS;
            np2->oper2.left = np1;          
            np2->oper2.right = parse_operand(pt, st);
            np1 = np2;
        } else if (tp->id == TK_MINUS) {
            scan_table_accept(st, TK_ANY);
            np2 = parse_node_new(pt);
            np2->type = EX_OPER2;
            np2->oper2.oper = OP_MINUS;
            np2->oper2.left = np1;          
            np2->oper2.right = parse_operand(pt, st);
            np1 = np2;
        } else  if (tp->id == TK_MULT) {
            scan_table_accept(st, TK_ANY);
            np2 = parse_node_new(pt);
            np2->type = EX_OPER2;
            np2->oper2.oper = OP_MULT;
            np2->oper2.left = np1;          
            np2->oper2.right = parse_operand(pt, st);
            np1 = np2;
        } else  if (tp->id == TK_DIV) {
            scan_table_accept(st, TK_ANY);
            np2 = parse_node_new(pt);
            np2->type = EX_OPER2;
            np2->oper2.oper = OP_DIV;
            np2->oper2.left = np1;          
            np2->oper2.right = parse_operand(pt, st);
            np1 = np2;
        }  else  if (tp->id == TK_SHIFT_LEFT) {
            scan_table_accept(st, TK_ANY);
            np2 = parse_node_new(pt);
            np2->type = EX_OPER2;
            np2->oper2.oper = OP_SHIFT_LEFT;
            np2->oper2.left = np1;          
            np2->oper2.right = parse_operand(pt, st);
            np1 = np2;
        } else  if (tp->id == TK_SHIFT_RIGHT) {
            scan_table_accept(st, TK_ANY);
            np2 = parse_node_new(pt);
            np2->type = EX_OPER2;
            np2->oper2.oper = OP_SHIFT_RIGHT;
            np2->oper2.left = np1;          
            np2->oper2.right = parse_operand(pt, st);
            np1 = np2;
        }  else if (tp->id == TK_OR) {
            scan_table_accept(st, TK_ANY);
            np2 = parse_node_new(pt);
            np2->type = EX_OPER2;
            np2->oper2.oper = OP_OR;
            np2->oper2.left = np1;          
            np2->oper2.right = parse_operand(pt, st);
            np1 = np2;
        }  else if (tp->id == TK_AND) {
            scan_table_accept(st, TK_ANY);
            np2 = parse_node_new(pt);
            np2->type = EX_OPER2;
            np2->oper2.oper = OP_AND;
            np2->oper2.left = np1;          
            np2->oper2.right = parse_operand(pt, st);
            np1 = np2;
        }  else if (tp->id == TK_XOR) {
            scan_table_accept(st, TK_ANY);
            np2 = parse_node_new(pt);
            np2->type = EX_OPER2;
            np2->oper2.oper = OP_XOR;
            np2->oper2.left = np1;          
            np2->oper2.right = parse_operand(pt, st);
            np1 = np2;
        }  else if (tp->id == TK_NOT) {
            scan_table_accept(st, TK_ANY);
            np2 = parse_node_new(pt);
            np2->type = EX_OPER1;
            np2->oper1.oper = OP_NOT;
            np2->oper1.operand = np2;  
            np1 = np2;
        }else if (tp->id == TK_EOT){   
           break;
        } else if (tp->id == TK_RPAREN) {
          scan_table_accept(st, TK_ANY);
            break;
        }
    }

    return np1;
}

int convertATOI(struct scan_token_st * tp, int base){
	//use length of the hex value so we can work from the back to the front
	int len = 0;
    while( tp->name[len] != '\0') { 
    	len++;
    }
    //start at the back
    int valueForCurrentColumn = 1;
    int value = 0;
    for(int i = len- 1; i >= 0; i--) {
    	int valueForCurrentLetter;
        if ('a' <= tp->name[i] && tp->name[i] <= 'f'){
        //the 87 comes from observation that 'a' of 97
        //should be 10 so subtract 87
        	valueForCurrentLetter = tp->name[i] - 87;
        } else if ('A' <= tp->name[i] && tp->name[i] <= 'F'){
            //the 55 comes from observation that 'A' of 65
            //should be 10 so subtract 55
            valueForCurrentLetter = tp->name[i] - 55;
        } else if ('0' <= tp->name[i] && tp->name[i] <= '9'){
            //the 48 comes from observation that '0' of 48
            //should be 0 so subtract 48
            valueForCurrentLetter = tp->name[i] - 48;
        }      
        value += valueForCurrentLetter * valueForCurrentColumn;
        //update the valueForCurrentColumn
        //because the next column goes up by a factor of 16
        valueForCurrentColumn *= base;
      }
  return value;
}

// Parse operands, which are defined in the EBNF to be 
// integer literals or unary minus or expressions 
struct parse_node_st * parse_operand(struct parse_table_st *pt,
                                     struct scan_table_st *st) {
    struct scan_token_st *tp;
    struct parse_node_st *np1;
    /*
        TODO 
        add case for unary minus e.g. "-1 + 1"
        add case for operands which are expressions (i.e. begin with '(')
            (hint: recurse to parse_expression)
    */
  	//return the next token if it match
    if (scan_table_accept(st, TK_INTLIT)) {
      	//get next token
        tp = scan_table_get(st, -1);;
      	//make a new node, this takes in the table 
        np1 = parse_node_new(pt);
        np1->type = EX_INTVAL;
      	//set the value
        np1->intval.value = atoi(tp->name);
    } else if (scan_table_accept(st, TK_LPAREN)) {
      	//get next token
        tp = scan_table_get(st, -1);
      	//try to parse the following expression
        return parse_expression(pt,st);
    } else if (scan_table_accept(st, TK_RPAREN)) {
      	//get next token
        tp = scan_table_get(st, -1);      
    } else if (scan_table_accept(st, TK_MINUS)) {
        np1= parse_node_new(pt);
        np1->type = EX_OPER1;
        np1->oper1.oper = OP_MINUS;
        np1->oper1.operand = parse_operand(pt,st);  
      } else  if (scan_table_accept(st, TK_HEXLIT)) {
        tp = scan_table_get(st, -1);
        np1 = parse_node_new(pt);
        np1->type = EX_INTVAL;    
        np1->intval.value = convertATOI(tp,  16);      
    } else  if (scan_table_accept(st, TK_BINLIT)) {
         tp = scan_table_get(st, -1);
         np1->type = EX_INTVAL;     
         np1->intval.value = convertATOI(tp,  2);
    } else  if (scan_table_accept(st, TK_NOT)) {
         tp = scan_table_get(st, -1);    
         np1 = parse_node_new(pt);
         np1->type = EX_OPER1;
         np1->oper1.oper = OP_NOT;
         np1->oper1.operand = parse_operand(pt,st);
    } else {
        parse_error("Bad operand");
    }
    return np1;
}
