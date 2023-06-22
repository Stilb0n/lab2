#include<iostream>
#include <memory>
#include <vector>


class Unit {
  public: using Flags = unsigned int;
  public: virtual~Unit() =
    default;
  virtual void add(const std::shared_ptr < Unit > & , Flags) {
    throw std::runtime_error("Not supported");
  }
  virtual std::string compile(unsigned int level = 0) const =
  0;
  protected: virtual std::string generateShift(unsigned int level) const {
    static
    const auto DEFAULT_SHIFT = " ";
    std::string result;
    for (unsigned int i = 0; i < level; ++i) {
      result += DEFAULT_SHIFT;
    }
    return result;
  }
};






class PrintOperatorUnit: public Unit {
  public: explicit PrintOperatorUnit(const std::string & text): m_text(text) {}
  std::string compile(unsigned int level = 0) const {
    return generateShift(level) + "printf( \"" + m_text + "\" );\n";
  }
  private: std::string m_text;
};

//std::string generateProgram2() {
//  ClassUnit myClass("MyClass");
//  myClass.add(
//    std::make_shared < MethodUnit > ("testFunc1", "void", 0),
//    ClassUnit::PUBLIC
//  );
//  myClass.add(
//    std::make_shared < MethodUnit > ("testFunc2", "void", MethodUnit::STATIC),
//    ClassUnit::PRIVATE
//  );
//  myClass.add(
//    std::make_shared < MethodUnit > ("testFunc3", "void", MethodUnit::VIRTUAL |
//      MethodUnit::CONST),
//    ClassUnit::PUBLIC
//  );
//  auto method = std::make_shared < MethodUnit > ("testFunc4", "void",
//    MethodUnit::STATIC);
//  method -> add(std::make_shared < PrintOperatorUnit > ( "(Hello, world!\n)"));
//  myClass.add(method, ClassUnit::PROTECTED);
//  return myClass.compile();
//}



class IMethodUnit: public Unit {};

 class IClassUnit: public Unit {};


class CsharpMethodUnit: public IMethodUnit {
public: enum Modifier {
  STATIC = 1,
    CONST = 1 << 1,
    VIRTUAL = 1 << 2
};
public: CsharpMethodUnit(const std::string & name,
    const std::string & returnType, Flags
  flags): m_name(name),
m_returnType(returnType),
m_flags(flags) {}
void add(const std::shared_ptr < Unit > & unit, Flags /* flags */ = 0) {
  m_body.push_back(unit);
}
std::string compile(unsigned int level = 0) const {
  std::string result = generateShift(level);
  if (m_flags & STATIC) {
    result += "static ";
  } else if (m_flags & VIRTUAL) {
    result += "virtual ";
  }
  result += m_returnType + " ";
  result += m_name + "()";
  if (m_flags & CONST) {
    result += " const";
  }
  result += " {\n";
  for (const auto & b: m_body) {
    result += b -> compile(level + 1);
  }
  result += generateShift(level) + "}\n";
  return result;
}
private: std::string m_name;
std::string m_returnType;
Flags m_flags;
std::vector < std::shared_ptr < Unit > > m_body;};
class CsharpClassUnit: public IClassUnit {
public: enum AccessModifier {
  PUBLIC,
  PROTECTED,
  PRIVATE
};
static
const std::vector < std::string > ACCESS_MODIFIERS;
public: explicit CsharpClassUnit(const std::string & name): m_name(name) {
  m_fields.resize(ACCESS_MODIFIERS.size());
}
void add (const std::shared_ptr < Unit > & unit, Flags flags) override{

  int accessModifier = PRIVATE;

  if (flags < ACCESS_MODIFIERS.size()) {
    accessModifier = flags;
  }
  m_fields[accessModifier].push_back(unit);
}
std::string  compile(unsigned int level = 0)  const override{
  std::string result = generateShift(level) + "class " + m_name + " {\n";

  for (size_t i = 0; i < ACCESS_MODIFIERS.size(); ++i) {
    if (m_fields[i].empty()) {
      continue;
    }
    result += ACCESS_MODIFIERS[i] + ":\n";
    for (const auto & f: m_fields[i]) {
      result += f -> compile(level + 1);
    }
    result += "\n";
  }
  result += generateShift(level) + "};\n";
  return result;
}
private: std::string m_name;
using Fields = std::vector < std::shared_ptr < Unit > > ;
std::vector < Fields > m_fields;
};
const std::vector< std::string > CsharpClassUnit::ACCESS_MODIFIERS = { "public",
"protected", "private" };

class JavaMethodUnit:public IMethodUnit { public: enum Modifier {
        STATIC = 1,
          CONST = 1 << 1,
          VIRTUAL = 1 << 2
      };
      public: JavaMethodUnit(const std::string & name,
          const std::string & returnType, Flags
        flags): m_name(name),
      m_returnType(returnType),
      m_flags(flags) {}
      void add(const std::shared_ptr < Unit > & unit, Flags /* flags */ = 0) {
        m_body.push_back(unit);
      }
      std::string compile(unsigned int level = 0) const {
        std::string result = generateShift(level);
        if (m_flags & STATIC) {
          result += "static ";
        } else if (m_flags & VIRTUAL) {
          result += "virtual ";
        }
        result += m_returnType + " ";
        result += m_name + "()";
        if (m_flags & CONST) {
          result += " const";
        }
        result += " {\n";
        for (const auto & b: m_body) {
          result += b -> compile(level + 1);
        }
        result += generateShift(level) + "}\n";
        return result;
      }
      private: std::string m_name;
      std::string m_returnType;
      Flags m_flags;
      std::vector < std::shared_ptr < Unit > > m_body;
    };
