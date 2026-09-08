#ifndef PAGE_SLOT_HPP
#define PAGE_SLOT_HPP

#include "menu_icon_encoding.hpp"
#include "page_slot_type.hpp"
#include "parameter_id.hpp"
#include "ui_state_id.hpp"

class PageSlot {
 public:
  PageSlot() : PageSlot("") {}
  PageSlot(const char* label) : label(label) {}
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

class ParameterSlot : public PageSlot {
 public:
  ParameterSlot(ParameterId id, const char* label) : PageSlot(label), id(id) {}

  ParameterId GetParameterId() const { return id; }

 private:
  ParameterId id;
};

#endif
