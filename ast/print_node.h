#pragma once

#include <cdk/ast/sequence_node.h>

namespace til {

  /**
   * Class for describing print nodes.
   */
  class print_node : public cdk::basic_node {
    cdk::sequence_node *_arguments;
    bool _append_newline;

  public:
    print_node(int lineno, cdk::sequence_node *arguments, bool append_newline = false) :
        cdk::basic_node(lineno), _arguments(arguments), _append_newline(append_newline) {
    }

    cdk::sequence_node *arguments() { return _arguments; }

    bool append_newline() { return _append_newline; }

    void accept(basic_ast_visitor *sp, int level) { sp->do_print_node(this, level); }

  };

} // til
