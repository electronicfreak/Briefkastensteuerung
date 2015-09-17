//BK-Steuerung-v1
//S Titz

//Walze
const int WS = A9;		//Walze Sense
const int WV = PA_2;	//Walze Vor
const int WZ = PA_4;
const int WE = PA_3;
//Klappe
const int KS = A8;
const int KV = PD_6;
const int KZ = PF_4;
const int KE = PD_7;
//Lichtschranke
const int LVH = PE_3;
const int LVD = PD_0;
const int LHH = PA_6;
const int LHD = PD_1;
//SigTerm
const int END = PF_2;
//Schalter
const int SA = PD_2;
const int SZ = PD_3;

//maxima
const int KM =  2000
const int WM =  2000
const int WC =  5

//timer
const int WLV = 5000
const int WLZ = 5000
const int LW = 3000 

void setup() {
	Serial.begin(9600);
	//IO?
	pinMode(WV, OUTPUT);
	pinMode(WZ, OUTPUT);
	pinMode(WE, OUTPUT);
	pinMode(KV, OUTPUT);
	pinMode(KZ, OUTPUT);
	pinMode(KE, OUTPUT);
	pinMode(END,OUTPUT); 
	
	pinMode(WS, INPUT);
	pinMode(KS, INPUT);
	pinMode(LVH,INPUT);
	pinMode(LVD,INPUT);
	pinMode(LHH,INPUT);
	pinMode(LHD,INPUT);
	
	pinMode(SA, INPUT_PULLUP);
	pinMode(SZ, INPUT_PULLUP);
}

void klappe_auf() {
	Serial.println("klappe auf");
	digitalWrite(KE, HIGH);
	digitalWrite(KZ, LOW);
	digitalWrite(KV, HIGH);
}

void klappe_zu() {
	Serial.println("klappe zu");
	digitalWrite(KE, HIGH);
	digitalWrite(KZ, HIGH);
	digitalWrite(KV, LOW);
}

void klappe_stop() {
	Serial.println("klappe stop");
	digitalWrite(KE, HIGH);
	digitalWrite(KZ, LOW);
	digitalWrite(KV, LOW);
}

void walze_vor() {
	Serial.println("walze_vor");
	digitalWrite(WE, HIGH);
	digitalWrite(WZ, LOW);
	digitalWrite(WV, HIGH);
}

void walze_zurueck() {
	Serial.println("walze_zurueck");
	digitalWrite(WE, HIGH);
	digitalWrite(WZ, HIGH);
	digitalWrite(WV, LOW);
}

void walze_stop() {
	Serial.println("walze_stop");
	digitalWrite(KE, LOW);
	digitalWrite(KZ, LOW);
	digitalWrite(KV, LOW);
}

int sensor_walze() {
	int ret = 0; 
	for (int i = 0; i <10 ;i++) {
		ret = ret + analogRead(WS);
		delay(10);
    }
	return (ret/10) < WM;
}

int sensor_klappe() {
	int ret = 0; 
	for (int i = 0; i <10 ;i++) {
		ret = ret + analogRead(KS);
		delay(10);
    }
	return (ret/10) < KM;
}

int schalter_auf() {
	return digitalRead(SA);
}

int schalter_zu() {
	return digitalRead(SZ);
}

void kill_all() {
	Serial.println("Kill em All!!!")
	digitalWrite(END, HIGH);
}

int lichtschrank_vorn_hell() {
		return digitalRead(LVH);
}

int lichtschrank_vorn_dunkel() {
		return digitalRead(LVD);
}

int lichtschrank_hinten_hell() {
		return digitalRead(LHH);
}

int lichtschrank_hinten_dunkel() {
		return digitalRead(LHD);
}

int lichtschranken() {
	if(lichtschrank_vorn_hell() == HIGH|| lichtschrank_vorn_dunkel() == HIGH || lichtschrank_hinten_hell() == HIGH || lichtschrank_hinten_dunkel() == HIGH)
		return HIGH;
	else
		return LOW;
}

void loop() {
	//diese Variablen müssen bei jedem neuen durchlauf bereinigt werden
	long start = 0;
	int schwer = 0;
	int durchgang = HIGH
	
	klappe_auf();
	while(!schalter_auf() && sensor_klappe()){
		//nix tun bis endschalter oder zu heftig klappe
	}
	klappe_stop();
	walze_vor();

	while(durchgang == HIGH) {//hier wird geklärt ob es noch einen durchgang gibt
		durchlauf = LOW
		start = millis();
		while((start+WLV)<millis() && schwer < WC) { 
			if(lichtschranken() == HIGH) {
				start = millis();
			}
			
			if(sensor_walze()) {
				schwer++;
				walze_zurueck();
				while((start+WLZ)<millis()) {
					//nixtun bis zeit zuende
				}
				walze_vor();
			}
		} 
		walze_stop();
		//TODO schwer counter prüfen
		
		start = millis();
		while((start+LW)<millis() && durchlauf == LOW) {
			if(lichtschranken() == HIGH) {// todo lichtschranke
				durchlauf = HIGH;
			}
			// nix tun bis zeit abläuft oder lichtschranke
		}
	}

	klappe_zu();
	// klappe schließen
	while(schalter_zu() == LOW && lichtschranken() == LOW) {
	// beim schließen auf LS achten und auf sense	
	}
	klappe_stop();
	if(schalter_zu() == HIGH) {
		kill_all();
		//wenn alles ohne unterbrechung klappte killsig
	}
}
