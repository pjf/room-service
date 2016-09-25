#include "LockEvent.h"
#include "Game.h"

// This should probably *release* the lock after a point, too!
void LockEvent::processEvent(print_f print, Game *game) {
    print("Applying lock to ");
    print(code);
    print(" using puzzle code ");
    print(puzzle);
    print("\n");

    game->map.lockDoor(code, puzzle);
}
