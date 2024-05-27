#pragma once

#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>
#include <string>
#include <vector>


namespace til {

  /**
   * Class for describing function definition nodes.
   */
  class function_definition_node: public cdk::expression_node {
    cdk::sequence_node *_declarations;
    til::block_node *_block;

  public:
      function_definition_node(int lineno, std::shared_ptr<cdk::basic_type> return_type,
              cdk::sequence_node *declarations, til::block_node *block) :
        
	    cdk::expression_node(lineno), _declarations(declarations), _block(block) {
            std::vector<std::shared_ptr<cdk::basic_type>> args;
            if (declarations) {
                for (auto declaration : declarations->nodes()) {
                    args.push_back(((cdk::expression_node*)declaration)->type());
                }
            }
            type(cdk::functional_type::create(args, return_type));
    }

  public:

    cdk::sequence_node *declarations() { return _declarations; }

    cdk::typed_node* declaration(size_t ax) {
      return dynamic_cast<cdk::typed_node*>(_declarations->node(ax));
    }

    til::block_node *block() { return _block; }

    void accept(basic_ast_visitor *sp, int level) { sp->do_function_definition_node(this, level); }

  };

}
