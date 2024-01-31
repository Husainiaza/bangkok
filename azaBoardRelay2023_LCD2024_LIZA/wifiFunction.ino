void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);


  lcd.setCursor(0,1);
  lcd.print(" Connecting to ");
  lcd.print(ssid);


  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  lcd.setCursor(0,2);
  lcd.print("WiFi connected");
  lcd.setCursor(0,3);
  lcd.print(WiFi.localIP());
  delay(5000);
}


void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
 
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();


  //----------letakkan kawalan GPIO disini-----------------------------

if (String(topic) == "liza/relay1") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(Relay01,HIGH);
      client.publish("liza/relay1/status", "RELAY 1 ON");
      lcd.setCursor(0,3);
      lcd.print(" FEEDER ON ");
     
      
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(Relay01,LOW);
      client.publish("liza/relay1", "RELAY 1 OFF");
      lcd.setCursor(0,3);
      lcd.print(" FEEDER OFF");
      
    }
  }

 if (String(topic) == "liza/relay2") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(Relay02,HIGH);
      client.publish("liza/relay2/status", "RELAY 2 ON");
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(Relay02,LOW);
       client.publish("liza/relay2/status", "RELAY 2 OFF");
    }
  }

   if (String(topic) == "liza/buzzer") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(buzzer,HIGH);
      client.publish("liza/buzzer/status", "RELAY 2 ON");
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(buzzer,LOW);
       client.publish("liza/buzzer/status", "RELAY 2 OFF");
    }
  }
  
  //----------end------------------------------------------------------
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    //----------Nama Client ID---------------------------------------------------
    if (client.connect("lizapolimasptsb0002")) {
    //-------------------------------------------------------------
      Serial.println("connected");
     
      //-------------------subscribe topik anda disini --------------------
      client.subscribe("liza/relay1");
      client.subscribe("liza/relay2");
      client.subscribe("liza/buzzer");
      //----------------------------end------------------------------------
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
