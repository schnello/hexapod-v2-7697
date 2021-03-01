#include <Arduino.h>

#ifndef __LREMOTE_H__
#include <LRemote.h>
#define __LREMOTE_H__
#endif

#include "linkit_control/ui_controls.h"
#include "hexapod/debug.h"
#include "hexapod/config.h"
#include "hexapod/hexapod.h"

#define REACT_DELAY hexapod::config::movementInterval


LRemoteSlider slider;
static Button buttonForward(hexapod::MOVEMENT_FORWARD, "Forward", 1, 3, 1, 1, RC_BLUE);
static Button buttonBackward(hexapod::MOVEMENT_BACKWARD, "Backward", 1, 4, 1, 1, RC_BLUE);
static Button buttonTL(hexapod::MOVEMENT_TURNLEFT, "TurnLeft", 0, 3, 1, 1, RC_GREEN);
static Button buttonTR(hexapod::MOVEMENT_TURNRIGHT, "TurnRight", 2, 3, 1, 1, RC_GREEN);
static Button buttonSL(hexapod::MOVEMENT_SHIFTLEFT, "ShiftLeft", 0, 4, 1, 1, RC_BLUE);
static Button buttonSR(hexapod::MOVEMENT_SHIFTRIGHT, "ShiftRight", 2, 4, 1, 1, RC_BLUE);
static Button buttonClimb(hexapod::MOVEMENT_CLIMB, "Climb", 1, 2, 1, 1, RC_ORANGE);

static Button buttonRotateX(hexapod::MOVEMENT_ROTATEX, "RotateX", 0, 0, 1, 1, RC_YELLOW);
static Button buttonRotateY(hexapod::MOVEMENT_ROTATEY, "RotateY", 1, 0, 1, 1, RC_YELLOW);
static Button buttonRotateZ(hexapod::MOVEMENT_ROTATEZ, "RotateZ", 2, 0, 1, 1, RC_YELLOW);
static Button buttonTwist(hexapod::MOVEMENT_TWIST, "Twist", 0, 1, 1, 1, RC_YELLOW);

static ButtonGroup btnGroup;

static void log_output(const char* log) {
  Serial.println(log);
}

void normal_setup(void) {
  LRemote.setName("Hexapod");
  LRemote.setOrientation(RC_PORTRAIT);
  LRemote.setGrid(3, 5);


  slider.setText("Slow --- Normal --- Fast");
  slider.setPos(0, 2);
  slider.setSize(3, 3);
  slider.setColor(RC_ORANGE);
  slider.setValueRange(-1, 1, 0);
  LRemote.addControl(slider);



  LRemote_addControls({
    &buttonForward, &buttonBackward,
    &buttonTL, &buttonTR,
    &buttonSL, &buttonSR,
    &buttonClimb, &buttonRotateX,
    &buttonRotateY, &buttonRotateZ,
    &buttonTwist
  });
  btnGroup.addControls({
    &buttonForward, &buttonBackward,
    &buttonTL, &buttonTR,
    &buttonSL, &buttonSR,
    &buttonClimb, &buttonRotateX,
    &buttonRotateY, &buttonRotateZ,
    &buttonTwist
  });

  LRemote.begin();
}

void normal_loop(void) {

  // check if we are connect by some 
  // BLE central device, e.g. an mobile app
  if(!LRemote.connected()) {
    delay(1000-REACT_DELAY);
  }

  auto t0 = millis();

  // Process the incoming BLE write request
  // and translate them to control events
  LRemote.process();  

  auto flag = btnGroup.getPressFlag();
  auto mode = hexapod::MOVEMENT_STANDBY;
  for (auto m = hexapod::MOVEMENT_STANDBY; m < hexapod::MOVEMENT_TOTAL; m++) {
    if (flag & (1<<m)) {
     mode = m;
      break;
    }
  }


if(slider.getValue() == -1 ){
switch (mode) {
  case 2:
	mode--;
	break;
  case 5:
   	mode--;
	break;
  case 6:
  	mode--;
	break;
  case 8:
    	mode--;
	break;
  case 11:
    	mode--;
	break;
  case 14:
    	mode--;
	break;
  case 17:
    	mode--;
    	break;

   }
}



if(slider.getValue() == 1 ){
switch (mode) {
  case 2:
   	mode++;
	break;
  case 5:
    	mode++;
	break;
  case 6:
    	mode++;
	break;
  case 8:
    	mode++;
	break;
  case 11:
    	mode++;
	break;
  case 14:
    	mode++;
	break;
  case 17:
    	mode++;
	break;

   }
}


//  Serial.print(mode);
  hexapod::Hexapod.processMovement(mode, REACT_DELAY);

  auto spent = millis() - t0;

  if(spent < REACT_DELAY) {
    // Serial.println(spent);
    delay(REACT_DELAY-spent);
  }
  else {
    Serial.println(spent);
  }
}
