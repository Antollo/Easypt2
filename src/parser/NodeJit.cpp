#include "Node.h"
#include "nobject.h"
#include "console.h"
#include <cassert>
#include <exception>
#include <algorithm>
#include <iterator>

void Node::toIntermediateLanguage(std::stringstream &result, Node::translationContextType &translationContext) const
{
    switch (_token & AB_MASK)
    {
    case COMPOUND_STATEMENT:
    {
        auto usedNames = getUsedNames(); // names used in this scope or deeper
        //auto declaredNames = getDeclaredNames(); // names declared in this scope
        //auto translationContextCopy = translationContext;
        //translationContextCopy.namesDeclaredInFunctionScope.insert(declaredNames.begin(), declaredNames.end());

        result << "// Used names: ";
        for (const auto &n : usedNames)
            result << static_cast<std::string>(n) << ", ";
        result << "\n";

        //result << "// Declared names: ";
        //for (const auto &n : declaredNames)
        //    result << static_cast<std::string>(n) << ", ";
        //result << "\n";

        //result << "// Names declared in function scope: ";
        //for (const auto &n : translationContext.namesDeclaredInFunctionScope)
        //    result << static_cast<std::string>(n) << ", ";
        //result << "\n";

        auto namesToCapture = usedNames;
        auto closureName = translationContext.getClosureName();
        //std::set_intersection(usedNames.begin(), usedNames.end(), translationContextCopy.namesDeclaredInFunctionScope.begin(), 
        //    translationContextCopy.namesDeclaredInFunctionScope.end(), std::back_inserter(namesToCapture));
        result << "closureType " << closureName << " = cl(objectPtrPtr, (contextType *context), (";
        for (const auto &n : namesToCapture)
            result << "(objectPtrPtr, " << static_cast<std::string>(n) << "),";
        if (!namesToCapture.empty())
            result.seekp(-1, result.cur);
        result << ") {\n";
        for (const auto &n : namesToCapture)
            result << "objectPtrPtr " << static_cast<std::string>(n) << " = env->" << static_cast<std::string>(n) << ";\n";
        for (const auto &child : children())
            child.toIntermediateLanguage(result, translationContext);
        result << "});\nwithContext(" << closureName << ");\n";
        break;

        

        /*size_t i = 0;
        stack localStack(&st);
        try
        {
            if (!_children.empty())
            {
                const size_t size = _children.size() - 1;
                for (; i < size; i++)
                    _children[i].evaluateVoid(localStack);
                i++;
                return _children.back().evaluate(localStack);
            }
        }
        catch (objectException &e)
        {
            _children[i].exception();
            throw e;
        }
        catch (std::exception &e)
        {
            _children[i].exception();
            throw e;
        }
        return nullptr;*/
    }


    case LET:
        result << "letOp(" << static_cast<name::codeType>(_text) << ", context);\n";
        break;


    default:
        console::error(toName() + " missing implementation"s);
        break;
    }
}