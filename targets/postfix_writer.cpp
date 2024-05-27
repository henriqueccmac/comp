#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include ".auto/all_nodes.h"  // all_nodes.h is automatically generated
#include "til_parser.tab.h"

//---------------------------------------------------------------------------

void til::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void til::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void til::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  if (_inFunctionBody || _inMain) {
    _pf.DOUBLE(node->value()); // load number to the stack
  }
  else {
    _pf.SDOUBLE(node->value()); // double is on the DATA segment
  }
}
void til::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
   ASSERT_SAFE_EXPRESSIONS;
   node->argument()->accept(this, lvl+2);
   _pf.INT(0);
   _pf.EQ();
}
void til::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
   ASSERT_SAFE_EXPRESSIONS;
  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}
void til::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JNZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.OR();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}
void til::postfix_writer::do_address_of_node(til::address_of_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // argument is an lvalue => already an address
  node->lvalue()->accept(this, lvl + 2);
}
void til::postfix_writer::do_alloc_node(til::alloc_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  _pf.INT(3);  // put 3 in the stack
  _pf.SHTL();  // shift left by 3 to multiply by 8 (size of a pointer)
  _pf.ALLOC(); // allocate
  _pf.SP();    // put base pointer in stack
}
void til::postfix_writer::do_block_node(til::block_node * const node, int lvl) {
   _symtab.push(); // add new level to scope
  if (node->declarations()) // declarations come first
    node->declarations()->accept(this, lvl + 2); 
  if (node->instructions()) // instructions are the last
    node->instructions()->accept(this, lvl + 2);
  _symtab.pop(); // remove scope
}
void til::postfix_writer::do_nullptr_node(til::nullptr_node * const node, int lvl) {
  if (_inFunctionBody || _inMain) { 
    _pf.INT(0); // load number to the stack
  }
  else {
    _pf.SINT(0); // double is on the DATA segment
  }
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  if (_inFunctionBody || _inMain) {
    _pf.INT(node->value()); // push an integer
  } else {
    _pf.SINT(node->value());
  }
}

void til::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value()); // output string characters

  /* leave the address on the stack */
  _pf.TEXT(); // return to the TEXT segment
  _pf.ADDR(mklbl(lbl1)); // the string to be printed
  // may need to change: if it is a global decl go to DATA
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_unary_minus_node(cdk::unary_minus_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement
}

void til::postfix_writer::do_unary_plus_node(cdk::unary_plus_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.ADD();
}
void til::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.SUB();
}
void til::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MUL();
}
void til::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.DIV();
}
void til::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void til::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}
void til::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}
void til::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}
void til::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}
void til::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}
void til::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}

void til::postfix_writer::do_sizeof_node(til::sizeof_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (node->expression()->is_typed(cdk::TYPE_INT)) {
    _pf.INT(4);
  } else if (node->expression()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.INT(8);
  } else if (node->expression()->is_typed(cdk::TYPE_STRING)) {
    _pf.INT(4);
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
  }
}

void til::postfix_writer::do_index_node(til::index_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (node->base()) { // it is a pointer
    node->base()->accept(this, lvl);
  }
  else {
    if (_functions.size() > 0) { // it is a return value
      _pf.LOCV(-_functions.at(_functions.size() - 1)->size()); 
      // return value is at the top of the stack
      // LOCV stands for LOCal Variable
    }
    else {
      std::cerr << "FATAL: " << node->lineno() << ": trying to use return value outside function" << std::endl;
    }
  }

  node->index()->accept(this, lvl);
  _pf.INT(3); // put 3 in the stack
  _pf.SHTL(); // shift left by 3 to multiply by 8 (size of a pointer)
  _pf.ADD();  // add pointer and index
}
//---------------------------------------------------------------------------

