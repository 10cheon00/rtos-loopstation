#include "ui_state_machine.h"

#include "display_messages.h"
#include "ui_panel_id_ui_state_mapping.h"
#include "ui_panel_id_parameter_id_binding.h"
#include "loopstation_parameter_store.h"

static void GetParametersFromUiPanelId(UiPanelId ui_panel_id, Parameter *parameters);

void UiStateMachine_Init(UiStateMachine *ui_state_machine, UiStateMachineContext *context,
                         UiState *init_state)
{
    ui_state_machine->context = context;
    ui_state_machine->current_state = init_state;
}

void UiStateMachine_TryTransition(UiStateMachine *ui_state_machine,
                                  UiStateEventId ui_state_event_id)
{}

void UiStateMachineContext_Init(UiStateMachineContext *ui_state_machine_context,
                                osMessageQueueId_t display_command_queue)
{
    ui_state_machine_context->display_command_queue = display_command_queue;
}

void UiStateMachine_RenderCurrentState(UiStateMachine *ui_state_machine)
{
    DisplayCommand command;

    command.type = DISPLAY_COMMAND_UI_STATE_RENDER;
    command.payload.ui_state_render.panel_id = ui_state_machine->current_state->ui_panel_id;
    GetParametersFromUiPanelId(ui_state_machine->current_state->ui_panel_id, command.payload.ui_state_render.parameter);
    osMessageQueuePut(ui_state_machine->context->display_command_queue, &command, 0,
                      DISPLAY_COMMAND_QUEUE_TIMEOUT_500MS_TO_TICKS);
}

static void GetParametersFromUiPanelId(UiPanelId ui_panel_id, Parameter *parameters)
{
    ParameterId *parameter_ids =
        UiPanelIdParameterIdBinding_GetParameterIdsFromUiPanelId(ui_panel_id);
    if (parameter_ids == NULL) {
        for (uint8_t i = 0; i < UI_PANEL_MAX_PARAMETER_COUNT; i++) {
            LoopStationParameterStore_CopyParameterValueFromParameterId(PARAMETER_ID_NONE, &parameters[i]);
        }

        return;
    }
    for (uint8_t i = 0; i < UI_PANEL_MAX_PARAMETER_COUNT; i++) {
        LoopStationParameterStore_CopyParameterValueFromParameterId(parameter_ids[i], &parameters[i]);
    }
}
