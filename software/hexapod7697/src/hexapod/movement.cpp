#include "movement.h"
#include "debug.h"
#include "config.h"

#include <cstdlib>

namespace hexapod {

    extern const MovementTable& standbyTable();
    extern const MovementTable& forwardslowTable();
    extern const MovementTable& forwardTable();
    extern const MovementTable& forwardfastTable();
    extern const MovementTable& backwardslowTable();
    extern const MovementTable& backwardTable();
    extern const MovementTable& backwardfastTable();
    extern const MovementTable& turnleftslowTable();
    extern const MovementTable& turnleftTable();
    extern const MovementTable& turnleftfastTable();
    extern const MovementTable& turnrightslowTable();
    extern const MovementTable& turnrightTable();
    extern const MovementTable& turnrightfastTable();
    extern const MovementTable& shiftleftslowTable();
    extern const MovementTable& shiftleftTable();
    extern const MovementTable& shiftleftfastTable();
    extern const MovementTable& shiftrightslowTable();
    extern const MovementTable& shiftrightTable();
    extern const MovementTable& shiftrightfastTable();
    extern const MovementTable& climbTable();
    extern const MovementTable& rotatexTable();
    extern const MovementTable& rotateyTable();
    extern const MovementTable& rotatezTable();
    extern const MovementTable& twistTable();

    const MovementTable kTable[MOVEMENT_TOTAL] {
        standbyTable(),
        forwardslowTable(),
        forwardTable(),
        forwardfastTable(),
        backwardslowTable(),
        backwardTable(),
        backwardfastTable(),
        turnleftslowTable(),
        turnleftTable(),
        turnleftfastTable(),
        turnrightslowTable(),
        turnrightTable(),
        turnrightfastTable(),
        shiftleftslowTable(),
        shiftleftTable(),
        shiftleftfastTable(),
        shiftrightslowTable(),
        shiftrightTable(),
        shiftrightfastTable(),
        climbTable(),
        rotatexTable(),
        rotateyTable(),
        rotatezTable(),
        twistTable(),
    };

    Movement::Movement(MovementMode mode):
        mode_{mode}, transiting_{false}
    {
    }

    void Movement::setMode(MovementMode newMode) {

        if (!kTable[newMode].entries) {
            LOG_INFO("Error: null movement of mode(%d)!", newMode);
            return;
        }

        mode_ = newMode;

        const MovementTable& table = kTable[mode_];

        index_ = table.entries[std::rand() % table.entriesCount];
        remainTime_ = config::movementSwitchDuration > table.stepDuration ? config::movementSwitchDuration : table.stepDuration;
    }

    const Locations& Movement::next(int elapsed) {

        const MovementTable& table = kTable[mode_];

        if (elapsed <= 0)
            elapsed = table.stepDuration;

        if (remainTime_ <= 0) {
            index_ = (index_ + 1)%table.length;
            remainTime_ = table.stepDuration;
        }
        if (elapsed >= remainTime_)
            elapsed = remainTime_;

        auto ratio = (float)elapsed / remainTime_;
        position_ += (table.table[index_] - position_)*ratio;
        remainTime_ -= elapsed;

        return position_;
    }
}
