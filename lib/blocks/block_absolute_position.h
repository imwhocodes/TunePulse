#ifndef BLOCK_ABSOLUTE_POSITION_H
#define BLOCK_ABSOLUTE_POSITION_H

#include "generic_block.h"

/**
 * @union AbsPosition
 * @brief Union to represent absolute position in terms of angle and rotations.
 */
union AbsPosition {
  /**
   * @struct
   * @brief Structure to split position into angle and rotations.
   */
  struct {
    int32_t angle;      // Angle component of the position.
    int32_t rotations;  // Rotations component of the position.
  } split;
  int64_t position;  // Combined 64-bit position value.

  /**
   * @brief Constructor to initialize the union.
   * @param r Rotations value.
   * @param a Angle value.
   */
  AbsPosition(int32_t r, int32_t a) {
    split.rotations = r;
    split.angle = a;
  }
};

/**
 * @class BlockAbsolutePosition
 * @brief Class to manage and calculate absolute position.
 */
class BlockAbsolutePosition {
  BLOCK_INPUT(uint32_t, angle_raw);      // Raw angle input.
  BLOCK_INPUT(AbsPosition, pos_offset);  // Position offset input.
  BLOCK_INPUT(int32_t, freq);            // Frequency input.

  BLOCK_OUTPUT(AbsPosition, position_raw);   // Raw position output.
  BLOCK_OUTPUT(AbsPosition, position_inst);  // Instantaneous position output.
  BLOCK_OUTPUT(int64_t, speed_inst);         // Instantaneous speed output.

 protected:
  uint8_t angle_prev_ = 2;  // Previous angle value for zero-crossing detection.

  /**
   * @brief Function to detect zero crossing in angle values.
   */
  void angleZCD();

 public:
  /**
   * @brief Constructor to initialize the block with inputs.
   * @param raw_angle Reference to raw angle input.
   * @param pos_offset Reference to position offset input.
   * @param freq Reference to frequency input.
   */
  BlockAbsolutePosition(const uint32_t& raw_angle,
                        const AbsPosition& pos_offset,
                        const int32_t& freq)
      : angle_raw_(raw_angle),
        pos_offset_(pos_offset),
        freq_(freq),
        position_raw_(0, 0),
        position_inst_(0, 0) {}

  /**
   * @brief Function to update the block's state.
   */
  void tick() {
    speed_inst_ = position_raw_.position;  // Memorizing the current position

    angleZCD();  // Updating the position with detection of zero crossing

    // Calculate the position difference and multiply by dt
    speed_inst_ = (position_raw_.position - speed_inst_) * freq_;

    // Update current position with offset correction
    position_inst_.position = position_raw_.position + pos_offset_.position;
  }
};

/**
 * @brief Function to detect zero crossing in angle values and update rotations.
 */
void BlockAbsolutePosition::angleZCD() {
  position_raw_.split.angle = angle_raw_;  // update angle

  // getting 2 highest bits of position value to speed up comparison
  uint8_t angle_curnt_ = angle_raw_ >> 30;

  // Check simplified difference between 2 consecutive values
  int8_t diff = angle_prev_ - angle_curnt_;

  // Handle overflow in angle value to update full rotations
  if (diff == 3)
    position_raw_.split.rotations++;
  else if (diff == -3)
    position_raw_.split.rotations--;

  angle_prev_ = angle_curnt_;  // update previous value
}

#endif  // BLOCK_ABSOLUTE_POSITION_H
