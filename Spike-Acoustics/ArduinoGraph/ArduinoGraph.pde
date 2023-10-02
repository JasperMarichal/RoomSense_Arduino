//Made with Processing : https://processing.org
import processing.serial.*;
Serial port;    // Create an object from Serial class
String val;     // Data received from the serial port

IntList data;

void setup() {
  port = new Serial(this, "COM3", 115200);
  data = new IntList();
  
  size(800, 600);
  noFill();
  stroke(#00ff00);
  strokeWeight(1);
}

void draw() {{
  while ( port.available() > 0) { // If data is available,
      val = port.readStringUntil('\n');         // read it and store it in val
      try {
        if(val != null) {
          int a = Integer.parseInt(val.replace("\r","").replace("\n", ""));
          data.append(a);
          if(data.size() > 5000) data.remove(0);
        }
      }catch (NumberFormatException ignored) {}
      println(data.size());
      //print(val); //print out in the console 
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
}