void til::postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  ASSERT_SAFE_EXPRESSIONS;

  const std::string &id = node->name();
  auto symbol = _symtab.find(id);
  if (symbol->global()) {
    _pf.ADDR(symbol->name()); // if global variable just use its name       
  }
  else {
    _pf.LOCAL(symbol->offset()); // if local variable use its offset
  }
}

void til::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.LDDOUBLE(); // double has size 8
  }
  else {
    // integers, pointers, and strings have size 4
    _pf.LDINT();
  }
}

void til::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl); // determine the new value
  _pf.DUP32();
  if (new_symbol() == nullptr) {
    node->lvalue()->accept(this, lvl); // where to store the value
  } else {
    _pf.DATA(); // variables are all global and live in DATA
    _pf.ALIGN(); // make sure we are aligned
    _pf.LABEL(new_symbol()->name()); // name variable location
    reset_new_symbol();
    _pf.SINT(0); // initialize it to 0 (zero)
    _pf.TEXT(); // return to the TEXT segment
    node->lvalue()->accept(this, lvl);  //DAVID: bah!
  }
  _pf.STINT(); // store the value at address
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_program_node(til::program_node * const node, int lvl) {
  // Note that til doesn't have functions. Thus, it doesn't need
  // a function node. However, it must start in the main function.
  // The ProgramNode (representing the whole program) doubles as a
  // main function node.

  // generate the main function (RTS mandates that its name be "_main")
  _pf.TEXT();
  _pf.ALIGN();
  _pf.GLOBAL("_main", _pf.FUNC());
  _pf.LABEL("_main");
  _pf.ENTER(0);  // til doesn't implement local variables

  node->statements()->accept(this, lvl);

  // end the main function
  _pf.INT(0);
  _pf.STFVAL32();
  _pf.LEAVE();
  _pf.RET();

  // these are just a few library function imports
  _pf.EXTERN("readi");
  _pf.EXTERN("printi");
  _pf.EXTERN("prints");
  _pf.EXTERN("println");
}
void til::postfix_writer::do_declaration_node(til::declaration_node * const node, int lvl) {
  // EMPTY
}
void til::postfix_writer::do_function_call_node(til::function_call_node * const node, int lvl) {
  size_t argsSize = 0;
  std::shared_ptr<cdk::functional_type> type;
  std::string id = ""; // function name
  if (node->function() != nullptr) {
    ASSERT_SAFE_EXPRESSIONS;
    type = std::dynamic_pointer_cast<cdk::functional_type>(node->function()->type());
    id = last_id();
  }
  else {
    auto __symbol = _type_group.at(_type_group.size() - 1); // get the last symbol in the stack
    type = std::dynamic_pointer_cast<cdk::functional_type>(__symbol.type()); // get the type of the last symbol
    id = __symbol.identifier();
  }
  if (node->arguments()->size() > 0) {
    for (int ax = node->arguments()->size() - 1; ax >= 0; ax--) {
      cdk::expression_node *arg = dynamic_cast<cdk::expression_node *>(node->arguments()->node(ax)); // get the argument
      arg->accept(this, lvl + 2);
      if (type->input(ax)->name() == cdk::TYPE_DOUBLE && arg->is_typed(cdk::TYPE_INT)) {
        _pf.I2D(); // convert int to double if needed
      }
      argsSize += type->input(ax)->size(); // add the size of the argument
    }
  }

  if (node->function() != nullptr) { // if it is a function
                                     //
    if (id != "") { 

      auto symbol = _symtab.find(id); // get the symbol
      
      if (symbol && symbol->qualifier() == tFOREIGN) {
        _pf.CALL(id); // call the function
      }

      else {
        node->function()->accept(this, lvl);
        _pf.BRANCH(); // branch to the function
      }
    
    } else {
      node->function()->accept(this, lvl);
      _pf.BRANCH(); // branch to the function
    }
    
    reset_last_id();
  
  } else {
    _pf.CALL(id); 
  }

  if (argsSize != 0) {
    _pf.TRASH(argsSize);
  }

  if (type->output(0)->name() == cdk::TYPE_INT || type->output(0)->name() == cdk::TYPE_POINTER || type->output(0)->name() == cdk::TYPE_STRING || type->output(0)->name() == cdk::TYPE_FUNCTIONAL) {
    _pf.LDFVAL32(); // if the output is an int, pointer, string, or functional
                    // load the value from the stack
  }
  else if (type->output(0)->name() == cdk::TYPE_DOUBLE)
  {
    _pf.LDFVAL64(); // if the output is a double, load the value from the stack
                    // double has size 8
  }
}
void til::postfix_writer::do_function_definition_node(til::function_definition_node * const node, int lvl) {
  // EMPTY
}
void til::postfix_writer::do_return_node(til::return_node * const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_evaluation_node(til::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->is_typed(cdk::TYPE_INT)) {
    _pf.TRASH(4); // delete the evaluated value
  } else if (node->argument()->is_typed(cdk::TYPE_STRING)) {
    _pf.TRASH(4); // delete the evaluated value's address
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void til::postfix_writer::do_print_node(til::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  for (size_t ix = 0; ix < node->arguments()->size(); ix++) {
    auto child = dynamic_cast<cdk::expression_node *>(node->arguments()->node(ix));

    std::shared_ptr<cdk::basic_type> etype = child->type();
    child->accept(this, lvl); // expression to print
                              
    if (etype->name() == cdk::TYPE_INT) {

      _functions_to_declare.insert("printi");
      _pf.CALL("printi");
      _pf.TRASH(4); // trash int
    }
    else if (etype->name() == cdk::TYPE_DOUBLE) {

      _functions_to_declare.insert("printd");
      _pf.CALL("printd");
      _pf.TRASH(8); // trash double
    }
    else if (etype->name() == cdk::TYPE_STRING) {

      _functions_to_declare.insert("prints");
      _pf.CALL("prints");
      _pf.TRASH(4); // trash char pointer
    }
    else {
      
      std::cerr << "cannot print expression of unknown type" << std::endl;
      return;
    }
  }

  if (node->append_newline()) {
    _functions_to_declare.insert("println");
    _pf.CALL("println");
  }
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_read_node(til::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.CALL("readi");
  _pf.LDFVAL32();
  node->accept(this, lvl);
  _pf.STINT();
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_loop_node(til::loop_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // label for the beginning of the loop
  _loop_cond.push_back(mklbl(lbl1)); // push the label to the stack
  node->condition()->accept(this, lvl); // determine the value
  _pf.JZ(mklbl(lbl2 = ++_lbl)); // jump if zero
  _loop_end.push_back(mklbl(lbl2)); // push the label to the stack
  node->block()->accept(this, lvl + 2); // generate the code for the block
  _pf.JMP(mklbl(lbl1)); // jump to the beginning of the loop
  _pf.LABEL(mklbl(lbl2)); // label for the end of the loop
  _loop_cond.pop_back(); // pop the label from the stack
  _loop_end.pop_back(); // pop the label from the stack
}

void til::postfix_writer::do_stop_node(til::stop_node * const node, int lvl) {
  if (_loop_end.size() != 0 && (int)_loop_end.size() >= node->level()) {

    _pf.JMP(_loop_end.at(_loop_end.size() - node->level())); // jump to the end of the loop
  }
  else {
    
      std::cerr << "FATAL: " << node->lineno() << ": trying to use stop outside loop" << std::endl;
  }
}

void til::postfix_writer::do_next_node(til::next_node * const node, int lvl) {
  if (_loop_cond.size() != 0 && (int)(_loop_cond.size()) >= node->level()) {

    _pf.JMP(_loop_cond.at(_loop_cond.size() - node->level())); // jump to the condition of the loop
  }
  else {

    std::cerr << "FATAL: " << node->lineno() << ": trying to use next outside loop" << std::endl;
  }
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_if_node(til::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_if_else_node(til::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}
