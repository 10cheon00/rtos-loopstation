#ifndef DSP_PROCESSABLE_HPP
#define DSP_PROCESSABLE_HPP

namespace DSP {

class Processable {
 public:
  virtual void Process() = 0;
};

}  // namespace DSP

#endif
