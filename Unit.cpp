#include<iostream>
#include <memory>
#include <vector>
class Unit {
public: using Flags = unsigned int;
    virtual~Unit() =
        default;
    virtual void add(Unit * dd, Flags) {
        throw std::runtime_error("Not supported");
    }
  //  virtual   std::string createPrint( );
    virtual std::string compile(unsigned int level = 0) const = 0;
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

class IMethodUnit : public Unit {
public: enum Modifier {
        STATIC = 1,
        CONST = 1 << 1,
        VIRTUAL = 1 << 2,
        FINAL = 1 << 3,
        ABSTRACT = 1 << 4
    };
public:
    std::string static name;
    std::string vozvr;
    int Num = 0;

};
class IClassUnit : public Unit { public: virtual void add(IMethodUnit* unit, Flags flags) = 0; };
class CsharpMethodUnit : public IMethodUnit {

public: CsharpMethodUnit( std::string  name,
                     std::string returnType, Flags
                         flags): m_name(name),
        m_returnType(returnType),
        m_flags(flags) {}

    ~CsharpMethodUnit(){

        for (  auto & elem : m_body)
        {
            delete (elem);
            ++elem;
        }
    };
    void add( Unit * dd , Flags)override {
        m_body.push_back(dd);
    };
    std::string compile(unsigned int level = 0) const override{
        std::string result = generateShift(level);
        if (m_flags & STATIC) {
            result += "static ";
        } else if (m_flags & VIRTUAL) {
            result += "virtual ";
        }
        result += m_returnType + " ";
        result += m_name + "()";
        if (m_flags & CONST)
        {
           result += " const";
        }
        result += " {\n";
        for (const auto & b: m_body) {
            result += b -> compile(level + 1);
        }
        result += generateShift(level) + "}\n";
        return result;
    };

    std::string m_name =" ";
    std::string m_returnType = " ";
    Flags m_flags;
    std::vector <  Unit * > m_body;};
class CMethodUnit : public IMethodUnit {

public: CMethodUnit( std::string  name,
                     std::string returnType, Flags
                         flags): m_name(name),
        m_returnType(returnType),
        m_flags(flags) {}

    ~CMethodUnit(){

        for (  auto & elem : m_body)
        {
            delete (elem);
            ++elem;
        }
    };
    void add( Unit * dd , Flags)override {
        m_body.push_back(dd);
    };
    std::string compile(unsigned int level = 0) const {
        std::string result = generateShift(level);
        if (m_flags & STATIC) {
            result += "static ";
        } else if (m_flags & VIRTUAL) {
            result += "virtual ";
        }
        result += m_returnType + " ";
        result += m_name + "()";
        if (m_flags & CONST)
        {
            result += " const";
        }
        result += " {\n";
        for (const auto & b: m_body) {
            result += b -> compile(level + 1);
        }
        result += generateShift(level) + "}\n";
        return result;
    };

    std::string m_name =" ";
    std::string m_returnType = " ";
    Flags m_flags;
    std::vector <  Unit * > m_body;};
class JavaMethodUnit : public IMethodUnit {

public: JavaMethodUnit( std::string  name,
                std::string returnType, Flags
                    flags): m_name(name),
        m_returnType(returnType),
        m_flags(flags) {}
    JavaMethodUnit(const JavaMethodUnit & obj)
    {
        this->m_returnType=obj.m_returnType;
        this->m_flags=obj.m_flags;
        this->m_name=obj.m_name;
        this->m_body=obj.m_body;
        this->name = obj.name;
    }
    ~JavaMethodUnit(){

        for (  auto & elem : m_body)
        {
            delete (elem);
            ++elem;
        }
    };
    void add( Unit * dd , Flags)override {
        m_body.push_back(dd);
    };
    std::string compile(unsigned int level = 0) const override{
        std::string result = generateShift(level);
        if (m_flags & STATIC) {
            result += "static ";
        } else if (m_flags & FINAL) {
            result += "final ";
        }
        if (m_flags & ABSTRACT)
        {
            result += "abstract ";
        }
        if (m_flags & (VIRTUAL|CONST))
        {throw "wrong modifier";}
        result += m_returnType + " ";
        result += m_name + "()";

        result += " {\n";
        for (const auto & b: m_body) {
            result += b -> compile(level + 1);
        }
        result += generateShift(level) + "}\n";
        return result;
    };

