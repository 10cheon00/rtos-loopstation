#ifndef PAGE_SLOT_HPP
#define PAGE_SLOT_HPP

#include "fx_parameter_id.hpp"
#include "fx_section.hpp"
#include "fx_slot.hpp"
#include "menu_icon_encoding.hpp"
#include "page_slot_type.hpp"
#include "parameter_id.hpp"
#include "parameter_value.hpp"
#include "ui_state_id.hpp"

class PageSlot {
 public:
  constexpr PageSlot() : PageSlot("") {}
  constexpr PageSlot(const char* label) : label(label) {}

  const char* GetLabel() const { return this->label; }

 private:
  const char* label;
};

class MenuSlot : public PageSlot {
 public:
  MenuSlot(MenuIconEncoding icon_encoding, UiStateMachine::Id state_id,
           const char* label)
      : PageSlot(label), icon_encoding(icon_encoding), state_id(state_id) {}

  MenuIconEncoding GetIconEncoding() const { return this->icon_encoding; }
  UiStateMachine::Id GetUiStateId() const { return this->state_id; }

 private:
  MenuIconEncoding icon_encoding;
  UiStateMachine::Id state_id;
};

struct SystemParameterRef {
  ParameterModel::Id id;
};

struct FxParameterRef {
  Fx::ParameterId id;
};

using ParameterRef =
    std::variant<std::monostate, SystemParameterRef, FxParameterRef>;

class ParameterSlot : public PageSlot {
 public:
  ParameterSlot(ParameterRef parameter_ref, const char* label)
      : PageSlot(label), parameter_ref(parameter_ref) {}

  ParameterRef& GetParameterRef() { return parameter_ref; }

 private:
  ParameterRef parameter_ref;
};

#endif
