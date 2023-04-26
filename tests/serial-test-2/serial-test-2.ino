// Sctipt - 4
// Since the get-readings arduino script raise some errors, these scripts are developed to test
// for identifing the errors are realted to that script.

void setup() {
    Serial.begin(115200);
}

char recievedChar;

void loop()
{
    if (Serial.available()){
        recievedChar = Serial.read();
        if (recievedChar == '1'){
            Serial.println("1,2,3,4,5,6,7,8,9");
        }
    }
}