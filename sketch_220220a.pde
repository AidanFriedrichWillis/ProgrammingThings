import controlP5.*;
import processing.serial.*;

ControlP5 cp5;
Serial port;
char option;
String s = "";
Boolean reading = false;
String textValue = "";
Textlabel mode;
 Textlabel response;
  Textlabel peopleFoundLB;
char responseST;
int peopleFound = 0;
int roomsScanned = 0;
int inst = 0;
int endCount = 0;
void setup(){

  size(800,350);
  textSize(100);
  port = new Serial(this,"COM7",9600); 
    cp5 = new ControlP5(this);

   mode = cp5.addTextlabel("modeLB")
                    .setText("")
                    .setPosition(0,50)
                    .setColorValue(0xffffff00)
                    .setFont(createFont("Georgia",20))
                    ;
   response = cp5.addTextlabel("responseLB")
                    .setText("")
                    .setPosition(0,200)
                    .setColorValue(0xffffff00)
                    .setFont(createFont("Georgia",20))
                    ;
   peopleFoundLB = cp5.addTextlabel("peopleFoundLB")
                    .setText("People Found: ")
                    .setPosition(0,10)
                    .setColorValue(0xffffff00)
                    .setFont(createFont("Georgia",20))
                    ;

}


void getData(){

    char zumo = port.readChar();
    
    if(zumo == 'c' || zumo == 's' || zumo == 'b'){
        
        responseST = zumo;
        if(zumo == 'c'){
            
            endCount++;
            if(endCount == 4){
                
                inst = 3;
            }
        
        }
      
    }
    else if(zumo == '+'){
      peopleFound++;
    }
    
    
}



void draw(){
    
  //if (frameCount % 5 == 0) {
  //  thread("getRes");
  //}
  
    getData();

   background(150,0,150);   
  
    text(responseST,500,70);
  cp5.get(Textlabel.class,"peopleFoundLB").setText("People Found: " + String.valueOf(peopleFound));

  if(inst == 1){
    normalMode();
  }
  else if(inst == 2){
     manualScreen();
  }
  else if(inst == 0){
    calibrateScreen();
   }
   else if (inst ==3){
   
     endScreen();
   }
   
   
}

void endScreen(){
  
  cp5.get(Textlabel.class,"modeLB").setText("FINSISHED YOU HAVE FOUND " + peopleFound+  " People2" );
  cp5.get(Textlabel.class,"responseLB").setText("");




}

void calibrateScreen(){

  
  cp5.get(Textlabel.class,"modeLB").setText("Press C to start the Calibration process");



}


void manualScreen(){
   
  cp5.get(Textlabel.class,"modeLB").setText("Currently In Manual Mode, press o to return to auto");
  
}


void normalMode(){
  

   cp5.get(Textlabel.class,"modeLB").setText("Currently In Auto Mode, press k for manual");

   
   if(responseST == 'c'){
          cp5.get(Textlabel.class,"responseLB").setText("At Wall, press L or R to go left/right,Z for uturn. Then press C to contrinue the search");
       
   }
   else if(responseST == 's'){
          cp5.get(Textlabel.class,"responseLB").setText("You have activated search mode, please press L or R to search the Room, then press C to continue search");
       
   }
   
   else if(responseST == 'b'){
     cp5.get(Textlabel.class,"responseLB").setText("Moving.....");
   }
       
   
  
}

void resetlables(){
          cp5.get(Textlabel.class,"responseLB").setText("");
        
}



void keyPressed(){
  
  port.write(key);
  
  if(key == 'k' && inst != 0){
    inst = 2;
    resetlables();
  }
  
  if((key == 'o') && (inst == 2 || inst !=0)){
    
    inst = 1;
  
  }
  
  if(key == 'c' && (inst != 1 || inst !=2)){
    
    inst = 1;
  
  }
  
}
