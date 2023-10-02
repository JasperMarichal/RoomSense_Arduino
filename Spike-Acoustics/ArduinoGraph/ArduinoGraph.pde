//Made with Processing : https://processing.org
import processing.serial.*;
Serial port;    // Create an object from Serial class
String val;     // Data received from the serial port

IntList data;
final int BUFFER_SIZE = 5000;

//To calculate average over last few frames
IntList numMeasurements; 
int newMeasurementsTotal;
final int NEWMEASUREMENTS_SAMPLESIZE = 20;

//Toggle HUD with <TAB>
boolean showHUD = true;
//Toggle with <SPACE>
boolean collectData = true;

void setup() {
  println("Serial Ports detected:");
  String[] ports = Serial.list();
  printArray(ports);
  if(ports.length > 0) {
    println("Connecting to port '"+ports[0]+"'...");
    port = new Serial(this, ports[0], 115200);
  }
  data = new IntList();
  
  numMeasurements = new IntList();
  
  size(800, 600);
  strokeWeight(1);
}

int fetchData() {
  int newData = 0;
  if(port == null) {
    //test data
    data.append(int(512+sin(frameCount/100.0)*60));
    newData++;
    if(data.size() > BUFFER_SIZE) data.remove(0);
    return newData;
  }
  while ( port.available() > 0) { // If data is available,
    val = port.readStringUntil('\n');         // read it and store it in val
    try {
      if(val != null) {
        int a = Integer.parseInt(val.replace("\r","").replace("\n", ""));
        data.append(a);
        newData++;
        if(data.size() > BUFFER_SIZE) data.remove(0);
      }
    }catch (NumberFormatException ignored) {}
  }
  return newData;
}

void draw() {
  if(collectData) {
    int nm = fetchData();
    numMeasurements.append(nm);
    newMeasurementsTotal += nm;
    if(numMeasurements.size() > NEWMEASUREMENTS_SAMPLESIZE) {
      newMeasurementsTotal -= numMeasurements.remove(0);
    }
    
  }
  
  int dmin = 400;
  int dmax = 600;
  float hpm = (height-10.0)/(dmax - dmin);
  float wpm = (width-40.0)/(data.size()+1);
  
  background(0);
  
  stroke(#00ff00);
  for(int m = 0; m < data.size()-1; m++) {
    line((m)*wpm, (height-5.0)-((data.get(m)-dmin)*hpm), (m+1)*wpm, (height-5.0)-((data.get(m+1)-dmin)*hpm)); 
  }
  
  if(showHUD) {
    if(mouseX > 5 && mouseX < width-45) {
      stroke(#880000, 128);
      line(mouseX, 0, mouseX, height);
      fill(#ff0000);
      int m = int(mouseX / wpm);
      circle((m)*wpm, (height-5.0)-((data.get(m)-dmin)*hpm), 5);
      text(data.get(m), 20, 45);
    }
    
    fill(#ffffff);
    textSize(20);
    text(int(frameRate)+" FPS", 20, 20);
    text("BUFF: "+data.size()+"/"+BUFFER_SIZE, 100, 20);
    text("+"+int(newMeasurementsTotal*frameRate/NEWMEASUREMENTS_SAMPLESIZE)+" M/s", 145, 45);
    
    fill(#00ff00);
    circle(width-40, (height-5.0)-((data.get(data.size()-1)-dmin)*hpm), 5);
    text(data.get(data.size()-1), width-35, (height)-((data.get(data.size()-1)-dmin)*hpm));
  }
}

void keyPressed() {
  if(key == TAB) {
    showHUD = !showHUD;
  }else if(key == ' ') {
    collectData = !collectData;
  }
}
