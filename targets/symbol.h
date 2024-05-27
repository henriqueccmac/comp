#pragma once

#include <string>
#include <memory>
#include <cdk/types/basic_type.h>

namespace til {

  class symbol {
    std::string _name; // name of the symbol
    bool _constant;    // true if the symbol is a constant, false otherwise
    int _qualifier;    // qualifier of the symbol external, public, forward
    std::shared_ptr<cdk::basic_type> _type;
    bool _initialized; // true if the symbol has been initialized, false otherwise
    int _offset = 0;   // offset in the stack frame
    bool _function;    // true if the symbol is a function, false otherwise
    
  public:
    symbol(bool constant, int qualifier, std::shared_ptr<cdk::basic_type> type, const std::string &name, bool initialized,
           bool function) : _name(name), _constant(constant), _qualifier(qualifier), _type(type), _initialized(initialized), _function(function)
    {  }

    virtual ~symbol()
    {
      // EMPTY
    }

    const std::string &name() const {
      return _name;
    }

    bool constant() const {
      return _constant;
    }

    int qualifier() const {
      return _qualifier;
    }

    std::shared_ptr<cdk::basic_type> type() const {
      return _type;
    }

    void set_type(std::shared_ptr<cdk::basic_type> t) {
      _type = t;
    }

    void set_qual(int qualifier) {
      _qualifier = qualifier;
    }

    bool is_typed(cdk::typename_type name) const {
      return _type->name() == name;
    }

    const std::string &identifier() const {
      return name();
    }

    bool initialized() const {
      return _initialized;
    }

    int offset() const {
      return _offset;
    }

    void set_offset(int ofs) {
      _offset = ofs;
    }

    bool isFunction() const {
      return _function;
    }

    bool global() const {
      return _offset == 0;
    }

    bool isVariable() const {
      return !_function;
    }
  };

} // til
