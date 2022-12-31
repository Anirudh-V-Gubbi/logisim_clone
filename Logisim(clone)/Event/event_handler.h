#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "../Logger/logger.h"
#include "key_event.h"
#include "mouse_event.h"
#include "application_event.h"
#include "../ViewportWindows/viewport_window.h"

class EventHandler {
public:
    static void DispatchEvent(Event& event, std::vector<std::unique_ptr<ViewportWindow>>& viewportWindows) {
        for(auto& vWindow : viewportWindows) {
            vWindow->HandleEvent(event);
        }
    }
};

#endif
