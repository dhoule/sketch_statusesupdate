/*
  SendATweet

  Demonstrates sending a tweet via a Twitter account using the Temboo Arduino Yun SDK.
  
  This example code is in the public domain.
*/

#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" // contains Temboo account information
#include "TwitterAccount.h"

/*** SUBSTITUTE YOUR VALUES BELOW: ***/

int numRuns = 1;   // execution count, so this sketch doesn't run forever
int maxRuns = 1;  // the max number of times the Twitter Update Choreo should run
unsigned long passed = 0; //Used to retweet after 30 minutes.

void setup() {
  Bridge.begin();
  Console.begin();
  // for debugging, wait until a serial console is connected
  delay(4000);
}

void loop()
{
  unsigned long current = millis();
  // only try to send the tweet if we haven't already sent it successfully OR 30 minutes has passed
  if ( (numRuns <= maxRuns) || ((current - passed) >= 1800000) ) {
    if((current - passed) >= 1800000){
      passed = current;
      numRuns = 1;
    }
    // define the text of the tweet we want to send. String(millis()) is used to create unique tweets.
    String tweetText("Your message here " + String(millis()));

    
    TembooChoreo StatusesUpdateChoreo;

    // invoke the Temboo client
    // NOTE that the client must be reinvoked, and repopulated with
    // appropriate arguments, each time its run() method is called.
    StatusesUpdateChoreo.begin();
    
    // set Temboo account credentials
    StatusesUpdateChoreo.setAccountName(TEMBOO_ACCOUNT);
    StatusesUpdateChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    StatusesUpdateChoreo.setAppKey(TEMBOO_APP_KEY);

    // identify the Temboo Library choreo to run (Twitter > Tweets > StatusesUpdate)
    StatusesUpdateChoreo.setChoreo("/Library/Twitter/Tweets/StatusesUpdate");

    // set the required choreo inputs
    // see https://www.temboo.com/library/Library/Twitter/Tweets/StatusesUpdate/ 
    // for complete details about the inputs for this Choreo
 
    // add the Twitter account information
    StatusesUpdateChoreo.addInput("AccessToken", TWITTER_ACCESS_TOKEN);
    StatusesUpdateChoreo.addInput("AccessTokenSecret", TWITTER_ACCESS_TOKEN_SECRET);
    StatusesUpdateChoreo.addInput("ConsumerKey", TWITTER_API_KEY);    
    StatusesUpdateChoreo.addInput("ConsumerSecret", TWITTER_API_SECRET);

    // and the tweet we want to send
    StatusesUpdateChoreo.addInput("StatusUpdate", tweetText);

    // tell the Process to run and wait for the results. The 
    // return code (returnCode) will tell us whether the Temboo client 
    // was able to send our request to the Temboo servers
    unsigned int returnCode = StatusesUpdateChoreo.run();

    // a return code of zero (0) means everything worked
    if (returnCode == 0) {
        Console.println("Success! Tweet sent!");
        numRuns++;
    } else {
      // a non-zero return code means there was an error
      // read and print the error message
      while (StatusesUpdateChoreo.available()) {
        char c = StatusesUpdateChoreo.read();
        Console.print(c);
      }
    } 
    StatusesUpdateChoreo.close();

    // do nothing for the next 90 seconds
    delay(50000);
  }
}
