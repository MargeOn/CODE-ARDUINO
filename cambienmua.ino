#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  
#define autogatmua 13 // công tắc gạt mưa.
#define washer 12 // công tắc phun nước.
#define intgatmua 11 // công tắc interrupt.
#define lowgatmua 10 // công tắc gạt mưa tốc độ thấp.
#define highgatmua 9 // công tắc gạt mưa tốc độ cao.
#define settimeup 8 // nút nhấn điều chỉnh định thời tăng lên.
#define settimedown 7 // nút nhấn điều chỉnh định thời giảm xuống.
#define washmotor 6 // chân 6 điều khiển motor phun nước.
const int rain = A0; // chân A0 lấy tín hiệu từ cảm biến mưa.
int gocquay = 130; // góc quay
int pos = 0;
int a = 0;
int timelh = 0;
int washing = 0;
int set = 1;
int w = 0;
int mode = 0;
Servo myservo;
//............................................................
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  myservo.attach(3);
  pinMode(autogatmua, INPUT_PULLUP);
  pinMode(washer, INPUT_PULLUP);
  pinMode(intgatmua, INPUT_PULLUP);
  pinMode(lowgatmua, INPUT_PULLUP);
  pinMode(highgatmua, INPUT_PULLUP);
  pinMode(settimeup, INPUT_PULLUP);
  pinMode(settimedown, INPUT_PULLUP);
  pinMode(rain, INPUT);
  pinMode(washmotor, OUTPUT);
  myservo.write(pos);
}
// Hàm điều khiển phun nước rửa kính.
void wash()
{
  // Nếu công tắc rửa kính bật, motor rửa kính sẽ phun nước rửa kính và hiển thị lên màn hình LCD.
  if((digitalRead(washer) == 0)){
    // cờ washing để đánh dấu motor rửa kính có hoạt động hay không.
    // washing = 1 là motor rửa kính đang hoạt động.
    washing = 1;
    lcd.setCursor(0, 1);
    lcd.print("WASHING    ");
    for (w = 0; w <= 20; w += 1) {
    digitalWrite(washmotor,HIGH);
    delay(20);
    digitalWrite(washmotor,LOW);
    }
    for (w = 10; w > 20; w -= 1) {
    digitalWrite(washmotor,HIGH);
    delay(20);
    digitalWrite(washmotor,LOW);
    }
  }
  // Nếu công tắc rửa kính tắt, motor rửa kính sẽ không hoạt động
  else{
    washing = 0; // motor rửa kính không hoạt động
    if ((digitalRead(intgatmua) == 0)){
      
    }
    else
    lcd.setCursor(0, 1);
    lcd.print("       ");
  }
}
// Hàm điều khiển tốc độ động cơ 
void timedelay()
{
  // cờ timelh == 2 thì khoảng delay nhỏ, động cơ quay nhanh.
  if (timelh == 2){
    delay(3);
  }
  // cờ timelh == 1 thì khoảng delay lớn, động cơ quay chậm.
  else if (timelh == 1){
   delay(7);
  }
}
// Trong chế độ INT, liệu nút nhấn định thời tăng hoặc giảm có được nhấn hay chưa.
void settimeint()
{
  // Khi nút nhấn định thời tăng được nhấn, thời gian ngắt quãng giữa những lần gạt tăng lên.
  if(digitalRead(settimeup) == 0)
  {
    while(digitalRead(settimeup) == 0){

    }
    if(set < 3){
      set += 1;
    }
    else{
      set = 1;
    }
  }
  // Khi nút nhấn định thời giảm được nhấn, thời gian ngắt quãng giữa những lần gạt giảm xuống.
  if(digitalRead(settimedown) == 0)
  {
    while(digitalRead(settimedown) == 0){

    }
    if(set > 1){
      set -= 1;
    }
    else{
      set = 3;
    }
  }
  // Hiển thị thời gian định thời lên màn hình LCD.
  // Nếu motor rửa kính không hoạt động, thời gian định thời mới được hiển thị.
  // Ngược lại motor rửa kính hoạt động, thời gian định thời bị ẩn đi.
  if(washing == 0){
    if(set == 1){
      lcd.setCursor(0, 1);
      lcd.print("TIMESET: 1S");
    }
    else if(set == 2){
      lcd.setCursor(0, 1);
      lcd.print("TIMESET: 2S");
    }
    else if(set == 3){
      lcd.setCursor(0, 1);
      lcd.print("TIMESET: 3S");
    }
  }
}
// Hàm điều khiển thời gian định thời trong chế độ INT.
void timeint()
{
  if(set == 1)
  delay(500);
  else if(set == 2)
  delay(1500);
  else if(set == 3)
  delay(2500);
}
// Hàm điều khiển động cơ servo.
void servo()
{
  for (pos = 0; pos <= gocquay; pos += 1) {
    myservo.write(pos);
    timedelay();
  }
  for (pos = gocquay; pos >= 0; pos -= 1) {
    myservo.write(pos);
    timedelay();
  }
}
// Hàm thực thi các chế độ AUTO, INT, LOW, HIGH, WASHER.
void loop()
{
  lcd.setCursor(0, 0);
  lcd.print(" PLEASE SELECT  ");
  lcd.setCursor(0, 1);
  lcd.print("      MODE      ");
  // khi bật chế độ auto
  if ((digitalRead(autogatmua) == 0)){
    while ((digitalRead(autogatmua) == 0))
    { 
      // Đọc giá trị analog từ cảm biến mưa
      a = analogRead(rain);
      // Nếu đã bật công tắc chế độ auto mà còn bật các công tắc còn lại thì 
      // motor sẽ không hoạt động và hiển thị yêu cầu chọn chế độ lên màn hình LCD .
      if((digitalRead(washer) == 0)||(digitalRead(lowgatmua) == 0)||(digitalRead(highgatmua) == 0)||(digitalRead(intgatmua) == 0)){
        lcd.setCursor(0, 0);
        lcd.print(" PLEASE SELECT  ");
        lcd.setCursor(0, 1);
        lcd.print("      MODE      ");
        mode = 1;
        break;
      }
      // Nếu đã chọn chế độ AUTO thì motor có thể sẽ không hoạt động
      // hoặc hoạt động tùy vào điều kiện thời tiết có mưa hay không.
      else
      {
        lcd.setCursor(0, 0);
        lcd.print("MODE: AUTO      ");
        // Nếu giá trị a càng gần 1023, trời không mưa.
        // Motor gạt mưa không hoạt động.
        if(a > 900){
          lcd.setCursor(0, 1);
          lcd.print("NO RAIN       ");
        }
        // Nếu giá trị a giảm nhiều, trời mưa lớn.
        else if(a <= 600)
        {
          lcd.setCursor(0, 1);
          lcd.print("RAIN: Heavy  ");
          Serial.println(a);
          timelh = 2; // motor gạt mưa quay với tốc độ nhanh.
          servo();
        }
        // Nếu giá trị a giảm ít nằm trong khoảng 600 đến 900, trời mưa nhẹ.
        else
        {
          lcd.setCursor(0, 1);
          lcd.print("RAIN: Slight  ");
          Serial.println(a);
          timelh = 1; // motor gạt mưa quay với tốc độ chậm.
          servo();
        }
      }
    }
    if(mode = 1){

    }
    else
    lcd.clear();
  }
  // khi bật chế độ INT.
  else if ((digitalRead(intgatmua) == 0))
  {
    while ((digitalRead(intgatmua) == 0))
    { 
      // Nếu đã bật công tắc chế độ INT mà còn bật các công tắc còn lại thì 
      // motor sẽ không hoạt động và hiển thị yêu cầu chọn chế độ lên màn hình LCD .
      if((digitalRead(autogatmua) == 0)||(digitalRead(lowgatmua) == 0)||(digitalRead(highgatmua) == 0)){
        lcd.setCursor(0, 0);
        lcd.print(" PLEASE SELECT  ");
        lcd.setCursor(0, 1);
        lcd.print("      MODE      ");
        mode = 1;
        break;
      }
      lcd.setCursor(0, 0);
      lcd.print("MODE: INT         ");
      wash(); // hàm điều khiển phun nước rửa kính.
      settimeint(); // cài đặt thời gian định thời.
      timelh = 1; // motor gạt mưa quay với tốc độ chậm.
      servo();
      settimeint(); // cài đặt thời gian định thời.
      timeint(); // thiết lập thời gian định thời.
    }
    if(mode = 1){

    }
    else
    lcd.clear();
  }
  // khi bật chế độ LOW
  else if ((digitalRead(lowgatmua) == 0))
  {
    while ((digitalRead(lowgatmua) == 0))
    {
      // Nếu đã bật công tắc chế độ LOW mà còn bật các công tắc còn lại thì 
      // motor sẽ không hoạt động và hiển thị yêu cầu chọn chế độ lên màn hình LCD .
      if((digitalRead(autogatmua) == 0)||(digitalRead(highgatmua) == 0)||(digitalRead(intgatmua) == 0)){
        lcd.setCursor(0, 0);
        lcd.print(" PLEASE SELECT  ");
        lcd.setCursor(0, 1);
        lcd.print("      MODE      ");
        mode = 1;
        break;
      }
      lcd.setCursor(0, 0);
      lcd.print("MODE: LOW         ");
      lcd.setCursor(0, 1);
      lcd.print("                  "); 
      wash(); // hàm điều khiển phun nước rửa kính.
      timelh = 1; // motor quay với tốc độ chậm.
      servo();
    } 
    if(mode = 1){

    }
    else
    lcd.clear();
  }
  // khi bật chế độ HIGH.
  else if((digitalRead(highgatmua) == 0))
  {
    while ((digitalRead(highgatmua) == 0))
    {
      // Nếu đã bật công tắc chế độ HIGH mà còn bật các công tắc còn lại thì 
      // motor sẽ không hoạt động và hiển thị yêu cầu chọn chế độ lên màn hình LCD .
      if((digitalRead(autogatmua) == 0)||(digitalRead(lowgatmua) == 0)||(digitalRead(intgatmua) == 0)){
        lcd.setCursor(0, 0);
        lcd.print(" PLEASE SELECT  ");
        lcd.setCursor(0, 1);
        lcd.print("      MODE      ");
        mode = 1;
        break;
      }
      lcd.setCursor(0, 0);
      lcd.print("MODE: HIGH        ");
      lcd.setCursor(0, 1);
      lcd.print("                  ");
      wash(); // hàm điều khiển phun nước rửa kính.
      timelh = 2; // motor quay với tốc độ nhanh.
      servo();
    }
    if(mode = 1){

    }
    else
    lcd.clear();
  }
}