class JavaClassUnit:public IClassUnit {
public: enum AccessModifier {
    PUBLIC,
    PROTECTED,
    PRIVATE
  };
  static
  const std::vector < std::string > ACCESS_MODIFIERS;
  public: explicit JavaClassUnit(const std::string & name): m_name(name) {
    m_fields.resize(ACCESS_MODIFIERS.size());
  }
  void add (const std::shared_ptr < Unit > & unit, Flags flags) override{

    int accessModifier = PRIVATE;

    if (flags < ACCESS_MODIFIERS.size()) {
      accessModifier = flags;
    }
    m_fields[accessModifier].push_back(unit);
  }
  std::string  compile(unsigned int level = 0)  const override{
    std::string result = generateShift(level) + "class " + m_name + " {\n";

    for (size_t i = 0; i < ACCESS_MODIFIERS.size(); ++i) {
      if (m_fields[i].empty()) {
        continue;
      }
      result += ACCESS_MODIFIERS[i] + ":\n";
      for (const auto & f: m_fields[i]) {
        result += f -> compile(level + 1);
      }
      result += "\n";
    }
    result += generateShift(level) + "};\n";
    return result;};
private: std::string m_name;
using Fields = std::vector < std::shared_ptr < Unit > > ;
std::vector < Fields > m_fields;
};
const std::vector< std::string > JavaClassUnit::ACCESS_MODIFIERS = { "public",
"protected", "private" };

class CMethodUnit:public IMethodUnit {
public: enum Modifier {
  STATIC = 1,
    CONST = 1 << 1,
    VIRTUAL = 1 << 2
};
public: CMethodUnit(const std::string & name,
    const std::string & returnType, Flags
  flags): m_name(name),
m_returnType(returnType),
m_flags(flags) {}
void add(const std::shared_ptr < Unit > & unit, Flags /* flags */ = 0) {
  m_body.push_back(unit);
}
std::string compile(unsigned int level = 0) const {
  std::string result = generateShift(level);
  if (m_flags & STATIC) {
    result += "static ";
  } else if (m_flags & VIRTUAL) {
    result += "virtual ";
  }
  result += m_returnType + " ";
  result += m_name + "()";
  if (m_flags & CONST) {
    result += " const";
  }
  result += " {\n";
  for (const auto & b: m_body) {
    result += b -> compile(level + 1);
  }
  result += generateShift(level) + "}\n";
  return result;
}
private: std::string m_name;
std::string m_returnType;
Flags m_flags;
std::vector < std::shared_ptr < Unit > > m_body;};
class CClassUnit:public IClassUnit {  public: enum AccessModifier {
        PUBLIC,
        PROTECTED,
        PRIVATE
      };
      static
      const std::vector < std::string > ACCESS_MODIFIERS;
      public: explicit CClassUnit(const std::string & name): m_name(name) {
        m_fields.resize(ACCESS_MODIFIERS.size());
      }
      void add(const std::shared_ptr < Unit > & unit, Flags flags) {

        int accessModifier = PRIVATE;

        if (flags < ACCESS_MODIFIERS.size()) {
          accessModifier = flags;
        }
        m_fields[accessModifier].push_back(unit);
      }
      std::string compile(unsigned int level = 0) const {
        std::string result = generateShift(level) + "class " + m_name + " {\n";

        for (size_t i = 0; i < ACCESS_MODIFIERS.size(); ++i) {
          if (m_fields[i].empty()) {
            continue;
          }
          result += ACCESS_MODIFIERS[i] + ":\n";
          for (const auto & f: m_fields[i]) {
            result += f -> compile(level + 1);
          }
          result += "\n";
        }
        result += generateShift(level) + "};\n";
        return result;
      }
      private: std::string m_name;
      using Fields = std::vector < std::shared_ptr < Unit > > ;
      std::vector < Fields > m_fields;

    };
const std::vector< std::string > CClassUnit::ACCESS_MODIFIERS = { "public",
"protected", "private" };



class Fabric{
public:
    virtual IClassUnit* createClass()=0;
    virtual IMethodUnit* createMethod()=0;
};

class JavaFabric: public Fabric {
    IClassUnit* createClass()override{
    return new JavaClassUnit("kek");
    }
    IMethodUnit* createMethod() override{
        return new JavaMethodUnit("foo","void",CsharpMethodUnit::STATIC);
        }
};
class CFabric: public Fabric {
    IClassUnit* createClass()override{
    return new CClassUnit("kek");
    }
    IMethodUnit* createMethod() override{
        return new CMethodUnit("foo","void",CsharpMethodUnit::STATIC);
        }
};
class CsharpFabric: public Fabric {
    IClassUnit* createClass()override{
    return new CsharpClassUnit("kek");
    }
    IMethodUnit* createMethod() override{
        return new CsharpMethodUnit("foo","void",CsharpMethodUnit::STATIC);
        }

};











