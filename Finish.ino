
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

WiFiClient wifiClient;

/* Set these to your desired credentials. */
const char *ssid = "CONG THANH";  //ENTER YOUR WIFI SETTINGS
const char *password = "Congthanh2001";

//Web/Server address to read/write from 
const char *host = "192.168.1.5";   //https://circuits4you.com website or IP address of server

//=======================================================================
  //=======================================================================
//                    MAIN
//=======================================================================
int value, real_value;
// VCC nối vào ESP8266 3V analog
// GND nối vào ESP8266 G analog
// D0 nối vào ESP8266 D1 digital
// A0 nối vào ESP8266 A0 analog
#define ledr D5
// Led Red into D5
#define ledg D6
// Led Green into D6
#define ledy D7
// Led 
#define relay D3
#define cbmua D2

void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  // put your setup code here, to run once:
  // Mở cổng Serial ở mức 9600
  Serial.begin(9600);
  // Dùng chân D1-GPIO5 dể làm cổng đọc digital, còn chân A0 vào thì không cần khai báo
  pinMode(ledr,OUTPUT);
  pinMode(ledy,OUTPUT);
  pinMode(ledg,OUTPUT);
  pinMode(relay,OUTPUT);
  pinMode(cbmua, INPUT);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  // Dùng hàm for để cộng dồn 10 lần đọc giá trị của cảm biến, sau đó lấy trung bình để được giá trị chính xác nhất
  for(int i = 0; i <=9; i++){
    real_value += analogRead(A0);
  }
  value = real_value/10;
// Đặt lại giá trị 0 để cho vòng lặp mới 
  real_value = 0;
// Set thang giá trị đàu và giá trị cuối để đưa giá trị về thang từ 0 - 100.
  int percent = map(value, 432, 1023, 0, 100);
  // Tính giá trị phần trăm thực. Chuyển điện thế từ 3.3v (khô) thành 3.3v (ẩm)
  percent = 100 - percent;
  Serial.print("Độ ẩm là: ");
  Serial.print(percent);
  Serial.println('%');
  int status_rain = digitalRead(cbmua);
  if(status_rain == LOW){
    digitalWrite(ledr, LOW);
    digitalWrite(ledg, LOW);
    digitalWrite(ledy, HIGH);
    digitalWrite(relay,LOW);
    Serial.println("Trời đang mưa");
  }else{
     Serial.println("Trời không mưa");
    if(percent < 60){
      digitalWrite(ledr, HIGH);
      digitalWrite(ledg, LOW);
      digitalWrite(ledy, LOW);
      digitalWrite(relay,HIGH);
      Serial.println("Độ ẩm thấp, máy bơm đang hoạt động");
      //chạy
    }else if(percent > 80){
      // tắt 
      digitalWrite(ledr, LOW);
      digitalWrite(ledg, LOW);
      digitalWrite(ledy, HIGH);
      digitalWrite(relay,LOW);
      Serial.println("Độ ẩm cao, máy bơm không hoạt động");
    }else{
      digitalWrite(ledr, LOW);
      digitalWrite(ledg, HIGH);
      digitalWrite(ledy, LOW);
      digitalWrite(relay,LOW);
      Serial.println("Độ ẩm bình thường, máy bơm không hoạt động");
    }
  }

  Serial.print("Giá trị analog: ");
  Serial.print(value);
  Serial.print(" && ");
  Serial.print("Giá trị digital: ");
  Serial.println(digitalRead(5));
  delay(1000);

    HTTPClient http;    //Declare object of class HTTPClient

  String ADCData, station, maybom, postData;

  station = status_rain;
  if(digitalRead(relay) == LOW) maybom = "Máy bơm đang tắt";
  else maybom = "Máy bơm đang bật";

  if(status_rain == HIGH) station = "Trời không mưa";
  else station = "Trời đang mưa";

  //Post Data
  postData = "doam=" + String(percent) + "&trangthaimua=" + station + "&trangthaibom=" + maybom;
  // Serial.println(postData);
  http.begin(wifiClient, "http://192.168.1.5:81/iotproject/postdemo.php");              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header

  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end();  //Close connection
  
  delay(3000);  //Post Data at every 5 seconds


}
