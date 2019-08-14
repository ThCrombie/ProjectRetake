#define CUSTOM_SETTINGS
#define INCLUDE_TEXT_TO_SPEECH_SHIELD
#define INCLUDE_VOICE_RECOGNIZER_SHIELD
#define INCLUDE_CLOCK_SHIELD

#include <OneSheeld.h>

const char firstCommand[]="what is the time";
const char secondCommand[]="weather";
const char thirdCommand[]="turn on";

char b[12];
char c[12];
int hour, minute;
String stringHour, stringMinute;

int ledPin = 13;

void setup()
{
  OneSheeld.begin();
  /* Set the LED pin as output. */
  pinMode(ledPin,OUTPUT);
}

void loop(){
  hour = Clock.getHours();
  minute = Clock.getMinutes();
  stringHour = String(hour);
  stringMinute = String(minute);
  
  /* Always check if a new voice command received. */ 
  if(VoiceRecognition.isNewCommandReceived()){
    /* Check if the voice command is the desired one. */
    if(!strcmp(firstCommand,VoiceRecognition.getLastCommand())){
      TextToSpeech.say("The time is...");
      printf("%d hour %d minute/n", &stringHour, &stringMinute);
      delay(500);
      TextToSpeech.say(stringHour);
      delay(300);
      TextToSpeech.say(stringMinute);
      TextToSpeech.say(" P M");
     }
     /* Check if the voice command is the desired one. */
     else if(!strcmp(secondCommand,VoiceRecognition.getLastCommand()))
     {
       /* 1Sheeld responds using text-to-speech. */
       TextToSpeech.say("the weather is pretty good sir");
     }
     /* Check if the voice command is the desired one. */
     else if(!strcmp(thirdCommand,VoiceRecognition.getLastCommand()))
     {
       digitalWrite(ledPin,HIGH);
       /* 1Sheeld responds using text-to-speech. */
       TextToSpeech.say("led turned on");
     }
     else
     {
       digitalWrite(ledPin,LOW);
       delay(1000);
       TextToSpeech.say("I do not understand. Could you repeat that?");
     }
   }
}
