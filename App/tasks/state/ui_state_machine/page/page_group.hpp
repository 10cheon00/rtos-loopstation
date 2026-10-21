#ifndef PAGE_GROUP_HPP
#define PAGE_GROUP_HPP

#include <cstddef>

#include "menu_icon_encoding.hpp"
#include "page.hpp"
#include "page_slot.hpp"
#include "parameter_id.hpp"
#include "ui_state_id.hpp"

class PageGroup {
 public:
  virtual ~PageGroup() = default;

  virtual std::size_t GetCount() const = 0;
  virtual Page& GetAt(std::size_t index) = 0;
};

template <std::size_t PageCount>
class FixedPageGroup : public PageGroup {
 public:
  template <typename... Pages>
  constexpr explicit FixedPageGroup(Pages... pages) : pages{pages...} {}

  std::size_t GetCount() const override { return pages.size(); }

  Page& GetAt(std::size_t index) { return pages[index]; }

 private:
  std::array<Page, PageCount> pages;
};

class NonePageGroup : public FixedPageGroup<1> {
 public:
  constexpr explicit NonePageGroup()
      : FixedPageGroup<1>(Page{
            PageSlot{},
            PageSlot{},
            PageSlot{},
            PageSlot{},
        }) {}
};

class HomePageGroup : public FixedPageGroup<1> {
 public:
  constexpr explicit HomePageGroup()
      : FixedPageGroup<1>(Page{
            PageSlot{},
            PageSlot{},
            PageSlot{},
            PageSlot{},
        }) {}
};

class SettingPageGroup : public FixedPageGroup<1> {
 public:
  SettingPageGroup()
      : FixedPageGroup<1>(Page{
            MenuSlot{MenuIconEncoding::SYSTEM,
                     UiStateMachine::Id::SYSTEM_SETTING, "SYSTEM\nSETTING"},
            PageSlot{},
            PageSlot{},
            PageSlot{},
        }) {}
};

class SystemSettingPageGroup : public FixedPageGroup<1> {
 public:
  SystemSettingPageGroup()
      : FixedPageGroup<1>(Page{
            ParameterSlot{
                ParameterRef{SystemParameterRef{
                    ParameterModel::Id::SYSTEM_SETTING_LCD_CONSTRAST}},
                "LCD Co-\nNTRAST"},
            ParameterSlot{
                ParameterRef{SystemParameterRef{
                    ParameterModel::Id::SYSTEM_SETTING_LCD_CONSTRAST}},
                "LCD Co-\nNTRAST"},
            PageSlot{},
            PageSlot{},
        }) {}
};

class FxTypePageGroup : public FixedPageGroup<1> {
 public:
  FxTypePageGroup()
      : FixedPageGroup<1>(Page{
            ParameterSlot{ParameterRef{FxParameterRef{
                              Fx::ParameterId::FX_SYSTEM_SELECTED_FX}},
                          "TYPE"},
            PageSlot{}, PageSlot{}, PageSlot{}}) {}
};

class NoneEditPageGroup : public FixedPageGroup<1> {
 public:
  NoneEditPageGroup()
      : FixedPageGroup<1>(
            Page{
                 ParameterSlot{ParameterRef{FxParameterRef{
                                  Fx::ParameterId::FX_SYSTEM_SELECTED_FX,
                              }},
                              "TYPE"},
                PageSlot{},
                PageSlot{},
                PageSlot{},
            }) {}
};

class LpfEditPageGroup : public FixedPageGroup<2> {
 public:
  LpfEditPageGroup()
      : FixedPageGroup<2>(
            Page{
                ParameterSlot{ParameterRef{FxParameterRef{
                                  Fx::ParameterId::FX_SYSTEM_SELECTED_FX,
                              }},
                              "TYPE"},
                ParameterSlot{ParameterRef{FxParameterRef{
                                  Fx::ParameterId::FX_LPF_RATE,
                              }},
                              "RATE"},
                ParameterSlot{ParameterRef{FxParameterRef{
                                  Fx::ParameterId::FX_LPF_DEPTH,
                              }},
                              "DEPTH"},
                ParameterSlot{ParameterRef{FxParameterRef{
                                  Fx::ParameterId::FX_LPF_RESONANCE,
                              }},
                              "RESONANCE"},
            },
            Page{
                ParameterSlot{ParameterRef{FxParameterRef{
                                  Fx::ParameterId::FX_LPF_CUTOFF,
                              }},
                              "CUTOFF"},
                PageSlot{},
                PageSlot{},
                PageSlot{},
            }) {}
};

#endif
