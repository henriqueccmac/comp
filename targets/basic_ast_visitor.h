#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <cdk/compiler.h>
#include <cdk/symbol_table.h>
#include "targets/symbol.h"

/* do not edit -- include node forward declarations */
#define __NODE_DECLARATIONS_ONLY__
#include ".auto/all_nodes.h"  // automatically generated
#undef __NODE_DECLARATIONS_ONLY__
/* do not edit -- end */

//!
//! Print nodes as XML elements to the output stream.
//!
class basic_ast_visitor {
protected:
  //! The owner compiler
  std::shared_ptr<cdk::compiler> _compiler;

private:

  // last symbol inserted in symbol table
  std::shared_ptr<til::symbol> _new_symbol;
  std::string _lastId;

protected:
  basic_ast_visitor(std::shared_ptr<cdk::compiler> compiler) :
      _compiler(compiler) {
  }

  bool debug() {
    return _compiler->debug();
  }

  std::ostream &os() {
    return *_compiler->ostream();
  }

public:
  virtual ~basic_ast_visitor() {
  }

public:
  std::shared_ptr<til::symbol> new_symbol() {
    return _new_symbol;
  }

  void set_new_symbol(std::shared_ptr<til::symbol> symbol) {
    _new_symbol = symbol;
  }

  void reset_new_symbol() {
    _new_symbol = nullptr;
  }
  std::string last_id()
  {
    return _lastId;
  }

  void reset_last_id()
  {
    _lastId = "";
  }

  void set_last_id(std::string id)
  {
    _lastId = id;
  }

public:
  // do not edit these lines
#define __IN_VISITOR_HEADER__
#define __PURE_VIRTUAL_DECLARATIONS_ONLY__
#include ".auto/visitor_decls.h"       // automatically generated
#undef __PURE_VIRTUAL_DECLARATIONS_ONLY__
#undef __IN_VISITOR_HEADER__
  // do not edit these lines: end

};
