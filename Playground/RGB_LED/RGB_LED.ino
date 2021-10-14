// www.elegoo.com
// 2016.12.8

// define pins
const int BLUE = 3;
const int GREEN = 5;
const int RED = 6;

const bool LOG = true;

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);

  randomSeed(analogRead(0));
  if (LOG) Serial.begin(9600);
}

void stepGradation(int COLOR1, int COLOR2) {
  const int maxVal = 20;
  const int stepVal = 1;

  // choose a value between 1 and 255 to change the color.
  int col1val = maxVal;
  int col2val = 0;

  // fades out color 1 and lights up color 2 full gradually
  for (size_t i = 0; i < maxVal;
       i += stepVal)
  {
    col1val -= stepVal;
    col2val += stepVal;
    analogWrite(COLOR1, col1val);
    analogWrite(COLOR2, col2val);
    delay(100);
  }
}

void RGB(int R, int G, int B) {
  if (LOG) {
    char buf[64];
    snprintf(buf, 64, "R[%d] G[%d] B[%d]", R, G, B);
    Serial.println(buf);
  }

  auto cap = [](int v) { return max(0, min(v, 225)); };
  analogWrite(RED, cap(R));
  analogWrite(GREEN, cap(G));
  analogWrite(BLUE, cap(B));
}

void loop() {
  stepGradation(RED, GREEN);
  stepGradation(GREEN, BLUE);
  stepGradation(BLUE, RED);

  delay(1000);

  auto rand = [](int v) { return v += (random(v / 2) - v); };
  for (size_t i = 0; i < 6; ++i) {
    RGB(rand(i * 10), rand(i * 10), rand(i * 10));
    delay(500);
  }

  RGB(0, 0, 0);
  delay(2000);
}
