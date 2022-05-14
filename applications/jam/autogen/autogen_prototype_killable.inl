#include "autogen_prototype_killable.hpp"
#include "../../jam/components/killablecomp.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::killable>(const ::killable& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::killable>();
        prot.typeName = "killable";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members.emplace("health", member_value("health", primitive_value{typeHash(obj.health), std::any(obj.health)}));
        return prot;
    }
}
