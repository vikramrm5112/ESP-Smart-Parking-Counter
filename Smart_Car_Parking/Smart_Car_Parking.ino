/*
  ESP32 Smart Parking Counter
  - 2 IR Sensors
  - 16x2 I2C LCD
  - Web Dashboard
  - Serial Monitor Output

  IR1 -> GPIO18
  IR2 -> GPIO19

  LCD SDA -> GPIO21
  LCD SCL -> GPIO22
*/

#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// =====================
// WiFi Credentials
// =====================
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// =====================
// Pins
// =====================
const int IR1 = 18;
const int IR2 = 19;

const int TOTAL_SLOTS = 2;

// LCD Address (usually 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

WebServer server(80);

// =====================
// Get Available Slots
// =====================
int getAvailableSlots() {

  // Most IR obstacle sensors go LOW when detecting an object.
  bool slot1Occupied = (digitalRead(IR1) == LOW);
  bool slot2Occupied = (digitalRead(IR2) == LOW);

  int occupied = 0;

  if (slot1Occupied) occupied++;
  if (slot2Occupied) occupied++;

  return TOTAL_SLOTS - occupied;
}

// =====================
// Main Web Page
// =====================
void handleRoot() {

  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Smart Parking System</title>

<style>
body{
  font-family:Arial, sans-serif;
  text-align:center;
  background:#f5f5f5;
  margin-top:50px;
}

.card{
  background:white;
  width:300px;
  margin:auto;
  padding:20px;
  border-radius:15px;
  box-shadow:0 0 10px rgba(0,0,0,0.2);
}

h1{
  color:#333;
}

#slots{
  font-size:80px;
  font-weight:bold;
  color:green;
}
</style>

</head>

<body>

<div class="card">
<h1>Parking Slots</h1>
<div id="slots">--</div>
<p>Available Spaces</p>
</div>

<script>
function updateSlots(){
  fetch('/slots')
    .then(response => response.text())
    .then(data => {
      document.getElementById('slots').innerHTML = data;
    });
}

setInterval(updateSlots,1000);
updateSlots();
</script>

</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

// =====================
// API Endpoint
// =====================
void handleSlots() {
  server.send(200, "text/plain", String(getAvailableSlots()));
}

// =====================
// Setup
// =====================
void setup() {

  Serial.begin(115200);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  // LCD
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Connecting...");
  lcd.setCursor(0, 1);
  lcd.print("To WiFi");

  // WiFi
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");

  // Web Server Routes
  server.on("/", handleRoot);
  server.on("/slots", handleSlots);

  server.begin();

  Serial.println("Web Server Started");
}

// =====================
// Loop
// =====================
void loop() {

  server.handleClient();

  static int lastSlots = -1;

  int available = getAvailableSlots();

  // Only update LCD if value changes
  if (available != lastSlots) {

    lastSlots = available;

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Parking Slots");

    lcd.setCursor(0, 1);
    lcd.print("Available:");
    lcd.print(available);

    Serial.print("Slots Available: ");
    Serial.println(available);
  }

  delay(100);
}