    std::string m_name =" ";
    std::string m_returnType = " ";
    Flags m_flags;
    std::vector <  Unit * > m_body;};
class CClassUnit: public IClassUnit {
public: enum AccessModifier {
        PUBLIC,
        PROTECTED,
        PRIVATE
    };
    static const std::vector < std::string > ACCESS_MODIFIERSC;
public: explicit CClassUnit(const std::string & name): m_name(name) {
        m_fields.resize(ACCESS_MODIFIERSC.size());
    }
    ~CClassUnit () {
        for (  auto & elem : m_fields)
        {
            for (  auto & elem2 : elem )
            {
                delete (elem2);
            }
        }
    }
    void add ( IMethodUnit *  method, Flags flags) override{
        int AccessModifier = PRIVATE;
        if (flags < ACCESS_MODIFIERSC.size()) {
            AccessModifier = flags;
        }
        if (flags >= 3 )
        {throw "wrong modifier";}
        m_fields[AccessModifier].push_back(method);
    }
    std::string  compile(unsigned int level = 0)  const override{
        std::string result = generateShift(level) + "class " + m_name + " {\n";

        for (size_t i = 0; i < ACCESS_MODIFIERSC.size(); ++i) {
            if (m_fields[i].empty()) {
                continue;
            }
            result += ACCESS_MODIFIERSC[i] + ":\n";
            for (const auto & f: m_fields[i]) {
                result += f -> compile(level + 1);
            }
            result += "\n";
        }
        result += generateShift(level) + "};\n";
        return result;
    }
private: std::string m_name;
    using Fields = std::vector < Unit * > ;
    std::vector < Fields > m_fields;
};
const std::vector< std::string > CClassUnit::ACCESS_MODIFIERSC = { "public",
                                                                    "protected", "private"};
class JavaClassUnit: public IClassUnit {
public: enum AccessModifier {
        PUBLIC,
        PROTECTED,
        PRIVATE
    };
    static const std::vector < std::string > ACCESS_MODIFIERS;
public: explicit JavaClassUnit(const std::string & name): m_name(name) {
        m_fields.resize(ACCESS_MODIFIERS.size());
    }
    ~JavaClassUnit () {
        for (  auto & elem : m_fields)
        {
            for (  auto & elem2 : elem )
            {
                delete (elem2);
            }
        }
    }
    void add ( IMethodUnit *  method, Flags flags) override{
        int AccessModifier = PRIVATE;
        if (flags < ACCESS_MODIFIERS.size()) {
            AccessModifier = flags;
        }
        if (flags >= 3 )
       {throw "wrong modifier";}
        m_fields[AccessModifier].push_back(method);
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
    using Fields = std::vector < Unit * > ;
    std::vector < Fields > m_fields;
};
const std::vector< std::string > JavaClassUnit::ACCESS_MODIFIERS = { "public",
                                                               "protected", "private"};
class CsharpClassUnit: public IClassUnit {
public: enum AccessModifier {
        PUBLIC,
        PROTECTED,
        PRIVATE,
        PRIVATE_PROTECTED,
        INTERNAL,
        PROTECTED_INTERNAL
    };
    static const std::vector < std::string > ACCESS_MODIFIERS;
public: explicit CsharpClassUnit(const std::string & name): m_name(name) {
        m_fields.resize(ACCESS_MODIFIERS.size());
    }
    ~CsharpClassUnit () {
        for (  auto & elem : m_fields)
        {
            for (  auto & elem2 : elem )
            {
                delete (elem2);
            }
        }
    }
    void add ( IMethodUnit *  method, Flags flags) override{
        int AccessModifier = PRIVATE;
        if (flags < ACCESS_MODIFIERS.size()) {
            AccessModifier = flags;
        }
        m_fields[AccessModifier].push_back(method);
    }
    std::string  compile(unsigned int level = 0)  const override{
        std::string result = generateShift(level) + "class " + m_name + " {\n";

        for (size_t i = 0; i < ACCESS_MODIFIERS.size(); ++i) {
            if (m_fields[i].empty()) {
                continue;
            }

            for (const auto & f: m_fields[i]) {
                result += ACCESS_MODIFIERS[i];
                result += f -> compile(level + 1);
            }
            result += "\n";
        }
        result += generateShift(level) + "};\n";
        return result;
    }
private: std::string m_name;
    using Fields = std::vector < Unit * > ;
    std::vector < Fields > m_fields;
};
const std::vector< std::string > CsharpClassUnit::ACCESS_MODIFIERS = { "public",
                                                                    "protected", "private","private protected","internal","protected internal"};


class IPrintOperatorUnit : public Unit {
public: virtual   std::string createPrint( )=0;

};

class CsharpPrintOperatorUnit : public IPrintOperatorUnit {
public:

