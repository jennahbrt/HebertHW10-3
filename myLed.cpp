#include <iostream>         // for the input/output
#include <stdlib.h>         // for the getenv call
#include <sys/sysinfo.h>    // for the system uptime call
#include <cgicc/Cgicc.h>    // the cgicc headers
#include <cgicc/CgiDefs.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include "GPIO.h"
#include<unistd.h> //for usleep

using namespace exploringBB;
using namespace std;
using namespace cgicc;

int main(){
   GPIO LED(46);
   LED.setDirection(OUTPUT); //using the GPIO class, setting it as the input
   Cgicc form; 
   string cmd;                      // the Set LED command
   // get the state of the form that was submitted - script calls itself
   bool isStatus = form.queryCheckbox("status");
   form_iterator it = form.getElement("cmd");  // the radio command
   if (it == form.getElements().end() || it->getValue()==""){
      cmd = "off";                     // if it is invalid use "off"
   }
   else { cmd = it->getValue(); }      // otherwise use submitted value
   char *value = getenv("REMOTE_ADDR");    // The remote IP address
 
   // generate the form but use states that are set in the submitted form
   cout << HTTPHTMLHeader() << endl;       // Generate the HTML form
   cout << html() << head() << title("LED Example") << head() << endl;
   cout << body() << h1("CPE 422/522 Project: Post LED Example") << endl;
   cout << body() << h1("Controlling a GPIO pin via a Web Browser") << endl;
   cout << "<form action=\"/cgi-bin/myLed.cgi\" method=\"POST\">\n";
   cout << "<div>Set LED: <input type=\"radio\" name=\"cmd\" value=\"on\""
        << ( cmd=="on" ? "checked":"") << "/> On ";
   cout << "<input type=\"radio\" name=\"cmd\" value=\"off\""
        << ( cmd=="off" ? "checked":"") << "/> Off ";
   cout << "<input type=\"radio\" name=\"cmd\" value=\"blink\""
        << ( cmd=="blink" ? "checked":"") << "/> Blink ";
   cout << "<input type=\"submit\" value=\"Set LED\" />" <<endl;
   cout << "<br><br><br>";
   cout << "Number of Blinks: <input type=\"text\"";
   cout << " name=\"numOfBlink\"value=\"\" </div></form>";
 
   // process the form data to change the LED state
   if (cmd=="on"){
	LED.setValue(HIGH);              // turn on
   }
   else if (cmd=="off") {
	LED.setValue(LOW);        // turn off
   }
   else if (cmd=="blink"){
  	for (int i=0; i<5; i++){
		LED.setValue(HIGH);
		usleep(500000); //micro-second sleep 0.5 seconds
		LED.setValue(LOW); 
		usleep(500000); //micro-second sleep 0.5 seconds
	}
   }
   else cout << "<div> Invalid command! </div>";        // not possible
   cout << "<div> The CGI REMOTE_ADDR value is " << value << "</div>";
   cout << body() << html();
   return 0;
}
