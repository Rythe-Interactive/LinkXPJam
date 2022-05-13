#include "autogen_reflector_error_t.hpp"
#include "../../core/common/result.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::common::error_t>(legion::core::common::error_t& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::common::error_t>();
        refl.typeName = "legion::core::common::error_t";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::common::error_t>(const legion::core::common::error_t& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::common::error_t>();
        refl.typeName = "legion::core::common::error_t";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