    explicit CsharpPrintOperatorUnit(const std::string &text) : m_text(text) {}
    void add(Unit * dd , Flags)override {std::cout<<"YaEstGroot";}
    std::string compile(unsigned int level = 0) const override {
        return "Console.WriteLine(\"" + m_text + "\");\n";
    }
    std::string generateShift(unsigned int level) const override {    return "Compiled code: " + m_text;}
    std::string createPrint( )override  {
        return "Console.WriteLine(\"" + m_text + "\");\n";
    }

private:
    std::string m_text;
};







class Fabric {
public:
    virtual IClassUnit* createClass(std::string name) = 0;
    virtual IMethodUnit* createMethod(std::string name, std::string vozvr,int Num) = 0;
    virtual IPrintOperatorUnit* createPrint2( std::string text)=0;
};

class CFabric: public Fabric {

    public:
    IClassUnit* createClass(std::string name)override
    {return new CClassUnit(name);}
    IMethodUnit* createMethod(std::string name, std::string vozvr, int Num) override
         {
         IMethodUnit* CMethodUnitPTR = new CMethodUnit(name,vozvr, Num);
         return CMethodUnitPTR;
          }
   IPrintOperatorUnit*  createPrint2 ( std::string  text)override  {return new CsharpPrintOperatorUnit(text);}
};
class JavaFabric: public Fabric {

    public: IClassUnit* createClass(std::string name)override{
        return new JavaClassUnit(name);
    }
        IMethodUnit* createMethod(std::string name, std::string vozvr, int Num) override {
        IMethodUnit* JavaMethodUnitPTR = new JavaMethodUnit(name,vozvr, Num);

        return JavaMethodUnitPTR;
    }
 IPrintOperatorUnit*  createPrint2 ( std::string  text)override  {return new CsharpPrintOperatorUnit(text);}
};

class CsharpFabric : public Fabric {
    public: IClassUnit* createClass(std::string name)override
    {
        return new CsharpClassUnit(name);
    }
    IMethodUnit* createMethod(std::string name, std::string vozvr, int Num) override {
        IMethodUnit* CsharpMethodUnitPTR = new CsharpMethodUnit(name,vozvr, Num);
        return CsharpMethodUnitPTR;
    }
 IPrintOperatorUnit*  createPrint2 ( std::string  text)override  {return new CsharpPrintOperatorUnit(text);}
};

//->createPrint(text)
std::string generateProgram2(Fabric* fabric) {
     try {
    IClassUnit* classA = fabric->createClass("keek");
    IMethodUnit* methodB = fabric->createMethod("testFunc1", "void", 0);
    IPrintOperatorUnit* printA = fabric->createPrint2("fa123");
    std::string a = methodB->compile();
    std::string b = classA->compile();
    classA->add(methodB,0);

 IMethodUnit* methodC = fabric->createMethod("testFunc3", "void",IMethodUnit::FINAL|IMethodUnit::STATIC);

        classA->add(methodC,0);

      IMethodUnit* methodD = fabric->createMethod("testFunc2", "void", 1);
           classA->add(methodD,1);

    auto method = fabric->createMethod("testFunc4", "void", 1);
  //  PrintOperatorUnit rofl("fff");
      IPrintOperatorUnit* lolkek = printA;

      method->add( lolkek,1);
    classA->add(method,2);
    return classA->compile();
     }
         catch(const char* ex){std::cout<<ex<<std::endl;}
}
