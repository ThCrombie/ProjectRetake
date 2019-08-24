#define CUSTOM_SETTINGS
#define INCLUDE_TEXT_TO_SPEECH_SHIELD
#define INCLUDE_VOICE_RECOGNIZER_SHIELD
#define INCLUDE_CLOCK_SHIELD
#define INCLUDE_INTERNET_SHIELD

#include <OneSheeld.h>
/* link for Cork's current weather on openweathermap.com, for the weather report:
 * http://api.openweathermap.org/data/2.5/weather?q=Cork,IE&appid=43d95b4cf5d0573e2dfe5186c160017a
 *
 * == http request for the weather in Cork city, Co. Cork. ==
 * uses the API key from: 
 * https://www.hackster.io/ahmedismail3115/arduino-based-amazon-echo-using-1sheeld-84fa6f  
 * (DIY 'Arduino Based Amazon Echo Using 1Sheeld' tutorial)
*/
HttpRequest request1("http://api.openweathermap.org/data/2.5/weather?q=Cork,IE&appid=43d95b4cf5d0573e2dfe5186c160017a");

// the two commands you can use to get the time
const char timeCommand[]="what time is it";
const char timeCommandSecond[]="how late is it";

// the command to get the weather (broken)
const char weathCommand[]="what is the weather like";

// command to turn lights on/off
const char lightsOnCommand[]="lights on";
const char lightsOffCommand[]="lights off";

// command to turn heating on/off
const char heatOnCommand[]="heating on";
const char heatOffCommand[]="heating off";

// commands to order a taxi
const char taxiCommand[]="can i get a taxi";
const char taxiCommandSecond[]="could you order me a taxi";
const char taxiCommandThird[]="i need a taxi";


char b[12];
char c[12];
int hour, minute;
String stringHour, stringMinute;

int reply;

void setup()
{
  OneSheeld.begin();
  request1.setOnSuccess(&onSuccess1);
  //Internet.performGet(request1);
  request1.getResponse().setOnJsonResponse(&onJsonReply1);
  request1.getResponse().setOnError(&onResponseError);

  // leds (lights, heat, 'ordering food' etc)
  pinMode(13, OUTPUT);  // pin 13 - lights (white led)
  pinMode(12, OUTPUT);  // pin 12 - heating (green led)
  pinMode(8, OUTPUT);  // pin 12 - taxi (red led)
  
}

void loop(){
  Clock.queryDateAndTime(); // start the clock_shield each loop
  
  hour = Clock.getHours();
  minute = Clock.getMinutes();
  stringHour = String(hour);
  stringMinute = String(minute);

  // commands to get a responce from TTS
  if(VoiceRecognition.isNewCommandReceived()){
    reply = rand() % (2 + 1 - 0) + 0; 
    // pick a number from 0 - 2 to give a variation of answers.
    if(!strcmp(timeCommand,VoiceRecognition.getLastCommand()) || !strcmp(timeCommandSecond,VoiceRecognition.getLastCommand())){

      if(reply == 0 ){
        TextToSpeech.say("The time is...");
      }else if (reply == 1){
        TextToSpeech.say("It is now...");
      }else{
        TextToSpeech.say("It is currently...");
      }
      
      delay(300);
      TextToSpeech.say(stringHour);
      delay(200);
      TextToSpeech.say(stringMinute);
      // if it's 12 (digital) or later, say PM, else it's AM.
      delay(100);
      if(hour >= 12){
        TextToSpeech.say(" P M");
        } else{
          TextToSpeech.say(" A M");
  }
     }
     // reply if asked about the weather
     else if(!strcmp(weathCommand,VoiceRecognition.getLastCommand()))
     {
       /* 1Sheeld responds using text-to-speech. */
       TextToSpeech.say("Checking...");
       //Terminal.println("Doing Internet.PerformGet...");
       Internet.performGet(request1);
       TextToSpeech.say("Sent request for weather data.");
       
     }
     // turn on lights
     else if(!strcmp(lightsOnCommand,VoiceRecognition.getLastCommand()))
     {
      delay(500);
      TextToSpeech.say("lights turned on");
      digitalWrite(13, HIGH);
     }
     // turn off lights
     else if(!strcmp(lightsOffCommand,VoiceRecognition.getLastCommand()))
     {
      delay(500);
      TextToSpeech.say("lights turned off");
      digitalWrite(13, LOW);
     }
     else if(!strcmp(heatOnCommand,VoiceRecognition.getLastCommand()))
     {
      delay(500);
      TextToSpeech.say("heating turned on");
      digitalWrite(12, HIGH);
     }
     // turn off lights
     else if(!strcmp(heatOffCommand,VoiceRecognition.getLastCommand()))
     {
      delay(500);
      TextToSpeech.say("heating turned off");
      digitalWrite(12, LOW);
     }
     // 'order a taxi' and blink led a few times
     else if(!strcmp(taxiCommand,VoiceRecognition.getLastCommand()) || !strcmp(taxiCommandSecond,VoiceRecognition.getLastCommand()) || !strcmp(taxiCommandThird,VoiceRecognition.getLastCommand()))
     {
      reply = rand() % (2 + 1 - 0) + 0; 
      delay(500);
      if(reply == 0 ){
        TextToSpeech.say("calling a taxi for you...");
      }else if (reply == 1){
        TextToSpeech.say("finding you a taxi...");
      }else{
        TextToSpeech.say("i'll search for a taxi now...");
      }
      
      
      
      delay(1000);
      digitalWrite(8, HIGH);
      delay(500);
      digitalWrite(8, LOW);
      delay(500);
      digitalWrite(8, HIGH);
      delay(500);
      digitalWrite(8, LOW);
      delay(500);
      digitalWrite(8, HIGH);
      delay(500);
      digitalWrite(8, LOW);
      delay(500);
      digitalWrite(8, HIGH);
      delay(500);
      digitalWrite(8, LOW);
      delay(500);
      digitalWrite(8, HIGH);
      delay(500);
      digitalWrite(8, LOW);
      delay(200);
      TextToSpeech.say("The taxi will be there shortly.");
     }
     else
     {
       // if Voice Recognition cant understand it will let the user know
       delay(1000);
       TextToSpeech.say("I do not understand.");
     }
   }
}

void onSuccess1(HttpResponse & response1)
{
  // code from the Alexa example: 
  
  /* Using the response to query the Json chain till the required value. */
  /* i.e. Get the value of 'main' in the first object of the array 'weather' in the response. */
  /* Providing that the response is in JSON format. */

  //Terminal.println("Debug: Reached onSuccess1 method");
  response1["weather"][0]["main"].query();
  TextToSpeech.say("Querying weather...");
  delay(200);    
}

void onJsonReply1(JsonKeyChain & hell,char * output)
{
  //Terminal.println("Debug: Reached onJsonReply1 method");
  if(hell == request1.getResponse()["weather"][0]["main"])
  {
   TextToSpeech.say("the weather today in Cork is");
  delay(2000);
  /* 1Sheeld responds using text-to-speech shield. */
  TextToSpeech.say(output);
    }/*else{
       TextToSpeech.say("Unable to get weather data.");
    }*/
}

void onResponseError(int errorNumber)
{
  //hour = errorNumber;
  //stringHour = String(errorNumber);
  TextToSpeech.say("Cannot get weather, error code.");
  //TextToSpeech.say(stringHour);
}
