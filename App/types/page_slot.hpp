#ifndef PAGE_SLOT_HPP
#define PAGE_SLOT_HPP

#include "menu_icon_encoding.hpp"
#include "panel_slot_type.hpp"
#include "parameter_id.hpp"
#include "ui_state_id.hpp"

class PageSlot {
 public:
  PageSlot() : PageSlot(PanelSlotType::NONE, "") {}
  PageSlot(PanelSlotType type, const char* label) : type(type), label(label) {}

  PanelSlotType GetType() const { return this->type; }
  const char* GetLabel() const { return this->label; }

 private:
  PanelSlotType type;  // 없어도 되는 필드지만, 큐에 전달할 때 타입을
                       // 명시해야하므로 보관한다.
  const char* label;
};

class MenuSlot : public PageSlot {
 public:
  MenuSlot(MenuIconEncoding icon_encoding, UiStateMachine::Id state_id,
           const char* label)
      : PageSlot(PanelSlotType::MENU, label),
        icon_encoding(icon_encoding),
        state_id(state_id) {}

  MenuIconEncoding GetIconEncoding() const { return this->icon_encoding; }
  UiStateMachine::Id GetUiStateId() const { return this->state_id; }

 private:
  MenuIconEncoding icon_encoding;
  UiStateMachine::Id state_id;
};

class ParameterSlot : public PageSlot {
 public:
  ParameterSlot(ParameterId id, const char* label)
      : PageSlot(PanelSlotType::PARAMETER, label), id(id) {}

  ParameterId GetParameterId() const { return id; }

 private:
  ParameterId id;
};

#endif
