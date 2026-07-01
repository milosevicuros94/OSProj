#ifndef _TRAP_HANDLER_HPP_
#define _TRAP_HANDLER_HPP_

class TrapHandler {
public:
    static void handleInternal();
    static void handleTimer();
    static void handleExternal();
};

#endif