#include "autogen_reflector_event_base.hpp"
#include "../../core/events/event.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::events::event_base>(legion::core::events::event_base& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::events::event_base>();
        refl.typeName = "legion::core::events::event_base";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::events::event_base>(const legion::core::events::event_base& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::events::event_base>();
        refl.typeName = "legion::core::events::event_base";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
