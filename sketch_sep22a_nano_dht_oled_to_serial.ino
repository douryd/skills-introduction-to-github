#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <DHT.h>

// DHT11 setup
#define DHTPIN 6           // DHT11 on D6
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// OLED setup
#define SCREEN_WIDTH 128   // OLED display width, in pixels
#define SCREEN_HEIGHT 64   // OLED display height, in pixels
#define OLED_RESET    -1   // Reset pin # (or -1 if sharing Arduino reset)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Serial setup
#define SERIAL_BAUD 19200
#define SERIAL_CONFIG 7E1

unsigned long lastRead = 0;
const unsigned long interval = 5000; // 5 seconds
unsigned long readingNum = 1;

void setup() {
  // Initialize Serial
  Serial.begin(SERIAL_BAUD, SERIAL_CONFIG);

  // Initialize DHT sensor
  dht.begin();

  // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Grove default I2C addr 0x3C
    Serial.println("OLED not found!");
    while(1);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("DHT11 Ready");
  display.display();

  delay(1000);
  display.clearDisplay();
  display.display();
}

void loop() {
  if (millis() - lastRead >= interval) {
    lastRead = millis();

    // Read DHT11
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Check if reads failed
    if (isnan(h) || isnan(t)) {
      display.clearDisplay();
      display.setCursor(0,0);
      display.setTextSize(1);
      display.print("DHT11 Error!");
      display.display();

      Serial.println("DHT11 read failed!");
      return;
    }

    // Display on OLED
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.print("T: ");
    display.print(t, 1);
    display.print("C");
    display.setCursor(0, 30);
    display.print("H: ");
    display.print(h, 1);
    display.print("%");
    display.display();

    // Send to Serial: format = "#<reading> T:<temp> H:<hum>"
    Serial.print("#");
    Serial.print(readingNum);
    Serial.print(" T:");
    Serial.print(t, 1);
    Serial.print("C H:");
    Serial.print(h, 1);
    Serial.println("%");

    readingNum++;
  }
}