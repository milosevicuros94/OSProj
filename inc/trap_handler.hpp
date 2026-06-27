#ifndef _TRAP_HANDLER_HPP
#define _TRAP_HANDLER_HPP

class TrapHandler {
public:
    static void handleInternal();
    static void handleTimer();
    static void handleExternal();
};

#endif