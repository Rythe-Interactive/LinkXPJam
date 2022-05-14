#include "autogen_reflector_killable.hpp"
#include "../../jam/components/killablecomp.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::killable>(::killable& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::killable>();
        refl.typeName = "killable";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("health", member_reference("health", primitive_reference{typeHash(obj.health), &obj.health}));
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::killable>(const ::killable& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::killable>();
        refl.typeName = "killable";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("health", member_reference("health", primitive_reference{typeHash(obj.health), &obj.health}));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
