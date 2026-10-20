#include "fx_page_group_map.hpp"

namespace FxPageGroupMap {

using PageGroupPointer = PageGroup*;

static NoneEditPageGroup none_edit_page_group{};
static LpfEditPageGroup lpf_edit_page_group{};

static EnumMap<Fx::Type, PageGroupPointer> FX_PAGE_GROUP_MAP{
    EnumEntry{Fx::Type::NONE, &none_edit_page_group},
    EnumEntry{Fx::Type::LPF, &lpf_edit_page_group},
};

PageGroup& Get(Fx::Type selected_fx) { return *FX_PAGE_GROUP_MAP[selected_fx]; }

};  // namespace FxPageGroupMap