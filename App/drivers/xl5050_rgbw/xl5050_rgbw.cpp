#include "xl5050_rgbw.hpp"

namespace Xl5050Rgbw {

Status Driver::Initialize(TIM_HandleTypeDef* htim) {
    if (init_state == InitState::INITED) {
        return Status::OK;
    }
    init_state = InitState::ON_INIT;
    
    this->htim = htim;
    
    init_state = InitState::INITED;
    return Status::OK;
}

}