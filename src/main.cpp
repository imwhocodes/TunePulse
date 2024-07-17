#include <Arduino.h>

class Block {
public:
  virtual void tick() = 0;
};

#define BLOCK_INPUT(type, name) \
  protected:                     \
  const type& name##_;

#define BLOCK_OUTPUT(type, name) \
  protected:                     \
  type name##_;                  \
  public:                        \
    const type& get_##name() const { return name##_; }

class RotateCoordinates : public Block {
  BLOCK_INPUT(float, input_sin);
  BLOCK_INPUT(float, input_cos);
  BLOCK_INPUT(float, rotation_sin);
  BLOCK_INPUT(float, rotation_cos);
  BLOCK_OUTPUT(float, output_sin);
  BLOCK_OUTPUT(float, output_cos);

public:
  RotateCoordinates(const float& input_sin,
                    const float& input_cos,
                    const float& rotation_sin,
                    const float& rotation_cos)
      : input_sin_(input_sin), input_cos_(input_cos),
        rotation_sin_(rotation_sin), rotation_cos_(rotation_cos) {}

  void tick() override {
    output_sin_ = input_sin_ * rotation_cos_ + input_cos_ * rotation_sin_;
    output_cos_ = input_cos_ * rotation_cos_ - input_sin_ * rotation_sin_;
  }
};

float input_sin = 0.0f, input_cos = 1.0f, rotation_sin = 0.707f, rotation_cos = 0.707f;

RotateCoordinates rotate1(input_sin, input_cos, rotation_sin, rotation_cos);
RotateCoordinates rotate2(rotate1.get_output_sin(), rotate1.get_output_cos(), rotation_sin, rotation_cos);

void setup() {
  SerialUSB.begin(9600);
  while (!SerialUSB);

  SerialUSB.println("Rotation Example");

  rotate1.tick();
  rotate2.tick();

  SerialUSB.print("Sin: ");
  SerialUSB.println(rotate2.get_output_sin());
  SerialUSB.print("Cos: ");
  SerialUSB.println(rotate2.get_output_cos());
}

void loop() {}
